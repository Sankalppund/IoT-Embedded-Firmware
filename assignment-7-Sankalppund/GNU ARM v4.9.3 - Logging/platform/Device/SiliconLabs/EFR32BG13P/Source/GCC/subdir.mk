################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/startup_efr32bg13p.c 

OBJS += \
./platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/startup_efr32bg13p.o 

C_DEPS += \
./platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/startup_efr32bg13p.d 


# Each subdirectory must supply rules for building sources it contributes
platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/startup_efr32bg13p.o: ../platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/startup_efr32bg13p.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__StackLimit=0x20000000' '-DINCLUDE_LOGGING=1' '-DHAL_CONFIG=1' '-D__STACK_SIZE=0x800' '-D__HEAP_SIZE=0xD00' '-DEFR32BG13P632F512GM48=1' -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\middleware\glib" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\middleware\glib\glib" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\middleware\glib\dmd" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\CMSIS\Include" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\emlib\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\hardware\kit\common\drivers" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\radio\rail_lib\protocol\ieee802154" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\emlib\src" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\hardware\kit\EFR32BG13_BRD4104A\config" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\protocol\bluetooth\ble_stack\inc\common" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\emdrv\uartdrv\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\app\bluetooth\common\util" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\emdrv\common\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\radio\rail_lib\chip\efr32\efr32xg1x" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\bootloader\api" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\radio\rail_lib\common" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\protocol\bluetooth\ble_stack\inc\soc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\hardware\kit\common\bsp" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\emdrv\sleep\src" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\hardware\kit\common\halconfig" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\Device\SiliconLabs\EFR32BG13P\Source\GCC" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\halconfig\inc\hal-config" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\radio\rail_lib\protocol\ble" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\emdrv\sleep\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\bootloader" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/startup_efr32bg13p.d" -MT"platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/startup_efr32bg13p.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


