// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "Sail.h"

int InitSail(SAIL* pSail)
{
	FILE* file = NULL;
	char line[MAX_BUF_LEN];
	char sendbuf[MAX_BUF_LEN];
	int sendbuflen = 0;

	// Default values.
	memset(line, 0, sizeof(line));
	memset(pSail->address, 0, sizeof(pSail->address));
	sprintf(pSail->address, IP_ADDR_SAIL);
	memset(pSail->port, 0, sizeof(pSail->port));
	sprintf(pSail->port, TCP_PORT_SAIL);
	pSail->MinAngle = MIN_ANGLE_SAIL;
	pSail->MaxAngle = MAX_ANGLE_SAIL;
	pSail->CalibrationSpeed = CALIBRATION_SPEED_SAIL;
	pSail->CalibrationTime = CALIBRATION_TIME_SAIL;
	pSail->CalibrationTorque = CALIBRATION_TORQUE_SAIL;
	pSail->NormalTorque = NORMAL_TORQUE_SAIL;

	// Load data from a file.
	file = fopen("Sail.txt", "r");
	if (file != NULL)
	{
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%32s", pSail->address) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%32s", pSail->port) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &pSail->MinAngle) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &pSail->MaxAngle) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &pSail->CalibrationSpeed) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &pSail->CalibrationTime) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &pSail->CalibrationTorque) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &pSail->NormalTorque) != 1) printf("Invalid configuration file.\n");
		if (fclose(file) != EXIT_SUCCESS) printf("fclose() failed.\n");
	}
	else
	{
		printf("Configuration file not found.\n");
	}

	// Connect to server.
	if (inittcpcli(&pSail->s, pSail->address, pSail->port) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	// Initialization.
	memset(sendbuf, 0, sizeof(sendbuf));
	sprintf(sendbuf, " \r");
	sendbuflen = (int)strlen(sendbuf);

	if (sendall(pSail->s, sendbuf, sendbuflen) != EXIT_SUCCESS)
	{
		releasetcpcli(pSail->s);
		return EXIT_FAILURE;
	}

	mSleep(100);

	return EXIT_SUCCESS;
}

int SetMotorTorqueSail(SAIL* pSail, int percent)
{
	char sendbuf[MAX_BUF_LEN];
	int sendbuflen = 0;

	percent = max(min(percent, 100), 0);

	// Prepare data to send to server.
	memset(sendbuf, 0, sizeof(sendbuf));
	sprintf(sendbuf, "Y 1 %d\r", percent);
	sendbuflen = (int)strlen(sendbuf);

	if (sendall(pSail->s, sendbuf, sendbuflen) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	mSleep(20);

	return EXIT_SUCCESS;
}

int SetMotorSpeedSail(SAIL* pSail, int val)
{
	char sendbuf[MAX_BUF_LEN];
	int sendbuflen = 0;

	if (abs(val) < MIN_MOTOR_SPEED_SAIL) val = 0;
	else if (val > MAX_MOTOR_SPEED_SAIL) val = MAX_MOTOR_SPEED_SAIL;
	else if (val < -MAX_MOTOR_SPEED_SAIL) val = -MAX_MOTOR_SPEED_SAIL;

	// Prepare data to send to server.
	memset(sendbuf, 0, sizeof(sendbuf));
	sprintf(sendbuf, "M%d\r", val);
	sendbuflen = (int)strlen(sendbuf);

	if (sendall(pSail->s, sendbuf, sendbuflen) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	mSleep(20);

	return EXIT_SUCCESS;
}

int SetMotorOriginSail(SAIL* pSail)
{
	char sendbuf[MAX_BUF_LEN];
	int sendbuflen = 0;

	// Prepare data to send to server.
	memset(sendbuf, 0, sizeof(sendbuf));
	sprintf(sendbuf, "O\r");
	sendbuflen = (int)strlen(sendbuf);

	if (sendall(pSail->s, sendbuf, sendbuflen) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	// Seem to take time...
	mSleep(100);

	return EXIT_SUCCESS;
}

int SetMaxAngleSail(SAIL* pSail, double angle)
{
	char sendbuf[MAX_BUF_LEN];
	int sendbuflen = 0;
	int val = 0;

	// Convert angle (in rad) into value for the motor.
	val = (int)(angle*(MAX_SAIL-MIN_SAIL)/(pSail->MaxAngle-pSail->MinAngle));

	val = max(min(val, MAX_SAIL), MIN_SAIL);

	// Prepare data to send to server.
	memset(sendbuf, 0, sizeof(sendbuf));
	sprintf(sendbuf, "R%d\r", -val);
	sendbuflen = (int)strlen(sendbuf);

	if (sendall(pSail->s, sendbuf, sendbuflen) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	mSleep(20);

	return EXIT_SUCCESS;
}

int CalibrateMotorSail(SAIL* pSail)
{
	if (SetMotorTorqueSail(pSail, pSail->CalibrationTorque) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}
	mSleep(1000);
	if (SetMotorSpeedSail(pSail, pSail->CalibrationSpeed) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}
	mSleep(pSail->CalibrationTime);
	if (SetMotorSpeedSail(pSail, 0) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}
	mSleep(1000);
	if (SetMotorOriginSail(pSail) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}
	mSleep(1000);
	if (SetMotorTorqueSail(pSail, pSail->NormalTorque) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}
	mSleep(1000);

	return EXIT_SUCCESS;
}

int ReleaseSail(SAIL* pSail)
{
	// Disconnect from server.
	if (releasetcpcli(pSail->s) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
