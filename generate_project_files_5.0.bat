@echo off
call "%~dp0devops\config.bat"

devops\misc\generate_project_files.bat "%enginePath_5.0%" "%UBTRelativePath5%" "%versionSelector%" "%projectName%"