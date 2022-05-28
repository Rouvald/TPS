@echo off

call "%~dp0\..\config.bat"

FOR /R "%sourceCodePath%" %%f IN (*.cpp, *.h, *.cs) DO (
    clang-format -i "%%f"
    echo %%f
)