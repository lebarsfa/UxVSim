// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#ifndef SAIL_H
#define SAIL_H

#include "Utils.h"

#define IP_ADDR_SAIL "172.20.5.4"
#define TCP_PORT_SAIL "4003"
#define MIN_MOTOR_SPEED_SAIL 20
#define MAX_MOTOR_SPEED_SAIL 20000
#define MIN_SAIL 0
#define MAX_SAIL 116000
#define MIN_ANGLE_SAIL 0.0
#define MAX_ANGLE_SAIL (M_PI/2.0)
#define CALIBRATION_SPEED_SAIL 2000
#define CALIBRATION_TIME_SAIL 65000
#define CALIBRATION_TORQUE_SAIL 60
#define NORMAL_TORQUE_SAIL 100

struct SAIL
{
	SOCKET s;
	char address[MAX_BUF_LEN];
	char port[MAX_BUF_LEN];
	double MinAngle;
	double MaxAngle;
	int CalibrationSpeed;
	int CalibrationTime;
	int CalibrationTorque;
	int NormalTorque;
};
typedef struct SAIL SAIL;

int InitSail(SAIL* pSail);
int SetMotorTorqueSail(SAIL* pSail, int percent);
int SetMotorSpeedSail(SAIL* pSail, int val);
int SetMotorOriginSail(SAIL* pSail);
int SetMaxAngleSail(SAIL* pSail, double angle);
int CalibrateMotorSail(SAIL* pSail);
int ReleaseSail(SAIL* pSail);

#endif // SAIL_H
