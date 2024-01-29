#include <iostream>
#include <string>
#include <vector>

#include "../drivers/uart/cuart.hpp"

int main(int argc, char **argv)
{
    uart serial("/dev/ttyS2");

    do
    {
        serial.writePort("YOUR COMMAND STRING HERE");
        std::cout << "YOUR COMMAND STRING HERE" << std::endl;
        sleep(1);
    } while (true);

    return 0;
}