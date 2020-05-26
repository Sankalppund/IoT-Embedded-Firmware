################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../platform/emdrv/sleep/src/sleep.c 

OBJS += \
./platform/emdrv/sleep/src/sleep.o 

C_DEPS += \
./platform/emdrv/sleep/src/sleep.d 


# Each subdirectory must supply rules for building sources it contributes
platform/emdrv/sleep/src/sleep.o: ../platform/emdrv/sleep/src/sleep.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__StackLimit=0x20000000' '-DHAL_CONFIG=1' '-D__STACK_SIZE=0x800' '-D__HEAP_SIZE=0xD00' '-DEFR32BG13P632F512GM48=1' -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund" -I"E:\Simplicity Studio\developer\toolchains\gnu_arm\4.9_2015q3\lib\gcc\arm-none-eabi\4.9.3\include" -I"E:\Simplicity Studio\developer\toolchains\gnu_arm\4.9_2015q3\lib\gcc\arm-none-eabi\4.9.3\include" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\middleware\glib" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\middleware\glib\glib" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\middleware\glib\dmd" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\CMSIS\Include" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\emlib\inc" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\hardware\kit\common\drivers" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\radio\rail_lib\protocol\ieee802154" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\emlib\src" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\hardware\kit\EFR32BG13_BRD4104A\config" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\protocol\bluetooth\ble_stack\inc\common" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\emdrv\uartdrv\inc" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\app\bluetooth\common\util" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\emdrv\common\inc" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\radio\rail_lib\chip\efr32\efr32xg1x" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\bootloader\api" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\radio\rail_lib\common" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\protocol\bluetooth\ble_stack\inc\soc" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\hardware\kit\common\bsp" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\emdrv\sleep\src" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\hardware\kit\common\halconfig" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\Device\SiliconLabs\EFR32BG13P\Source\GCC" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\emdrv\gpiointerrupt\inc" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\halconfig\inc\hal-config" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\radio\rail_lib\protocol\ble" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\emdrv\sleep\inc" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"E:\IoT_Git_Hub_Repository\assignment-2-Sankalppund\platform\bootloader" -O2 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"platform/emdrv/sleep/src/sleep.d" -MT"platform/emdrv/sleep/src/sleep.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


