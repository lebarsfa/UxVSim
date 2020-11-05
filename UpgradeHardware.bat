cd /d "%~dp0"

set HARDWARE_PATH=%~d0\Personal data\Software\Other\Software development\C++\Robotics\Current\UxVCtrl\Hardware

copy /B /Y /Z "%HARDWARE_PATH%\RS232Port.h" .\RS232Port.h"

copy /B /Y /Z "%HARDWARE_PATH%\IM483I.h" .\IM483I.h"
copy /B /Y /Z "%HARDWARE_PATH%\NMEAProtocol.h" .\NMEAProtocol.h"
copy /B /Y /Z "%HARDWARE_PATH%\MT.h" .\MT.h
copy /B /Y /Z "%HARDWARE_PATH%\Ontrak.h" .\Ontrak.h"
copy /B /Y /Z "%HARDWARE_PATH%\SBG.h" .\SBG.h"
copy /B /Y /Z "%HARDWARE_PATH%\SSC32.h" .\SSC32.h

pause
