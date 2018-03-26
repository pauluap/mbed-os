
To aid in replication of the F769NI Ethernet issues on the Discovery board.
This branch provides several things to assist.

There are defines in `features/FEATURE_LWIP/lwip-interface/lwip-eth/arch/TARGET_STM/stm32xx_emac.c`
`F769_ETH_CONFIG_SRAM1_FAIL`, `F769_ETH_CONFIG_SRAM1_OK`, `F769_ETH_CONFIG_SRAM2`

The version of GCC_ARM used is
`arm-none-eabi-gcc (GNU Tools for ARM Embedded Processors 6-2017-q1-update) 6.3.1 20170215 (release) [ARM/embedded-6-branch revision 245512]`

While the various mbed_lib.json files have modified such that a generated `mbed_config.h` file will match, it is recommended that the `mbed_config.h` and `Makefile` provided in this branch be used because:
- Regeneration will create a different timestamp, no md5 match confirmation
- the Makefile has been modified to:
  - Move main.cpp and stm32xx_emac.c to the end of the link order to make the emac memory object location easier to manipulate
  - Build with the C++11 standard with -O3 optimizations
  - Generate a map file

The MD5 of the bin files built using the three test cases are:


```
F769_ETH_CONFIG_SRAM1_FAIL
                0x000000002001fff4                DMARxDscrTab
                0x0000000020020074                EthHandle
                0x00000000200200bc                Rx_Buff
                0x000000002002188c                Tx_Buff
                0x000000002002305c                DMATxDscrTab
7400951a73b32755d84ffe77c9d31662  BUILD/mbed-os.bin
```

```
F769_ETH_CONFIG_SRAM1_OK
                0x000000002002001c                DMARxDscrTab
                0x000000002002009c                EthHandle
                0x00000000200200e4                Rx_Buff
                0x00000000200218b4                Tx_Buff
                0x0000000020023084                DMATxDscrTab
fe736707a863c27ec9773844119ffd25  BUILD/mbed-os.bin
```

```
F769_ETH_CONFIG_SRAM2
                0x000000002007c000                DMARxDscrTab
                0x000000002007c080                DMATxDscrTab
                0x000000002007c100                Rx_Buff
                0x000000002007d8d0                Tx_Buff
270baf05b6aad3fc3b3708f050492dda  BUILD/mbed-os.bin
```

To generate ethernet pressure, the modpoll tool (http://www.modbusdriver.com/modpoll.html) is recommended.  The Windows version works, but the Linux version generates higher throughput.

The command is
`modpoll <ip-address> -r 1 -c 72 -t 4:hex -l 0`

Running four in parallel should cause a failure to occur within five minutes, although I have seen a single client eventually fail.

The expected failure result is deafness, the system stops responding to Ethernet traffic, but will periodically send out ARP queries.

If Wireshark is used to monitor, the failure can be captured as well as the eventual deafness. An example failure result is:
![image](https://user-images.githubusercontent.com/573966/37927913-14e799be-30f0-11e8-8d02-a0586cfd24ae.png)
![image](https://user-images.githubusercontent.com/573966/37927976-3e44322c-30f0-11e8-952e-6980b1d1a60b.png)



For those with logic analyzers, the pin numbers in the source is in accordance to how I have connected my analyzer, not the board pins.  Sorry.  The mapping is mirrored - D15 <-> D0, D14 <-> D1, ... D0 <-> D15.

A discernible difference between the failing and non-failing cases is the behavior of the `RxBufUnavailable` signal.  In the passing cases, the signal is never asserted whereas in the failing cases, it is asserted on the very first response and then is continually asserted until failure occurs.
An example of the failing case is:
![image](https://user-images.githubusercontent.com/573966/37927780-dadf3358-30ef-11e8-8880-6517e5ae550a.png)
And the point of actual failure:
![image](https://user-images.githubusercontent.com/573966/37927864-fd8284b4-30ef-11e8-960a-6513f7ea4181.png)

Whereas the non-failing case looks like this:
![image](https://user-images.githubusercontent.com/573966/37927170-31df4852-30ee-11e8-9b65-abf2e07c7517.png)


