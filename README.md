# SimpleDPS
Opensource firmware for the DPS3005 and other


Inspired by:
 https://github.com/kanflo/opendps
 https://johan.kanflo.com/hacking-the-dps5005/
 


Unlock-STM32 Flash:

$ openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg &

$ telnet localhost 4444

 reset halt
 flash banks
 stm32f1x mass_erase 0
 stm32f1x unlock 0






