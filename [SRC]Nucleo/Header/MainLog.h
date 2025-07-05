#if !defined(AFX_MAIN_H__0089D9E3_74E6_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_MAIN_H__0089D9E3_74E6_11D2_A8E6_00001C7030A6__INCLUDED_

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include "XSocket.h"
#include "Msg.h"
#include "Client.h"
#include "StrTok.h"
#include "Game.h"
#include "Map.h"
#include "Account.h"
#include "MapServer.h"

#include "../General/GlobalDef.h"
#include "../../Shared/netmsg.h"

#define DEF_MAXMAPSERVERSOCK		11 //sockets used by Wlservers
#define DEF_MAXCLIENTSOCK			2000
#define DEF_MSGQUEUESIZE		    100000
#define DEF_MAXGAME					100 //total game servers
#define DEF_MAXMAPS					1000
#define DEF_MAXACCOUNTS				3000 // Old 1000 -> New 3000
#define DEF_MAXCHARACTER			4000 //4000
#define DEF_MAXHWLSERVERLIST		10
#define DEF_MAXMAPSERVERS			15
#define DEF_MAXCLIENTS				10
#define DEF_MAXPLAYERACCOUNTS		5000

#define DEF_INDEX4_MSGID	0
#define DEF_INDEX3_MSGID	2
#define DEF_INDEX2_MSGTYPE	4

#define DEF_USE_ENCRYPTION			0

extern void UpdateScreen();

static void memcpy_secure(void * cIn, const void * cOut, size_t n) {
	if ((cIn == NULL) || (cOut == NULL) || (n <= 0) || IsBadReadPtr(cIn, n) || IsBadReadPtr(cOut, n)) return;

	ZeroMemory(cIn, sizeof(cIn));
	memcpy(cIn, cOut, n);
}
class CMainLog
{
public:
	HWND  m_hWnd;
	CMainLog(HWND hWnd);
	~CMainLog(void);

	class CMsg		* m_pMsgQueue[DEF_MSGQUEUESIZE];			// 10DCCh
	class CClient	* m_pClientList[DEF_MAXCLIENTSOCK];
	class CGame	* m_pGameList[DEF_MAXGAME];
	class CMap	* m_pMapList[DEF_MAXMAPS];
	class CAccount	* m_pAccountList[DEF_MAXACCOUNTS];
	class CCharacter  * m_pCharList[DEF_MAXCHARACTER];
	class CMapServer	* m_pMapServerList[DEF_MAXCLIENTS];

	char m_pMsgBuffer[DEF_MSGBUFFERSIZE + 1];
	int m_iQueueHead, m_iQueueTail;	
	char m_cMainServerAddress[20], m_cMapServerAddress[20];
	int m_iMainServerInternalPort, m_iMainServerPort;
	int m_iValidAccounts, m_iValidCharacters, m_iTotalGame;

	int m_iTotalMapServerSock;
	int m_iTotalMainLogSock;
	BOOL bMainActivation, bShutDown;

	DWORD m_dwMinute;

	void MsgProcess();
	BOOL bGetMsgQuene(char * pFrom, char * pData, DWORD * pMsgSize, int * pIndex, char * pKey);
	BOOL bPutMsgQuene(char cFrom, char * pData, DWORD dwMsgSize, int iIndex, char cKey);
	void OnTimer();
	BOOL bAccept(class XSocket * pXSock);
	BOOL bAccept_MapServer(class XSocket * pXSock);
	bool bInit();
	void OnClientSubLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void SendEventToClient(DWORD dwMsgID, WORD wMsgType, char * pData, DWORD dwMsgSize, int iClientH);
	void DeleteAccount(char cAccountName[11] = NULL);
	BOOL SaveAccountInfo(int iAccount, char cAccountName[11] = NULL, char cTemp[11] = NULL, char cCharName[11] = NULL, char cMode = NULL, int iClientH = NULL);
	void SaveInfo(char cFileName[255], char *pData,  DWORD dwStartSize);
	
	//functions used for messages recieved
	//void RegisterSocketWL(int iClientH, char *pData, bool bRegister);
	void RegisterGameServer(int iClientH, char *pData, bool bRegister);
	BOOL bClientRegisterMaps(int iClientH, char *pData);
	void RequestLogin(int iClientH, char *pData);
	void ResponseCharacter(int iClientH, char *pData);
	void DeleteCharacter(int iClientH, char *pData);
	//void ChangePassword(int iClientH, char *pData);
	void RequestEnterGame(int iClientH, char *pData);

	//geting or checking account info
	int GetAccountInfo(int iClientH, char cAccountName[11], char cAccountPass[11], char cWorldName[30], int * iAccount);

	//void CleanupLogFiles();
	BOOL bReadServerConfigFile(char *cFn);

	void CreateNewCharacterFile(char *pData, char * cNewCharName, char * cAccountName, char * cPassword, char cNewGender, char cNewSkin, int iTempAppr1, int iTempAppr2, int iTempAppr3, char cNewStr, char cNewVit, char cNewDex, char cNewInt, char cNewMag, char cNewChr, short sAppr1, short sAppr2, short sAppr3, short sAppr4);
	int iGetCharacterData(char * cCharName, char * cMapName, short * sAppr1, short * sAppr2, short * sAppr3, short * sAppr4, int * iApprColor, char * cSex, char * cSkin, int * iLevel, int * iExp, int * iStr, int * iVit, int * iDex, int * iInt, int * iMag, int * iCharisma, int * iSaveYear, int * iSaveMonth, int * iSaveDay, int * iSaveHour, int * iSaveMinute);
	
	//void ClientRegisterGameserver(int iClientH, char * pData);
	void ResponseRegisterGameServer(int iClientH, BOOL bSuccesfull);

	BOOL m_bGameServerList;
	char  m_cWorldName[30];

	int OnPlayerAccountMessage(DWORD dwMsgID, char * pData, char * pData2, char * pData3, int iLevel);

	void OnMapServerSubLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void OnMapServerRead(int iClientH);
	void RequestPlayerData(int iClientH, char *pData);
	void VerifyCharacterIntegrity(char *cCharacterName, char *cAccountName, int *iLevel, char *cGuildName, char *cGuildRank, DWORD *dwGuildGUID);
	void VerifyGuildIntegrity(char *cGuildName, DWORD *dwGuildGUID);

	void EnterGameConfirm(int iClientH, char *pData);
	int iGetCharacterInformation(char * cAccountName, char * cCharacterName, int * iLevel);

	void RequestSavePlayerData(int iClientH, char *pData, DWORD dwMsgSize, BOOL bVar1, BOOL bVar2);
	void RequestNoSaveLogout(int iClientH, char *pData);
	void ClearAccountStatus(char cAccountName[11]);

	int RequestSetAccountWaitStatus(char *pData);

	// Guild Functions
	void RequestCreateNewGuild(int iClientH, char *pData);
	void RequestDisbandGuild(int iClientH, char *pData);
	void UpdateGuildInfoNewGuildsman(int iClientH, char *pData);
	void UpdateGuildInfoDeleteGuildman(int iClientH, char *pData);

	//void CheckClientTimeout();

	char G_cClientCheckCount;
	void DeleteAccountTimmer();
	void RequestDeleteAccount(char *pData);

	void DeslogAcc(char cAccountName[11]);

	BOOL bCheckValidName(char *pStr);
	void CreateNewAccount(int iClientH, char *pData);
	int GetAccountInfo2(int iClientH, char cAccountName[11], char cEmail[52], char cAccountPass[11], char cWorldName[30], int* iAccount);
	void ChangePassword(int iClientH, char* pData);
};

#endif