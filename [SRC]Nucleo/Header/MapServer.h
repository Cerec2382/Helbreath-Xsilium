#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "XSocket.h"

class CMapServer
{
public:

	CMapServer(HWND hWnd);
	virtual ~CMapServer();

	char  m_cMode;					// 4h
	char  m_cGameServerName[11];	// 5h
	char  m_cIPaddress[20];			// 10h
	class XSocket * m_pXSock;		// 24h
};
