/**
 * @file ICommunicationDriver.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-23
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef COMMUNICATION_DRIVER_HPP
#define COMMUNICATION_DRIVER_HPP
#include <iostream>
#include <string>
#include <vector>

class CommunicationDriver
{
protected:
    int m_fd;
    int m_isOpen;

public:
    virtual ~CommunicationDriver(){};
    /**
     * @brief chcek connection if is available or not
     * @return true if the connection is established
     * @return false if the connection is not established
     */
    virtual bool isOpened() = 0;
    /**
     * @brief close driver connection
     * @return true if the close connection is successful
     * @return false if the close connection is failed
     */
    virtual bool closePort() = 0;
    /**
     * @brief
     *
     * @return true
     * @return false
     */
    virtual bool init(std::string path) = 0;
    /**
     * @brief
     *
     * @param buf
     * @param count
     * @return ssize_t On success, the number of bytes written is returned.  On error,
               -1 is returned, and errno is set to indicate the error.
     */
    virtual ssize_t writePort(const std::string buff) = 0;
    /**
     * @brief
     *
     * @param buff
     * @param length
     * @return ssize_t
     */
    virtual ssize_t readPort(std::string &buff,int sizeRead) = 0;
};

#endif // COMMUNICATION_DRIVER_HPP