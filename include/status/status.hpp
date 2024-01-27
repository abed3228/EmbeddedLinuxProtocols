
#ifndef STATUS_HPP
#define STATUS_HPP

enum class status
{
    SUCCESS = 0,
    FAILURE,
    PENDING,
    ERROR,
    PORT_ERROR // Added new status for port-related errors
};

#endif // STATUS_HPP