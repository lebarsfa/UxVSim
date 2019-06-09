// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "Globals.h"

DWORD WINAPI SimRudderThread(void* pParam)
{
	UNREFERENCED_PARAMETER(pParam);

	if (LaunchSingleCliTCPSrv("4004", handleclirudder, NULL) != EXIT_SUCCESS)
	{
		exit(EXIT_FAILURE);
	}

	return 0;
}

DWORD WINAPI SimSailThread(void* pParam)
{
	UNREFERENCED_PARAMETER(pParam);

	if (LaunchSingleCliTCPSrv("4003", handleclisail, NULL) != EXIT_SUCCESS)
	{
		exit(EXIT_FAILURE);
	}

	return 0;
}

DWORD WINAPI SimCompassThread(void* pParam)
{
	UNREFERENCED_PARAMETER(pParam);

	if (LaunchSingleCliTCPSrv("4007", handleclicompass, NULL) != EXIT_SUCCESS)
	{
		exit(EXIT_FAILURE);
	}

	return 0;
}

DWORD WINAPI SimWeatherStationThread(void* pParam)
{
	UNREFERENCED_PARAMETER(pParam);

	if (LaunchSingleCliTCPSrv("4001", handlecliweatherstation, NULL) != EXIT_SUCCESS)
	{
		exit(EXIT_FAILURE);
	}

	return 0;
}

DWORD WINAPI SimSerialIOInterfaceThread(void* pParam)
{
	UNREFERENCED_PARAMETER(pParam);

	if (LaunchSingleCliTCPSrv("4002", handlecliserialiointerface, NULL) != EXIT_SUCCESS)
	{
		exit(EXIT_FAILURE);
	}

	return 0;
}

DWORD WINAPI SimProbeThread(void* pParam)
{
	UNREFERENCED_PARAMETER(pParam);

	if (LaunchSingleCliTCPSrv("4008", handlecliprobe, NULL) != EXIT_SUCCESS)
	{
		exit(EXIT_FAILURE);
	}

	return 0;
}

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

int LoadConfig()
{
	FILE* file = NULL;
	char line[MAX_BUF_LEN];

	// Default values.
	bEnableSimulator = 1;
	bEmbeddedCom = 1;
	bResetFiles = 1;
	bRealTime = 1;

	loop_sleep_time = 20; // Time step (in ms).

	// Reference GPS position (in decimal degrees).
	//lat0 = 48.3920; long0 = -4.4280; // Coordonnées GPS de la sortie du port du Moulin Blanc.
	lat0 = 48.3904; long0 = -4.4245;	

	// State variables.
	x = 0.0; y = 0.0; theta = 3*M_PI/8; v = 0.5; omega = -0.02; phi = 0.02; phiPoint = 0.0; 
	deltag = 0.0; deltavmax = 0.0;

	// Longueur totale de la coque 3.65 m.
	// Largeur totale de la coque 0.86 m.

	// 130 -> 3.65
	// 70 -> rg+rv=2
	// 170 -> longueur du mat=4.77
	// 150*50 -> longueur*largeur grande voile=4.21*1.40
	// 80*30 -> longueur*largeur petite voile=2.25*0.84
	// 80 -> distance centre de poussée à G=2.25

	// Parameters.
	m = 200.0; // Mass (en kg).
	Jx = 12.5; Jz = 50.0; // Inertial moments.
	rg = 1.5; // Distance from the rudder to G (in m).
	rv = 0.5; // Distance from the mast to G (in m).
	l = 0.5; // Distance from the sail center of pressure to the mast (in m).
	alphatheta = 125.0; // Yaw friction coefficient.
	alphaf = 20.0; // Forward friction coefficient.
	alphag = 75.0; // Rudder force coefficient.
	alphav = 30.0; // Sail force coefficient.
	alphaphi = 40.0; // Roll friction coefficient.
	alphaw = 50.0; // Perturbations due to waves coefficient.
	hv = 2.0; // Distance from the sail center of pressure to G (in m).
	leq = 0.2; // Length of the equivalent pendulum in roll (in m).

	deltavminreal = 0.30; // Min equivalent opening angle of the real sail (in rad).
	deltavmaxreal = 1.20; // Max equivalent opening angle of the real sail (en rad).

	// Wind.
	V_med = 7.0; // Wind speed (in m/s).
	V_var = 3.0; // Wind speed max variation (in m/s).
	psi_med = 3*M_PI/2; // Wind direction (in rad).
	psi_var = M_PI/8.0; // Wind direction max variation (in rad).
	beta = 0.01; // Current due to the wind (fraction of the wind speed).

	// Current.
	Vc_med = 0.1; // Current speed (in m/s).
	Vc_var = 0.1; // Current speed max variation (in m/s).
	psic_med = 0.0*M_PI/2; // Current direction (in rad).
	psic_var = 1.0*M_PI/8.0; // Current direction max variation (in rad).

	// Waves.
	hw_var = 0.75; // Amplitude of the waves (in m).

	// Measurement errors.
	gps_error = 0.5; // GPS error (in m).
	compass_error = 0.1; // Compass error (in rad).

	// Load data from a file.
	file = fopen("VSim.txt", "r");
	if (file != NULL)
	{
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bEnableSimulator) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bEmbeddedCom) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bResetFiles) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bRealTime) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &loop_sleep_time) != 1) printf("Invalid configuration file.\n");

		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &lat0) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &long0) != 1) printf("Invalid configuration file.\n");
		// State variables.
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &x) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &y) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &theta) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &v) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &omega) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &phi) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &phiPoint) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &deltag) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &deltavmax) != 1) printf("Invalid configuration file.\n");
		// Parameters.
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &m) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &Jx) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &Jz) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &rg) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &rv) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &l) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &alphatheta) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &alphaf) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &alphag) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &alphav) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &alphaphi) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &alphaw) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &hv) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &leq) != 1) printf("Invalid configuration file.\n");

		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &deltavminreal) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &deltavmaxreal) != 1) printf("Invalid configuration file.\n");
		// Wind.
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &V_med) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &V_var) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &psi_med) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &psi_var) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &beta) != 1) printf("Invalid configuration file.\n");
		// Current.
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &Vc_med) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &Vc_var) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &psic_med) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &psic_var) != 1) printf("Invalid configuration file.\n");
		// Waves.	
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &hw_var) != 1) printf("Invalid configuration file.\n");
		// Measurement errors.	
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &gps_error) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &compass_error) != 1) printf("Invalid configuration file.\n");

		if (fclose(file) != EXIT_SUCCESS) printf("fclose() failed.\n");
	}
	else
	{
		printf("Configuration file not found.\n");
	}

	return EXIT_SUCCESS;
}

int main()
{
	// Intermediate variables.
	double fg = 0, fv = 0, gamma = 0, deltav = 0, deltavmaxsimu = 0;

	double latitude = 0, longitude = 0, heading = 0, winddir = 0, windspeed = 0;
	double roll = 0, pitch = 0, yaw = 0.0;

	double t = 0, t0 = 0, dt = 0;
	CHRONO chrono;

	FILE* fwind = NULL;
	FILE* fcurrent = NULL;
	FILE* fwaves = NULL;
	FILE* frudder = NULL;
	FILE* fsail = NULL;
	FILE* fcompass = NULL;
	FILE* fweatherstation = NULL;
	FILE* fserialiointerface = NULL;
	FILE* fprobe = NULL;
	double tfile1 = 0, tfile2 = 0, tfile3 = 0, tfile4 = 0, tfile5 = 0;

	LoadConfig();

	if (bResetFiles)
	{
		fwind = fopen("SimWind.csv", "w");fclose(fwind);
		fcurrent = fopen("SimCurrent.csv", "w");fclose(fcurrent);
		fwaves = fopen("SimWaves.csv", "w");fclose(fwaves);
		frudder = fopen("SimRudder.csv", "w");fclose(frudder);
		fsail = fopen("SimSail.csv", "w");fclose(fsail);
		fcompass = fopen("SimCompass.csv", "w");fclose(fcompass);
		fweatherstation = fopen("SimWeatherStation.csv", "w");fclose(fweatherstation);
		fserialiointerface = fopen("SimSerialIOInterface.csv", "w");fclose(fserialiointerface);
		fprobe = fopen("SimProbe.csv", "w");fclose(fprobe);
		printf("Simulated data files reset.\n");
	}

	if (bEmbeddedCom)
	{
		printf("Communication part with the embedded programs enabled.\n");
		CreateThread(NULL, 0, SimRudderThread, NULL, 0, NULL);
		CreateThread(NULL, 0, SimSailThread, NULL, 0, NULL);
		CreateThread(NULL, 0, SimCompassThread, NULL, 0, NULL);
		CreateThread(NULL, 0, SimWeatherStationThread, NULL, 0, NULL);
		CreateThread(NULL, 0, SimSerialIOInterfaceThread, NULL, 0, NULL);
		CreateThread(NULL, 0, SimProbeThread, NULL, 0, NULL);
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
	frudder = fopen("SimRudder.csv", "r");
	fsail = fopen("SimSail.csv", "r");
	fcompass = fopen("SimCompass.csv", "a");
	fweatherstation = fopen("SimWeatherStation.csv", "a");
	if ((fwind == NULL)||(fcurrent == NULL)||(fwaves == NULL)||
		(frudder == NULL)||(fsail == NULL)||(fcompass == NULL)||(fweatherstation == NULL))
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
		GetLine_fsail(fsail, &tfile5, &deltavmax);

		// State update.
		// The model is described in "L. Jaulin Modélisation et commande d'un bateau à voile, CIFA2004, Douz (Tunisie)".

		deltavmaxsimu = deltavminreal+deltavmax*(deltavmaxreal-deltavminreal)/(M_PI/2.0);

		gamma=cos(theta-psi)+cos(deltavmaxsimu);
		if (gamma<0) deltav=M_PI-theta+psi; // Voile en drapeau.
		else if (sin(theta-psi)>0) deltav=deltavmaxsimu; else deltav=-deltavmaxsimu;
		fg = alphag*v*sin(deltag);
		fv = alphav*V*sin(theta+deltav-psi);
		x += (v*cos(theta)+beta*V*cos(psi)+Vc*cos(psic))*dt;
		y += (v*sin(theta)+beta*V*sin(psi)+Vc*sin(psic))*dt;
		theta += omega*dt;
		omega += (1/Jz)*((l-rv*cos(deltav))*fv-rg*cos(deltag)*fg-alphatheta*omega+alphaw*hw)*dt;
		v     += (1/m)*(sin(deltav)*fv-sin(deltag)*fg-alphaf*v*v)*dt;
		phiPoint += (-alphaphi*phiPoint/Jx+fv*hv*cos(deltav)*cos(phi)/Jx-m*9.81*leq*sin(phi)/Jx)*dt;
		phi += phiPoint*dt;

		// Outputs.
		roll = fmod_2PI(phi+compass_error*(2.0*rand()/(double)RAND_MAX-1.0))*180.0/M_PI;
		pitch = fmod_2PI(0.0+compass_error*(2.0*rand()/(double)RAND_MAX-1.0))*180.0/M_PI;
		yaw = fmod_2PI(theta+compass_error*(2.0*rand()/(double)RAND_MAX-1.0)-M_PI/2.0)*180.0/M_PI;
		RefCoordSystem2GPS(lat0, long0, 
			x+gps_error*(2.0*rand()/(double)RAND_MAX-1.0), 
			y+gps_error*(2.0*rand()/(double)RAND_MAX-1.0), 
			&latitude, &longitude);
		heading = (fmod_2PI(-((theta+deltav)-M_PI/2.0)+M_PI)+M_PI)*180.0/M_PI;
		winddir = (fmod_2PI(-(psi+M_PI-M_PI/2.0)+M_PI)+M_PI)*180.0/M_PI;
		windspeed = V;

		if ((fprintf(fcompass, "%f;%f;%f;%f;\n", t, roll, pitch, yaw) <= 0)|
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
		if ((fflush(fcompass) != EXIT_SUCCESS)|(fflush(fweatherstation) != EXIT_SUCCESS))
		{
			printf("fflush() failed.\n");
#ifdef _DEBUG
			fprintf(stdout, "Press ENTER to continue . . . ");
			(void)getchar();
#endif // _DEBUG
			return EXIT_FAILURE;
		}

		printf("%f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", 
			t, x, y, theta, v, phi, deltag, deltav, deltavmaxsimu, deltavmax);

		fprintf(logsimfile, "%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;\n", 
			t, x, y, theta, v, omega, phi, phiPoint, deltag, deltavmax, V, psi, Vc, psic, hw, 
			fg, fv, gamma, deltav, deltavmaxsimu, 
			roll, pitch, yaw, latitude, longitude, winddir, windspeed, heading);
		fflush(logsimfile);

		if (bRealTime) mSleep(loop_sleep_time);
	}

	fclose(logsimfile);

	if ((fclose(fweatherstation) != EXIT_SUCCESS)|(fclose(fcompass) != EXIT_SUCCESS)|
		(fclose(fsail) != EXIT_SUCCESS)|(fclose(frudder) != EXIT_SUCCESS)|
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
