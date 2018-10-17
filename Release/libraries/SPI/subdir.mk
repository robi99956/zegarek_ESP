################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\SPI\SPI.cpp 

LINK_OBJ += \
.\libraries\SPI\SPI.cpp.o 

CPP_DEPS += \
.\libraries\SPI\SPI.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\SPI\SPI.cpp.o: C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\SPI\SPI.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\tools\xtensa-lx106-elf-gcc\1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-IC:\eclipse\eclipse_ESP8266\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0/tools/sdk/include" "-IC:\eclipse\eclipse_ESP8266\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0/tools/sdk/lwip2/include" "-IC:\eclipse\eclipse_ESP8266\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0/tools/sdk/libc/xtensa-lx106-elf/include" "-IX:/ESP8266/zegar/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -MMD -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC -DTCP_MSS=536   -DARDUINO=10802 -DARDUINO_ESP8266_WEMOS_D1MINI -DARDUINO_ARCH_ESP8266 -DARDUINO_BOARD="ESP8266_WEMOS_D1MINI"  -DESP8266   -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\cores\esp8266" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\variants\d1" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266HTTPClient" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266HTTPClient\src" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WebServer" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WebServer\src" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WiFi" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WiFi\src" -I"C:\eclipse\eclipse_ESP8266\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\SPI" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


