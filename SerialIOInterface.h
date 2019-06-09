// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#ifndef SERIALIOINTERFACE_H
#define SERIALIOINTERFACE_H

#include "Utils.h"

#define IP_ADDR_SERIALIOINTERFACE "172.20.5.4"
#define TCP_PORT_SERIALIOINTERFACE "4002"

// ADR2000 (address 0 in daisy chain).
// Analog inputs.
#define CURRENT_GENERATION_CHANNEL 0
#define CURRENT_CONSUMPTION_CHANNEL 1
#define BATTERY_VOLTAGE_CHANNEL 2
// Digital port.
#define IRIDIUM_CHANNEL 6
#define SAIL_MOTOR_CHANNEL 7

// ADR2205 (address 1 in daisy chain).
// Relays.
#define ARMADEUS_CHANNEL 0 // Connected to an active-low relay.
#define BOTTOM_PUMP_CHANNEL 1
#define SURFACE_PUMP_CHANNEL 2
#define WIFI_CHANNEL 3 // Connected to an active-low relay.
#define PROBE_CHANNEL 4
// Digital inputs.
#define WATER_INGRESS0_CHANNEL 0
#define WATER_INGRESS1_CHANNEL 1

#define ANALOG_INPUT2CURRENT(value) (((double)value/4095.0)*5.0)
#define ANALOG_INPUT2VOLTAGE(value) (((double)value/4095.0)*5.0*15.0)

struct SERIALIOINTERFACE
{
	SOCKET s;
	char address[MAX_BUF_LEN];
	char port[MAX_BUF_LEN];
};
typedef struct SERIALIOINTERFACE SERIALIOINTERFACE;

int InitSerialIOInterface(SERIALIOINTERFACE* pSerialIOInterface);
int SetRelaySerialIOInterface(SERIALIOINTERFACE* pSerialIOInterface, int channel, int bOpen);
int SetDigitalRelaySerialIOInterface(SERIALIOINTERFACE* pSerialIOInterface, int channel, int bOpen);
int GetAnalogInputSerialIOInterface(SERIALIOINTERFACE* pSerialIOInterface, int channel, int* pValue);
int GetDigitalInputSerialIOInterface(SERIALIOINTERFACE* pSerialIOInterface, int channel, int* pValue);
int ReleaseSerialIOInterface(SERIALIOINTERFACE* pSerialIOInterface);

#endif // SERIALIOINTERFACE_H
