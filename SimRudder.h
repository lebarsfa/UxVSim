// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#ifndef SIMRUDDER_H
#define SIMRUDDER_H

#include "Rudder.h"

int handleclirudder(SOCKET sockcli, void* pParam);

#endif // SIMRUDDER_H
