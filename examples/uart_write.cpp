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
        serial.writeData("YOUR COMMAND STRING HERE");
        std::cout << "YOUR COMMAND STRING HERE" << std::endl;
        sleep(1);
    } while (!exit_while);

    return 0;
}
