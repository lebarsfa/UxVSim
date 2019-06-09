// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "SerialIOInterface.h"

int InitSerialIOInterface(SERIALIOINTERFACE* pSerialIOInterface)
{
	FILE* file = NULL;
	char line[MAX_BUF_LEN];
	char sendbuf[MAX_BUF_LEN];
	int sendbuflen = 0;

	// Default values.
	memset(line, 0, sizeof(line));
	memset(pSerialIOInterface->address, 0, sizeof(pSerialIOInterface->address));
	sprintf(pSerialIOInterface->address, IP_ADDR_SERIALIOINTERFACE);
	memset(pSerialIOInterface->port, 0, sizeof(pSerialIOInterface->port));
	sprintf(pSerialIOInterface->port, TCP_PORT_SERIALIOINTERFACE);

	// Load data from a file.
	file = fopen("SerialIOInterface.txt", "r");
	if (file != NULL)
	{
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%32s", pSerialIOInterface->address) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%32s", pSerialIOInterface->port) != 1) printf("Invalid configuration file.\n");
		if (fclose(file) != EXIT_SUCCESS) printf("fclose() failed.\n");
	}
	else
	{
		printf("Configuration file not found.\n");
	}

	// Connect to server.
	if (inittcpcli(&pSerialIOInterface->s, pSerialIOInterface->address, pSerialIOInterface->port) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	// Reset the digital port to avoid the relays turning on unexpectedly when the port is configured.
	memset(sendbuf, 0, sizeof(sendbuf));
	sprintf(sendbuf, "0SPA00000000\r");
	sendbuflen = (int)strlen(sendbuf);

	if (sendall(pSerialIOInterface->s, sendbuf, sendbuflen) != EXIT_SUCCESS)
	{
		releasetcpcli(pSerialIOInterface->s);
		return EXIT_FAILURE;
	}

	mSleep(50);

	// Configuration of the digital port as output.
	memset(sendbuf, 0, sizeof(sendbuf));
	sprintf(sendbuf, "0CPA00000000\r");
	sendbuflen = (int)strlen(sendbuf);

	if (sendall(pSerialIOInterface->s, sendbuf, sendbuflen) != EXIT_SUCCESS)
	{
		releasetcpcli(pSerialIOInterface->s);
		return EXIT_FAILURE;
	}

	mSleep(50);

	return EXIT_SUCCESS;
}

int SetRelaySerialIOInterface(SERIALIOINTERFACE* pSerialIOInterface, int channel, int bOpen)
{
	char sendbuf[MAX_BUF_LEN];
	int sendbuflen = 0;
#ifdef CHECK_STATE_SERIALIOINTERFACE
	char recvbuf[MAX_BUF_LEN];
	int recvbuflen = 0;
	int state = 0;
#endif // CHECK_STATE_SERIALIOINTERFACE

	// Prepare data to send to server.
	memset(sendbuf, 0, sizeof(sendbuf));
	if (bOpen)
	{
		sprintf(sendbuf, "1RK%d\r", channel);
	}
	else
	{
		sprintf(sendbuf, "1SK%d\r", channel);
	}
	sendbuflen = (int)strlen(sendbuf);

	if (sendall(pSerialIOInterface->s, sendbuf, sendbuflen) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	mSleep(25);

#ifdef CHECK_STATE_SERIALIOINTERFACE
	// Check state : 1RPK%d\r returns 0 if opened or 1 if closed. 

	// Prepare data to send to server.
	memset(sendbuf, 0, sizeof(sendbuf));
	sprintf(sendbuf, "1RPK%d\r", channel);
	sendbuflen = (int)strlen(sendbuf);

	if (sendall(pSerialIOInterface->s, sendbuf, sendbuflen) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	mSleep(25);

	// Prepare the buffer that should receive data from the server.
	memset(recvbuf, 0, sizeof(recvbuf));
	recvbuflen = 2; // The last character must be a 0 to be a valid string for sscanf.

	if (recvall(pSerialIOInterface->s, recvbuf, recvbuflen) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	mSleep(10);

	// Display and analyze received data.
	//printf("Received : \"%s\"\n", recvbuf);
	if (sscanf(recvbuf, "%d", &state) != 1)
	{
		printf("Invalid data received.\n");
		return EXIT_FAILURE;
	}

	if (((bOpen == 1)&&(state == 1))||((bOpen == 0)&&(state == 0)))
	{
		printf("Inconsitent relay state.\n");
		return EXIT_FAILURE;
	}
#endif // CHECK_STATE_SERIALIOINTERFACE

	return EXIT_SUCCESS;
}

int SetDigitalRelaySerialIOInterface(SERIALIOINTERFACE* pSerialIOInterface, int channel, int bOpen)
{
	char sendbuf[MAX_BUF_LEN];
	int sendbuflen = 0;
#ifdef CHECK_STATE_SERIALIOINTERFACE
	char recvbuf[MAX_BUF_LEN];
	int recvbuflen = 0;
	int state = 0;
#endif // CHECK_STATE_SERIALIOINTERFACE

	// Prepare data to send to server.
	memset(sendbuf, 0, sizeof(sendbuf));
	if (bOpen)
	{
		sprintf(sendbuf, "0RESPA%d\r", channel);
	}
	else
	{
		sprintf(sendbuf, "0SETPA%d\r", channel);
	}
	sendbuflen = (int)strlen(sendbuf);

	if (sendall(pSerialIOInterface->s, sendbuf, sendbuflen) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	mSleep(25);

#ifdef CHECK_STATE_SERIALIOINTERFACE
	// Check state : 0RPA%d\r returns 0 if reset or 1 if set.

	// Prepare data to send to server.
	memset(sendbuf, 0, sizeof(sendbuf));
	sprintf(sendbuf, "0RPA%d\r", channel);
	sendbuflen = (int)strlen(sendbuf);

	if (sendall(pSerialIOInterface->s, sendbuf, sendbuflen) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	mSleep(25);

	// Prepare the buffer that should receive data from the server.
	memset(recvbuf, 0, sizeof(recvbuf));
	recvbuflen = 2; // The last character must be a 0 to be a valid string for sscanf.

	if (recvall(pSerialIOInterface->s, recvbuf, recvbuflen) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	mSleep(10);

	// Display and analyze received data.
	//printf("Received : \"%s\"\n", recvbuf);
	if (sscanf(recvbuf, "%d", &state) != 1)
	{
		printf("Invalid data received.\n");
		return EXIT_FAILURE;
	}

	if (((bOpen == 1)&&(state == 1))||((bOpen == 0)&&(state == 0)))
	{
		printf("Inconsitent digital relay state.\n");
		return EXIT_FAILURE;
	}
#endif // CHECK_STATE_SERIALIOINTERFACE

	return EXIT_SUCCESS;
}

int GetAnalogInputSerialIOInterface(SERIALIOINTERFACE* pSerialIOInterface, int channel, int* pValue)
{
	char sendbuf[MAX_BUF_LEN];
	int sendbuflen = 0;
	char recvbuf[MAX_BUF_LEN];
	int recvbuflen = 0;

	// Prepare data to send to server.
	memset(sendbuf, 0, sizeof(sendbuf));
	sprintf(sendbuf, "0RD%d\r", channel);
	sendbuflen = (int)strlen(sendbuf);

	if (sendall(pSerialIOInterface->s, sendbuf, sendbuflen) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	mSleep(25);

	// Prepare the buffer that should receive data from the server.
	memset(recvbuf, 0, sizeof(recvbuf));
	recvbuflen = 5; // The last character must be a 0 to be a valid string for sscanf.

	if (recvall(pSerialIOInterface->s, recvbuf, recvbuflen) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	mSleep(10);

	// Display and analyze received data.
	//printf("Received : \"%s\"\n", recvbuf);
	if (sscanf(recvbuf, "%d", pValue) != 1)
	{
		printf("Invalid data received.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int GetDigitalInputSerialIOInterface(SERIALIOINTERFACE* pSerialIOInterface, int channel, int* pValue)
{
	char sendbuf[MAX_BUF_LEN];
	int sendbuflen = 0;
	char recvbuf[MAX_BUF_LEN];
	int recvbuflen = 0;

	// Prepare data to send to server.
	memset(sendbuf, 0, sizeof(sendbuf));
	sprintf(sendbuf, "1RPA%d\r", channel);
	sendbuflen = (int)strlen(sendbuf);

	if (sendall(pSerialIOInterface->s, sendbuf, sendbuflen) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	mSleep(25);

	// Prepare the buffer that should receive data from the server.
	memset(recvbuf, 0, sizeof(recvbuf));
	recvbuflen = 2; // The last character must be a 0 to be a valid string for sscanf.

	if (recvall(pSerialIOInterface->s, recvbuf, recvbuflen) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	mSleep(10);

	// Display and analyze received data.
	//printf("Received : \"%s\"\n", recvbuf);
	if (sscanf(recvbuf, "%d", pValue) != 1)
	{
		printf("Invalid data received.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int ReleaseSerialIOInterface(SERIALIOINTERFACE* pSerialIOInterface)
{
	// Disconnect from server.
	if (releasetcpcli(pSerialIOInterface->s) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
