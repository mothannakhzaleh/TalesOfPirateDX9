:: Server Release
call :MKLINK "%cd%\..\server\AccountServer\AccountServer.exe" "%cd%\Server\AccountServer\Bin\AccountServer.exe"
call :MKLINK "%cd%\..\server\AccountServer\AccountServer.pdb" "%cd%\Server\AccountServer\Bin\AccountServer.pdb"
call :MKLINK "%cd%\..\server\GroupServer\GroupServer.exe" "%cd%\Server\GroupServer\Bin\GroupServer.exe"
call :MKLINK "%cd%\..\server\GroupServer\GroupServer.pdb" "%cd%\Server\GroupServer\Bin\GroupServer.pdb"
call :MKLINK "%cd%\..\server\GateServer\GateServer.exe" "%cd%\Server\GateServer\Bin\GateServer.exe"
call :MKLINK "%cd%\..\server\GateServer\GateServer.pdb" "%cd%\Server\GateServer\Bin\GateServer.pdb"
call :MKLINK "%cd%\..\server\GameServer\GameServer.exe" "%cd%\Server\GameServer\Bin\GameServer.exe"
call :MKLINK "%cd%\..\server\GameServer\GameServer.pdb" "%cd%\Server\GameServer\Bin\GameServer.pdb"

:: Client Release
call :MKLINK "%cd%\..\client\system\Game.exe" "%cd%\Client\bin\system\Game.exe"
call :MKLINK "%cd%\..\client\system\Game.pdb" "%cd%\Client\bin\system\Game.pdb"
call :MKLINK "%cd%\..\client\system\MindPower3D_D8R.dll" "%cd%\Engine\lib\MindPower3D_D8R.dll"
call :MKLINK "%cd%\..\client\system\MindPower3D_D8R.pdb" "%cd%\Engine\lib\MindPower3D_D8R.pdb"

pause

:MKLINK
if exist "%~1" del /F /Q "%~1"
mklink "%~1" "%~2"
EXIT/B 0