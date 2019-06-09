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

#include "OSNet.h"
#include "OSMisc.h"

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

union usShort_MT
{
	short v;
	unsigned char c[2];
};
typedef union usShort_MT usShort_MT;

union uShort_MT
{
	unsigned short v;
	unsigned char c[2];
};
typedef union uShort_MT uShort_MT;

union usInt_MT
{
	int v;
	unsigned char c[4];
};
typedef union usInt_MT usInt_MT;

union uInt_MT
{
	unsigned int v;
	unsigned char c[4];
};
typedef union uInt_MT uInt_MT;

union uFloat_MT
{
	float v;
	unsigned char c[4];
};
typedef union uFloat_MT uFloat_MT;

union uDouble_MT
{
	double v;
	unsigned char c[8];
};
typedef union uDouble_MT uDouble_MT;

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

inline int ConvertToDoubleMT(int OutputSettings, unsigned char* buf, int offset, double* pValue)
{
	usInt_MT usl;
	usShort_MT uss;
	uInt_MT ul;
	LARGE_INTEGER li;
	uFloat_MT uf;

	switch (OutputSettings & OUTPUT_FORMAT_MASK)
	{
	case OUTPUT_FORMAT_FIXED_POINT_SIGNED_1220:
		usl.c[0] = buf[3+offset];
		usl.c[1] = buf[2+offset];
		usl.c[2] = buf[1+offset];
		usl.c[3] = buf[0+offset];
		*pValue = (double)usl.v/1048576.0;
		return offset+4;
	case OUTPUT_FORMAT_FIXED_POINT_SIGNED_1632:
		uss.c[0] = buf[5+offset];
		uss.c[1] = buf[4+offset];
		ul.c[0] = buf[3+offset];
		ul.c[1] = buf[2+offset];
		ul.c[2] = buf[1+offset];
		ul.c[3] = buf[0+offset];
		li.HighPart = (long)uss.v;
		li.LowPart = ul.v;
		*pValue = (double)li.QuadPart/4294967296.0;
		return offset+6;
	default:
		uf.c[0] = buf[3+offset];
		uf.c[1] = buf[2+offset];
		uf.c[2] = buf[1+offset];
		uf.c[3] = buf[0+offset];
		*pValue = (double)uf.v;
		return offset+4;
	}
}

inline void SetMTChecksum(unsigned char* buf, int buflen)
{
	int i = 0;
	uInt_MT checksum;

	// If all message bytes excluding the preamble are summed and the lower byte value 
	// of the result equals zero, the message is valid and it may be processed. The 
	// checksum value of the message should be included in the summation.
	checksum.v = 0;
	for (i = 1; i < buflen-1; i++)
	{
		checksum.v += buf[i];
	}
	buf[buflen-1] = (unsigned char)(256-(int)checksum.c[0]);
}

inline int CheckMTChecksum(unsigned char* buf, int buflen)
{
	int i = 0;
	uInt_MT checksum;

	// If all message bytes excluding the preamble are summed and the lower byte value 
	// of the result equals zero, the message is valid and it may be processed. The 
	// checksum value of the message should be included in the summation.
	checksum.v = 0;
	for (i = 1; i < buflen; i++)
	{
		checksum.v += buf[i];
	}

	if (checksum.c[0] != 0)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

inline int AnalyzeMTMessage(unsigned char* buf, int buflen, int addr, int mid)
{
	// Check number of bytes.
	if (buflen < MIN_STANDARD_BUF_LEN_COMPASS)
	{
		//printf("Invalid number of bytes.\n");
		return EXIT_FAILURE;
	}
	// Check preamble.
	if (buf[0] != PREAMBLE_COMPASS)
	{
		//printf("Invalid preamble.\n");
		return EXIT_FAILURE;
	}
	// Check device address.
	if (buf[1] != (unsigned char)addr)
	{
		//printf("Invalid device address.\n");
		return EXIT_FAILURE;
	}
	// Check message identifier.
	if (buf[2] != (unsigned char)mid)
	{
		//printf("Invalid message identifier.\n");
		return EXIT_FAILURE;
	}
	// Check number of data bytes.
	if (buf[3] == EXT_LEN_COMPASS)
	{
		//printf("Unhandled number of data bytes.\n");
		return EXIT_FAILURE;
	}
	if (buflen < buf[3]+MIN_STANDARD_BUF_LEN_COMPASS)
	{
		//printf("Invalid number of data bytes.\n");
		return EXIT_FAILURE;
	}
	// Checksum.
	if (CheckMTChecksum(buf, buf[3]+MIN_STANDARD_BUF_LEN_COMPASS) != EXIT_SUCCESS)
	{ 
		//printf("Invalid checksum.\n");
		return EXIT_FAILURE;	
	}

	return EXIT_SUCCESS;
}

inline int FindMTMessage(unsigned char* buf, int buflen, int addr, int mid, unsigned char** pFoundmsg, int* pFoundmsglen)
{
	*pFoundmsg = buf;
	*pFoundmsglen = buflen;

	while (AnalyzeMTMessage(*pFoundmsg, *pFoundmsglen, addr, mid) != EXIT_SUCCESS)
	{
		(*pFoundmsg)++;
		(*pFoundmsglen)--;
		if (*pFoundmsglen < MIN_STANDARD_BUF_LEN_COMPASS)
		{
			*pFoundmsg = NULL;
			*pFoundmsglen = 0;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

inline int FindLatestMTMessage(unsigned char* buf, int buflen, int addr, int mid, unsigned char** pFoundmsg, int* pFoundmsglen)
{
	unsigned char* ptr = NULL;
	int len = 0;

	if (FindMTMessage(buf, buflen, addr, mid, &ptr, &len) != EXIT_SUCCESS)
	{
		*pFoundmsg = NULL;
		*pFoundmsglen = 0;
		return EXIT_FAILURE;
	}
	for (;;) 
	{
		// Save the position of the beginning of the message.
		*pFoundmsg = ptr;
		*pFoundmsglen = len;

		// Search just after the message.
		if (FindMTMessage(*pFoundmsg+((*pFoundmsg)[3]+MIN_STANDARD_BUF_LEN_COMPASS), *pFoundmsglen-((*pFoundmsg)[3]+MIN_STANDARD_BUF_LEN_COMPASS), 
			addr, mid, &ptr, &len) != EXIT_SUCCESS)
		{
			break;
		}
	}

	return EXIT_SUCCESS;
}

inline int GetMTMessageCompass(COMPASS* pCompass, int addr, int mid, unsigned char* databuf, int databuflen, int* pNbdatabytes)
{
	char recvbuf[MAX_NB_BYTES_COMPASS];
	char savebuf[MAX_NB_BYTES_COMPASS];
	int BytesReceived = 0, Bytes = 0, recvbuflen = 0;
	unsigned char* ptr = NULL;
	int len = 0;

	// Prepare the buffers.
	memset(recvbuf, 0, sizeof(recvbuf));
	memset(savebuf, 0, sizeof(savebuf));
	recvbuflen = MAX_NB_BYTES_COMPASS-1; // The last character must be a 0 to be a valid string for sscanf.
	BytesReceived = 0;

	// As the sensor always sends data, any old data must be discarded.
	Bytes = recv(pCompass->s, recvbuf, recvbuflen, 0);
	if (Bytes <= 0)
	{
		printf("recv() failed or timed out.\n");
		return EXIT_FAILURE;
	}
	BytesReceived += Bytes;

	while (Bytes == recvbuflen)
	{
		memcpy(savebuf, recvbuf, Bytes);

		Bytes = recv(pCompass->s, recvbuf, recvbuflen, 0);
		if (Bytes <= 0)
		{
			printf("recv() failed or timed out.\n");
			return EXIT_FAILURE;
		}
		BytesReceived += Bytes;
	}

	if (BytesReceived < recvbuflen)
	{
		while (FindLatestMTMessage((unsigned char*)recvbuf, recvbuflen, addr, mid, &ptr, &len) != EXIT_SUCCESS)
		{
			if (BytesReceived >= recvbuflen) return EXIT_FAILURE;

			Bytes = recv(pCompass->s, recvbuf+BytesReceived, recvbuflen-BytesReceived, 0);
			if (Bytes <= 0)
			{
				printf("recv() failed or timed out.\n");
				return EXIT_FAILURE;
			}
			BytesReceived += Bytes;
		}
	}
	else
	{
		memmove(recvbuf+recvbuflen-Bytes, recvbuf, Bytes);
		memcpy(recvbuf, savebuf+Bytes, recvbuflen-Bytes);

		if (FindLatestMTMessage((unsigned char*)recvbuf, recvbuflen, addr, mid, &ptr, &len) != EXIT_SUCCESS) return EXIT_FAILURE;
	}

	// Check number of data bytes before copy.
	if (databuflen < ptr[3])
	{
		printf("Too small data buffer.\n");
		return EXIT_FAILURE;
	}

	// Copy the data bytes of the message.
	if (ptr[3] > 0)
	{
		*pNbdatabytes = ptr[3];
		memcpy(databuf, ptr+4, *pNbdatabytes);
	}

	return EXIT_SUCCESS;
}

inline int InitCompass(COMPASS* pCompass)
{
	FILE* file = NULL;
	char line[MAX_BUF_LEN];
	unsigned char gotoconfigbuf[] = {PREAMBLE_COMPASS,ADDR_COMPASS,GO_TO_CONFIG_MID,0x00,0x00};
	unsigned char reqconfigurationbuf[] = {PREAMBLE_COMPASS,ADDR_COMPASS,REQ_CONFIGURATION_MID,0x00,0x00};
	unsigned char gotomeasurementbuf[] = {PREAMBLE_COMPASS,ADDR_COMPASS,GO_TO_MEASUREMENT_MID,0x00,0x00};
	unsigned char databuf[MAX_BUF_LEN];
	int nbdatabytes = 0;
	uShort_MT OutputMode;
	uInt_MT OutputSettings;

	// Default values.
	memset(line, 0, sizeof(line));
	memset(pCompass->address, 0, sizeof(pCompass->address));
	sprintf(pCompass->address, IP_ADDR_COMPASS);
	memset(pCompass->port, 0, sizeof(pCompass->port));
	sprintf(pCompass->port, TCP_PORT_COMPASS);
	pCompass->rollorientation = ROLL_ORIENTATION_COMPASS;
	pCompass->rollp1 = ROLL_P1_COMPASS;
	pCompass->rollp2 = ROLL_P2_COMPASS;
	pCompass->pitchorientation = PITCH_ORIENTATION_COMPASS;
	pCompass->pitchp1 = PITCH_P1_COMPASS;
	pCompass->pitchp2 = PITCH_P2_COMPASS;
	pCompass->yaworientation = YAW_ORIENTATION_COMPASS;
	pCompass->yawp1 = YAW_P1_COMPASS;
	pCompass->yawp2 = YAW_P2_COMPASS;

	// Load data from a file.
	file = fopen("Compass.txt", "r");
	if (file != NULL)
	{
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%32s", pCompass->address) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%32s", pCompass->port) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &pCompass->rollorientation) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &pCompass->rollp1) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &pCompass->rollp2) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &pCompass->pitchorientation) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &pCompass->pitchp1) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &pCompass->pitchp2) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &pCompass->yaworientation) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &pCompass->yawp1) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &pCompass->yawp2) != 1) printf("Invalid configuration file.\n");
		if (fclose(file) != EXIT_SUCCESS) printf("fclose() failed.\n");
	}
	else
	{
		printf("Configuration file not found.\n");
	}

	// Connect to server.
	if (inittcpcli(&pCompass->s, pCompass->address, pCompass->port) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	// Go to configuration state.
	SetMTChecksum(gotoconfigbuf, sizeof(gotoconfigbuf));
	if (sendall(pCompass->s, (char*)gotoconfigbuf, sizeof(gotoconfigbuf)) != EXIT_SUCCESS)
	{
		releasetcpcli(pCompass->s);
		return EXIT_FAILURE;
	}

	memset(databuf, 0, sizeof(databuf));
	nbdatabytes = 0;
	if (GetMTMessageCompass(pCompass, ADDR_COMPASS, GO_TO_CONFIG_ACK_MID, databuf, sizeof(databuf), &nbdatabytes)
		!= EXIT_SUCCESS)
	{ 
		releasetcpcli(pCompass->s);
		return EXIT_FAILURE;	
	}

	// Request the current configuration.
	SetMTChecksum(reqconfigurationbuf, sizeof(reqconfigurationbuf));
	if (sendall(pCompass->s, (char*)reqconfigurationbuf, sizeof(reqconfigurationbuf)) != EXIT_SUCCESS)
	{
		releasetcpcli(pCompass->s);
		return EXIT_FAILURE;
	}

	memset(databuf, 0, sizeof(databuf));
	nbdatabytes = 0;
	if (GetMTMessageCompass(pCompass, ADDR_COMPASS, CONFIGURATION_MID, databuf, sizeof(databuf), &nbdatabytes)
		!= EXIT_SUCCESS)
	{ 
		releasetcpcli(pCompass->s);
		return EXIT_FAILURE;	
	}

	// Analyze the configuration returned.
	OutputMode.c[0] = databuf[105];
	OutputMode.c[1] = databuf[104];
	OutputSettings.c[0] = databuf[109];
	OutputSettings.c[1] = databuf[108];
	OutputSettings.c[2] = databuf[107];
	OutputSettings.c[3] = databuf[106];

	pCompass->OutputMode = OutputMode.v;
	pCompass->OutputSettings = OutputSettings.v;

	// Go to measurement state.
	SetMTChecksum(gotomeasurementbuf, sizeof(gotomeasurementbuf));
	if (sendall(pCompass->s, (char*)gotomeasurementbuf, sizeof(gotomeasurementbuf)) != EXIT_SUCCESS)
	{
		releasetcpcli(pCompass->s);
		return EXIT_FAILURE;
	}

	memset(databuf, 0, sizeof(databuf));
	nbdatabytes = 0;
	if (GetMTMessageCompass(pCompass, ADDR_COMPASS, GO_TO_MEASUREMENT_ACK_MID, databuf, sizeof(databuf), &nbdatabytes)
		!= EXIT_SUCCESS)
	{ 
		releasetcpcli(pCompass->s);
		return EXIT_FAILURE;	
	}

	return EXIT_SUCCESS;
}

inline int GetMTDataCompass(COMPASS* pCompass, MTDATA* pMTData)
{
	unsigned char databuf[MAX_BUF_LEN];
	int nbdatabytes = 0;
	int offset = 0;
	uShort_MT us;
	//uInt_MT ul;

	memset(databuf, 0, sizeof(databuf));
	nbdatabytes = 0;
	if (GetMTMessageCompass(pCompass, ADDR_COMPASS, MTDATA_MID, databuf, sizeof(databuf), &nbdatabytes)
		!= EXIT_SUCCESS)
	{ 
		return EXIT_FAILURE;	
	}

	// Analyze data.
	if (pCompass->OutputMode & RAW_BIT)
	{
		// Un-calibrated RAW inertial data output mode.
		offset += 20;
	}

	if (pCompass->OutputMode & GPS_PVT_BIT)
	{
		// GPS PVT data output mode.
		offset += 44;
	}

	if (pCompass->OutputMode & TEMPERATURE_BIT)
	{
		// Temperature data output mode.
		offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->temp);
	}

	if (pCompass->OutputMode & CALIBRATED_BIT)
	{
		// Calibrated data output mode.
		if ((pCompass->OutputSettings & ACCELERATION) == 0)
		{
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->accX);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->accY);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->accZ);
		}
		if ((pCompass->OutputSettings & RATE_OF_TURN) == 0)
		{
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->gyrX);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->gyrY);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->gyrZ);
		}
		if ((pCompass->OutputSettings & MAGNETOMETER) == 0)
		{
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->magX);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->magY);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->magZ);
		}
	}

	if (pCompass->OutputMode & ORIENTATION_BIT)
	{
		switch (pCompass->OutputSettings & ORIENTATION_MODE_MASK)
		{
		case EULER_ANGLES:
			// Orientation data output mode - Euler angles.
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->roll);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->pitch);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->yaw);
			break;
		case MATRIX:
			// Orientation data output mode - Matrix.
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->a);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->b);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->c);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->d);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->e);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->f);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->g);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->h);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->i);
			break;
		default:
			// Orientation data output mode - Quaternion.
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->q0);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->q1);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->q2);
			offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->q3);
			break;
		}
	}

	if (pCompass->OutputMode & AUXILIARY_BIT)
	{
		// Auxiliary data output mode.
		if ((pCompass->OutputSettings & ANALOG_1) == 0)
		{
			us.c[0] = databuf[1+offset];
			us.c[1] = databuf[0+offset];
			offset += 2;
			pMTData->Ain_1 = us.v;
		}
		if ((pCompass->OutputSettings & ANALOG_2) == 0)
		{
			us.c[0] = databuf[1+offset];
			us.c[1] = databuf[0+offset];
			offset += 2;
			pMTData->Ain_2 = us.v;
		}
	}

	if (pCompass->OutputMode & POSITION_BIT)
	{
		// Position data output mode - LLA.
		offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->Lat);
		offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->Long);
		offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->Alt);
	}

	if (pCompass->OutputMode & VELOCITY_BIT)
	{
		// Velocity data output mode - VelXYZ.
		offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->Vel_X);
		offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->Vel_Y);
		offset = ConvertToDoubleMT(pCompass->OutputSettings, databuf, offset, &pMTData->Vel_Z);
	}

	if (pCompass->OutputMode & STATUS_BIT)
	{
		// Status (1 byte).
		offset += 1;
	}

	if (pCompass->OutputSettings & SAMPLE_COUNTER)
	{
		// Sample counter (2 bytes).
		us.c[0] = databuf[1+offset];
		us.c[1] = databuf[0+offset];
		offset += 2;
		pMTData->TS = us.v;
	}
	if (pCompass->OutputSettings & UTC_TIME)
	{
		// UTC Time (12 bytes).
		offset += 12;
	}

	pCompass->LastMTData = *pMTData;

	return EXIT_SUCCESS;
}

inline int GetAnglesCompass(COMPASS* pCompass, double* pRoll, double* pPitch, double* pYaw)
{
	MTDATA MTData;
	double roll = 0.0, pitch = 0.0, yaw = 0.0;

	memset(&MTData, 0, sizeof(MTDATA));	
	if (GetMTDataCompass(pCompass, &MTData) != EXIT_SUCCESS)
	{ 
		return EXIT_FAILURE;	
	}

	if (pCompass->OutputMode & ORIENTATION_BIT)
	{
		switch (pCompass->OutputSettings & ORIENTATION_MODE_MASK)
		{
		case EULER_ANGLES:
			// Orientation data output mode - Euler angles.
			roll = MTData.roll*M_PI/180.0;
			pitch = MTData.pitch*M_PI/180.0;
			yaw = MTData.yaw*M_PI/180.0;
			break;
		case MATRIX:
			// Orientation data output mode - Matrix.
			roll = atan2(MTData.f,MTData.i);
			pitch = -asin(MTData.c);
			yaw = atan2(MTData.b,MTData.a);
			break;
		default:
			// Orientation data output mode - Quaternion.
			roll = atan2(2*MTData.q2*MTData.q3+2*MTData.q0*MTData.q1,2*sqr(MTData.q0)+2*sqr(MTData.q3)-1);
			pitch = -asin(2*MTData.q1*MTData.q3-2*MTData.q0*MTData.q2);
			yaw = atan2(2*MTData.q1*MTData.q2+2*MTData.q0*MTData.q3,2*sqr(MTData.q0)+2*sqr(MTData.q1)-1);
			break;
		}
	}

	// Apply corrections (magnetic, orientation of the sensor w.r.t. coordinate system...).
	*pRoll = fmod_2PI(roll+pCompass->rollorientation+pCompass->rollp1*cos(roll+pCompass->rollp2));
	*pPitch = fmod_2PI(pitch+pCompass->pitchorientation+pCompass->pitchp1*cos(pitch+pCompass->pitchp2));
	*pYaw = fmod_2PI(yaw+pCompass->yaworientation+pCompass->yawp1*cos(yaw+pCompass->yawp2));

	return EXIT_SUCCESS;
}

inline int ReleaseCompass(COMPASS* pCompass)
{
	// Disconnect from server.
	if (releasetcpcli(pCompass->s) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

#endif // COMPASS_H
