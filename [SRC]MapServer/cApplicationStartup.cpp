#pragma warning (disable : 4474)
#include "cApplicationStartup.h"
#include <stdio.h>
#include "sql.h"
#include "Header/winmain.h"
#include "Header/Game.h"
#include "General/UserMessages.h"
#include "language/resource.h"
#include <shellapi.h>
#include <shlobj.h>
#include "Header/Event.h"
#include "Header/XSocket.h"

Event::Handler g_ev;

#define WM_USER_TIMERSIGNAL		WM_USER + 500
#pragma warning(disable : 4996)

#define ID_BUTTON 501
#define clNavy							0x800000
#define clWhite							0xFFFFFF

static HWND    List1;
static HWND    List2;
static HWND    List3;
static HWND    List4;
static HWND    List5;
static HWND    Edit1;
static HWND    Button1;
static HWND    Button2;
static HWND    Button4;
static HWND    Button5;
static HWND    Button6;
static HINSTANCE BCX_hInstance;
static int     BCX_ScaleX;
static int     BCX_ScaleY;
static char    BCX_ClassName[2048];
HWND    BCX_Listbox(char*, HWND, int, int, int, int, int, int = 0, int = -1);
HWND    BCX_Listbox2(char*, HWND, int, int, int, int, int, int = 0, int = -1);
HWND    BCX_Listbox3(char*, HWND, int, int, int, int, int, int = 0, int = -1);
HWND    BCX_Listbox4(char*, HWND, int, int, int, int, int, int = 0, int = -1);
HWND    BCX_Listbox5(char*, HWND, int, int, int, int, int, int = 0, int = -1);
HWND    BCX_Editbox(char*, HWND, int, int, int, int, int, int = 0, int = -1);
HWND    BCX_Button(char*, HWND, int, int, int, int, int, int = 0, int = -1);
HWND    BCX_Button2(char*, HWND, int, int, int, int, int, int = 0, int = -1);
HWND    BCX_Button4(char*, HWND, int, int, int, int, int, int = 0, int = -1);
HWND    BCX_Button5(char*, HWND, int, int, int, int, int, int = 0, int = -1);
HWND    BCX_Button6(char*, HWND, int, int, int, int, int, int = 0, int = -1);
int ItemCount = 0, ItemCount2 = 0, ItemCount3 = 0, ItemCount4 = 0, ItemCount5 = 0;
int J = 0, i = 0;

char			szAppClass[32];
HWND			G_hWnd = NULL;
char			G_cMsgList[120 * 50];
BOOL            G_cMsgUpdated = FALSE;
char            G_cTxt[512];
char			G_cData50000[50000];
MMRESULT        G_mmTimer = NULL;

char			G_cLogBuffer[30000];
char			G_cLogBuffer2[30000];
char			G_cLogBuffer3[30000];
char			G_cLogBuffer4[30000];
char			G_cLogBuffer5[30000];
char			G_cLogBuffer6[30000];
char			G_cLogBuffer7[30000];
char			G_cLogBuffer8[30000];
char			G_cLogBuffer9[30000];
char			G_cLogBuffer10[30000];
//lalov9 drops
char			G_cLogBuffer11[30000];

short			G_sLogCounter;
DWORD			G_dwLogTime;

class XSocket * G_pListenSock = NULL;
class XSocket * G_pLogSock = NULL;
class CMapServer *   c_map = NULL;

int             G_iQuitProgramCount = 0;
BOOL			G_bIsThread = TRUE;
BOOL            G_bShutdown = FALSE;
char			G_cCrashTxt[50000];

MMRESULT G_mmTimerCopy = NULL;
bool started;

void Borrado()
{
	G_cMsgUpdated = TRUE;
	SendMessage(List5, (UINT)LB_RESETCONTENT, 0, 0);
	SendMessage(List5, (UINT)LB_SETCURSEL, ItemCount5, 0);
	ItemCount5 = 0;
	ChatLogs("Limpio!");
}

void _StopTimer(MMRESULT timerid)
{
	TIMECAPS caps;

	if (timerid != 0) {
		timeKillEvent(timerid);
		timerid = 0;
		timeGetDevCaps(&caps, sizeof(caps));
		timeEndPeriod(caps.wPeriodMin);
	}
}

void PutLogList(char * cMsg)
{
	char cTempBuffer[512];
	SYSTEMTIME SysTime;
	DWORD dwTime;
	FILE * pLogFile;

	std::cout << cMsg << std::endl;
	/*G_cMsgUpdated = FALSE;
	SendMessage(List1, (UINT)LB_ADDSTRING, (WPARAM)0, (LPARAM)cMsg);
	SendMessage(List1, (UINT)LB_SETCURSEL, ItemCount, 0);
	ItemCount++;*/

	dwTime = timeGetTime();

	if (G_sLogCounter == 0) G_dwLogTime = dwTime;

	G_sLogCounter++;

	GetLocalTime(&SysTime);

	ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
	wsprintf(cTempBuffer, "%02d:%02d:%02d\t", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	strcat(cTempBuffer, cMsg);
	strcat(cTempBuffer, "\n");

	strcat(G_cLogBuffer, cTempBuffer);

	if (G_sLogCounter >= 100 || (dwTime - G_dwLogTime > 10 * 1000)) {
		ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
		_mkdir("..\\ServerLogs\\MapServer");

		wsprintf(cTempBuffer, "..\\ServerLogs\\MapServer\\MapServerLogs [%02d-%02d-%04d].log", SysTime.wDay, SysTime.wMonth, SysTime.wYear);

		pLogFile = fopen(cTempBuffer, "at");
		if (pLogFile == NULL) return;
		fwrite(G_cLogBuffer, 1, strlen(G_cLogBuffer), pLogFile);
		fclose(pLogFile);

		G_sLogCounter = 0;
		ZeroMemory(G_cLogBuffer, sizeof(G_cLogBuffer));

		if (pLogFile != NULL) fclose(pLogFile);
	}
}


void PutGMLogData(char * cStr)
{
	char cTempBuffer[512];
	SYSTEMTIME SysTime;
	DWORD dwTime;
	FILE * pLogFile;

	dwTime = timeGetTime();

	if (G_sLogCounter == 0) G_dwLogTime = dwTime;

	G_sLogCounter++;

	GetLocalTime(&SysTime);

	ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
	wsprintf(cTempBuffer, "%02d:%02d:%02d\t", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	strcat(cTempBuffer, cStr);
	strcat(cTempBuffer, "\n");

	strcat(G_cLogBuffer4, cTempBuffer);

	if (G_sLogCounter >= 100 || (dwTime - G_dwLogTime > 10 * 1000)) {
		ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
		_mkdir("..\\ServerLogs\\GM");

		wsprintf(cTempBuffer, "..\\ServerLogs\\GM\\GMLogs [%02d-%02d-%04d].log", SysTime.wDay, SysTime.wMonth, SysTime.wYear);
		pLogFile = fopen(cTempBuffer, "at");

		if (pLogFile == NULL) return;

		fwrite(G_cLogBuffer4, 1, strlen(G_cLogBuffer4), pLogFile);
		fclose(pLogFile);
		G_sLogCounter = 0;
		ZeroMemory(G_cLogBuffer4, sizeof(G_cLogBuffer4));
		if (pLogFile != NULL) fclose(pLogFile);
	}
}


void CharacterLogList(char * cMsg)
{
	char cTempBuffer[512];
	SYSTEMTIME SysTime;
	DWORD dwTime;
	FILE * pLogFile;

	std::cout << cMsg << std::endl;

	/*G_cMsgUpdated = FALSE;
	SendMessage(List2, (UINT)LB_ADDSTRING, (WPARAM)0, (LPARAM)cMsg);
	SendMessage(List2, (UINT)LB_SETCURSEL, ItemCount2, 0);
	ItemCount2++;*/

	dwTime = timeGetTime();

	if (G_sLogCounter == 0) G_dwLogTime = dwTime;

	G_sLogCounter++;

	GetLocalTime(&SysTime);

	ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
	wsprintf(cTempBuffer, "%02d:%02d:%02d\t", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	strcat(cTempBuffer, cMsg);
	strcat(cTempBuffer, "\n");

	strcat(G_cLogBuffer7, cTempBuffer);

	if (G_sLogCounter >= 100 || (dwTime - G_dwLogTime > 10 * 1000)) {
		ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
		_mkdir("..\\ServerLogs\\Characters");

		wsprintf(cTempBuffer, "..\\ServerLogs\\Characters\\CharactersLogs [%02d-%02d-%04d].log", SysTime.wDay, SysTime.wMonth, SysTime.wYear);
		pLogFile = fopen(cTempBuffer, "at");

		if (pLogFile == NULL) return;

		fwrite(G_cLogBuffer7, 1, strlen(G_cLogBuffer7), pLogFile);
		fclose(pLogFile);
		G_sLogCounter = 0;
		ZeroMemory(G_cLogBuffer7, sizeof(G_cLogBuffer7));
		if (pLogFile != NULL) fclose(pLogFile);
	}
}

void ConfigList(char * cMsg)
{
	/*G_cMsgUpdated = FALSE;
	SendMessage(List3, (UINT)LB_ADDSTRING, (WPARAM)0, (LPARAM)cMsg);
	SendMessage(List3, (UINT)LB_SETCURSEL, ItemCount3, 0);
	ItemCount3++;*/
}

void UpdateConfigList(char * cMsg)
{

	std::cout << cMsg << std::endl;
	/*G_cMsgUpdated = FALSE;
	ItemCount3 = ItemCount3 - 4;
	SendMessage(List3, (UINT)LB_DELETESTRING, ItemCount3, 0);
	SendMessage(List3, (UINT)LB_ADDSTRING, (WPARAM)0, (LPARAM)cMsg);
	SendMessage(List3, (UINT)LB_SETCURSEL, ItemCount3, 0);
	ItemCount3 = ItemCount3 + 4*/;
}

void ErrorList(char * cMsg)
{
	char cTempBuffer[512];
	SYSTEMTIME SysTime;
	DWORD dwTime;
	FILE * pLogFile;

	std::cout << cMsg << std::endl;
	/*G_cMsgUpdated = FALSE;
	SendMessage(List4, (UINT)LB_ADDSTRING, (WPARAM)0, (LPARAM)cMsg);
	SendMessage(List4, (UINT)LB_SETCURSEL, ItemCount4, 0);
	ItemCount4++;
*/

	dwTime = timeGetTime();

	if (G_sLogCounter == 0) G_dwLogTime = dwTime;

	G_sLogCounter++;

	GetLocalTime(&SysTime);

	ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
	wsprintf(cTempBuffer, "%02d:%02d:%02d\t", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	strcat(cTempBuffer, cMsg);
	strcat(cTempBuffer, "\n");

	strcat(G_cLogBuffer9, cTempBuffer);

	if (G_sLogCounter >= 100 || (dwTime - G_dwLogTime > 10 * 1000)) {
		ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
		_mkdir("..\\ServerLogs\\MapServer Errors");

		wsprintf(cTempBuffer, "..\\ServerLogs\\MapServer Errors\\MS Errors [%02d-%02d-%04d].log", SysTime.wDay, SysTime.wMonth, SysTime.wYear);
		pLogFile = fopen(cTempBuffer, "at");

		if (pLogFile == NULL) return;

		fwrite(G_cLogBuffer9, 1, strlen(G_cLogBuffer9), pLogFile);
		fclose(pLogFile);
		G_sLogCounter = 0;
		ZeroMemory(G_cLogBuffer9, sizeof(G_cLogBuffer9));
		if (pLogFile != NULL) fclose(pLogFile);
	}
}

void ChatLogs(char * cMsg)
{
	char cTempBuffer[512];
	SYSTEMTIME SysTime;
	DWORD dwTime;
	FILE * pLogFile;

	std::cout << cMsg << std::endl;
	/*/ *G_cMsgUpdated = FALSE;
	SendMessage(List5, (UINT)LB_ADDSTRING, (WPARAM)0, (LPARAM)cMsg);
	SendMessage(List5, (UINT)LB_SETCURSEL, ItemCount5, 0);
	ItemCount5++; * /*/

		dwTime = timeGetTime();

	if (G_sLogCounter == 0) G_dwLogTime = dwTime;

	G_sLogCounter++;

	GetLocalTime(&SysTime);

	ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
	wsprintf(cTempBuffer, "%02d:%02d:%02d\t", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	strcat(cTempBuffer, cMsg);
	strcat(cTempBuffer, "\n");

	strcat(G_cLogBuffer3, cTempBuffer);

	if (G_sLogCounter >= 100 || (dwTime - G_dwLogTime > 10 * 1000)) {
		ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
		_mkdir("..\\ServerLogs\\Chats");

		wsprintf(cTempBuffer, "..\\ServerLogs\\Chats\\ChatsLogs [%02d-%02d-%04d].log", SysTime.wDay, SysTime.wMonth, SysTime.wYear);
		pLogFile = fopen(cTempBuffer, "at");

		if (pLogFile == NULL) return;

		fwrite(G_cLogBuffer3, 1, strlen(G_cLogBuffer3), pLogFile);
		fclose(pLogFile);
		G_sLogCounter = 0;
		ZeroMemory(G_cLogBuffer3, sizeof(G_cLogBuffer3));
		if (pLogFile != NULL) fclose(pLogFile);
	}
}


void PutLogTradeFileList(char * cStr)
{
	char cTempBuffer[512];
	SYSTEMTIME SysTime;
	DWORD dwTime;
	FILE * pLogFile;

	dwTime = timeGetTime();

	if (G_sLogCounter == 0) G_dwLogTime = dwTime;

	G_sLogCounter++;

	GetLocalTime(&SysTime);

	ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
	wsprintf(cTempBuffer, "%02d:%02d:%02d\t", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	strcat(cTempBuffer, cStr);
	strcat(cTempBuffer, "\n");

	strcat(G_cLogBuffer10, cTempBuffer);

	if (G_sLogCounter >= 100 || (dwTime - G_dwLogTime > 10 * 1000)) {
		ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
		_mkdir("..\\ServerLogs\\TradeLogs");

		wsprintf(cTempBuffer, "..\\ServerLogs\\TradeLogs\\LogTrade [%02d-%02d-%04d].log", SysTime.wDay, SysTime.wMonth, SysTime.wYear);

		pLogFile = fopen(cTempBuffer, "at");

		if (pLogFile == NULL) return;

		fwrite(G_cLogBuffer10, 1, strlen(G_cLogBuffer10), pLogFile);
		fclose(pLogFile);
		G_sLogCounter = 0;
		ZeroMemory(G_cLogBuffer10, sizeof(G_cLogBuffer10));
		if (pLogFile != NULL) fclose(pLogFile);
	}
}

void PutLogCoinsFileList(char * cStr)
{
	char cTempBuffer[512];
	SYSTEMTIME SysTime;
	DWORD dwTime;
	FILE * pLogFile;

	dwTime = timeGetTime();

	if (G_sLogCounter == 0) G_dwLogTime = dwTime;

	G_sLogCounter++;

	GetLocalTime(&SysTime);

	ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
	wsprintf(cTempBuffer, "%02d:%02d:%02d\t", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	strcat(cTempBuffer, cStr);
	strcat(cTempBuffer, "\n");

	strcat(G_cLogBuffer5, cTempBuffer);

	if (G_sLogCounter >= 100 || (dwTime - G_dwLogTime > 10 * 1000)) {
		ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
		_mkdir("..\\ServerLogs\\Coins");

		wsprintf(cTempBuffer, "..\\ServerLogs\\Coins\\CoinsLogs [%02d-%02d-%04d].log", SysTime.wDay, SysTime.wMonth, SysTime.wYear);
		pLogFile = fopen(cTempBuffer, "at");

		if (pLogFile == NULL) return;

		fwrite(G_cLogBuffer5, 1, strlen(G_cLogBuffer5), pLogFile);
		fclose(pLogFile);
		G_sLogCounter = 0;
		ZeroMemory(G_cLogBuffer5, sizeof(G_cLogBuffer5));
		if (pLogFile != NULL) fclose(pLogFile);
	}
}

void PutLogHacksFileList(char * cStr)
{
	char cTempBuffer[512];
	SYSTEMTIME SysTime;
	DWORD dwTime;
	FILE * pLogFile;

	dwTime = timeGetTime();

	std::cout << cStr << std::endl;
	/*G_cMsgUpdated = FALSE;
	SendMessage(List4, (UINT)LB_ADDSTRING, (WPARAM)0, (LPARAM)cStr);
	SendMessage(List4, (UINT)LB_SETCURSEL, ItemCount4, 0);
	ItemCount4++;*/

	GetLocalTime(&SysTime);

	ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
	wsprintf(cTempBuffer, "%02d:%02d:%02d\t", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	strcat(cTempBuffer, cStr);
	strcat(cTempBuffer, "\n");

	strcat(G_cLogBuffer6, cTempBuffer);

	ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
	_mkdir("..\\ServerLogs\\Hacks");

	wsprintf(cTempBuffer, "..\\ServerLogs\\Hacks\\HacksLogs [%02d-%02d-%04d].log", SysTime.wDay, SysTime.wMonth, SysTime.wYear);
	pLogFile = fopen(cTempBuffer, "at");

	if (pLogFile == NULL) return;

	fwrite(G_cLogBuffer6, 1, strlen(G_cLogBuffer6), pLogFile);
	fclose(pLogFile);
	ZeroMemory(G_cLogBuffer6, sizeof(G_cLogBuffer6));
	if (pLogFile != NULL) fclose(pLogFile);
}

//Agregado LogDrop Lalov9
void PutLogDrops(char * cStr)
{
	char cTempBuffer[512];
	SYSTEMTIME SysTime;
	DWORD dwTime;
	FILE * pLogFile;

	dwTime = timeGetTime();

	if (G_sLogCounter == 0) G_dwLogTime = dwTime;

	G_sLogCounter++;
	std::cout << cStr << std::endl;
	/*/ *G_cMsgUpdated = FALSE;
	SendMessage(List4, (UINT)LB_ADDSTRING, (WPARAM)0, (LPARAM)cStr);
	SendMessage(List4, (UINT)LB_SETCURSEL, ItemCount4, 0);
	ItemCount4++; * /*/

		GetLocalTime(&SysTime);

	ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
	wsprintf(cTempBuffer, "%02d:%02d:%02d\t", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	strcat(cTempBuffer, cStr);
	strcat(cTempBuffer, "\n");

	strcat(G_cLogBuffer11, cTempBuffer);

	if (G_sLogCounter >= 100 || (dwTime - G_dwLogTime > 10 * 1000)) {

		ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
		_mkdir("..\\ServerLogs\\Drops");

		wsprintf(cTempBuffer, "..\\ServerLogs\\Drops\\DropsLogs [%02d-%02d-%04d].log", SysTime.wDay, SysTime.wMonth, SysTime.wYear);
		pLogFile = fopen(cTempBuffer, "at");

		if (pLogFile == NULL) return;

		fwrite(G_cLogBuffer11, 1, strlen(G_cLogBuffer11), pLogFile);
		fclose(pLogFile);

		G_sLogCounter = 0;

		ZeroMemory(G_cLogBuffer11, sizeof(G_cLogBuffer11));
		if (pLogFile != NULL) fclose(pLogFile);
	}
}

void PutLogOnlinesFileList(char * cStr)
{
	char cTempBuffer[512];
	FILE * pLogFile;

	_mkdir("..\\ServerLogs\\OnlineUsers");

	ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
	wsprintf(cTempBuffer, "..\\ServerLogs\\OnlineUsers\\OnlineUsers.log");

	pLogFile = fopen(cTempBuffer, "w");

	if (pLogFile == NULL) return;

	fwrite(cStr, 1, strlen(cStr), pLogFile);
	fclose(pLogFile);
	if (pLogFile != NULL) fclose(pLogFile);
}

void PutLogItemsList(char * cMsg)
{
	char cTempBuffer[512];
	SYSTEMTIME SysTime;
	DWORD dwTime;
	FILE * pLogFile;

	dwTime = timeGetTime();

	if (G_sLogCounter == 0) G_dwLogTime = dwTime;

	G_sLogCounter++;

	GetLocalTime(&SysTime);

	ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
	wsprintf(cTempBuffer, "%02d:%02d:%02d\t", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	strcat(cTempBuffer, cMsg);
	strcat(cTempBuffer, "\n");

	strcat(G_cLogBuffer2, cTempBuffer);

	if (G_sLogCounter >= 100 || (dwTime - G_dwLogTime > 10 * 1000)) {
		ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
		_mkdir("..\\ServerLogs\\Items");

		wsprintf(cTempBuffer, "..\\ServerLogs\\Items\\ItemLogs [%02d-%02d-%04d].log", SysTime.wDay, SysTime.wMonth, SysTime.wYear);

		pLogFile = fopen(cTempBuffer, "at");
		if (pLogFile == NULL) return;
		fwrite(G_cLogBuffer2, 1, strlen(G_cLogBuffer2), pLogFile);
		fclose(pLogFile);

		G_sLogCounter = 0;
		ZeroMemory(G_cLogBuffer2, sizeof(G_cLogBuffer2));

		if (pLogFile != NULL) fclose(pLogFile);
	}
}

void PutEkAndPkLogFileList(char * cMsg)
{
	char cTempBuffer[512];
	SYSTEMTIME SysTime;
	DWORD dwTime;
	FILE * pLogFile;

	dwTime = timeGetTime();

	if (G_sLogCounter == 0) G_dwLogTime = dwTime;

	G_sLogCounter++;

	GetLocalTime(&SysTime);

	ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
	wsprintf(cTempBuffer, "%02d:%02d:%02d\t", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	strcat(cTempBuffer, cMsg);
	strcat(cTempBuffer, "\n");

	strcat(G_cLogBuffer8, cTempBuffer);

	if (G_sLogCounter >= 100 || (dwTime - G_dwLogTime > 10 * 1000)) {
		ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
		_mkdir("..\\ServerLogs\\EksAndPks");

		wsprintf(cTempBuffer, "..\\ServerLogs\\EksAndPks\\EksAndPksLogs [%02d-%02d-%04d].log", SysTime.wDay, SysTime.wMonth, SysTime.wYear);
		pLogFile = fopen(cTempBuffer, "at");

		if (pLogFile == NULL) return;
		fwrite(G_cLogBuffer8, 1, strlen(G_cLogBuffer8), pLogFile);
		fclose(pLogFile);

		G_sLogCounter = 0;
		ZeroMemory(G_cLogBuffer8, sizeof(G_cLogBuffer8));

		if (pLogFile != NULL) fclose(pLogFile);
	}
}

bool Initialize()
{
	WSADATA data;

	auto version = MAKEWORD(2, 2);
	auto err = WSAStartup(version, &data);
	if (err)
		return false;

	return true;
}

void Deinitialize()
{
	WSACleanup();
}

void OnDestroy()
{
	if (G_pListenSock != NULL) delete G_pListenSock;
	if (G_pLogSock != NULL) delete G_pLogSock;

	if (c_map != NULL) {
		c_map->Quit();
		delete c_map;
	}

	if (G_mmTimer != NULL) _StopTimer(G_mmTimer);
	///_TermWinsock();
	Deinitialize();

	PostQuitMessage(0);
}


void OnAccept()
{
	c_map->bAccept(G_pListenSock);
}

void CALLBACK _TimerFunc(UINT wID, UINT wUser, DWORD dwUSer, DWORD dw1, DWORD dw2)
{
	PostMessage(G_hWnd, WM_USER_TIMERSIGNAL, wID, NULL);
}

LRESULT CALLBACK BackgroundWindowProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
		switch (wParam) {
		case 1012:	
			break;
		case 1013:	
			break;
		case 1010:	
			char commandtxt[100];
			if (commandtxt != NULL) {
				
			}
			break;
		}
		break;

	case WM_CREATE:
		break;

	case WM_KEYDOWN:
		c_map->OnKeyDown(wParam, lParam);
		return (DefWindowProc(hWnd, message, wParam, lParam));
		break;

	case WM_KEYUP:
		c_map->OnKeyUp(wParam, lParam);
		return (DefWindowProc(hWnd, message, wParam, lParam));
		break;

	case WM_USER_STARTGAMESIGNAL:
		c_map->OnStartGameSignal();
		break;

	case WM_USER_TIMERSIGNAL:
		if (!G_bShutdown) {
			c_map->OnTimer(NULL);
		}
		break;

	case WM_USER_ACCEPT:
		OnAccept();
		break;

	case WM_PAINT:
		//OnPaint();
		break;

	case WM_DESTROY:
		G_bShutdown = TRUE;
		OnDestroy();
		break;

	case WM_CLOSE:
		if (c_map->bOnClose()) {
			return (DefWindowProc(hWnd, message, wParam, lParam));
		}
		break;
	case WM_ONGATESOCKETEVENT:
		if (G_bShutdown == TRUE) break;
		break;

	case WM_ONLOGSOCKETEVENT:
		if (G_bShutdown == TRUE) break;
		c_map->OnMainLogSocketEvent(message, wParam, lParam);
		break;

	default:
		if (G_bShutdown == TRUE) break;
		if ((message >= WM_ONLOGSOCKETEVENT + 1) && (message <= WM_ONLOGSOCKETEVENT + DEF_MAXSUBLOGSOCK))
			c_map->OnSubLogSocketEvent(message, wParam, lParam);

		if ((message >= WM_ONCLIENTSOCKETEVENT) && (message < WM_ONCLIENTSOCKETEVENT + DEF_MAXCLIENTS))
			c_map->OnClientSocketEvent(message, wParam, lParam);

		return (DefWindowProc(hWnd, message, wParam, lParam));
	}

	return NULL;
}

void cApplicationStartup::Startup()
{
	started = false;

	printf("(!) Starting CoreServer \n");
	static unsigned short _usCnt = 0;
	register MSG msg;

	InitializeSockets();	
	StartCoreserver();

	G_mmTimerCopy = this->StartTimer(1);

	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!GetMessage(&msg, NULL, 0, 0)) {
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else WaitMessage();
	}
}

cApplicationStartup::cApplicationStartup()
{
}

cApplicationStartup::~cApplicationStartup()
{
}

MMRESULT cApplicationStartup::StartTimer(DWORD dwTime)
{
	TIMECAPS caps;
	MMRESULT timerid;

	timeGetDevCaps(&caps, sizeof(caps));
	timeBeginPeriod(caps.wPeriodMin);
	timerid = timeSetEvent(dwTime, 0, _TimerFunc, 0, (UINT)TIME_PERIODIC);

	return timerid;
}

HWND cApplicationStartup::CreateBackgroundWindow()
{
	WNDCLASS wndclass;
	CHAR *ProviderClass = "AsyncSelect";
	HWND Window;
	char cTitle[100];

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = &BackgroundWindowProcess;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = NULL;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = ProviderClass;

	if (RegisterClass(&wndclass) == 0)
	{
		printf("RegisterClass() failed with error %d\n", GetLastError());
		return NULL;
	}
	else {
		//printf("RegisterClass() is OK!\n");
	}

	Window = CreateWindowEx(
		0,
		ProviderClass,
		cTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		NULL,
		NULL);

	if (Window == NULL)
	{
		printf("CreateWindow() failed with error %d\n", GetLastError());
		return NULL;
	}
	else {
		//printf("CreateWindow() is OK!\n");
	}
	return Window;
}

void cApplicationStartup::InitializeSockets()
{
	if (Initialize() == FALSE) {
		printf("Socket 2.2 not found! Cannot execute program.", "ERROR", MB_ICONEXCLAMATION | MB_OK);
		PostQuitMessage(0);
		return;
	}
}

void cApplicationStartup::StartCoreserver()
{	
	G_hWnd = CreateBackgroundWindow();
	
	c_map = new CMapServer(G_hWnd);
	if (c_map->bInit() == FALSE) {
		printf("(!!!) STOPPED!");
		PutLogList("(!!!) STOPPED!");
		return;
	}
	
	started = true;

	G_pListenSock = new class XSocket(G_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
	G_pListenSock->bListen(c_map->m_cGameServerAddr, c_map->m_iGameServerPort, WM_USER_ACCEPT);

}
