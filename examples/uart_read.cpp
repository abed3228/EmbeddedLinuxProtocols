#include <iostream>
#include <string>
#include <vector>
#include <csignal>
#include <cstdlib>

#include "../protocols/low_level/uart/cuart.hpp"

bool exit_while = false;

void signalHandler(int signalNumber)
{
    if (signalNumber == SIGINT)
    {
        exit_while = true;
        std::cout << "exit" << std::endl;
    }
}

int main(int argc, char **argv)
{
    uart serial("/dev/ttyS0");
    // Set the signal handler for SIGTERM
    std::signal(SIGINT, signalHandler);

    do
    {
        std::string strRead;
        std::cout << serial.readData(strRead, 255, 10) << " " << strRead << std::endl;
    } while (!exit_while);

    return 0;
}
