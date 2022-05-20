@echo off

:: for UE5 need change enginePath and UBTRelativePath
set enginePath=c:\Program Files\Epic Games\UE_4.26
set UBTRelativePath=Engine\Binaries\DotNET\UnrealBuildTool.exe
::
set versionSelector=c:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe
set projectName=TPS.uproject

devops/generate_project_files.bat "%enginePath%" "%UBTRelativePath%" "%versionSelector%" "%projectName%"