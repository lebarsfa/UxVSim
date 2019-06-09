// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#ifndef RUDDER_H
#define RUDDER_H

#include "OSNet.h"
#include "OSMisc.h"

#define IP_ADDR_RUDDER "172.20.5.4"
#define TCP_PORT_RUDDER "4004"
#define MIN_PW_RUDDER 1000
#define MID_PW_RUDDER 1500
#define MAX_PW_RUDDER 2000
#define MIN_ANGLE_RUDDER (-M_PI/4.0)
#define MAX_ANGLE_RUDDER (M_PI/4.0)

struct RUDDER
{
	SOCKET s;
	char address[MAX_BUF_LEN];
	char port[MAX_BUF_LEN];
	int minpw;
	int midpw;
	int maxpw;
	double MinAngle;
	double MaxAngle;
};
typedef struct RUDDER RUDDER;

int InitRudder(RUDDER* pRudder);
int SetAngleRudder(RUDDER* pRudder, double angle);
int CheckRudder(RUDDER* pRudder);
int ReleaseRudder(RUDDER* pRudder);

#endif // RUDDER_H
