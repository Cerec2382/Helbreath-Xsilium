//rank system hbarg
#pragma once
using namespace std;

#include <windows.h>
#include <iostream>

#define MAXRANKEXP 31//20

class CRank
{
public:
	CRank();
	~CRank();

	void clear();
	void init();
	void update();
	void read();
	bool is_wanted_line(const string & line, string str);
	string get_line(string file, string value1);
	string getvalue(string val);
	int m_iMaxrankexp[MAXRANKEXP];
};

