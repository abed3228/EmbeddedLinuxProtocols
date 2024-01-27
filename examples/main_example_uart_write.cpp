#include <iostream>
#include <string>
#include <vector>

#include "../include/UART/CUART.hpp"

int main(int argc, char **argv)
{
    UART uart;

    uart.init("/dev/ttyS2");

    do
    {
        uart.writePort("YOUR COMMAND STRING HERE");
        sleep(1);
    } while (true);

    return 0;
}