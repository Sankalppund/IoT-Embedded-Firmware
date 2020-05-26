################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../protocol/bluetooth/bt_mesh/src/bg_application_properties.c \
../protocol/bluetooth/bt_mesh/src/mesh_lib.c \
../protocol/bluetooth/bt_mesh/src/mesh_sensor.c \
../protocol/bluetooth/bt_mesh/src/mesh_serdeser.c 

OBJS += \
./protocol/bluetooth/bt_mesh/src/bg_application_properties.o \
./protocol/bluetooth/bt_mesh/src/mesh_lib.o \
./protocol/bluetooth/bt_mesh/src/mesh_sensor.o \
./protocol/bluetooth/bt_mesh/src/mesh_serdeser.o 

C_DEPS += \
./protocol/bluetooth/bt_mesh/src/bg_application_properties.d \
./protocol/bluetooth/bt_mesh/src/mesh_lib.d \
./protocol/bluetooth/bt_mesh/src/mesh_sensor.d \
./protocol/bluetooth/bt_mesh/src/mesh_serdeser.d 


# Each subdirectory must supply rules for building sources it contributes
protocol/bluetooth/bt_mesh/src/bg_application_properties.o: ../protocol/bluetooth/bt_mesh/src/bg_application_properties.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DMESH_LIB_NATIVE=1' '-D__HEAP_SIZE=0x1200' '-D__STACK_SIZE=0x1000' '-DHAL_CONFIG=1' '-DINCLUDE_LOGGING=1' '-DEFR32BG13P632F512GM48=1' -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\hardware\kit\common\drivers" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\CMSIS\Include" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\protocol\bluetooth\bt_mesh\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\hardware\kit\common\bsp" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\hardware\kit\EFR32BG13_BRD4104A\config" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emlib\src" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\protocol\bluetooth\bt_mesh\inc\soc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\protocol\bluetooth\bt_mesh\inc\common" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emlib\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\radio\rail_lib\common" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\protocol\bluetooth\bt_mesh\src" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\Device\SiliconLabs\EFR32BG13P\Source\GCC" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\bootloader\api" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\hardware\kit\common\halconfig" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\radio\rail_lib\chip\efr32\efr32xg1x" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\radio\rail_lib\protocol\ble" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\common\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\sleep\src" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\radio\rail_lib\protocol\ieee802154" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\halconfig\inc\hal-config" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\sleep\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\uartdrv\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\middleware\glib" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\middleware\glib\dmd" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\middleware\glib\glib" -O0 -fno-builtin -flto -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"protocol/bluetooth/bt_mesh/src/bg_application_properties.d" -MT"protocol/bluetooth/bt_mesh/src/bg_application_properties.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

protocol/bluetooth/bt_mesh/src/mesh_lib.o: ../protocol/bluetooth/bt_mesh/src/mesh_lib.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DMESH_LIB_NATIVE=1' '-D__HEAP_SIZE=0x1200' '-D__STACK_SIZE=0x1000' '-DHAL_CONFIG=1' '-DINCLUDE_LOGGING=1' '-DEFR32BG13P632F512GM48=1' -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\hardware\kit\common\drivers" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\CMSIS\Include" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\protocol\bluetooth\bt_mesh\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\hardware\kit\common\bsp" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\hardware\kit\EFR32BG13_BRD4104A\config" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emlib\src" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\protocol\bluetooth\bt_mesh\inc\soc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\protocol\bluetooth\bt_mesh\inc\common" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emlib\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\radio\rail_lib\common" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\protocol\bluetooth\bt_mesh\src" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\Device\SiliconLabs\EFR32BG13P\Source\GCC" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\bootloader\api" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\hardware\kit\common\halconfig" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\radio\rail_lib\chip\efr32\efr32xg1x" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\radio\rail_lib\protocol\ble" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\common\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\sleep\src" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\radio\rail_lib\protocol\ieee802154" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\halconfig\inc\hal-config" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\sleep\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\uartdrv\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\middleware\glib" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\middleware\glib\dmd" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\middleware\glib\glib" -O0 -fno-builtin -flto -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"protocol/bluetooth/bt_mesh/src/mesh_lib.d" -MT"protocol/bluetooth/bt_mesh/src/mesh_lib.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

protocol/bluetooth/bt_mesh/src/mesh_sensor.o: ../protocol/bluetooth/bt_mesh/src/mesh_sensor.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DMESH_LIB_NATIVE=1' '-D__HEAP_SIZE=0x1200' '-D__STACK_SIZE=0x1000' '-DHAL_CONFIG=1' '-DINCLUDE_LOGGING=1' '-DEFR32BG13P632F512GM48=1' -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\hardware\kit\common\drivers" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\CMSIS\Include" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\protocol\bluetooth\bt_mesh\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\hardware\kit\common\bsp" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\hardware\kit\EFR32BG13_BRD4104A\config" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emlib\src" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\protocol\bluetooth\bt_mesh\inc\soc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\protocol\bluetooth\bt_mesh\inc\common" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emlib\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\radio\rail_lib\common" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\protocol\bluetooth\bt_mesh\src" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\Device\SiliconLabs\EFR32BG13P\Source\GCC" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\bootloader\api" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\hardware\kit\common\halconfig" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\radio\rail_lib\chip\efr32\efr32xg1x" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\radio\rail_lib\protocol\ble" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\common\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\sleep\src" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\radio\rail_lib\protocol\ieee802154" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\halconfig\inc\hal-config" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\sleep\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\uartdrv\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\middleware\glib" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\middleware\glib\dmd" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\middleware\glib\glib" -O0 -fno-builtin -flto -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"protocol/bluetooth/bt_mesh/src/mesh_sensor.d" -MT"protocol/bluetooth/bt_mesh/src/mesh_sensor.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

protocol/bluetooth/bt_mesh/src/mesh_serdeser.o: ../protocol/bluetooth/bt_mesh/src/mesh_serdeser.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DMESH_LIB_NATIVE=1' '-D__HEAP_SIZE=0x1200' '-D__STACK_SIZE=0x1000' '-DHAL_CONFIG=1' '-DINCLUDE_LOGGING=1' '-DEFR32BG13P632F512GM48=1' -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\hardware\kit\common\drivers" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\CMSIS\Include" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\protocol\bluetooth\bt_mesh\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\hardware\kit\common\bsp" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\hardware\kit\EFR32BG13_BRD4104A\config" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emlib\src" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\protocol\bluetooth\bt_mesh\inc\soc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\protocol\bluetooth\bt_mesh\inc\common" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emlib\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\radio\rail_lib\common" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\protocol\bluetooth\bt_mesh\src" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\Device\SiliconLabs\EFR32BG13P\Source\GCC" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\bootloader\api" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\hardware\kit\common\halconfig" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\radio\rail_lib\chip\efr32\efr32xg1x" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\radio\rail_lib\protocol\ble" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\common\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\sleep\src" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\radio\rail_lib\protocol\ieee802154" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\halconfig\inc\hal-config" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\sleep\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\emdrv\uartdrv\inc" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\middleware\glib" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\middleware\glib\dmd" -I"C:\Users\sanka\SimplicityStudio\v4_workspace\course-project-Sankalppund\platform\middleware\glib\glib" -O0 -fno-builtin -flto -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"protocol/bluetooth/bt_mesh/src/mesh_serdeser.d" -MT"protocol/bluetooth/bt_mesh/src/mesh_serdeser.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


