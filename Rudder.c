// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "Rudder.h"

int InitRudder(RUDDER* pRudder)
{
	FILE* file = NULL;
	char line[MAX_BUF_LEN];

	// Default values.
	memset(line, 0, sizeof(line));
	memset(pRudder->address, 0, sizeof(pRudder->address));
	sprintf(pRudder->address, IP_ADDR_RUDDER);
	memset(pRudder->port, 0, sizeof(pRudder->port));
	sprintf(pRudder->port, TCP_PORT_RUDDER);
	pRudder->minpw = MIN_PW_RUDDER;
	pRudder->midpw = MID_PW_RUDDER;
	pRudder->maxpw = MAX_PW_RUDDER;
	pRudder->MinAngle = MIN_ANGLE_RUDDER;
	pRudder->MaxAngle = MAX_ANGLE_RUDDER;

	// Load data from a file.
	file = fopen("Rudder.txt", "r");
	if (file != NULL)
	{
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%32s", pRudder->address) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%32s", pRudder->port) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &pRudder->minpw) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &pRudder->midpw) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &pRudder->maxpw) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &pRudder->MinAngle) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &pRudder->MaxAngle) != 1) printf("Invalid configuration file.\n");
		if (fclose(file) != EXIT_SUCCESS) printf("fclose() failed.\n");
	}
	else
	{
		printf("Configuration file not found.\n");
	}

	// Connect to server.
	if (inittcpcli(&pRudder->s, pRudder->address, pRudder->port) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int SetAngleRudder(RUDDER* pRudder, double angle)
{
	char sendbuf[MAX_BUF_LEN];
	int sendbuflen = 0;
	int pw = 0;

	// Convert angle (in rad) into rudder pulse width (in us).
	pw = pRudder->midpw+(int)(angle*(pRudder->maxpw-pRudder->minpw)/(pRudder->MaxAngle-pRudder->MinAngle));

	pw = max(min(pw, pRudder->maxpw), pRudder->minpw);

	// Prepare data to send to server.
	memset(sendbuf, 0, sizeof(sendbuf));
	sprintf(sendbuf, "#0P%d\r", pw);
	sendbuflen = (int)strlen(sendbuf);

	if (sendall(pRudder->s, sendbuf, sendbuflen) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}
	
	mSleep(20);

	return EXIT_SUCCESS;
}

int CheckRudder(RUDDER* pRudder)
{
	if (SetAngleRudder(pRudder, -M_PI/16.0) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}
	mSleep(2000);
	if (SetAngleRudder(pRudder, M_PI/16.0) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}
	mSleep(2000);
	if (SetAngleRudder(pRudder, 0.0) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}
	mSleep(2000);

	return EXIT_SUCCESS;
}

int ReleaseRudder(RUDDER* pRudder)
{
	// Disconnect from server.
	if (releasetcpcli(pRudder->s) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
