@echo off
:: Cambia al directorio del primer programa
cd /d "CursedV1 (Arg Version)\Nucleo"
start "Nucleo" "Nucleo.exe"

:: Cambia al directorio del segundo programa
cd /d "CursedV1 (Arg Version)\MapServer"
start "MapServer" "MapServer.exe"

:: Cambia al directorio del tercer programa
cd /d "CursedV1 (Arg Version)\MapServer"
start "ICMPServer" "ICMPServer.exe"

:: Fin del archivo BAT
:: exit