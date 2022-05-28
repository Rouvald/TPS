@echo off
call "%~dp0devops\config.bat"

devops\misc\generate_project_files.bat "%enginePath_4.27%" "%UBTRelativePath4%" "%versionSelector%" "%projectName%"