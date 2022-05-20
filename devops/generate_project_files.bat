@echo off

set enginePath=%~1
set UBTRelativePath=%~2
set versionSelector=%~3
set projectPath=%CD%\%~4

:: switch engine version
::"%versionSelector%" -switchversionsilent "%projectPath%" "%enginePath%"

:: generate project files
"%enginePath%\%UBTRelativePath%" -projectfiles - progress -project="%projectPath%"