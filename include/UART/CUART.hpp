/**
 * @file CUART.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-23
 *
 * @copyright Copyright (c) 2024
 *
 */

// TODO: do change in runtime configuration

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
    enum BaudRate
    {
        BAUD_2400 = B2400,
        BAUD_9600 = B9600,
        BAUD_19200 = B19200,
        BAUD_38400 = B38400,
        BAUD_57600 = B57600,
        BAUD_115200 = B115200
    };
    UART();
    UART(BaudRate baudRate, char parity, int dataBits, int stopBits);
    UART(std::string port, BaudRate baudRate, char parity, int dataBits, int stopBits);
    UART(std::string port, BaudRate baudRate, char parity, int dataBits, int stopBits, int timeout, int vmin);
    ~UART();
    bool init(std::string path);
    bool isOpened();
    bool closePort();
    ssize_t writePort(const std::string buff);
    ssize_t readPort(std::string &buff, int sizeRead);

private:
    BaudRate m_baudRate;
    int m_parity; // m_options.c_cflag
    int m_dataBits;
    int m_stopBits; // m_options.c_cflag
    int m_vtime;
    int m_vmin;
    struct termios m_options;

    // Private copy constructor (deleted)
    UART(const UART &) = delete;
    // Private assignment operator (deleted)
    UART &operator=(const UART &) = delete;

    bool setBaudRate(BaudRate baudRate);
    bool setParity(char parity);
    bool setStopBits(int stopBits);
    bool setDataBits(int dataBits);
    bool setTimeouts(int timeouts);
    bool setMinCharacter(int minCharacter);
};

#endif // UART_H