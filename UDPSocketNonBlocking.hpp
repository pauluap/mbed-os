/**
 * Copyright 2018 UniEnergy Technologies
 */

#pragma once

#include "features/netsocket/UDPSocket.h"

#include <iostream>
#include "rtos/Kernel.h"

namespace multicast {

/**
 * @brief Wrapper for Mbed's UDPSocket class
 *
 * This class is itended to default the socket to nonblocking
 */
class UDPSocketNonBlocking : public ::UDPSocket
{
    using MbedUDPSocket = ::UDPSocket;

public:
    UDPSocketNonBlocking() { set_blocking(false); }
    UDPSocketNonBlocking(const UDPSocketNonBlocking&) = default;

    template <typename S>
    UDPSocketNonBlocking(S *stack) : MbedUDPSocket(stack) { set_blocking(false); }
};

} /* namespace multicast */
