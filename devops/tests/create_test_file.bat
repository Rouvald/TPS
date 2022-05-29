@echo off

call "%~dp0\..\config.bat"

:begin
set /p testClassName= "Enter test class name :"
if [%testClassName%]==[] goto:begin
set /p testRelativePath= "Enter relative to [Source\%testRelativePath%] directory (use \ symbol for subdirs) :"

rem .h / .cpp file names
set testCppFileName=%testClassName%.cpp
set testHeaderFileName=%testClassName%.h

rem full paths to .h / .cpp files to create
set testAbsoluteDir=%sourceCodePath%\%projectPureName%\%testRelativePath%
if [%testRelativePath%]==[] set testAbsoluteDir=%sourceCodePath%\%projectPureName%
set testCppFilePath=%testAbsoluteDir%\%testCppFileName%
set testHeaderFilePath=%testAbsoluteDir%\%testHeaderFileName%

rem Configuration
echo.
echo ========== Files to be created: ==========
echo %testCppFilePath%
echo %testHeaderFilePath%
echo ==========================================
echo.
set /p userConfirmed= "Confirm? [Y/N or (E)xit] :"
if %userConfirmed% == N goto:begin
if %userConfirmed% == n goto:begin
if %userConfirmed% == E goto:eof
if %userConfirmed% == e goto:eof

rem create dir
if not exist "%testAbsoluteDir%" mkdir "%testAbsoluteDir%"

rem full paths .h / .cpp templates files
set testCppTemplateFilePath=%projectRoot%\devops\tests\templates\Test.cpp.template
set testHeaderTemplateFilePath=%projectRoot%\devops\tests\templates\Test.h.template

rem template file vars
rem path with \
set tempPath=%projectPureName%\%testRelativePath%\%testClassName%.h
rem replace \ with / for include string
set TEXT_INCLUDE_FILE="%tempPath:\=/%"
set "OR=^|"
set "AND=^&"

rem remove old files if exist
del /q "%testCppFilePath%"
del /q "%testHeaderFilePath%"

rem create actual files
call :createTemplate "%testCppTemplateFilePath%" , "%testCppFilePath%"
call :createTemplate "%testHeaderTemplateFilePath%" , "%testHeaderFilePath%"

rem clang-format
call "%~dp0\..\misc\format_all_files.bat"

::rem generate project files
::call "%~dp0generate_project_files_4.27.bat"

echo %TEXT_INCLUDE_FILE_1%
goto:eof

rem function to create .h / .cpp from template
:createTemplate
set templateName=%~1
set fileToWriteIn=%~2
for /f "usebackq tokens=*" %%a in ("%templateName%") do (
    if %%a == NEW_LINE (
        echo.>>"%fileToWriteIn%"
    ) else (
        call echo %%a>>"%fileToWriteIn%"
    )
)
