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

class ICommunicationDriver
{
protected:
    int m_fd;
    uint32_t m_isOpen;

public:
    virtual ~ICommunicationDriver(){};
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
    virtual bool closeDevice() = 0;
    /**
     * @brief oprn driver connection
     * @return true if the open connection is successful
     * @return false if the open connection is failed
     */
    virtual bool openDevice(std::string path) = 0;
    /**
     * @brief
     *
     * @param buf
     * @param count
     * @return ssize_t On success, the number of bytes written is returned.  On error,
               -1 is returned, and errno is set to indicate the error.
     */
    virtual ssize_t writeData(const std::string buff) = 0;
    /**
     * @brief
     *
     * @param buff
     * @param length
     * @return ssize_t
     */
    virtual ssize_t readData(std::string &buff, uint32_t sizeRead, uint32_t maxTimeOut) = 0;
};

#endif // COMMUNICATION_DRIVER_HPP