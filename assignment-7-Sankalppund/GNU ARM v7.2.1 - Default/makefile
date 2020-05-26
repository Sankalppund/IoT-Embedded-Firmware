################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include protocol/bluetooth/lib/EFR32BG13P/GCC/subdir.mk
-include platform/middleware/glib/glib/subdir.mk
-include platform/middleware/glib/dmd/display/subdir.mk
-include platform/emlib/src/subdir.mk
-include platform/emdrv/sleep/src/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG13P/Source/subdir.mk
-include hardware/kit/common/drivers/subdir.mk
-include hardware/kit/common/bsp/subdir.mk
-include app/bluetooth/common/util/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Logging/src/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Logging/platform/middleware/glib/glib/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Logging/platform/middleware/glib/dmd/display/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Logging/platform/emlib/src/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Logging/platform/emdrv/sleep/src/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Logging/platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Logging/platform/Device/SiliconLabs/EFR32BG13P/Source/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Logging/hardware/kit/common/drivers/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Logging/hardware/kit/common/bsp/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Logging/app/bluetooth/common/util/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Logging/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default/src/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default/platform/middleware/glib/glib/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default/platform/middleware/glib/dmd/display/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default/platform/emlib/src/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default/platform/emdrv/sleep/src/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default/platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default/platform/Device/SiliconLabs/EFR32BG13P/Source/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default/hardware/kit/common/drivers/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default/hardware/kit/common/bsp/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default/app/bluetooth/common/util/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default\ -\ EFR32BG13P632F512GM48/src/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default\ -\ EFR32BG13P632F512GM48/platform/middleware/glib/glib/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default\ -\ EFR32BG13P632F512GM48/platform/middleware/glib/dmd/display/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default\ -\ EFR32BG13P632F512GM48/platform/emlib/src/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default\ -\ EFR32BG13P632F512GM48/platform/emdrv/sleep/src/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default\ -\ EFR32BG13P632F512GM48/platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default\ -\ EFR32BG13P632F512GM48/platform/Device/SiliconLabs/EFR32BG13P/Source/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default\ -\ EFR32BG13P632F512GM48/hardware/kit/common/drivers/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default\ -\ EFR32BG13P632F512GM48/hardware/kit/common/bsp/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default\ -\ EFR32BG13P632F512GM48/app/bluetooth/common/util/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default\ -\ EFR32BG13P632F512GM48/subdir.mk
-include GNU\ ARM\ v4.9.3\ -\ Default/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: assignment-2-Sankalppund_2_2.axf

# Tool invocations
assignment-2-Sankalppund_2_2.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\efr32bg13p632f512gm48.ld" -Xlinker --gc-sections -Xlinker -Map="assignment-2-Sankalppund_2_2.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o assignment-2-Sankalppund_2_2.axf -Wl,--start-group "./app/bluetooth/common/util/infrastructure.o" "./app.o" "./application_properties.o" "./gatt_db.o" "./gecko_main.o" "./init_app.o" "./init_board.o" "./init_mcu.o" "./pti.o" "./hardware/kit/common/bsp/bsp_stk.o" "./hardware/kit/common/drivers/display.o" "./hardware/kit/common/drivers/displayls013b7dh03.o" "./hardware/kit/common/drivers/displaypalemlib.o" "./hardware/kit/common/drivers/i2cspm.o" "./hardware/kit/common/drivers/mx25flash_spi.o" "./hardware/kit/common/drivers/retargetio.o" "./hardware/kit/common/drivers/retargetserial.o" "./hardware/kit/common/drivers/udelay.o" "./platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/startup_efr32bg13p.o" "./platform/Device/SiliconLabs/EFR32BG13P/Source/system_efr32bg13p.o" "./platform/emdrv/sleep/src/sleep.o" "./platform/emlib/src/em_assert.o" "./platform/emlib/src/em_burtc.o" "./platform/emlib/src/em_cmu.o" "./platform/emlib/src/em_core.o" "./platform/emlib/src/em_cryotimer.o" "./platform/emlib/src/em_crypto.o" "./platform/emlib/src/em_emu.o" "./platform/emlib/src/em_eusart.o" "./platform/emlib/src/em_gpio.o" "./platform/emlib/src/em_i2c.o" "./platform/emlib/src/em_letimer.o" "./platform/emlib/src/em_msc.o" "./platform/emlib/src/em_rmu.o" "./platform/emlib/src/em_rtcc.o" "./platform/emlib/src/em_se.o" "./platform/emlib/src/em_system.o" "./platform/emlib/src/em_timer.o" "./platform/emlib/src/em_usart.o" "./platform/middleware/glib/dmd/display/dmd_display.o" "./platform/middleware/glib/glib/bmp.o" "./platform/middleware/glib/glib/glib.o" "./platform/middleware/glib/glib/glib_bitmap.o" "./platform/middleware/glib/glib/glib_circle.o" "./platform/middleware/glib/glib/glib_font_narrow_6x8.o" "./platform/middleware/glib/glib/glib_font_normal_8x8.o" "./platform/middleware/glib/glib/glib_font_number_16x20.o" "./platform/middleware/glib/glib/glib_line.o" "./platform/middleware/glib/glib/glib_polygon.o" "./platform/middleware/glib/glib/glib_rectangle.o" "./platform/middleware/glib/glib/glib_string.o" "./src/Letimer0.o" "./src/display.o" "./src/gecko_ble_errors.o" "./src/gpio.o" "./src/i2c.o" "./src/i2c_interrupt.o" "./src/log.o" "./src/main.o" "./src/state_machine.o" "C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\platform\radio\rail_lib\autogen\librail_release\librail_efr32xg13_gcc_release.a" "C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\protocol\bluetooth\lib\EFR32BG13P\GCC\libbluetooth.a" "C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\protocol\bluetooth\lib\EFR32BG13P\GCC\binapploader.o" "C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\protocol\bluetooth\lib\EFR32BG13P\GCC\libpsstore.a" "C:\Users\sanka\SimplicityStudio\v4_workspace\assignment-2-Sankalppund_2_2\protocol\bluetooth\lib\EFR32BG13P\GCC\libmbedtls.a" -lm -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: assignment-2-Sankalppund_2_2.hex'
	arm-none-eabi-objcopy -O ihex "assignment-2-Sankalppund_2_2.axf" "assignment-2-Sankalppund_2_2.hex"
	@echo ' '

	@echo 'Building bin file: assignment-2-Sankalppund_2_2.bin'
	arm-none-eabi-objcopy -O binary "assignment-2-Sankalppund_2_2.axf" "assignment-2-Sankalppund_2_2.bin"
	@echo ' '

	@echo 'Building s37 file: assignment-2-Sankalppund_2_2.s37'
	arm-none-eabi-objcopy -O srec "assignment-2-Sankalppund_2_2.axf" "assignment-2-Sankalppund_2_2.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "assignment-2-Sankalppund_2_2.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) assignment-2-Sankalppund_2_2.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
