#include <iostream>

#include "mbed.h"

#include "DigitalOut.h"
#include "EthernetInterface.h"
#include "Kernel.h"
#include "Thread.h"
#include "Serial.h"

EthernetInterface net;
EventQueue queue;
TCPServer srv;
TCPSocket sock;
Thread client_thread;
Thread greedy1;
Thread greedy2;
Thread greedy3;
Thread greedy4;
Thread greedy5;
Thread greedy6;
Thread greedy7;
Thread greedy8;
Thread greedy9;
Thread greedy10;
EventQueue client_queue;

uint8_t xmit_data[100];
uint8_t rcv_data[100];

uint8_t rcvd;
uint8_t xmtd;
bool socket_open;
const uint16_t MB_QUERY_SIZE = 12;
const uint16_t MB_RESP_SIZE = 153;

mbed::Serial saleae(D1, D0, 1843200);

// name how it appears to saleae
// the test setup is oriented such that D0
// connects to saleae:15 and so forth
mbed::DigitalOut out_D0(D15);
mbed::DigitalOut out_D1(D14);
mbed::DigitalOut out_D2(D13);
mbed::DigitalOut out_D3(D12);
mbed::DigitalOut out_D4(D11);
mbed::DigitalOut out_D5(D10);
mbed::DigitalOut out_D6(D9);
mbed::DigitalOut out_D7(D8);

mbed::DigitalOut out_D8(D7);
mbed::DigitalOut out_D9(D6);
mbed::DigitalOut out_D10(D5);
mbed::DigitalOut out_D11(D4);
mbed::DigitalOut out_D12(D3);
mbed::DigitalOut out_D13(D2);

void DoTransmit()
{
    out_D5 = 0;
    out_D6 = 1;
    if (socket_open)
    {
        out_D1 = 1;
        nsapi_size_or_error_t xmit;

        xmit_data[0] = rcv_data[0];
        xmit_data[1] = rcv_data[1];
        xmit_data[2] = rcv_data[2];
        xmit_data[3] = rcv_data[3];
        xmit_data[4] = 0x00;
        xmit_data[5] = 0x93;
        xmit_data[6] = rcv_data[6];
        xmit_data[7] = 0x03;
        xmit_data[8] = 0x90;

        xmtd = 0;
        do {
            uint16_t desired = MB_RESP_SIZE - xmtd;
            xmit = sock.send(&xmit_data[xmtd], desired);
            saleae.printf("%d = send(&[%u], %u)", xmit, xmtd, desired);
            /* Transmit to completetion */
            if (NSAPI_ERROR_WOULD_BLOCK == xmit) { Thread::yield(); }
            else if (NSAPI_ERROR_OK >= xmit) {
                /* set to satisify the while loop exit condition */
                sock.close();
                socket_open = false;
            }
            else
            {
                xmtd += xmit;
            }
        } while (xmtd != MB_RESP_SIZE);

        /* clear transmitted, prep for the next one */
        xmtd = 0;
        out_D1 = 0;
    }
    out_D6 = 0;
}

void SocketSignalHandler()
{
    out_D3 = 0;
    out_D4 = 1;

    if (socket_open)
    {
        nsapi_size_or_error_t rcv;
        out_D0 = 1;

        do {
            uint16_t desired;
            if (rcvd < 8) { desired = 8 - rcvd; }
            else          { desired = MB_QUERY_SIZE - rcvd; }
            rcv = sock.recv(&rcv_data[rcvd], desired);
            saleae.printf("%d = recv(&[%u], %u)", rcv, rcvd, desired);
            /* permit block so that it doesn't get caught in the error condition */
            if (NSAPI_ERROR_WOULD_BLOCK == rcv) {}
            else if (NSAPI_ERROR_OK >= rcv) {
                /* set to satisify the while loop exit condition */
                rcv = NSAPI_ERROR_WOULD_BLOCK;
                sock.close();
                socket_open = false;
            }
            else
            {
                rcvd += rcv;

                /* complete Modbus query message, immediately transmit response */
                if (rcvd == MB_QUERY_SIZE)
                {
                    /* clear received, prep for the next one */
                    rcvd = 0;
                    out_D5 = 1;
                    //Thread::wait(2);
                    client_queue.event(DoTransmit)();
                }
            }
        } while (rcv != NSAPI_ERROR_WOULD_BLOCK);
        out_D0 = 0;
    }

    out_D4 = 0;
}

void Kicker()
{
    out_D2 = 1;
    out_D3 = 1;
    client_queue.event(SocketSignalHandler)();
    out_D2 = 0;
}

void ConnectClient()
{
    out_D8 = 0;
    sock.set_blocking(false);
    rcvd = 0;
    xmtd = 0;
    socket_open = true;
    SocketSignalHandler();
}

void SrvSignal()
{
    out_D9 = 1;
    out_D8 = 1;
    nsapi_error_t res = srv.accept(&sock);
    std::cout << rtos::Kernel::get_ms_count() << ": Accept attempt resulted in: " << res << "\n";
    if (NSAPI_ERROR_OK == res)
    {
        client_queue.call(ConnectClient);
    }
    out_D9 = 0;
}

void Greedy() { while (true) { __NOP(); } }

int main() {
    std::cout << "Starting at: " << rtos::Kernel::get_ms_count() << "\n";
    net.connect();

    std::cout << "IP address is: " << net.get_ip_address() << "\n";

    //greedy1.start(Callback<void()>(Greedy));
    //greedy2.start(Callback<void()>(Greedy));
    //greedy3.start(Callback<void()>(Greedy));
    //greedy4.start(Callback<void()>(Greedy));
    //greedy5.start(Callback<void()>(Greedy));
    //greedy6.start(Callback<void()>(Greedy));
    //greedy7.start(Callback<void()>(Greedy));
    //greedy8.start(Callback<void()>(Greedy));
    //greedy9.start(Callback<void()>(Greedy));
    //greedy10.start(Callback<void()>(Greedy));

    client_thread.start(mbed::Callback<void()>(&client_queue, &EventQueue::dispatch_forever));
    sock.sigio(Kicker);

    srv.open(&net);
    srv.bind(502);
    srv.set_blocking(false);
    srv.sigio(queue.event(SrvSignal));
    srv.listen();
    queue.dispatch_forever();
}
