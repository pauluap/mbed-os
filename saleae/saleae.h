#ifndef SALEAE
#define SALEAE

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void proxy_D0( const int set);
void proxy_D1( const int set);
void proxy_D2( const int set);
void proxy_D3( const int set);
void proxy_D4( const int set);
void proxy_D5( const int set);
void proxy_D6( const int set);
void proxy_D7( const int set);
void proxy_D8( const int set);
void proxy_D9( const int set);
void proxy_D10(const int set);
void proxy_D11(const int set);
void proxy_D12(const int set);
void proxy_D13(const int set);

void pulse_D0();
void pulse_D1();
void pulse_D2();
void pulse_D3();
void pulse_D4();
void pulse_D5();
void pulse_D6();
void pulse_D7();
void pulse_D8();
void pulse_D9();
void pulse_D10();
void pulse_D11();
void pulse_D12();
void pulse_D13();

void dip_D0();
void dip_D1();
void dip_D2();
void dip_D3();
void dip_D4();
void dip_D5();
void dip_D6();
void dip_D7();
void dip_D8();
void dip_D9();
void dip_D10();
void dip_D11();
void dip_D12();
void dip_D13();

void write_u32(const uint32_t v);

#ifdef __cplusplus
}
#endif

#endif /* SALEAE */
