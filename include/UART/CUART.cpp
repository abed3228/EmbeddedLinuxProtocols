#include "CUART.hpp"

UART::UART()
{
    m_isOpen = false;
    m_fd = -1;

    setBaudRate(BaudRate::BAUD_115200);
    setDataBits(8);
    setParity('n');
    setStopBits(1);
    setTimeouts(5);
    setMinCharacter(1);
    std::cout << "UART ctor" << std::endl;
}
UART::~UART()
{
    closePort();
    std::cout << "UART dtor" << std::endl;
}
bool UART::isOpened()
{
    return m_isOpen;
}
bool UART::closePort()
{
    if (!isOpened() && close(m_fd))
        return true;
    return false;
}
bool UART::init(std::string path)
{
    // Open the serial port for read and write, without controlling terminal
    int fd = open(path.c_str(), O_RDWR | O_NOCTTY);
    if (fd == -1)
        return false;

    m_isOpen = true;

    // Get the current configuration of the serial interface
    if (tcgetattr(fd, &m_options) != 0)
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

    // Set timeouts
    m_options.c_cc[VTIME] = m_vtime; // Timeout in deciseconds
    m_options.c_cc[VMIN] = m_vmin;   // Minimum number of characters to read

    // Apply the configuration
    if (tcsetattr(fd, TCSANOW, &m_options) != 0)
        return false;

    return true;
}
ssize_t UART::writePort(const std::string buff)
{
    return write(m_fd, buff.c_str(), buff.length());
    ;
}
ssize_t UART::readPort(std::string &buff, int sizeRead)
{
    return read(m_fd, &buff[0], sizeRead);
}

bool UART::setBaudRate(BaudRate baudRate)
{
    m_baudRate = baudRate;
}
bool UART::setParity(char parity)
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

bool UART::setStopBits(int stopBits)
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
bool UART::setDataBits(int dataBits)
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

bool UART::setTimeouts(int timeouts)
{
    // Set read timeout (vtime) based on the provided timeouts parameter
    // vtime is set in tenths of a second (deciseconds)
    m_vtime = timeouts;
    return true; // Indicate successful setting
}

bool UART::setMinCharacter(int minCharacter)
{
    // Set the minimum number of characters to read based on the input parameter
    m_vmin = minCharacter;

    return true; // Indicate successful setting
}
