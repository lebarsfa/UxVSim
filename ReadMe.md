Simulation of the protocol of various devices on a simulated moving boat.

Default configuration (see `UxVSim.txt`) is to simulate a motorboat (`robid` parameter set to `0x00000400`, with a Pololu Jrk device to control the rudder, a SBG Ellipse AHRS and a NMEA (`$GPRMC` sentence) GPS), to simulate a sailboat change `robid` parameter to e.g. `0x00002000` or `0x00008000` (different devices will be simulated depending on robid). You might want also to change the initial position of the simulated robot (see `lat0`, `long0` parameters) as well as wind, currents and waves conditions (see `psi_med`, `psi_var`, `beta`, `Vc_med`, `Vc_var`, `psic_med`, `psic_var`, `hw_var` parameters).

Example:
If `robid` is `0x00008000`, a sailboat with the following devices is simulated:
- Furuno WS200 weather station. It sends NMEA messages through a TCP server on port 4001 (use e.g. `telnet 127.0.0.1 4001` to see them).
- Razor AHRS. It sends messages similar to `#YPR=yaw,pitch,roll` through a TCP server on port 4007 (use e.g. `telnet 127.0.0.1 4007` to see them).
- Lynxmotion SSC-32 servo controller to move the rudder. It waits for commands from `#0P1000\r` (move the rudder so that the sailboat turns counterclockwise) to `#0P2000\r` (move the rudder so that the sailboat turns clockwise) through a TCP server on port 4004. The value 1000-2000 corresponds to the pulse width in us of the PWM signal sent to a typical servomotor. Be careful to the carriage return character `\r`, which might not correspond by default to the ENTER key on the keyboard (might not work by default with the telnet command, characters options can be changed in HyperTerminal on Windows if needed).
- IM483I motor controller to limit the max angle of the sail w.r.t. the hull. It waits for commands from `R0\r` (sail aligned with the hull) to `R116000\r` (sail allowed to open perpendiculary to the hull) through a TCP server on port 4003. Be careful to the carriage return character `\r`, which might not correspond by default to the ENTER key on the keyboard (might not work by default with the telnet command, characters options can be changed in HyperTerminal on Windows if needed).

Superseded by most XXXInterface functionalities in https://github.com/ENSTABretagneRobotics/UxVCtrl.
