#include <iostream>
#include <string>
#include <vector>

#include "../protocols/low_level/uart/cuart.hpp"

int main(int argc, char **argv)
{
    uart serial("/dev/ttyS7");

    do
    {
        std::string strRead;
        std::cout << serial.readPort(strRead, 255) << " " << strRead << std::endl;
        sleep(1);
    } while (true);

    return 0;
}