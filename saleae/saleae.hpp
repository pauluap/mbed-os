#ifndef SALEAE_HPP
#define SALEAE_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include "saleae.h"

#ifdef __cplusplus
}
#endif

#include "DigitalOut.h"
#include "Serial.h"

extern mbed::DigitalOut out_D0;
extern mbed::DigitalOut out_D1;
extern mbed::DigitalOut out_D2;
extern mbed::DigitalOut out_D3;
extern mbed::DigitalOut out_D4;
extern mbed::DigitalOut out_D5;
extern mbed::DigitalOut out_D6;
extern mbed::DigitalOut out_D7;

extern mbed::DigitalOut out_D8;
extern mbed::DigitalOut out_D9;
extern mbed::DigitalOut out_D10;
extern mbed::DigitalOut out_D11;
extern mbed::DigitalOut out_D12;
extern mbed::DigitalOut out_D13;

extern mbed::Serial saleae;

#endif /* SALEAE_HPP */
