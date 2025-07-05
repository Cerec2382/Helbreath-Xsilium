#pragma once

#include <winsock2.h>
#include <windows.h>

class cApplicationStartup
{
public:
	void worker_function2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void thread2();
	void thread5();
	void thread6();
	void thread3();
	void thread4();
	void thread1();
	void Startup();
	cApplicationStartup();
	~cApplicationStartup();
private:
	MMRESULT StartTimer(DWORD dwTime);
	HWND CreateBackgroundWindow();
	HWND CreateBackgroundWindow2();
	void InitializeSockets();
	void StartLoginServer();
	void StartCoreserver2();
};

