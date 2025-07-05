/*
#include <windows.h>
#include "../Header/Wmain.h"
#include "../Header/MainLog.h"
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include "../General/GlobalDef.h"

char		szAppClass[1000];
WNDCLASS	wnd;
HWND		G_hWnd;
WSADATA		wsInfo;

BOOL G_cMsgUpdated = FALSE;
char G_cMsgList[120*50];
char            G_cTxt[500];
class CMainLog *G_pMainLog;
//probando laloramos
//char			GcTxt2[500];

//Timer
MMRESULT        G_mmTimer = NULL;
BOOL			G_bIsThread = TRUE;
static HINSTANCE BCX_hInstance;
static int     BCX_ScaleX, BCX_ScaleY;
static char    BCX_ClassName[2048];

static HWND    List1;
static HWND    List2;
static HWND    List3;
static HWND    List4;
static HWND    List5;
static HWND    Button1;
static HWND    Button2;
static HWND    Edit1;

HWND hSv;
HWND BCX_Listbox(char*, HWND, int, int, int, int, int, int = 0, int = -1);
HWND BCX_Listbox2(char*, HWND, int, int, int, int, int, int = 0, int = -1);
HWND BCX_Listbox3(char*, HWND, int, int, int, int, int, int = 0, int = -1);
HWND BCX_Listbox4(char*, HWND, int, int, int, int, int, int = 0, int = -1);
HWND BCX_Listbox5(char*, HWND, int, int, int, int, int, int = 0, int = -1);

HWND    BCX_Button(char*, HWND, int, int, int, int, int, int = 0, int = -1);
HWND    BCX_Button2(char*, HWND, int, int, int, int, int, int = 0, int = -1);

HWND    BCX_Editbox(char*, HWND, int, int, int, int, int, int = 0, int = -1);

HINSTANCE hInst;

int ItemCount = 0, ItemCount2 = 0, ItemCount3 = 0, ItemCount5 = 0;
class XSocket * G_pInternalLogSock = NULL;

LRESULT CALLBACK WindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg){
		//---------Buttons---------//
		case WM_COMMAND:
			switch (wParam) {
				case 1010:
					G_cMsgUpdated = TRUE;
					SendMessage(List1,(UINT)LB_RESETCONTENT, 0, 0);
					ItemCount = 0;
					PutLogList("Limpio!");
					break;

				case 1011:
					if (!LoginEnabled) {
						Button1 = BCX_Button("Enabled", G_hWnd, 1011, 730, 8, 90, 20); // Server Logs Button
						PutLogList("Login Enabled!");
						LoginEnabled = true;
						G_pXMainLogSock = new class XSocket(G_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
						G_pXMainLogSock->bListen(G_pMainLog->m_cMainServerAddress, G_pMainLog->m_iMainServerPort, WM_USER_ACCEPT);
					}
					break;

				case 1013:
					char cWhat[256];
					GetWindowText(Edit1, cWhat, 256);
					if (cWhat != NULL) {
						SetWindowText(Edit1, NULL);
						SetFocus(Edit1);
						ParseCommand(cWhat);
					}
					break;
				
			}
			break;

		case WM_CLOSE:
			if (MessageBox(hWnd, "Quit Main log server?", "Main-LOG", MB_YESNO + MB_ICONINFORMATION) == IDYES) {
				return DefWindowProc(hWnd, WM_CLOSE, wParam, lParam);
			}
			break;
		
		case WM_DESTROY:
			OnDestroy();
			break;

		case WM_PAINT:
			OnPaint();
			break;

		case WM_USER_TIMERSIGNAL:
			G_pMainLog->OnTimer();
			break;

		case WM_ONMAPSERVERSOCKETEVENT: // From MapServer
			G_pMainLog->bAccept_MapServer(G_pInternalLogSock);
			break;

		case WM_USER_ACCEPT: //from anything
			G_pMainLog->bAccept(G_pXMainLogSock);
			break;

		default:
			if ((uMsg >= WM_ONCLIENTSOCKETEVENT) && (uMsg <= WM_ONCLIENTSOCKETEVENT + DEF_MAXCLIENTSOCK)) {
				G_pMainLog->OnClientSubLogSocketEvent(uMsg, wParam, lParam);
			}
			else if ((uMsg >= WM_ONMAPSERVERSOCKETEVENT) && (uMsg <= WM_ONMAPSERVERSOCKETEVENT + DEF_MAXCLIENTS)) {
				G_pMainLog->OnMapServerSubLogSocketEvent(uMsg, wParam, lParam);
			}
			
			return DefWindowProc(hWnd,uMsg,wParam,lParam);
			break;
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	wsprintf(szAppClass, "World Log Server%d", hInstance);
	G_bIsMainLogActive = TRUE;
	LoginEnabled = false;
	if (!InitApplication( hInstance)) return (FALSE);
	if (!InitInstance(hInstance, nCmdShow)) return (FALSE);
	Initialize();
	EventLoop();
	return 0;
}

BOOL InitApplication( HINSTANCE hInstance)
{     
	WNDCLASS  wc;

	wc.style = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS);
	wc.lpfnWndProc   = (WNDPROC)WindowProc;             
	wc.cbClsExtra    = 0;                            
	wc.cbWndExtra    = sizeof (int);             
	wc.hInstance     = hInstance;                    
	wc.hIcon         = NULL;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);  
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
//	wc.lpszMenuName  = (LPCSTR)IDR_MENU1;
	wc.lpszMenuName  = NULL; 
	wc.lpszClassName = szAppClass;                   

	return (RegisterClass(&wc));
}

BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	char cTitle[100];

	wsprintf(cTitle, "Login Server - Helbreath Cursed"); //LzWorld

	G_hWnd = CreateWindowEx(0,  // WS_EX_TOPMOST,
        szAppClass,
        cTitle,
        WS_VISIBLE | // so we don't have to call ShowWindow
        //WS_POPUP |   // non-app window
        //WS_CAPTION | // so our menu doesn't look ultra-goofy
        WS_SYSMENU |  // so we get an icon in the tray
        WS_MINIMIZEBOX | 
		WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
        CW_USEDEFAULT,
        870, //GetSystemMetrics(SM_CXSCREEN),
        540, //GetSystemMetrics(SM_CYSCREEN),
        NULL,
        NULL,
        hInstance,
        NULL );


	if (!G_hWnd) return (FALSE);
	WNDCLASS Wc;
	strcpy(BCX_ClassName,"ListBox1");
	BCX_ScaleX       = 1;
	BCX_ScaleY       = 1;
	BCX_hInstance    =  hInstance;
	Wc.style         =  CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	Wc.lpfnWndProc   =  WindowProc;
	Wc.cbClsExtra    =  0;
	Wc.cbWndExtra    =  0;
	Wc.hInstance     =  hInstance;
	Wc.hIcon         =  LoadIcon(NULL,IDI_WINLOGO);
	Wc.hCursor       =  LoadCursor(NULL,IDC_ARROW);
	Wc.hbrBackground =  (HBRUSH)(COLOR_BTNFACE+1);
	Wc.lpszMenuName  =  NULL;
	Wc.lpszClassName =  BCX_ClassName;
	RegisterClass(&Wc);
	
	List1 = BCX_Listbox("", G_hWnd, 1009, 10, 205, 290, 280); // Server Logs 
	List2 = BCX_Listbox2("", G_hWnd, 1014, 10, 25, 290, 160); // Configs List
	List3 = BCX_Listbox3("", G_hWnd, 1012, 315, 25, 290, 240); // Configs List
	List4 = BCX_Listbox4("", G_hWnd, 1015, 315, 285, 290, 200); // Error List 
	List5 = BCX_Listbox5("", G_hWnd, 1016, 620, 285, 220, 170); // Error List 

	Button1 = BCX_Button("Disabled", G_hWnd, 1011, 730, 8, 90, 20); // Server Logs Button
	Button2 = BCX_Button2("Send", G_hWnd, 1013, 780, 466, 60, 20); // Character Logs Button
	Edit1 = BCX_Editbox("", G_hWnd, 1018, 620, 466, 150, 20); // Chat List

	ShowWindow(G_hWnd, nCmdShow);    
	UpdateWindow(G_hWnd);
	return (TRUE);                 
}

int EventLoop()
{
	static unsigned short _usCnt = 0; 
	register MSG msg;

	while(1) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if(!GetMessage(&msg, NULL, 0, 0)) {
				return msg.wParam;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			UpdateScreen();
		}
		else WaitMessage();
	}
}

void ThreadProc(void *ch)
{
	while (G_bIsThread == TRUE) {
		if (G_pMainLog = NULL) G_pMainLog->OnTimer();
		Sleep(1);
	}
	ExitThread(0);
}

void Initialize()
{
	if (_InitWinsock() == FALSE) {
		MessageBox(G_hWnd, "Socket 1.1 not found! Cannot execute program.","ERROR", MB_ICONEXCLAMATION | MB_OK);
		PostQuitMessage(0);
		return;
	}

	G_pMainLog = new class CMainLog(G_hWnd);
	if (G_pMainLog == NULL) {
		MessageBox(G_hWnd, "Init fail - Invalid WorldLServer!","ERROR",MB_OK+MB_ICONERROR);
		return;
	}
	
	if (G_pMainLog->bInit() == FALSE) {
		MessageBox(G_hWnd, "Init fail - Missing required .cfg files!","ERROR",MB_OK+MB_ICONERROR);
		return;
	}

	G_sLogCounter = G_dwAccountTime = 0; / *m_cGMLogCount = m_cItemLogCount = m_cCrusadeLogCount = m_cPutEkAndPkLogCount = 0;
	m_cPutCoinsLogCount = m_cPutHacksLogCount = m_cPutTradeLogCount = m_cPutCharacterLogCount = m_cPutChatLogCount = 0;* /

	ZeroMemory(G_cLogBuffer, sizeof(G_cLogBuffer));
	/ *ZeroMemory(m_cGMLogBuffer, sizeof(m_cGMLogBuffer));
	ZeroMemory(m_cItemLogBuffer, sizeof(m_cItemLogBuffer));
	ZeroMemory(m_cCrusadeLogBuffer, sizeof(m_cCrusadeLogBuffer));
	ZeroMemory(m_cPutEkAndPkLogBuffer, sizeof(m_cPutEkAndPkLogBuffer));
	ZeroMemory(m_cPutCoinsLogBuffer, sizeof(m_cPutCoinsLogBuffer));
	ZeroMemory(m_cPutHacksLogBuffer, sizeof(m_cPutHacksLogBuffer));
	ZeroMemory(m_cPutTradeLogBuffer, sizeof(m_cPutTradeLogBuffer));
	ZeroMemory(m_cPutCharacterLogBuffer, sizeof(m_cPutCharacterLogBuffer));
	ZeroMemory(m_cPutChatLogBuffer, sizeof(m_cPutChatLogBuffer));* /

	G_sAccountCounter = 0;
	ZeroMemory(G_cAccountBuffer, sizeof(G_cAccountBuffer));

	G_bIsMainLogActive = TRUE;
	G_pInternalLogSock = new class XSocket(G_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
	G_pInternalLogSock->bListen(G_pMainLog->m_cMainServerAddress, G_pMainLog->m_iMainServerInternalPort, WM_ONMAPSERVERSOCKETEVENT);

	
	PutLogList("Log-Socket Listening... Server Activated.");
	//G_mmTimer = _StartTimer(10);
	G_mmTimer = _StartTimer(1);

	//CommandList("List Comand: '/ help' ");
}

void OnDestroy()
{
	if (G_pMainLog != NULL) delete G_pMainLog;
	if (G_pXMainLogSock != NULL) delete G_pXMainLogSock;
	if (G_pInternalLogSock != NULL) delete G_pInternalLogSock;
	_TermWinsock();
	if (G_mmTimer != NULL) _StopTimer(G_mmTimer);
	PostQuitMessage(0);
}

void UpdateScreen()
{
	if (G_cMsgUpdated == TRUE) {
		InvalidateRect(G_hWnd, NULL, TRUE);
		G_cMsgUpdated = FALSE;
	}
}

void OnPaint()
{
	HDC hdc, MemDCLogo;
	PAINTSTRUCT ps;
	HBITMAP bmp1; //Bitmap header
	char cTxt[100];

	hdc = BeginPaint(G_hWnd, &ps);
	SetBkMode(hdc,TRANSPARENT);
	
	if (G_pMainLog != NULL) {}

	bmp1 = (HBITMAP)LoadImage(NULL, "LogoNucleo.bmp", IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE);

	MemDCLogo = CreateCompatibleDC(hdc);

	SelectObject(MemDCLogo, bmp1);

	BitBlt(hdc, 635, 50, 290, 400, MemDCLogo, 0, 0, SRCCOPY); 

	SetTextColor(hdc, 0x00D7FF);
	//TextOut(hdc, 10, 490, "LOGIN STATUS:", strlen("LOGIN STATUS:"));
	TextOut(hdc, 620, 10, "LOGIN STATUS:", strlen("LOGIN STATUS:"));

	wsprintf(cTxt, "SERVER CONFIGS:");
	TextOut(hdc, 10, 10, cTxt, strlen(cTxt));

	wsprintf(cTxt, "SERVER LOGS:");
	TextOut(hdc, 10, 190, cTxt, strlen(cTxt));

	wsprintf(cTxt, "ACCOUNTS LOGS:");
	TextOut(hdc, 315, 10, cTxt, strlen(cTxt));

	wsprintf(cTxt, "ERROR LOGS:");
	TextOut(hdc, 315, 270, cTxt, strlen(cTxt));

	wsprintf(cTxt, "COMMAND LIST:");
	TextOut(hdc, 620, 270, cTxt, strlen(cTxt));
	
	EndPaint(G_hWnd, &ps);
}

void ConfigList(char * cMsg)
{
	G_cMsgUpdated = FALSE;
	SendMessage(List2, (UINT)LB_ADDSTRING, (WPARAM)0, (LPARAM)cMsg);
	SendMessage(List2, (UINT)LB_SETCURSEL, ItemCount2, 0);
	ItemCount2++;
}

void UpdateConfigList(char * cMsg)
{
	G_cMsgUpdated = FALSE;
	ItemCount2 = ItemCount2 --;
	SendMessage(List2, (UINT)LB_DELETESTRING, ItemCount2, 0);
	SendMessage(List2, (UINT)LB_ADDSTRING, (WPARAM)0, (LPARAM)cMsg);
	SendMessage(List2, (UINT)LB_SETCURSEL, ItemCount2, 0);
	ItemCount2 = ItemCount2 ++;
}

void CommandList(char * cMsg)
{
	G_cMsgUpdated = FALSE;
	SendMessage(List5, (UINT)LB_ADDSTRING, (WPARAM)0, (LPARAM)cMsg);
	SendMessage(List5, (UINT)LB_SETCURSEL, ItemCount5, 0);
	ItemCount5++;
}

void PutLogList(char * cMsg)
{
	char cTempBuffer[512];
	SYSTEMTIME SysTime;
	DWORD dwTime;
	FILE * pLogFile;

	G_cMsgUpdated = TRUE;

	G_cMsgUpdated = FALSE;
	SendMessage(List1,(UINT)LB_ADDSTRING,(WPARAM)0,(LPARAM)cMsg);
	SendMessage(List1,(UINT)LB_SETCURSEL,ItemCount,0);
	ItemCount++;

	dwTime = timeGetTime();

	if (G_sLogCounter == 0) G_dwLogTime = dwTime;

	G_sLogCounter++;

	GetLocalTime(&SysTime);

	ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
	wsprintf(cTempBuffer, "%02d:%02d:%02d\t", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	strcat(cTempBuffer, cMsg);
	strcat(cTempBuffer, "\n");

	strcat(G_cLogBuffer, cTempBuffer);

	if (G_sLogCounter >= 100 || (dwTime - G_dwLogTime  >  10 * 1000)) {
		ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
		_mkdir("..\\ServerLogs\\Nucleo");

		wsprintf(cTempBuffer, "..\\ServerLogs\\Nucleo\\NucleoLog [%02d-%02d-%04d].log", SysTime.wDay, SysTime.wMonth, SysTime.wYear);

		pLogFile = fopen(cTempBuffer, "at");
		if (pLogFile == NULL) return;
		fwrite(G_cLogBuffer, 1, strlen(G_cLogBuffer), pLogFile);
		fclose(pLogFile);

		G_sLogCounter = 0;
		ZeroMemory(G_cLogBuffer, sizeof(G_cLogBuffer));

		if (pLogFile != NULL) fclose(pLogFile);
	}
}

void AccountLogList(char * cMsg)
{
	char cTempBuffer[512];
	SYSTEMTIME SysTime;
	DWORD dwTime;
	FILE * pAccountFile;

	G_cMsgUpdated = TRUE;

	G_cMsgUpdated = FALSE;
	SendMessage(List3, (UINT)LB_ADDSTRING, (WPARAM)0, (LPARAM)cMsg);
	SendMessage(List3, (UINT)LB_SETCURSEL, ItemCount3, 0);
	ItemCount3++;

	dwTime = timeGetTime();

	if (G_sAccountCounter == 0) G_dwAccountTime = dwTime;

	G_sAccountCounter++;

	GetLocalTime(&SysTime);

	ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
	wsprintf(cTempBuffer, "%02d:%02d:%02d\t", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	strcat(cTempBuffer, cMsg);
	strcat(cTempBuffer, "\n");

	strcat(G_cAccountBuffer, cTempBuffer);

	if (G_sAccountCounter >= 100 || (dwTime - G_dwAccountTime  >  10 * 1000)) {
		ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
		_mkdir("..\\ServerLogs\\Accounts");

		wsprintf(cTempBuffer, "..\\ServerLogs\\Accounts\\AccountLog [%02d-%02d-%04d].log", SysTime.wDay, SysTime.wMonth, SysTime.wYear);

		pAccountFile = fopen(cTempBuffer, "at");
		if (pAccountFile == NULL) return;
		fwrite(G_cAccountBuffer, 1, strlen(G_cAccountBuffer), pAccountFile);
		fclose(pAccountFile);

		G_sAccountCounter = 0;
		ZeroMemory(G_cAccountBuffer, sizeof(G_cAccountBuffer));

		if (pAccountFile != NULL) fclose(pAccountFile);
	}
	
}

void ErrorLogList(char * cMsg)
{
	char cTempBuffer[512];
	SYSTEMTIME SysTime;
	DWORD dwTime;
	//FILE * pErrorFile;

	G_cMsgUpdated = TRUE;

	G_cMsgUpdated = FALSE;
	SendMessage(List3, (UINT)LB_ADDSTRING, (WPARAM)0, (LPARAM)cMsg);
	SendMessage(List3, (UINT)LB_SETCURSEL, ItemCount3, 0);
	ItemCount3++;

	/ *dwTime = timeGetTime();

	if (G_sAccountCounter == 0) G_dwAccountTime = dwTime;

	G_sAccountCounter++;

	GetLocalTime(&SysTime);

	ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
	wsprintf(cTempBuffer, "%02d:%02d:%02d\t", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	strcat(cTempBuffer, cMsg);
	strcat(cTempBuffer, "\n");

	strcat(G_cAccountBuffer, cTempBuffer);

	if (G_sAccountCounter >= 100 || (dwTime - G_dwAccountTime  >  10 * 1000)) {
		ZeroMemory(cTempBuffer, sizeof(cTempBuffer));
		_mkdir("..\\ServerLogs\\NucleoErrors");

		wsprintf(cTempBuffer, "..\\ServerLogs\\NucleoErrors\\ErrosLogs [%02d-%02d-%04d].log", SysTime.wDay, SysTime.wMonth, SysTime.wYear);

		pErrorFile = fopen(cTempBuffer, "at");
		if (pErrorFile == NULL) return;
		fwrite(G_cAccountBuffer, 1, strlen(G_cAccountBuffer), pErrorFile);
		fclose(pErrorFile);

		G_sAccountCounter = 0;
		ZeroMemory(G_cAccountBuffer, sizeof(G_cAccountBuffer));

		if (pAccountFile != NULL) fclose(pAccountFile);
	}* /

}

void CALLBACK _TimerFunc(UINT wID, UINT wUser, DWORD dwUSer, DWORD dw1, DWORD dw2)
{
	PostMessage(G_hWnd, WM_USER_TIMERSIGNAL, wID, NULL);
}

MMRESULT _StartTimer(DWORD dwTime)
{
	TIMECAPS caps;
	MMRESULT timerid;

	timeGetDevCaps(&caps, sizeof(caps));
	timeBeginPeriod(caps.wPeriodMin);
	timerid = timeSetEvent(dwTime,0,_TimerFunc,0, (UINT)TIME_PERIODIC);

	return timerid;
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

//----------------------------MAIN CMD!---------------------------------//
HWND BCX_Listbox(char* Text,HWND hWnd,int id,int X,int Y,int W,int H,int Style,int Exstyle)
{
	HWND  A;
	if (!Style) Style = WS_VSCROLL | WS_HSCROLL | WS_VISIBLE | WS_CHILD | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | WS_TABSTOP;
	if (Exstyle == -1) Exstyle=WS_EX_CLIENTEDGE;

	A = CreateWindowEx(Exstyle, "Listbox", NULL, Style, X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY, hWnd, (HMENU)id, BCX_hInstance, NULL);
	SendMessage(A, (UINT)WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), (LPARAM)MAKELPARAM(FALSE,0));

	return A;
}

HWND BCX_Listbox2(char* Text, HWND hWnd, int id, int X, int Y, int W, int H, int Style, int Exstyle)
{
	HWND  A;
	if (!Style) Style = WS_VSCROLL | WS_HSCROLL | WS_VISIBLE | WS_CHILD | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | WS_TABSTOP;
	if (Exstyle == -1) Exstyle = WS_EX_CLIENTEDGE;
	
	A = CreateWindowEx(Exstyle, "Listbox", NULL, Style, X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY, hWnd, (HMENU)id, BCX_hInstance, NULL);
	SendMessage(A, (UINT)WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), (LPARAM)MAKELPARAM(FALSE, 0));
	return A;
}

HWND BCX_Listbox3(char* Text, HWND hWnd, int id, int X, int Y, int W, int H, int Style, int Exstyle)
{
	HWND  A;
	if (!Style) Style = WS_VSCROLL | WS_HSCROLL | WS_VISIBLE | WS_CHILD | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | WS_TABSTOP;
	if (Exstyle == -1) Exstyle = WS_EX_CLIENTEDGE;

	A = CreateWindowEx(Exstyle, "Listbox", NULL, Style, X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY, hWnd, (HMENU)id, BCX_hInstance, NULL);
	SendMessage(A, (UINT)WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), (LPARAM)MAKELPARAM(FALSE, 0));
	return A;
}

HWND BCX_Listbox4(char* Text, HWND hWnd, int id, int X, int Y, int W, int H, int Style, int Exstyle)
{
	HWND  A;
	if (!Style) Style = WS_VSCROLL | WS_HSCROLL | WS_VISIBLE | WS_CHILD | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | WS_TABSTOP;
	if (Exstyle == -1) Exstyle = WS_EX_CLIENTEDGE;

	A = CreateWindowEx(Exstyle, "Listbox", NULL, Style, X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY, hWnd, (HMENU)id, BCX_hInstance, NULL);
	SendMessage(A, (UINT)WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), (LPARAM)MAKELPARAM(FALSE, 0));
	return A;
}

HWND BCX_Listbox5(char* Text, HWND hWnd, int id, int X, int Y, int W, int H, int Style, int Exstyle)
{
	HWND  A;
	if (!Style) Style = WS_VSCROLL | WS_HSCROLL | WS_VISIBLE | WS_CHILD | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | WS_TABSTOP;
	if (Exstyle == -1) Exstyle = WS_EX_CLIENTEDGE;

	A = CreateWindowEx(Exstyle, "Listbox", NULL, Style, X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY, hWnd, (HMENU)id, BCX_hInstance, NULL);
	SendMessage(A, (UINT)WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), (LPARAM)MAKELPARAM(FALSE, 0));
	return A;
}

HWND BCX_Button(char* Text,HWND hWnd,int id,int X,int Y,int W,int H,int Style,int Exstyle)
{
	HWND  A;
	if (!Style) Style = WS_CHILD | WS_VISIBLE | WS_BORDER;
        
	A = CreateWindowEx(0, "Button", Text, Style, X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY, hWnd, (HMENU)id, BCX_hInstance, NULL);
    SendMessage(A, (UINT)WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), (LPARAM)MAKELPARAM(FALSE,0));
    return A;
}

HWND BCX_Button2(char* Text, HWND hWnd, int id, int X, int Y, int W, int H, int Style, int Exstyle)
{
	HWND  A;
	if (!Style) Style = WS_CHILD | WS_VISIBLE | WS_BORDER;

	A = CreateWindowEx(0, "Button", Text, Style, X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY, hWnd, (HMENU)id, BCX_hInstance, NULL);
	SendMessage(A, (UINT)WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), (LPARAM)MAKELPARAM(FALSE, 0));
	return A;
}

HWND BCX_Editbox(char* Text, HWND hWnd, int id, int X, int Y, int W, int H, int Style, int Exstyle)
{
	HWND  A;
	if (!Style)
	{
		Style = WS_VISIBLE | WS_CHILD | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_TABSTOP;
	}
	if (Exstyle == -1)
	{
		Exstyle = WS_EX_CLIENTEDGE;
	}
	A = CreateWindowEx(Exstyle, "EDIT", NULL, Style,
		X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY,
		hWnd, (HMENU)id, BCX_hInstance, NULL);
	SendMessage(A, (UINT)WM_SETFONT, (WPARAM)GetStockObject
		(DEFAULT_GUI_FONT), (LPARAM)MAKELPARAM(FALSE, 0));
	return A;
}


void ParseCommand(char* pMsg)
{
	if (pMsg == NULL) return;

	char   seps[] = "= \t\n";
	char   * token, *token2;
	class  CStrTok * pStrTok;
	char * cName[11];
	char buff[256];
	BOOL bFlag;
	int i;
	pStrTok = new class CStrTok(pMsg, seps);
	token = pStrTok->pGet();
	token2 = pStrTok->pGet();
	bFlag = false;

	ZeroMemory(cName, sizeof(cName));

	SendMessage(List5, (UINT)LB_RESETCONTENT, 0, 0);
	ItemCount5 = 0;

	if (memcmp(pMsg, "/help", 5) == 0) {
		bFlag = TRUE;
		wsprintf(buff, "Command List:");
		CommandList(buff);
		wsprintf(buff, "****************************************************");
		CommandList(buff);
		wsprintf(buff, "/deslog [Acc]");
		CommandList(buff);
		wsprintf(buff, "* Force Login Out Account");
		CommandList(buff);
		wsprintf(buff, "----------------------------------------------------------------------");
		CommandList(buff);

		wsprintf(buff, "/help");
		CommandList(buff);
		wsprintf(buff, "* List of commands");
		CommandList(buff);
		return;
	}

	if (memcmp(pMsg, "/deslog ", 8) == 0) {
		bFlag = TRUE;
		wsprintf(buff, "Login Out Acc: %s", token);
		CommandList(buff);
		G_pMainLog->DeslogAcc(token);
		return;
	}

	if ((!bFlag) && (pMsg != NULL)) {
		wsprintf(buff, "%s - Comando Invalido", pMsg);
		CommandList(buff);
		CommandList("List Comand: '/help' ");
		return;
	}
}*/