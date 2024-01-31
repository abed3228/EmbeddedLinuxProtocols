#include "cuart.hpp"

uart::uart(const std::string path)
{
    m_isOpen = false;
    m_fd = -1;

    setBaudRate(default_baudRate);
    setDataBits(default_dataBits);
    setParity(default_parity);
    setStopBits(default_stopBits);
    setTimeouts(default_timeout);
    setMinCharacter(default_vmin);

    if (!openDevice(path))
        throw std::runtime_error("Failed to open port");
    m_isOpen = true;
    if (!initSerialInterface())
    {
        closeDevice();
        throw std::runtime_error("Failed to configure port");
    }
}
uart::uart(std::string path, BaudRate baudRate, char parity, uint32_t dataBits,
           uint32_t stopBits)
{
    m_isOpen = false;
    m_fd = -1;

    setBaudRate(baudRate);
    setDataBits(dataBits);
    setParity(parity);
    setStopBits(stopBits);

    if (!openDevice(path))
        throw std::runtime_error("Failed to open port");
    m_isOpen = true;
    if (!initSerialInterface())
    {
        closeDevice();
        throw std::runtime_error("Failed to configure port");
    }
}
uart::~uart()
{
    closeDevice();
}
bool uart::isOpened()
{
    return m_isOpen;
}
bool uart::openDevice(std::string path)
{
    // Open the serial port for read and write, without controlling terminal
    m_fd = open(path.c_str(), O_RDWR | O_NOCTTY);
    if (m_fd == -1)
        return false;
    m_isOpen = true;
    m_fdPoll.fd = m_fd;
    m_fdPoll.events = POLLIN;
    return true;
}
bool uart::closeDevice()
{
    if (!isOpened() && close(m_fd))
        return true;
    return false;
}
bool uart::initSerialInterface()
{
    // Get the current configuration of the serial interface
    if (tcgetattr(m_fd, &m_options) != 0)
        return false;

    // Set input and output baud rates
    cfsetispeed(&m_options, m_baudRate);
    cfsetospeed(&m_options, m_baudRate);

    // Setting Parity and Stop Bits
    m_options.c_cflag &= ~PARENB; // Clear parity enable
    m_options.c_cflag &= ~CSTOPB; // Clear stop bits
    m_options.c_cflag &= ~CSIZE;  // Clear character size bits
    m_options.c_iflag = IGNBRK;
    m_options.c_lflag = 0;
    m_options.c_oflag = 0;
    m_options.c_cflag |= m_parity;   // Set parity bits
    m_options.c_cflag |= m_stopBits; // Set stop bits
    m_options.c_cflag |= m_dataBits; // Set data bits
    m_options.c_iflag &= ~(IXON | IXOFF | IXANY);
    // TODO:options.c_iflag = IGNBRK;options.c_lflag = 0;options.c_oflag = 0; options.c_iflag &= ~(IXON | IXOFF | IXANY);

    // Enable receiver and set local mode
    m_options.c_cflag |= (CLOCAL | CREAD);

    // Apply the configuration
    if (tcsetattr(m_fd, TCSANOW, &m_options) != 0)
        return false;

    return true;
}
ssize_t uart::writeData(const std::string buff)
{
    return write(m_fd, buff.c_str(), buff.length());
}

ssize_t uart::readData(std::string &buff, uint32_t sizeRead, uint32_t maxTimeOut)
{
    // Calculate end time
    auto endTime = std::chrono::steady_clock::now() + std::chrono::seconds(maxTimeOut);

    while (std::chrono::steady_clock::now() < endTime)
    {
        int ret = poll(&m_fdPoll, 1, m_timeout * 1000);
        if (ret > 0 && (m_fdPoll.revents & POLLIN))
        {
            buff.resize(sizeRead);
            return read(m_fd, &buff[0], sizeRead);
        }
        else if (ret < 0)
            return ERR_IO_ERROR;
        /*
        else{
            //timeout
        }
        */
    }

    // Timeout reached
    return ERR_TIMEOUT;
}

bool uart::setBaudRate(BaudRate baudRate)
{
    m_baudRate = baudRate;
    return true;
}
bool uart::setParity(char parity)
{
    // Clear current parity settings
    m_parity &= ~(PARENB | PARODD);

    switch (parity)
    {
    case 'N': // Fall-through intended
    case 'n':
        // No parity, PARENB is already cleared
        break;
    case 'E': // Fall-through intended
    case 'e':
        m_parity |= PARENB;  // Enable parity
        m_parity &= ~PARODD; // Clear odd parity -> even parity
        break;
    case 'O': // Fall-through intended
    case 'o':
        m_parity |= (PARENB | PARODD); // Enable parity and set odd parity
        break;
    default:
        // Optionally handle unexpected input; currently ignored
        break;
    }

    return true; // Indicate successful setting
}

bool uart::setStopBits(uint32_t stopBits)
{
    switch (stopBits)
    {
    case 2:
        m_stopBits |= CSTOPB; // Set 2 stop bits
        break;
    case 1:
    default:
        m_stopBits &= ~CSTOPB; // Set 1 stop bit (default)
        break;
    }
    return true; // Indicate successful setting
}
bool uart::setDataBits(uint32_t dataBits)
{
    // Clear the current data bits setting
    m_dataBits &= ~CSIZE;

    // Set the new data bits setting based on the input
    switch (dataBits)
    {
    case 5:
        m_dataBits |= CS5; // Set 5 data bits
        break;
    case 6:
        m_dataBits |= CS6; // Set 6 data bits
        break;
    case 7:
        m_dataBits |= CS7; // Set 7 data bits
        break;
    case 8:
    default:
        m_dataBits |= CS8; // Set 8 data bits (default)
        break;
    }

    return true; // Indicate successful setting
}

bool uart::setTimeouts(uint32_t timeouts)
{
    // Set read timeout (vtime) based on the provided timeouts parameter
    // vtime is set in tenths of a second (deciseconds)
    m_timeout = timeouts;
    m_time.tv_sec = m_timeout;
    m_time.tv_nsec = 0;
    return true; // Indicate successful setting
}

bool uart::setMinCharacter(uint32_t minCharacter)
{
    // Set the minimum number of characters to read based on the input parameter
    m_vmin = minCharacter;

    return true; // Indicate successful setting
}
