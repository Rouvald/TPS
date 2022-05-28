@echo off

call "%~dp0\..\config.bat"

"%runUATPath%" BuildCookRun ^
-project="%projectPath%" ^
-platform="%platform%" ^
-clientconfig="%configuration%" ^
-archivedirectory="%archievePath%" ^
-build -cook -package -stage -archive -pak -allmaps