#include <iostream>

#include "mbed.h"

#include "EthernetInterface.h"
#include "EventFlags.h"
#include "Kernel.h"
#include "Thread.h"
#include "Serial.h"

EthernetInterface net;
TCPServer srv;

const uint16_t MB_QUERY_SIZE = 12;
const uint16_t MB_RESP_SIZE = 153;
static volatile uint8_t dummy[72250];

struct Client
{
    Client() : rcvd(0), xmtd(0), socket_open(false), id(0) {}
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

                /* Transmit to completetion */
                if (NSAPI_ERROR_WOULD_BLOCK == xmit) { Thread::yield(); }
                else if (NSAPI_ERROR_OK >= xmit) {
                    std::cout << rtos::Kernel::get_ms_count() << ": Tx Socket " << id << " Result: " << xmit << ". Closing\n";
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
                uint16_t desired = MB_QUERY_SIZE - rcvd;
                rcv = sock.recv(&rcv_data[rcvd], desired);
                /* permit block so that it doesn't get caught in the error condition */
                if (NSAPI_ERROR_WOULD_BLOCK == rcv) {}
                else if (NSAPI_ERROR_OK >= rcv) {
                    std::cout << rtos::Kernel::get_ms_count() << ": Rx Socket " << id << " Result: " << rcv << ". Closing\n";
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
                        DoTransmit();
                    }
                }
            } while (rcv != NSAPI_ERROR_WOULD_BLOCK);
        }
    }

    void Connect()
    {
        rcvd = 0;
        xmtd = 0;
        sock.set_blocking(false);
        socket_open = true;
        SocketSignalHandler();
    }

    void Start()
    {
        sock.sigio(mbed::Callback<void()>(this, &Client::Kicker));
        while (1)
        {
            flags.wait_any(1);
            SocketSignalHandler();
        }
    }

    void Kicker()
    {
        flags.set(1);
    }

    uint8_t xmit_data[200];
    uint8_t rcv_data[200];

    uint16_t rcvd;
    uint16_t xmtd;
    bool socket_open;
    Thread thread;
    EventFlags flags;
    TCPSocket sock;
    uint16_t id;

};

static const uint8_t CLIENT_COUNT = 5;
Client clients[CLIENT_COUNT];
EventFlags srv_flags;

void srv_raise()
{
    srv_flags.set(1);
}

int main() {
    dummy[0] = 1;
    uint8_t idx = 0;
    TCPSocket sock;
    TCPSocket* psock;
    std::cout << "Starting at: " << rtos::Kernel::get_ms_count() << "\n";
    net.connect();
    std::cout << "IP address is: " << net.get_ip_address() << "\n";

    Client* pclient;

    for (idx = 0; idx < CLIENT_COUNT; idx++)
    {
        clients[idx].id = idx;
        clients[idx].thread.start(mbed::Callback<void()>(&clients[idx], &Client::Start));
    }

    srv.open(&net);
    srv.bind(502);
    srv.set_blocking(false);
    srv.sigio(mbed::Callback<void()>(srv_raise));
    srv.listen(1);

    nsapi_error_t res;
    while (true)
    {
        srv_flags.wait_any(1);

        do
        {
            psock = &sock;
            pclient = nullptr;

            for (idx = 0; idx < CLIENT_COUNT; idx++)
            {
                if (!clients[idx].socket_open)
                {
                    pclient = &clients[idx];
                    psock = &pclient->sock;
                    break;
                }
            }

            res = srv.accept(psock);
            std::cout << rtos::Kernel::get_ms_count() << ": Accept attempt resulted in: " << res << "\n";
            if (NSAPI_ERROR_OK == res)
            {
                if (pclient)
                {
                    psock->set_blocking(false);
                    pclient->socket_open = true;
                    pclient->flags.set(1);
                }
                else
                {
                    psock->close();
                }
            }
        } while (res != NSAPI_ERROR_WOULD_BLOCK);
    }
}
