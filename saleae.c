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

uint8_t xmit_data[200];
uint8_t rcv_data[200];

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

extern "C" {
inline void proxy_D0( const bool set) {out_D0  = set; }
inline void proxy_D1( const bool set) {out_D1  = set; }
inline void proxy_D2( const bool set) {out_D2  = set; }
inline void proxy_D3( const bool set) {out_D3  = set; }
inline void proxy_D4( const bool set) {out_D4  = set; }
inline void proxy_D5( const bool set) {out_D5  = set; }
inline void proxy_D6( const bool set) {out_D6  = set; }
inline void proxy_D7( const bool set) {out_D7  = set; }
inline void proxy_D8( const bool set) {out_D8  = set; }
inline void proxy_D9( const bool set) {out_D9  = set; }
inline void proxy_D10(const bool set) {out_D10 = set; }
inline void proxy_D11(const bool set) {out_D11 = set; }
inline void proxy_D12(const bool set) {out_D12 = set; }
inline void proxy_D13(const bool set) {out_D13 = set; }
}

