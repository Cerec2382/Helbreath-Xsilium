@echo off
set "BASE_DIR=%~dp0"

echo BASE_DIR: "%BASE_DIR%"
echo.

:: Primer programa
echo Cambiando a: "%BASE_DIR%Nucleo"
cd /d "%BASE_DIR%Nucleo"
echo Ejecutando: Nucleo.exe
start "" "Nucleo.exe"
echo.

:: Segundo programa
echo Cambiando a: "%BASE_DIR%MapServer"
cd /d "%BASE_DIR%MapServer"
echo Ejecutando: MapServer.exe
start "" "MapServer.exe"
echo.

:: Tercer programa
echo Cambiando a: "%BASE_DIR%MapServer"
cd /d "%BASE_DIR%MapServer"
echo Ejecutando: ICMPServer.exe
start "" "ICMPServer.exe"
echo.

pause
