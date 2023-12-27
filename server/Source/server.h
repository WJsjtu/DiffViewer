#pragma once

#ifdef _WIN32
#ifdef SERVER_DLL
#define SERVER_PORT __declspec(dllexport)
#else
#define SERVER_PORT __declspec(dllimport)
#endif
#endif

extern "C" __declspec(dllexport) bool __stdcall create(const char* host_str, const char* directory_str, int port);

extern "C" __declspec(dllexport) void __stdcall destroy(int port);
