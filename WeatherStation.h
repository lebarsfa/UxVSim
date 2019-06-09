// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#ifndef WEATHERSTATION_H
#define WEATHERSTATION_H

#include "Utils.h"

#define IP_ADDR_WEATHERSTATION "172.20.5.4"
#define TCP_PORT_WEATHERSTATION "4001"

#define MAX_NB_BYTES_WEATHERSTATION 2048

struct WEATHERSTATION
{
	SOCKET s;
	char address[MAX_BUF_LEN];
	char port[MAX_BUF_LEN];
};
typedef struct WEATHERSTATION WEATHERSTATION;

int InitWeatherStation(WEATHERSTATION* pWeatherStation);
int GetDataWeatherStation(WEATHERSTATION* pWeatherStation, double* pLatitude, double* pLongitude, 
						  double* pWindDir, double* pWindSpeed, double* pHeading);
int ReleaseWeatherStation(WEATHERSTATION* pWeatherStation);

#endif // WEATHERSTATION_H
