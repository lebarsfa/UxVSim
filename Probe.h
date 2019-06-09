// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#ifndef PROBE_H
#define PROBE_H

#include "Utils.h"

#define IP_ADDR_PROBE "172.20.5.4"
#define TCP_PORT_PROBE "4008"

#define SENDBUF_LEN_PROBE 8
#define RECVBUF_LEN_PROBE 9

#define ADDR_PROBE 0x01
#define READ_COMMAND_PROBE 0x03
#define NB_REGISTERS_READ_H_PROBE 0x00
#define NB_REGISTERS_READ_L_PROBE 0x01

#define NB_DATA_BYTES_PROBE 4

#define TEMPERATURE_REG_L_PROBE 0x00
#define PH_REG_L_PROBE 0x06
#define CONDUCTIVITY_REG_L_PROBE 0x0A
#define SALINITY_REG_L_PROBE 0x10
#define TURBIDITY_REG_L_PROBE 0xA6
#define DOSAT_REG_L_PROBE 0xB2
#define DOMGL_REG_L_PROBE 0xB4
#define DEPTH_REG_L_PROBE 0x22
#define CHLOROPHYLL_REG_L_PROBE 0xD0
#define NOSLEEP_REG_L_PROBE 0x0C

// In ms.
#define STARTUP_DELAY_PROBE 4000
#define SHUTDOWN_DELAY_PROBE 4000
#define WAKEUP_DELAY_PROBE 500
#define SLEEP_DELAY_PROBE 12000
#define MAX_MEASUREMENT_DELAY_PROBE 6500

struct PROBE
{
	SOCKET s;
	char address[MAX_BUF_LEN];
	char port[MAX_BUF_LEN];
};
typedef struct PROBE PROBE;

int InitProbe(PROBE* pProbe);
int WakeUpProbe(PROBE* pProbe);
int GetValueProbe(PROBE* pProbe, int data_reg_l, double* pValue);
int GetDefaultValuesProbe(PROBE* pProbe, double* pTemperature, double* ppH, double* pConductivity, 
						  double* pSalinity, double* pTurbidity, double* pDOSat, double* pDOMgL, 
						  double* pDepth, double* pChlorophyll);
int ReleaseProbe(PROBE* pProbe);

#endif // PROBE_H
