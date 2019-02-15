#pragma once

#include <cstdint>
#include <memory>

#include "EthernetInterface.h"
#include "EventQueue.h"
#include "Thread.h"

#include "UdpHandler.hpp"

namespace multicast {

class UdpSupervisor
{
public:
    UdpSupervisor(EthernetInterface* stack);

    void Start();

protected:
    void _Start();
    void _SendUdpMessage();

    EthernetInterface*          stack_;
    events::EventQueue          queue_;
    rtos::Thread                thread_;
    std::unique_ptr<UdpHandler> handler_;
};

} /* namespace multicast */
