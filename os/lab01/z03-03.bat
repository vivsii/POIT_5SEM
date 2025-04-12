@echo off
if "%1"=="" (
    echo Ошибка: введите расширение файла.
    echo Пример использования: z03-03.bat txt
    pause
    exit /b
)
set "ext=%1"
if not exist "%ext%" (
    mkdir "%ext%"
)

move *.%ext% "%ext%"
