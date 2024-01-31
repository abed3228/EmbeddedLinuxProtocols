#include <sys/types.h> /* Primitive System Data Types */
#include <poll.h>
#include <chrono>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "../include/communicationDriver/ICommunicationDriver.hpp"
#include "../../include/status/status.hpp"

class I2C : protected ICommunicationDriver
{
public:
    I2C(const std::string &devicePath, int deviceAddress);
    ~I2C();

    bool isOpened();
    bool closeDevice();
    bool openDevice(std::string path);
    ssize_t writeData(const std::string buff);
    ssize_t readData(std::string &buff, uint32_t sizeRead, uint32_t maxTimeOut);

private:
    int m_addr;
};
