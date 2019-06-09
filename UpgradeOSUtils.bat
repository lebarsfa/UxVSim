cd /d "%~dp0"

set OSUTILS_PATH=%~d0\Personal data\Software\Other\Software development\C++\Robotics\Current\OSUtils

copy /B /Y /Z "%OSUTILS_PATH%\OSComputerRS232Port.c" .\OSComputerRS232Port.c"
copy /B /Y /Z "%OSUTILS_PATH%\OSComputerRS232Port.h" .\OSComputerRS232Port.h"
copy /B /Y /Z "%OSUTILS_PATH%\OSCore.c" .\OSCore.c"
copy /B /Y /Z "%OSUTILS_PATH%\OSCore.h" .\OSCore.h"
copy /B /Y /Z "%OSUTILS_PATH%\OSNet.c" .\OSNet.c"
copy /B /Y /Z "%OSUTILS_PATH%\OSNet.h" .\OSNet.h"
copy /B /Y /Z "%OSUTILS_PATH%\OSMisc.c" .\OSMisc.c"
copy /B /Y /Z "%OSUTILS_PATH%\OSMisc.h" .\OSMisc.h"
copy /B /Y /Z "%OSUTILS_PATH%\OSTime.c" .\OSTime.c"
copy /B /Y /Z "%OSUTILS_PATH%\OSTime.h" .\OSTime.h"

pause
