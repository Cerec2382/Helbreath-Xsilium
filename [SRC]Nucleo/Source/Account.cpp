#include "../Header/Account.h"

CAccount::CAccount(char * cDataName, char * m_cWorldName, int iClient)
{
	ZeroMemory(cAccountName, sizeof(cAccountName));	
	ZeroMemory(cWorldName, sizeof(cWorldName));
	memcpy(cAccountName, cDataName, 11);
	memcpy(cWorldName, m_cWorldName, 30);
	ZeroMemory(cPassword,sizeof(cPassword));
	ZeroMemory(cCharName, sizeof(cCharName));
	ZeroMemory(cEmail, sizeof(cEmail));
	iAccountValid = 0;
	m_iAccntYear = m_iAccntMonth = m_iAccntDay = 0;
	m_iPassYear = m_iPassMonth = m_iPassDay = 0;
	dAccountID = -1;
	iClientH = iClient;
	cAccountType = 0;

	// Ex WL
	dwLogInTime = NULL;
	ZeroMemory(m_cOnGameServer, sizeof(m_cOnGameServer));
	m_iLevel = 0;

	InGame = false;
}

CAccount::~CAccount()
{

}

CCharacter::CCharacter(char * cDataName, int m_iTracker)
{
	ZeroMemory(cCharacterName, sizeof(cCharacterName));			
	memcpy(cCharacterName, cDataName, 11);
	iTracker = m_iTracker;
}

CCharacter::~CCharacter()
{

}