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

#include "OSNet.h"
#include "OSMisc.h"

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


inline char* FindNMEASentence(char sentencebegin[7], char* str)
{
	char* ptr = 0;
	char* foundstr = 0;

	ptr = strstr(str, sentencebegin);
	if (!ptr)
	{
		// Could not find the beginning of the sentence.
		return 0;
	}

	// Save the position of the beginning of the sentence.
	foundstr = ptr;

	// Check if the sentence is complete.
	ptr = strstr(foundstr+strlen(sentencebegin), "\r");
	if (!ptr)
	{
		// The sentence is incomplete.
		return 0;
	}

	return foundstr;
}

inline char* FindLatestNMEASentence(char sentencebegin[7], char* str)
{
	char* ptr = 0;
	char* foundstr = 0;

	ptr = FindNMEASentence(sentencebegin, str);
	while (ptr) 
	{
		// Save the position of the beginning of the sentence.
		foundstr = ptr;

		// Search just after the beginning of the sentence.
		ptr = FindNMEASentence(sentencebegin, foundstr+strlen(sentencebegin));
	}

	return foundstr;
}

inline void ComputeNMEAchecksum(char* sentence, char checksum[4])
{
	int i = 0;
	char res = 0;

	memset(checksum, 0, 4);
	while (sentence[i])
	{
		if (sentence[i] == '$')
		{
			i++;
			continue;
		}
		if (sentence[i] == '*')
		{
			break;
		}
		res ^= sentence[i];
		i++;
	}

	sprintf(checksum, "*%02X", (int)res);
}


int InitWeatherStation(WEATHERSTATION* pWeatherStation);
int GetDataWeatherStation(WEATHERSTATION* pWeatherStation, double* pLatitude, double* pLongitude, 
						  double* pWindDir, double* pWindSpeed, double* pHeading);
int ReleaseWeatherStation(WEATHERSTATION* pWeatherStation);

#endif // WEATHERSTATION_H
