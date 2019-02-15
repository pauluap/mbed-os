#include "UdpHandler.hpp"

#include <cstring>

namespace multicast {

UdpHandler::UdpHandler(EthernetInterface* stack, EventQueue* queue) :
    stack_(stack),
    queue_(queue)
{}

void UdpHandler::Start()
{
    auto res = socket_.open(stack_);

    if (res == NSAPI_ERROR_OK)
    {
        socket_.sigio(mbed::callback(this, &UdpHandler::_SocketCallback));
        socket_.bind(udp_handler_port);
        _JoinMulticastGroup();
        _HandleSocket();
    }
    else
    {
        queue_->call_in(socket_retry_ms, this, &UdpHandler::Start);
    }
}

void UdpHandler::_SocketCallback()
{
    queue_->call(this, &UdpHandler::_HandleSocket);
}

void UdpHandler::_HandleSocket()
{
    uint8_t buffer[1024];

    auto res = socket_.recvfrom(nullptr, buffer, 1024);
    if (res > 0)
    {
        buffer[res] = 0;
        std::cout << buffer;
    }
}

void UdpHandler::_JoinMulticastGroup()
{
    SocketAddress mc;
    uint8_t bytes[4];
    bytes[0] = multicast_main_group;

    bytes[1] = 0;
    bytes[2] = 1;

    for (uint8_t i = 2; i <= 24; i++)
    {
        bytes[3] = i;
        mc.set_ip_bytes(bytes, nsapi_version::NSAPI_IPv4);
        socket_.join_multicast_group(mc);
    }
}

void UdpHandler::Transmit()
{
    uint8_t bytes[4];

    bytes[0] = multicast_main_group;
    bytes[1] = 0;
    bytes[2] = 1;
    bytes[3] = 1;

    address_.set_ip_bytes(bytes, nsapi_version::NSAPI_IPv4);
    address_.set_port(udp_handler_port);

    uint8_t buffer[1024];
    buffer[0] = 'h';
    buffer[1] = 'e';
    buffer[2] = 'l';
    buffer[3] = 'l';
    buffer[4] = 'o';
    buffer[5] =  0;

    socket_.sendto(address_, buffer, 5);
}

} /* namespace multicast */
