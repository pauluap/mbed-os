#include <iostream>

#include "mbed.h"

#include "EthernetInterface.h"

#include "UdpSupervisor.hpp"

EthernetInterface net;

int main()
{
    net.connect();
    std::cout << "IP address is: " << net.get_ip_address() << "\n";

    multicast::UdpSupervisor supervisor(&net);
    supervisor.Start();

    while (true) {}
}
