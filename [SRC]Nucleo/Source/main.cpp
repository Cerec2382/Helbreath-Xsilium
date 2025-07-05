#include <stdio.h>
#include "cApplicationStartup.h"
#include<stdio.h>
#include <iostream>
#include <thread>
#include <new> // Necesario para std::bad_alloc
#include <exception>
#include <chrono>

using namespace std;

int main() {
	try {
		cApplicationStartup *applicationStartup = new cApplicationStartup();
		applicationStartup->Startup();
	}
	catch (bad_alloc& ba)
	{
		char tmp[1000];
		ZeroMemory(tmp, sizeof(tmp));
		wsprintf(tmp, "bad_alloc caught : %s", ba.what());
		//PutLogList(tmp);
	}
	catch (std::exception& e) {
		//PutLogList((char*)e.what().c_str());
	}
	catch (...) {
	//	PutLogList("Exception occured");
	}
}

