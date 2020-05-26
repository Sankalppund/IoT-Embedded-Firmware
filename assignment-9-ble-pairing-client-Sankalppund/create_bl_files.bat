@echo off

:: THIS FILE IS GENERATED BY APPBUILDER, DO NOT MODIFY
:: CONFIGURE VIA ENV VARS

:: use PATH_GCCARM env var to override default path for gcc-arm
if "%PATH_GCCARM%"=="" (
  set OBJCOPY="E:\Simplicity Studio\developer\toolchains\gnu_arm\4.9_2015q3\bin\arm-none-eabi-objcopy.exe"
) else (
  set OBJCOPY=%PATH_GCCARM%\bin\arm-none-eabi-objcopy.exe
)

:: use PATH_SCMD env var to override default path for Simplicity Commander
if "%PATH_SCMD%"=="" (
  set COMMANDER="E:\Simplicity Studio\developer\adapter_packs\commander\commander.exe"
) else (
  set COMMANDER=%PATH_SCMD%\commander.exe
)

:: use PATH_OUT env var to override the full path for the .out file

:: default file extension of GCC and IAR
set FILE_EXTENSION_GCC="*.axf"
set FILE_EXTENSION_IAR="*.out"

:: output path of the OTA and UART DFU gbl files (relative to project root)
set PATH_GBL=output_gbl

:: names of the OTA and UART DFU output files
set OTA_APPLO_NAME=apploader
set OTA_APPLI_NAME=application
set UARTDFU_FULL_NAME=full

:: names of the sign and encypt key files
set GBL_SIGING_KEY_FILE=app-sign-key.pem
set GBL_ENCRYPT_KEY_FILE=app-encrypt-key.txt

:: change the working dir to the dir of the batch file, which should be in the project root
cd %~dp0

for /f "delims=" %%i in ('dir *.axf *.out /b/s/od') do set PATH_OUT=%%i
if "%PATH_OUT%"=="" (
  echo "Error: neither %FILE_EXTENSION_GCC% nor %FILE_EXTENSION_IAR% found"
  echo Was the project compiled and linked successfully?
  pause
  goto:eof
)

if not exist "%OBJCOPY%" (
  echo Error: gcc-arm objcopy not found at '%OBJCOPY%'
  echo Use PATH_GCCARM env var to override default path for gcc-arm.
  pause
  goto:eof
)

if not exist "%COMMANDER%" (
  echo Error: Simplicity Commander not found at '%COMMANDER%'
  echo Use PATH_SCMD env var to override default path for Simplicity Commander.
  pause
  goto:eof
)

echo **********************************************************************
echo Converting .out to .gbl files
echo **********************************************************************
echo.
echo .out file used:
echo %PATH_OUT%
echo.
echo output folders:
echo %~dp0%PATH_GBL%

if not exist %PATH_GBL% (
  mkdir %PATH_GBL%
)

:: create the GBL files
echo.
echo **********************************************************************
echo Creating %OTA_APPLO_NAME%.gbl for OTA
echo **********************************************************************
echo.
%OBJCOPY% -O srec -j .text_apploader* "%PATH_OUT%" "%PATH_GBL%\%OTA_APPLO_NAME%.srec"
if errorlevel 1 (
  pause
  goto:eof
)
%COMMANDER% gbl create "%PATH_GBL%\%OTA_APPLO_NAME%.gbl" --app "%PATH_GBL%\%OTA_APPLO_NAME%.srec"

echo.
echo **********************************************************************
echo Creating %OTA_APPLI_NAME%.gbl for OTA
echo **********************************************************************
echo.
%OBJCOPY% -O srec -j .text_application* "%PATH_OUT%" "%PATH_GBL%\%OTA_APPLI_NAME%.srec"
if errorlevel 1 (
  pause
  goto:eof
)
%COMMANDER% gbl create "%PATH_GBL%\%OTA_APPLI_NAME%.gbl" --app "%PATH_GBL%\%OTA_APPLI_NAME%.srec"

:: create the full GBL files for UART DFU
echo.
echo **********************************************************************
echo Creating %UARTDFU_FULL_NAME%.gbl for UART DFU
echo **********************************************************************
echo.
%OBJCOPY% -O srec -R .text_bootloader* "%PATH_OUT%" "%PATH_GBL%\%UARTDFU_FULL_NAME%.srec"
if errorlevel 1 (
  pause
  goto:eof
)
%COMMANDER% gbl create "%PATH_GBL%\%UARTDFU_FULL_NAME%.gbl" --app "%PATH_GBL%\%UARTDFU_FULL_NAME%.srec"

:: create encrypted GBL file for secure boot if encrypt-key file exist
if exist %GBL_ENCRYPT_KEY_FILE% (
  echo.
  echo **********************************************************************
  echo Creating encrypted .gbl files
  echo **********************************************************************
  echo.
  %COMMANDER% gbl create "%PATH_GBL%\%OTA_APPLO_NAME%-encrypted.gbl" --app "%PATH_GBL%\%OTA_APPLO_NAME%.srec" --encrypt %GBL_ENCRYPT_KEY_FILE%
  echo.
  %COMMANDER% gbl create "%PATH_GBL%\%OTA_APPLI_NAME%-encrypted.gbl" --app "%PATH_GBL%\%OTA_APPLI_NAME%.srec" --encrypt %GBL_ENCRYPT_KEY_FILE%
  echo.
  %COMMANDER% gbl create "%PATH_GBL%\%UARTDFU_FULL_NAME%-encrypted.gbl" --app "%PATH_GBL%\%UARTDFU_FULL_NAME%.srec" --encrypt %GBL_ENCRYPT_KEY_FILE%
)

:: create signed GBL file for secure boot if sign-key file exists
if exist %GBL_SIGING_KEY_FILE% (
  echo.
  echo **********************************************************************
  echo Creating signed .gbl files
  echo **********************************************************************
  echo.
  %COMMANDER% convert "%PATH_GBL%\%OTA_APPLO_NAME%.srec" --secureboot --keyfile %GBL_SIGING_KEY_FILE% -o "%PATH_GBL%\%OTA_APPLO_NAME%-signed.srec"
  if exist "%PATH_GBL%\%OTA_APPLO_NAME%-signed.srec" (
    %COMMANDER% gbl create "%PATH_GBL%\%OTA_APPLO_NAME%-signed.gbl" --app "%PATH_GBL%\%OTA_APPLO_NAME%-signed.srec" --sign %GBL_SIGING_KEY_FILE%
  )
  echo.
  %COMMANDER% convert "%PATH_GBL%\%OTA_APPLI_NAME%.srec" --secureboot --keyfile %GBL_SIGING_KEY_FILE% -o "%PATH_GBL%\%OTA_APPLI_NAME%-signed.srec"
  %COMMANDER% gbl create "%PATH_GBL%\%OTA_APPLI_NAME%-signed.gbl" --app "%PATH_GBL%\%OTA_APPLI_NAME%-signed.srec" --sign %GBL_SIGING_KEY_FILE%
  echo.
  if exist "%PATH_GBL%\%OTA_APPLO_NAME%-signed.srec" (
    %COMMANDER% convert "%PATH_GBL%\%OTA_APPLO_NAME%-signed.srec" "%PATH_GBL%\%OTA_APPLI_NAME%-signed.srec" -o "%PATH_GBL%\%UARTDFU_FULL_NAME%-signed.srec"
  ) else (
    copy "%PATH_GBL%\%OTA_APPLI_NAME%-signed.srec" "%PATH_GBL%\%UARTDFU_FULL_NAME%-signed.srec" >NUL
  )
  %COMMANDER% gbl create "%PATH_GBL%\%UARTDFU_FULL_NAME%-signed.gbl" --app "%PATH_GBL%\%UARTDFU_FULL_NAME%-signed.srec" --sign %GBL_SIGING_KEY_FILE%
  
  :: create signed and encrypted GBL file for if both sign-key and encrypt-key file exist
  if exist %GBL_ENCRYPT_KEY_FILE% (
    echo.
    echo **********************************************************************
    echo Creating signed and encrypted .gbl files
    echo **********************************************************************
    echo.
    if exist "%PATH_GBL%\%OTA_APPLO_NAME%-signed.srec" (
      %COMMANDER% gbl create "%PATH_GBL%\%OTA_APPLO_NAME%-signed-encrypted.gbl" --app "%PATH_GBL%\%OTA_APPLO_NAME%-signed.srec" --encrypt %GBL_ENCRYPT_KEY_FILE% --sign %GBL_SIGING_KEY_FILE%
      echo.
    )
    %COMMANDER% gbl create "%PATH_GBL%\%OTA_APPLI_NAME%-signed-encrypted.gbl" --app "%PATH_GBL%\%OTA_APPLI_NAME%-signed.srec" --encrypt %GBL_ENCRYPT_KEY_FILE% --sign %GBL_SIGING_KEY_FILE%
    echo.
    %COMMANDER% gbl create "%PATH_GBL%\%UARTDFU_FULL_NAME%-signed-encrypted.gbl" --app "%PATH_GBL%\%UARTDFU_FULL_NAME%-signed.srec" --encrypt %GBL_ENCRYPT_KEY_FILE% --sign %GBL_SIGING_KEY_FILE%
  )
)

:: clean up output dir
del "%PATH_GBL%\*.srec"

pause