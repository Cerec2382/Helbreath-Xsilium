//rank system hbarg
#include "..\Header\Rank.h"
#include "..\Header\Game.h"
#include "..\Header\StrTok.h"

CRank::CRank()
{
	init();	
}

CRank::~CRank()
{
}
void CRank::clear()
{
	for (int i = 0; i < MAXRANKEXP; i++)
	{
		m_iMaxrankexp[i] = 0;
	}
}

void CRank::init()
{
	PutLogList("(!) Reading Rank settings...");
	clear();
	read();
}

void CRank::update()
{
	PutLogList("(!) Update Rank settings...");
	clear();
	read();
}

bool CRank::is_wanted_line(const string & line, string str)
{
	return (line.find(str) != string::npos);
}

string CRank::get_line(string file, string value1)
{
	ifstream fin(file);

	string line;

	while (getline(fin, line))
	{
		if (is_wanted_line(line, value1))
			return line;
	}

	return "#";
}

string CRank::getvalue(string val)
{
	string result = get_line("Configs\\RankSettings.cfg", val);
	if (string(result) == "#") return result;
	else result.erase(0, val.length());
	return result;
}

void CRank::read()
{
	for (int i = 0; i < MAXRANKEXP; i++)
	{
		string stmp;
		char ctemp[50];
		wsprintf(ctemp, "maxrankexp-level%d = ", i + 1);
		stmp = getvalue(ctemp);
		if (string(stmp) == "#") continue;
		m_iMaxrankexp[i] = atoi((char*)stmp.c_str());
	}		
}