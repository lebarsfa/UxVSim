// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "SSC32Interface.h"
#include "PololuInterface.h"
#include "IM483IInterface.h"
#include "RazorAHRSInterface.h"
#include "MTInterface.h"
#include "NMEAInterface.h"
#include "OntrackInterface.h"
#include "MESInterface.h"
#include "ProbeInterface.h"
#include "Config.h"

int GetLastNewLineFromFile(FILE* file, char* line, int nbbytes)
{
	char buf[MAX_BUF_LEN];
	int bChanged = 0;

	if (nbbytes > MAX_BUF_LEN)
	{
		return EXIT_FAILURE;
	}

	// Wait for a new line of data in the file.
	memset(buf, 0, sizeof(buf));
	while (fgets(buf, sizeof(buf), file) != NULL)
	{
		if ((buf[0] == 0)||(buf[0] == '\n'))
		{
			memset(buf, 0, sizeof(buf));
			continue;
		}
		bChanged = 1;
		memcpy(line, buf, nbbytes);
		memset(buf, 0, sizeof(buf));
	}
	if (ferror(file) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}
	if (!bChanged)
	{
		return EXIT_NOT_CHANGED;
	}

	return EXIT_SUCCESS;
}

int GetLine_fwind(FILE* file, double* ptfile, double* pV, double* ppsi)
{
	char buf[MAX_BUF_LEN];	

	// Wait for a new line of data in the file.
	memset(buf, 0, sizeof(buf));
	while (fgets(buf, sizeof(buf), file) != NULL)
	{
		if ((buf[0] == 0)||(buf[0] == '\n'))
		{
			memset(buf, 0, sizeof(buf));
			continue;
		}
		if (sscanf(buf, "%lf;%lf;%lf\n", ptfile, pV, ppsi) != 3)
		{
			printf("fscanf() failed.\n");
#ifdef _DEBUG
			fprintf(stdout, "Press ENTER to continue . . . ");
			(void)getchar();
#endif // _DEBUG
			exit(EXIT_FAILURE);
		}
		memset(buf, 0, sizeof(buf));
	}
	if (ferror(file) != EXIT_SUCCESS)
	{
		printf("ferror() failed.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

int GetLine_fcurrent(FILE* file, double* ptfile, double* pVc, double* ppsic)
{
	char buf[MAX_BUF_LEN];	

	// Wait for a new line of data in the file.
	memset(buf, 0, sizeof(buf));
	while (fgets(buf, sizeof(buf), file) != NULL)
	{
		if ((buf[0] == 0)||(buf[0] == '\n'))
		{
			memset(buf, 0, sizeof(buf));
			continue;
		}
		if (sscanf(buf, "%lf;%lf;%lf\n", ptfile, pVc, ppsic) != 3)
		{
			printf("fscanf() failed.\n");
#ifdef _DEBUG
			fprintf(stdout, "Press ENTER to continue . . . ");
			(void)getchar();
#endif // _DEBUG
			exit(EXIT_FAILURE);
		}
		memset(buf, 0, sizeof(buf));
	}
	if (ferror(file) != EXIT_SUCCESS)
	{
		printf("ferror() failed.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

int GetLine_fwaves(FILE* file, double* ptfile, double* phw)
{
	char buf[MAX_BUF_LEN];	

	// Wait for a new line of data in the file.
	memset(buf, 0, sizeof(buf));
	while (fgets(buf, sizeof(buf), file) != NULL)
	{
		if ((buf[0] == 0)||(buf[0] == '\n'))
		{
			memset(buf, 0, sizeof(buf));
			continue;
		}
		if (sscanf(buf, "%lf;%lf\n", ptfile, phw) != 2)
		{
			printf("fscanf() failed.\n");
#ifdef _DEBUG
			fprintf(stdout, "Press ENTER to continue . . . ");
			(void)getchar();
#endif // _DEBUG
			exit(EXIT_FAILURE);
		}
		memset(buf, 0, sizeof(buf));
	}
	if (ferror(file) != EXIT_SUCCESS)
	{
		printf("ferror() failed.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

int GetLine_frudder(FILE* file, double* ptfile, double* pdeltag)
{
	char buf[MAX_BUF_LEN];	

	// Wait for a new line of data in the file.
	memset(buf, 0, sizeof(buf));
	while (fgets(buf, sizeof(buf), file) != NULL)
	{
		if ((buf[0] == 0)||(buf[0] == '\n'))
		{
			memset(buf, 0, sizeof(buf));
			continue;
		}
		if (sscanf(buf, "%lf;%lf\n", ptfile, pdeltag) != 2)
		{
			printf("fscanf() failed.\n");
#ifdef _DEBUG
			fprintf(stdout, "Press ENTER to continue . . . ");
			(void)getchar();
#endif // _DEBUG
			exit(EXIT_FAILURE);
		}
		memset(buf, 0, sizeof(buf));
	}
	if (ferror(file) != EXIT_SUCCESS)
	{
		printf("ferror() failed.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

int GetLine_fjrk(FILE* file, double* ptfile, double* pdeltag)
{
	char buf[MAX_BUF_LEN];	

	// Wait for a new line of data in the file.
	memset(buf, 0, sizeof(buf));
	while (fgets(buf, sizeof(buf), file) != NULL)
	{
		if ((buf[0] == 0)||(buf[0] == '\n'))
		{
			memset(buf, 0, sizeof(buf));
			continue;
		}
		if (sscanf(buf, "%lf;%lf\n", ptfile, pdeltag) != 2)
		{
			printf("fscanf() failed.\n");
#ifdef _DEBUG
			fprintf(stdout, "Press ENTER to continue . . . ");
			(void)getchar();
#endif // _DEBUG
			exit(EXIT_FAILURE);
		}
		memset(buf, 0, sizeof(buf));
	}
	if (ferror(file) != EXIT_SUCCESS)
	{
		printf("ferror() failed.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

int GetLine_fsail(FILE* file, double* ptfile, double* pdeltavmax)
{
	char buf[MAX_BUF_LEN];	

	// Wait for a new line of data in the file.
	memset(buf, 0, sizeof(buf));
	while (fgets(buf, sizeof(buf), file) != NULL)
	{
		if ((buf[0] == 0)||(buf[0] == '\n'))
		{
			memset(buf, 0, sizeof(buf));
			continue;
		}
		if (sscanf(buf, "%lf;%lf\n", ptfile, pdeltavmax) != 2)
		{
			printf("fscanf() failed.\n");
#ifdef _DEBUG
			fprintf(stdout, "Press ENTER to continue . . . ");
			(void)getchar();
#endif // _DEBUG
			exit(EXIT_FAILURE);
		}
		memset(buf, 0, sizeof(buf));
	}
	if (ferror(file) != EXIT_SUCCESS)
	{
		printf("ferror() failed.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

int main()
{
	THREAD_IDENTIFIER SSC32InterfaceThreadId;
	THREAD_IDENTIFIER PololuInterfaceThreadId;
	THREAD_IDENTIFIER IM483IInterfaceThreadId;
	THREAD_IDENTIFIER RazorAHRSInterfaceThreadId;
	THREAD_IDENTIFIER MTInterfaceThreadId;
	THREAD_IDENTIFIER NMEAInterfaceThreadId;
	THREAD_IDENTIFIER OntrackInterfaceThreadId;
	THREAD_IDENTIFIER MESInterfaceThreadId;
	THREAD_IDENTIFIER ProbeInterfaceThreadId;

	// Intermediate variables.
	double fg = 0, fv = 0, gamma = 0, deltav = 0, deltavmaxsimu = 0;

	double latitude = 0, longitude = 0, heading = 0, winddir = 0, windspeed = 0;
	double roll = 0, pitch = 0, yaw = 0.0;

	double t = 0, t0 = 0, dt = 0, dval = 0;
	CHRONO chrono;

	FILE* fwind = NULL;
	FILE* fcurrent = NULL;
	FILE* fwaves = NULL;
	FILE* frudder = NULL;
	FILE* fjrk = NULL;
	FILE* fsail = NULL;
	FILE* frazorahrs = NULL;
	FILE* fcompass = NULL;
	FILE* fweatherstation = NULL;
	FILE* fserialiointerface = NULL;
	FILE* fmes = NULL;
	FILE* fprobe = NULL;
	double tfile1 = 0, tfile2 = 0, tfile3 = 0, tfile4 = 0, tfile5 = 0;

	INIT_DEBUG;

#if !defined(_WIN32) && defined(ENABLE_VALGRIND_DEBUG)
	VALGRIND_DO_LEAK_CHECK;
#endif // !defined(_WIN32) && defined(ENABLE_VALGRIND_DEBUG)

	// Disable buffering for printf()...
	setbuf(stdout, NULL);

	srand(GetTickCount());

	InitNet();

	LoadConfig();

#ifdef _WIN32
	// Prevent display/system sleep...
	SetThreadExecutionState(ES_CONTINUOUS|ES_DISPLAY_REQUIRED);
	//SetThreadExecutionState(ES_CONTINUOUS|ES_SYSTEM_REQUIRED);
#else
#ifndef DISABLE_IGNORE_SIGPIPE
	// See https://stackoverflow.com/questions/17332646/server-dies-on-send-if-client-was-closed-with-ctrlc...
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		PRINT_DEBUG_WARNING(("signal failed. \n"));
	}
#endif // DISABLE_IGNORE_SIGPIPE
#endif // _WIN32

	if (bResetFiles)
	{
		fwind = fopen("SimWind.csv", "w");fclose(fwind);
		fcurrent = fopen("SimCurrent.csv", "w");fclose(fcurrent);
		fwaves = fopen("SimWaves.csv", "w");fclose(fwaves);
		frudder = fopen("SSC32Interface.csv", "w");fclose(frudder);
		fjrk = fopen("PololuInterface.csv", "w");fclose(fjrk);
		fsail = fopen("IM483IInterface.csv", "w");fclose(fsail);
		frazorahrs = fopen("RazorAHRSInterface.csv", "w");fclose(frazorahrs);
		fcompass = fopen("MTInterface.csv", "w");fclose(fcompass);
		fweatherstation = fopen("NMEAInterface.csv", "w");fclose(fweatherstation);
		fserialiointerface = fopen("OntrackInterface.csv", "w");fclose(fserialiointerface);
		fmes = fopen("MESInterface.csv", "w");fclose(fmes);
		fprobe = fopen("ProbeInterface.csv", "w");fclose(fprobe);
		printf("Simulated data files reset.\n");
	}

	if (bEmbeddedCom)
	{
		printf("Communication part with the embedded programs enabled.\n");
		//CreateDefaultThread(SSC32InterfaceThread, NULL, &SSC32InterfaceThreadId);
		CreateDefaultThread(PololuInterfaceThread, NULL, &PololuInterfaceThreadId);
		//CreateDefaultThread(IM483IInterfaceThread, NULL, &IM483IInterfaceThreadId);
		CreateDefaultThread(RazorAHRSInterfaceThread, NULL, &RazorAHRSInterfaceThreadId);
		//CreateDefaultThread(MTInterfaceThread, NULL, &MTInterfaceThreadId);
		CreateDefaultThread(NMEAInterfaceThread, NULL, &NMEAInterfaceThreadId);
		//CreateDefaultThread(OntrackInterfaceThread, NULL, &OntrackInterfaceThreadId);
		CreateDefaultThread(MESInterfaceThread, NULL, &MESInterfaceThreadId);
		//CreateDefaultThread(ProbeInterfaceThread, NULL, &ProbeInterfaceThreadId);
	}
	else
	{
		printf("Communication part with the embedded programs disabled.\n");
	}

	if (!bEnableSimulator)
	{
		printf("Built-in simulator disabled.\n");
		fprintf(stdout, "Press ENTER to continue . . . \n");
		(void)getchar();
		return EXIT_SUCCESS;
	}
	else
	{
		printf("Built-in simulator enabled.\n");
	}

	fwind = fopen("SimWind.csv", "r");
	fcurrent = fopen("SimCurrent.csv", "r");
	fwaves = fopen("SimWaves.csv", "r");
	frudder = fopen("SSC32Interface.csv", "r");
	fjrk = fopen("PololuInterface.csv", "r");
	fsail = fopen("IM483IInterface.csv", "r");
	frazorahrs = fopen("RazorAHRSInterface.csv", "a");
	fcompass = fopen("MTInterface.csv", "a");
	fweatherstation = fopen("NMEAInterface.csv", "a");
	if ((fwind == NULL)||(fcurrent == NULL)||(fwaves == NULL)||
		(frudder == NULL)||(fjrk == NULL)||(fsail == NULL)||(frazorahrs == NULL)||(fcompass == NULL)||(fweatherstation == NULL))
	{
		printf("fopen() failed.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if ((fseek(fwind, 0, SEEK_END) != EXIT_SUCCESS)||
		(fseek(fcurrent, 0, SEEK_END) != EXIT_SUCCESS)||
		(fseek(fwaves, 0, SEEK_END) != EXIT_SUCCESS)||
		(fseek(frudder, 0, SEEK_END) != EXIT_SUCCESS)||
		(fseek(fjrk, 0, SEEK_END) != EXIT_SUCCESS)||
		(fseek(fsail, 0, SEEK_END) != EXIT_SUCCESS))
	{
		printf("fseek() failed.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	t = 0;

	StartChrono(&chrono);

	sprintf(logsimfilename, "logsim_%s.csv", strtime_fns());

	logsimfile = fopen(logsimfilename, "w");
	if (logsimfile == NULL)
	{
		printf("Unable to create log file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fprintf(logsimfile, 
		"t (in s);x (in m);y (in m);theta (in rad);v (in m/s);omega (in rad/s);phi (in rad);phiPoint (in rad/s);"
		"deltag (in rad);deltavmax (in rad);V (in m/s);psi (in rad);Vc (in m/s);psic (in rad);hw (in m);"
		"fg;fv;gamma;deltav (in rad);deltavmaxsimu (in rad);"
		"roll (in rad);pitch (in rad);yaw (in rad);latitude (in decimal degrees);longitude (in decimal degrees);"
		"winddir (in rad);windspeed (in m/s);heading (in rad);\n"
		); 
	fflush(logsimfile);

	for (;;)
	{
		t0 = t;
		t = bRealTime?GetTimeElapsedChronoQuick(&chrono):t0+(double)loop_sleep_time/1000.0;
		dt = t-t0;

		// Inputs.
		V = V_med+V_var*(2.0*rand()/(double)RAND_MAX-1.0);
		psi = psi_med+psi_var*(2.0*rand()/(double)RAND_MAX-1.0);

		Vc = Vc_med+Vc_var*(2.0*rand()/(double)RAND_MAX-1.0);
		psic = psic_med+psic_var*(2.0*rand()/(double)RAND_MAX-1.0);

		hw = hw_var*(2.0*rand()/(double)RAND_MAX-1.0);

		GetLine_fwind(fwind, &tfile1, &V, &psi);
		GetLine_fcurrent(fcurrent, &tfile2, &Vc, &psic);
		GetLine_fwaves(fwaves, &tfile3, &hw);

		GetLine_frudder(frudder, &tfile4, &deltag);
		GetLine_fjrk(fjrk, &tfile4, &deltag);
		GetLine_fsail(fsail, &tfile5, &deltavmax);

		// State update.


		// Motorboat...

		fg = alphag*v*sin(deltag-0.0);
		fv = alphav*V;
		x += (v*cos(theta)+Vc*cos(psic))*dt;
		y += (v*sin(theta)+Vc*sin(psic))*dt;
		theta += omega*dt;
		omega += (1/Jz)*(rg*cos(deltag)*fg-alphatheta*omega)*dt;
		v     += (1/m)*(fv-sin(deltag)*fg-alphaf*v*v)*dt;
		phiPoint += (-alphaphi*phiPoint/Jx+fv*hv*cos(deltav)*cos(phi)/Jx-m*9.81*leq*sin(phi)/Jx)*dt;
		phi += phiPoint*dt;

		// Outputs.
		roll = fmod_2PI(phi+compass_error*(2.0*rand()/(double)RAND_MAX-1.0))*180.0/M_PI;
		pitch = fmod_2PI(0.0+compass_error*(2.0*rand()/(double)RAND_MAX-1.0))*180.0/M_PI;
		yaw = fmod_2PI(-theta+M_PI/2.0+compass_error*(2.0*rand()/(double)RAND_MAX-1.0)-M_PI/2.0)*180.0/M_PI;
		RefCoordSystem2GPS(lat0, long0, 0,
			x+gps_error*(2.0*rand()/(double)RAND_MAX-1.0), 
			y+gps_error*(2.0*rand()/(double)RAND_MAX-1.0), 
			0, 
			&latitude, &longitude, &dval, EAST_NORTH_UP_COORDINATE_SYSTEM);
		heading = (fmod_2PI(-((theta)-M_PI/2.0)+M_PI)+M_PI)*180.0/M_PI;
		winddir = (fmod_2PI(-(psi+M_PI-M_PI/2.0)+M_PI)+M_PI)*180.0/M_PI;
		windspeed = V;

		if ((fprintf(frazorahrs, "%f;%f;%f;%f;\n", t, roll, pitch, yaw) <= 0)|
			(fprintf(fcompass, "%f;%f;%f;%f;\n", t, roll, pitch, yaw) <= 0)|
			(fprintf(fweatherstation, "%f;%f;%f;%f;%f;%f;\n", 
			t, latitude, longitude, heading, winddir, windspeed) <= 0))
		{
			printf("fprintf() failed.\n");
#ifdef _DEBUG
			fprintf(stdout, "Press ENTER to continue . . . ");
			(void)getchar();
#endif // _DEBUG
			return EXIT_FAILURE;
		}
		if ((fflush(frazorahrs) != EXIT_SUCCESS)|(fflush(fcompass) != EXIT_SUCCESS)|(fflush(fweatherstation) != EXIT_SUCCESS))
		{
			printf("fflush() failed.\n");
#ifdef _DEBUG
			fprintf(stdout, "Press ENTER to continue . . . ");
			(void)getchar();
#endif // _DEBUG
			return EXIT_FAILURE;
		}

		//printf("%f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", 
		//	t, x, y, theta, v, phi, deltag, deltav, deltavmaxsimu, deltavmax);

		fprintf(logsimfile, "%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;\n", 
			t, x, y, theta, v, omega, phi, phiPoint, deltag, deltavmax, V, psi, Vc, psic, hw, 
			fg, fv, gamma, deltav, deltavmaxsimu, 
			roll, pitch, yaw, latitude, longitude, winddir, windspeed, heading);
		fflush(logsimfile);

		if (bRealTime) mSleep(loop_sleep_time);
	}

	fclose(logsimfile);

	if ((fclose(fweatherstation) != EXIT_SUCCESS)|(fclose(fcompass) != EXIT_SUCCESS)|(fclose(frazorahrs) != EXIT_SUCCESS)|
		(fclose(fsail) != EXIT_SUCCESS)|(fclose(fjrk) != EXIT_SUCCESS)|(fclose(frudder) != EXIT_SUCCESS)|
		(fclose(fwaves) != EXIT_SUCCESS)|(fclose(fcurrent) != EXIT_SUCCESS)|(fclose(fwind) != EXIT_SUCCESS))
	{
		printf("fclose() failed.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
