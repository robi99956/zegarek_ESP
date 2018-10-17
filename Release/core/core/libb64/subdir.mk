################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\cores\esp8266\libb64\cdecode.c \
C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\cores\esp8266\libb64\cencode.c 

C_DEPS += \
.\core\core\libb64\cdecode.c.d \
.\core\core\libb64\cencode.c.d 

AR_OBJ += \
.\core\core\libb64\cdecode.c.o \
.\core\core\libb64\cencode.c.o 


# Each subdirectory must supply rules for building sources it contributes
core\core\libb64\cdecode.c.o: C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\cores\esp8266\libb64\cdecode.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\tools\xtensa-lx106-elf-gcc\1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-gcc" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-IC:\eclipse\eclipse_ESP8266\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0/tools/sdk/include" "-IC:\eclipse\eclipse_ESP8266\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0/tools/sdk/lwip2/include" "-IC:\eclipse\eclipse_ESP8266\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0/tools/sdk/libc/xtensa-lx106-elf/include" "-IX:/ESP8266/zegar/Release/core" -c -Wall -Wextra -Os -g -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -falign-functions=4 -MMD -std=gnu99 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC -DTCP_MSS=536   -DARDUINO=10802 -DARDUINO_ESP8266_WEMOS_D1MINI -DARDUINO_ARCH_ESP8266 -DARDUINO_BOARD="ESP8266_WEMOS_D1MINI"  -DESP8266   -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\cores\esp8266" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\variants\d1" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266HTTPClient" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266HTTPClient\src" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WebServer" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WebServer\src" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WiFi" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WiFi\src" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\SPI" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core\core\libb64\cencode.c.o: C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\cores\esp8266\libb64\cencode.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\tools\xtensa-lx106-elf-gcc\1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-gcc" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-IC:\eclipse\eclipse_ESP8266\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0/tools/sdk/include" "-IC:\eclipse\eclipse_ESP8266\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0/tools/sdk/lwip2/include" "-IC:\eclipse\eclipse_ESP8266\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0/tools/sdk/libc/xtensa-lx106-elf/include" "-IX:/ESP8266/zegar/Release/core" -c -Wall -Wextra -Os -g -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -falign-functions=4 -MMD -std=gnu99 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC -DTCP_MSS=536   -DARDUINO=10802 -DARDUINO_ESP8266_WEMOS_D1MINI -DARDUINO_ARCH_ESP8266 -DARDUINO_BOARD="ESP8266_WEMOS_D1MINI"  -DESP8266   -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\cores\esp8266" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\variants\d1" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266HTTPClient" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266HTTPClient\src" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WebServer" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WebServer\src" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WiFi" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WiFi\src" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\SPI" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


