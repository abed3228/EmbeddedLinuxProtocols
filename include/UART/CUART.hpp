/**
 * @file CUART.hpp
 * @author Alnkib Abed (abed3228@gmail.com)
 * @brief UART class for handling serial communication.
 * @version 0.2
 *
 * This class provides functionalities for UART (Universal Asynchronous
 * Receiver/Transmitter) communication including opening and closing ports,
 * configuring the serial interface, and performing read/write operations.
 *
 * @copyright Copyright (c) 2024
 */

// TODO: do change in runtime configuration
// TODO: Adding support for theards for receiving and sending and vectors (send, read)
#ifndef UART_H
#define UART_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <termios.h>   // POSIX terminal control definitions
#include <unistd.h>    /* Symbolic Constants */
#include <sys/types.h> /* Primitive System Data Types */
#include <errno.h>     /* Errors */
#include <stdio.h>     /* Input/Output */
#include <sys/wait.h>  /* Wait for Process Termination */
#include <stdlib.h>    /* General Utilities */
#include <fcntl.h>
#include "../CommunicationDriver/ICommunicationDriver.hpp"
#include "../status/status.hpp"

class UART : protected CommunicationDriver
{

public:
    /**
     * @brief Enumeration for supported baud rates.
     */
    enum BaudRate
    {
        BAUD_2400 = B2400,
        BAUD_9600 = B9600,
        BAUD_19200 = B19200,
        BAUD_38400 = B38400,
        BAUD_57600 = B57600,
        BAUD_115200 = B115200
    };

    /**
     * @brief Constructs a new UART object with default settings.
     * @param path The device path for the UART interface.
     */
    UART(const std::string path);

    /**
     * @brief Constructs a new UART object with custom settings.
     * @param path The device path for the UART interface.
     * @param baudRate The baud rate for the UART communication.
     * @param parity The parity setting ('N' for None, 'E' for Even, 'O' for Odd).
     * @param dataBits The number of data bits.
     * @param stopBits The number of stop bits.
     * @param timeout The read timeout in seconds.
     * @default timeout = 5
     * @param vmin The minimum number of characters for non-canonical read.
     * @default vmin = 0

     */
    UART(std::string path, BaudRate baudRate, char parity, uint32_t dataBits, uint32_t stopBits, uint32_t timeout, uint32_t vmin);

    /**
     * @brief Destructor for UART object.
     */
    ~UART();

    /**
     * @brief Checks if the UART port is open.
     * @return true If the port is open.
     * @return false If the port is closed.
     */
    bool isOpened();

    /**
     * @brief Writes data to the UART port.
     * @param buff The data buffer to write.
     * @return ssize_t The number of bytes written or -1 if an error occurred.
     */
    ssize_t writePort(const std::string buff);

    /**
     * @brief Reads data from the UART port.
     * @param buff Reference to the buffer where read data will be stored.
     * @param sizeRead The number of bytes to read.
     * @return ssize_t The number of bytes read or -1 if an error occurred.
     */
    ssize_t readPort(std::string &buff, uint32_t sizeRead);

private:
    BaudRate m_baudRate;
    char m_parity; // m_options.c_cflag
    uint32_t m_dataBits;
    uint32_t m_stopBits; // m_options.c_cflag
    uint32_t m_timeout;
    uint32_t m_vmin;
    struct termios m_options;

    // Private copy constructor (deleted)
    UART(const UART &) = delete;
    // Private assignment operator (deleted)
    UART &operator=(const UART &) = delete;

    /**
     * @brief Opens the UART port with the specified path.
     * @param path The device path for the UART interface.
     * @return true If the port was successfully opened.
     * @return false If the port could not be opened.
     */
    bool openPort(std::string path);

    /**
     * @brief Closes the currently opened UART port.
     * @return true If the port was successfully closed.
     * @return false If the port could not be closed.
     */
    bool closePort();

    /**
     * @brief Initializes the serial interface with the current settings.
     * @return true If the initialization was successful.
     * @return false If the initialization failed.
     */
    bool initSerialInterface();

    /**
     * @brief Sets the baud rate for the UART communication.
     * @param baudRate The baud rate to be set (from the BaudRate enum).
     * @return true If the baud rate was successfully set.
     * @return false If the baud rate could not be set.
     */
    bool setBaudRate(BaudRate baudRate);

    /**
     * @brief Sets the parity for the UART communication.
     * @param parity The parity setting ('N' for None, 'E' for Even, 'O' for Odd).
     * @return true If the parity was successfully set.
     * @return false If the parity could not be set.
     */
    bool setParity(char parity);

    /**
     * @brief Sets the number of stop bits for the UART communication.
     * @param stopBits The number of stop bits (typically 1 or 2).
     * @return true If the stop bits were successfully set.
     * @return false If the stop bits could not be set.
     */
    bool setStopBits(uint32_t stopBits);

    /**
     * @brief Sets the number of data bits for the UART communication.
     * @param dataBits The number of data bits (typically 5, 6, 7, or 8).
     * @return true If the data bits were successfully set.
     * @return false If the data bits could not be set.
     */
    bool setDataBits(uint32_t dataBits);

    /**
     * @brief Sets the timeouts for the UART communication.
     * @param timeouts The read timeout in seconds.
     * @return true If the timeouts were successfully set.
     * @return false If the timeouts could not be set.
     */
    bool setTimeouts(uint32_t timeouts);

    /**
     * @brief Sets the minimum number of characters for non-canonical read.
     * @param minCharacter The minimum number of characters to be read.
     * @return true If the minimum character setting was successfully set.
     * @return false If the minimum character setting could not be set.
     */
    bool setMinCharacter(uint32_t minCharacter);

    /**
     * @brief default values
     * @param default_baudRate Default baud rate for UART communication.
     * @param default_parity Default parity setting for UART communication, 'n' for no parity.
     * @param default_dataBits Default number of data bits in each character for UART communication.
     * @param default_stopBits Default number of stop bits used in UART communication.
     * @param default_timeout Default timeout for read operations in UART communication, in seconds.
     * @param default_vmin Default minimum number of characters for non-canonical read in UART communication.
     */
    static constexpr BaudRate default_baudRate = BaudRate::BAUD_115200;
    static constexpr uint32_t default_parity = 'n';
    static constexpr uint32_t default_dataBits = 8;
    static constexpr uint32_t default_stopBits = 1;
    static constexpr uint32_t default_timeout = 5;
    static constexpr uint32_t default_vmin = 0;
};

#endif // UART_H