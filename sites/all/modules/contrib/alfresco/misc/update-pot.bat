@echo off
set POTX_PATH=../potx/potx-cli.php
cd..
start /B /WAIT cmd.exe /C php %POTX_PATH%
move /Y general.pot translations/alfresco.pot
del installer.pot
pause
