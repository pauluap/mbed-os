
#pragma once

#include <cstdint>

#include "EthernetInterface.h"
#include "EventQueue.h"

#include "UDPSocketNonBlocking.hpp"

namespace multicast {

class UdpHandler
{
public:
    enum { udp_handler_port = 0x1437  };
    enum { multicast_main_group = 239 };
    enum { socket_retry_ms = 2000     };

    UdpHandler(EthernetInterface* stack, EventQueue* queue);

    void Start();

    void Transmit();

protected:
    void _JoinMulticastGroup();
    void _SocketCallback();
    void _HandleSocket();

    EthernetInterface*   stack_;
    EventQueue*          queue_;
    UDPSocketNonBlocking socket_;
    SocketAddress        address_;
};

} /* namespace multicast */
