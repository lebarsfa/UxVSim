// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#ifndef COMPASS_H
#define COMPASS_H

#include "Utils.h"

#define IP_ADDR_COMPASS "172.20.5.4"
#define TCP_PORT_COMPASS "4007"

// Correction coefficients.
#define ROLL_ORIENTATION_COMPASS 0.0
#define ROLL_P1_COMPASS 0.0
#define ROLL_P2_COMPASS 0.0
#define PITCH_ORIENTATION_COMPASS 0.0
#define PITCH_P1_COMPASS 0.0
#define PITCH_P2_COMPASS 0.0
#define YAW_ORIENTATION_COMPASS 0.0
#define YAW_P1_COMPASS 0.0
#define YAW_P2_COMPASS 0.0

#define MAX_NB_BYTES_COMPASS 2048

#define MIN_STANDARD_BUF_LEN_COMPASS 5

#define PREAMBLE_COMPASS 0xFA
#define ADDR_COMPASS 0xFF
#define EXT_LEN_COMPASS 0xFF

// Wake-up and state messages.
#define GO_TO_CONFIG_MID 0x30
#define GO_TO_CONFIG_ACK_MID 0x31
#define GO_TO_MEASUREMENT_MID 0x10
#define GO_TO_MEASUREMENT_ACK_MID 0x11
#define RESET_MID 0x40
#define RESET_ACK_MID 0x41

// Informational messages.
#define ERROR_MID 0x42
#define REQ_GPS_STATUS_MID 0xA6
#define GPS_STATUS_MID 0xA7

// Configuration messages.
#define REQ_CONFIGURATION_MID 0x0C
#define CONFIGURATION_MID 0x0D
#define REQ_PERIOD_MID 0x04
#define REQ_PERIOD_ACK_MID 0x05
#define REQ_OUTPUT_SKIP_FACTOR_MID 0xD4
#define REQ_OUTPUT_SKIP_FACTOR_ACK_MID 0xD5
#define REQ_OUTPUT_MODE_MID 0xD0
#define REQ_OUTPUT_MODE_ACK_MID 0xD1
#define REQ_OUTPUT_SETTINGS_MID 0xD2
#define REQ_OUTPUT_SETTINGS_ACK_MID 0xD3

// Data-related messages.
#define REQ_DATA_MID 0x34
#define MTDATA_MID 0x32

// Output mode.
#define TEMPERATURE_BIT 0x0001
#define CALIBRATED_BIT 0x0002
#define ORIENTATION_BIT 0x0004
#define AUXILIARY_BIT 0x0008
#define POSITION_BIT 0x0010
#define VELOCITY_BIT 0x0020
#define STATUS_BIT 0x0800
#define GPS_PVT_BIT 0x1000
#define RAW_BIT 0x4000

// Output settings.
#define TIMESTAMP_OUTPUT_MASK 0x00000003
#define NO_TIMESTAMP 0x00000000
#define SAMPLE_COUNTER 0x00000001
#define UTC_TIME 0x00000002
#define SAMPLE_COUNTER_UTC_TIME 0x00000003

#define ORIENTATION_MODE_MASK 0x0000000C
#define QUATERNION 0x00000000
#define EULER_ANGLES 0x00000004
#define MATRIX 0x00000008

#define CALIBRATION_MODE_MASK 0x00000070
#define ACCELERATION 0x00000010
#define RATE_OF_TURN 0x00000020
#define MAGNETOMETER 0x00000040

#define OUTPUT_FORMAT_MASK 0x00000300
#define OUTPUT_FORMAT_FLOAT 0x00000000
#define OUTPUT_FORMAT_FIXED_POINT_SIGNED_1220 0x00000100
#define OUTPUT_FORMAT_FIXED_POINT_SIGNED_1632 0x00000200

#define AUXILIARY_MODE_MASK 0x00000C00
#define ANALOG_1 0x00000400
#define ANALOG_2 0x00000800

struct MTDATA
{
	double temp;
	double accX, accY, accZ;
	double gyrX, gyrY, gyrZ;
	double magX, magY, magZ;
	double q0, q1, q2, q3;
	double roll, pitch, yaw;
	double a, b, c, d, e, f, g, h, i;
	unsigned short Ain_1, Ain_2; 
	double Lat, Long, Alt;
	double Vel_X, Vel_Y, Vel_Z;
	unsigned short TS; 
};
typedef struct MTDATA MTDATA;

struct COMPASS
{
	SOCKET s;
	char address[MAX_BUF_LEN];
	char port[MAX_BUF_LEN];
	MTDATA LastMTData;
	int OutputMode;
	int OutputSettings;
	double rollorientation;
	double rollp1;
	double rollp2;
	double pitchorientation;
	double pitchp1;
	double pitchp2;
	double yaworientation;
	double yawp1;
	double yawp2;
};
typedef struct COMPASS COMPASS;

void SetMTChecksum(unsigned char* buf, int buflen);
int CheckMTChecksum(unsigned char* buf, int buflen);
int AnalyzeMTMessage(unsigned char* buf, int buflen, int addr, int mid);
int FindMTMessage(unsigned char* buf, int buflen, int addr, int mid, unsigned char** pFoundmsg, int* pFoundmsglen);
int FindLatestMTMessage(unsigned char* buf, int buflen, int addr, int mid, unsigned char** pFoundmsg, int* pFoundmsglen);
int GetMTMessageCompass(COMPASS* pCompass, int addr, int mid, unsigned char* databuf, int databuflen, int* pNbdatabytes);
int InitCompass(COMPASS* pCompass);
int GetMTDataCompass(COMPASS* pCompass, MTDATA* pMTData);
int GetAnglesCompass(COMPASS* pCompass, double* pRoll, double* pPitch, double* pYaw);
int ReleaseCompass(COMPASS* pCompass);

#endif // COMPASS_H
