#include "UdpSupervisor.hpp"

#include "MakeUnique.hpp"

namespace multicast {

UdpSupervisor::UdpSupervisor(EthernetInterface* stack) :
    stack_(stack)
{}

void UdpSupervisor::Start()
{
    queue_.call(this, &UdpSupervisor::_Start);
    thread_.start(mbed::Callback<void()>(&queue_, &events::EventQueue::dispatch_forever));
}

void UdpSupervisor::_Start()
{
    handler_ = multicast::make_unique<UdpHandler>(stack_, &queue_);
    handler_->Start();
    queue_.call_every(1000, this, &UdpSupervisor::_SendUdpMessage);
}

void UdpSupervisor::_SendUdpMessage()
{
    handler_->Transmit();
}

} /* namespace multicast */
