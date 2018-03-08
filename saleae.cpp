
#include "mbed.h"

#include "DigitalOut.h"
#include "Serial.h"

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
void proxy_D0( const int set) { out_D0  = !!set; }
void proxy_D1( const int set) { out_D1  = !!set; }
void proxy_D2( const int set) { out_D2  = !!set; }
void proxy_D3( const int set) { out_D3  = !!set; }
void proxy_D4( const int set) { out_D4  = !!set; }
void proxy_D5( const int set) { out_D5  = !!set; }
void proxy_D6( const int set) { out_D6  = !!set; }
void proxy_D7( const int set) { out_D7  = !!set; }
void proxy_D8( const int set) { out_D8  = !!set; }
void proxy_D9( const int set) { out_D9  = !!set; }
void proxy_D10(const int set) { out_D10 = !!set; }
void proxy_D11(const int set) { out_D11 = !!set; }
void proxy_D12(const int set) { out_D12 = !!set; }
void proxy_D13(const int set) { out_D13 = !!set; }

void pulse_D0()  { out_D0  = true; __NOP(); out_D0  = false; }
void pulse_D1()  { out_D1  = true; __NOP(); out_D1  = false; }
void pulse_D2()  { out_D2  = true; __NOP(); out_D2  = false; }
void pulse_D3()  { out_D3  = true; __NOP(); out_D3  = false; }
void pulse_D4()  { out_D4  = true; __NOP(); out_D4  = false; }
void pulse_D5()  { out_D5  = true; __NOP(); out_D5  = false; }
void pulse_D6()  { out_D6  = true; __NOP(); out_D6  = false; }
void pulse_D7()  { out_D7  = true; __NOP(); out_D7  = false; }
void pulse_D8()  { out_D8  = true; __NOP(); out_D8  = false; }
void pulse_D9()  { out_D9  = true; __NOP(); out_D9  = false; }
void pulse_D10() { out_D10 = true; __NOP(); out_D10 = false; }
void pulse_D11() { out_D11 = true; __NOP(); out_D11 = false; }
void pulse_D12() { out_D12 = true; __NOP(); out_D12 = false; }
void pulse_D13() { out_D13 = true; __NOP(); out_D13 = false; }

void write_u32(const uint32_t value) { saleae.printf("0x%08X", value); }
}

