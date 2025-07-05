#include <windows.h>
#include <stdio.h>
#include <stdlib.h>


class CGame  
{
public:
	CGame (int iClientH, char *cGameServerName, char *cGameServerIP, int iGameServerPort, char *cWorldName, int iTotalMaps);
	virtual ~CGame ();
	char  m_cGameName[20];
	char  m_cGameServerAddress[16];
	int   m_iGamePort;
	char  m_cWorldName[30];					// 4h
	int m_iTotalMaps;
	int m_iTrackSock; //

	char m_cGameServerMode;				// 4h
	char m_cGameServerName[11];			// 0Ch
	int  m_iGameServerPort;				// 28h
};