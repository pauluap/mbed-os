#include <iostream>

#include "mbed.h"

#include "EthernetInterface.h"
#include "Kernel.h"
#include "Thread.h"
#include "Serial.h"
#include "saleae.h"
#include "DigitalOut.h"

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
//
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

uint8_t xmit_data[200];
uint8_t rcv_data[200];

uint8_t rcvd;
uint8_t xmtd;
bool socket_open;
const uint16_t MB_QUERY_SIZE = 12;
const uint16_t MB_RESP_SIZE = 153;


extern "C" {
void proxy_D0( const int set) {out_D0  = !!set; }
void proxy_D1( const int set) {out_D1  = !!set; }
void proxy_D2( const int set) {out_D2  = !!set; }
void proxy_D3( const int set) {out_D3  = !!set; }
void proxy_D4( const int set) {out_D4  = !!set; }
void proxy_D5( const int set) {out_D5  = !!set; }
void proxy_D6( const int set) {out_D6  = !!set; }
void proxy_D7( const int set) {out_D7  = !!set; }
void proxy_D8( const int set) {out_D8  = !!set; }
void proxy_D9( const int set) {out_D9  = !!set; }
void proxy_D10(const int set) {out_D10 = !!set; }
void proxy_D11(const int set) {out_D11 = !!set; }
void proxy_D12(const int set) {out_D12 = !!set; }
void proxy_D13(const int set) {out_D13 = !!set; }
}


void DoTransmit()
{
    if (socket_open)
    {
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

            sock.set_blocking(true);
            xmit = sock.send(&xmit_data[xmtd], desired);
            sock.set_blocking(false);

            //saleae.printf("%d = send(&[%u], %u)", xmit, xmtd, desired);
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
    }
}

void SocketSignalHandler()
{
    if (socket_open)
    {
        nsapi_size_or_error_t rcv;

        do {
            uint16_t desired;
            if (rcvd < 8) { desired = 8 - rcvd; }
            else          { desired = MB_QUERY_SIZE - rcvd; }
            rcv = sock.recv(&rcv_data[rcvd], desired);
            //saleae.printf("%d = recv(&[%u], %u)", rcv, rcvd, desired);
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
                    //Thread::wait(2);
                    client_queue.event(DoTransmit)();
                }
            }
        } while (rcv != NSAPI_ERROR_WOULD_BLOCK);
    }
}

void Kicker()
{
    client_queue.event(SocketSignalHandler)();
}

void ConnectClient()
{
    sock.set_blocking(false);
    rcvd = 0;
    xmtd = 0;
    socket_open = true;
    SocketSignalHandler();
}

void SrvSignal()
{
    nsapi_error_t res = srv.accept(&sock);
    std::cout << rtos::Kernel::get_ms_count() << ": Accept attempt resulted in: " << res << "\n";
    if (NSAPI_ERROR_OK == res)
    {
        client_queue.call(ConnectClient);
    }
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
