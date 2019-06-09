// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "WeatherStation.h"

int InitWeatherStation(WEATHERSTATION* pWeatherStation)
{
	FILE* file = NULL;
	char line[MAX_BUF_LEN];

	// Default values.
	memset(line, 0, sizeof(line));
	memset(pWeatherStation->address, 0, sizeof(pWeatherStation->address));
	sprintf(pWeatherStation->address, IP_ADDR_WEATHERSTATION);
	memset(pWeatherStation->port, 0, sizeof(pWeatherStation->port));
	sprintf(pWeatherStation->port, TCP_PORT_WEATHERSTATION);

	// Load data from a file.
	file = fopen("WeatherStation.txt", "r");
	if (file != NULL)
	{
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%32s", pWeatherStation->address) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%32s", pWeatherStation->port) != 1) printf("Invalid configuration file.\n");
		if (fclose(file) != EXIT_SUCCESS) printf("fclose() failed.\n");
	}
	else
	{
		printf("Configuration file not found.\n");
	}

	// Connect to server.
	if (inittcpcli(&pWeatherStation->s, pWeatherStation->address, pWeatherStation->port) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int GetDataWeatherStation(WEATHERSTATION* pWeatherStation, double* pLatitude, double* pLongitude, 
						  double* pWindDir, double* pWindSpeed, double* pHeading)
{
	char recvbuf[MAX_NB_BYTES_WEATHERSTATION];
	char savebuf[MAX_NB_BYTES_WEATHERSTATION];
	int BytesReceived = 0, Bytes = 0, recvbuflen = 0;
	char* ptr_GPGGA = NULL;
	char* ptr_HCHDG = NULL;
	char* ptr_WIMDA = NULL;

	// Temporary buffers for sscanf().
	char c0 = 0, c1 = 0, c2 = 0;
	double f0 = 0, f1 = 0, f2 = 0;

	double utc = 0;
	double pressure = 0, temperature = 0;
	char cpressure = 0, ctemperature = 0;
	double winddir = 0, windspeed = 0;
	char cwinddir = 0, cwindspeed = 0;
	int latdeg = 0, longdeg = 0;
	double latmin = 0, longmin = 0;
	char szlatdeg[3];
	char szlongdeg[4];
	char north = 0, east = 0;
	int GPS_quality_indicator = 0;
	double heading = 0, deviation = 0, variation = 0; 
	char dev_east = 0, var_east = 0;

	// Prepare the buffers.
	memset(recvbuf, 0, sizeof(recvbuf));
	memset(savebuf, 0, sizeof(savebuf));
	recvbuflen = MAX_NB_BYTES_WEATHERSTATION-1; // The last character must be a 0 to be a valid string for sscanf.
	BytesReceived = 0;

	// As the sensor always sends data, any old data must be discarded.
	Bytes = recv(pWeatherStation->s, recvbuf, recvbuflen, 0);
	if (Bytes <= 0)
	{
		printf("recv() failed or timed out.\n");
		return EXIT_FAILURE;
	}
	BytesReceived += Bytes;

	while (Bytes == recvbuflen)
	{
		memcpy(savebuf, recvbuf, Bytes);

		Bytes = recv(pWeatherStation->s, recvbuf, recvbuflen, 0);
		if (Bytes <= 0)
		{
			printf("recv() failed or timed out.\n");
			return EXIT_FAILURE;
		}
		BytesReceived += Bytes;
	}

	if (BytesReceived < recvbuflen)
	{
		ptr_GPGGA = FindLatestNMEASentence("$GPGGA", recvbuf);
		ptr_HCHDG = FindLatestNMEASentence("$HCHDG", recvbuf);
		ptr_WIMDA = FindLatestNMEASentence("$WIMDA", recvbuf);

		while (!(ptr_GPGGA && ptr_HCHDG && ptr_WIMDA))
		{
			if (BytesReceived >= recvbuflen) return EXIT_FAILURE;

			Bytes = recv(pWeatherStation->s, recvbuf+BytesReceived, recvbuflen-BytesReceived, 0);
			if (Bytes <= 0)
			{
				printf("recv() failed or timed out.\n");
				return EXIT_FAILURE;
			}
			BytesReceived += Bytes;

			ptr_GPGGA = FindLatestNMEASentence("$GPGGA", recvbuf);
			ptr_HCHDG = FindLatestNMEASentence("$HCHDG", recvbuf);
			ptr_WIMDA = FindLatestNMEASentence("$WIMDA", recvbuf);
		}
	}
	else
	{
		memmove(recvbuf+recvbuflen-Bytes, recvbuf, Bytes);
		memcpy(recvbuf, savebuf+Bytes, recvbuflen-Bytes);

		ptr_GPGGA = FindLatestNMEASentence("$GPGGA", recvbuf);
		ptr_HCHDG = FindLatestNMEASentence("$HCHDG", recvbuf);
		ptr_WIMDA = FindLatestNMEASentence("$WIMDA", recvbuf);

		if (!(ptr_GPGGA && ptr_HCHDG && ptr_WIMDA)) return EXIT_FAILURE;
	}

	// Display and analyze received data.
	//printf("Received : \"%s\"\n", recvbuf);

	// GPS data.
	memset(szlatdeg, 0, sizeof(szlatdeg));
	memset(szlongdeg, 0, sizeof(szlongdeg));

	if (sscanf(ptr_GPGGA, "$GPGGA,%lf,%c%c%lf,%c,%c%c%c%lf,%c,%d", &utc, 
		&szlatdeg[0], &szlatdeg[1], &latmin, &north, 
		&szlongdeg[0], &szlongdeg[1], &szlongdeg[2], &longmin, &east,
		&GPS_quality_indicator) != 11)
	{
		printf("Error reading data from the weather station.\n");
		return EXIT_FAILURE;
	}

	if (GPS_quality_indicator == 0) 
	{
		printf("GPS fix not available or invalid.\n");
		return EXIT_FAILURE;
	}

	latdeg = atoi(szlatdeg);
	longdeg = atoi(szlongdeg);

	// Heading data.
	if (sscanf(ptr_HCHDG, "$HCHDG,%lf,%lf,%c,%lf,%c", 
		&heading, &deviation, &dev_east, &variation, &var_east) != 5)
	{
		printf("Error reading data from the weather station.\n");
		return EXIT_FAILURE;
	}

	// Wind data.
	if (sscanf(ptr_WIMDA, "$WIMDA,%lf,%c,%lf,%c,%lf,%c,,,,,,,%lf,%c,%lf,%c,%lf,%c,%lf,%c", 
		&f0, &c0, &pressure, &cpressure, &temperature, &ctemperature, &f1, &c1,  
		&winddir, &cwinddir, &f2, &c2, &windspeed, &cwindspeed) != 14)
	{
		printf("Error reading data from the weather station.\n");
		return EXIT_FAILURE;
	}

	// Convert GPS latitude and longitude in decimal.
	*pLatitude = (north == 'N')?(latdeg+latmin/60.0):-(latdeg+latmin/60.0);
	*pLongitude = (east == 'E')?(longdeg+longmin/60.0):-(longdeg+longmin/60.0);

	// Convert heading to angle in rad.
	*pHeading = heading*M_PI/180.0;

	// Convert wind direction to angle in rad.
	*pWindDir = winddir*M_PI/180.0;

	*pWindSpeed = windspeed; 

	return EXIT_SUCCESS;
}

int ReleaseWeatherStation(WEATHERSTATION* pWeatherStation)
{
	// Disconnect from server.
	if (releasetcpcli(pWeatherStation->s) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
