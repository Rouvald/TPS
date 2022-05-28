@echo off

rem Engine params
set enginePath_4.27=c:\UE_4.27
set enginePath_5.0=d:\UE_5.0

set UBTRelativePath4=Engine\Binaries\DotNET\UnrealBuildTool.exe
set UBTRelativePath5=Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe
set versionSelector=c:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe

rem Engine version for packaging
set runUATPath=%enginePath_4.27%\Engine\Build\BatchFiles\RunUAT.bat

rem Project params
set projectRoot=c:\UE4_Projects\TPS\
set projectName=TPS.uproject
set projectPath=%projectRoot%\%projectName%

rem Build params
set platform=Win64
set configuration=Development
set archievePath=%projectRoot%\Build

rem Other params
set sourceCodePath=%projectRoot%\Source
set dirsToRemove=Intermediate DerivedDataCache Saved Binaries .vs Build
set filesToRemove=*.sln