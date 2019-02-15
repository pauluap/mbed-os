#include "mbed.h"
#include "stm32f769i_discovery_sdram.h"

static const uint32_t SDRAM_BASE{0xC0000000};
static const uint32_t SDRAM_SIZE{4 * 32 * (1 << 20)};

static bool initialized = false;

extern "C" caddr_t _sbrk(int incr) {
    static unsigned char* heap = (unsigned char*)SDRAM_BASE;
    unsigned char*        prev_heap = heap;
    unsigned char*        new_heap = heap + incr;

    if (!initialized) {
        BSP_SDRAM_Init();
        initialized = true;
    }
    if (new_heap >= (unsigned char*)(SDRAM_BASE + SDRAM_SIZE)) {
        return (caddr_t)-1;
    }

    heap = new_heap;
    return (caddr_t) prev_heap;
}
