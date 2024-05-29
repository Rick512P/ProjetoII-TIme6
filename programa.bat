@echo off

:menu
cls
echo Selecione uma opção:
echo 1. Rodar Programa
echo 2. Rodar Programa para Testes
echo 3. Limpar área
echo 4. Sair

set /p opcao=Opção:

if "%opcao%"=="1" goto rodar_programa
if "%opcao%"=="2" goto rodar_programa_testes
if "%opcao%"=="3" goto limpar_area
if "%opcao%"=="4" goto sair

echo Opção inválida!
pause
goto menu

:rodar_programa
gcc -o executavel Arquivos-c/*.c
cls
executavel.exe
pause
goto menu

:rodar_programa_testes
if exist executavel.exe (
    del executavel.exe
)
gcc -o executavel Arquivos-c/*.c -g
cls
executavel.exe
pause
goto menu

:limpar_area
if exist executavel.exe (
    del executavel.exe
)
cls
goto menu

:sair
if exist executavel.exe (
    del executavel.exe
)
cls
pause
