// Item.cpp: implementation of the CItem class.
//
//////////////////////////////////////////////////////////////////////

#include "../Header/Item.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItem::CItem()
{
	ZeroMemory(m_cName, sizeof(m_cName));
	m_sSprite = 0;
	m_sSpriteFrame = 0;
											  
	m_sItemEffectValue1 = 0;
	m_sItemEffectValue2 = 0;
	m_sItemEffectValue3 = 0; 

	m_sItemEffectValue4 = 0;
	m_sItemEffectValue5 = 0;
	m_sItemEffectValue6 = 0; 

	m_dwCount = 1;
	m_sTouchEffectType   = 0;
	m_sTouchEffectValue1 = 0;
	m_sTouchEffectValue2 = 0;
	m_sTouchEffectValue3 = 0;
	
	m_cItemColor = 0;
	m_sItemSpecEffectValue1 = 0;
	m_sItemSpecEffectValue2 = 0;
	m_sItemSpecEffectValue3 = 0;

	m_sSpecialEffectValue1 = 0;
	m_sSpecialEffectValue2 = 0;
	
	m_wCurLifeSpan = 0;
	m_dwAttribute   = 0;

	m_cCategory = NULL;
	m_sIDnum    = 0;

	m_bIsForSale = FALSE;

	m_bagPositionX = 40;
    m_bagPositionY = 30;
	sIndex = -1;
	m_dwTimeDrop = -1;

	m_iCoinVal = 0;
}

CItem::~CItem()
{

}

void CItem::SetBagPosition(short positionX, short positionY)
{
    m_bagPositionX = positionX;
    m_bagPositionY = positionY;
}

short CItem::GetBagPositionX()
{
	return m_bagPositionX;
}
short CItem::GetBagPositionY()
{
	return m_bagPositionY;
}

// Bank Page's
void CItem::SetBankPosition(short BankPosition)
{
	m_sBankPosition = BankPosition;
}

short CItem::GetBankPosition()
{
	return m_sBankPosition;
}

//heaton 2024
bool CItem::IsArmor()
{
	return strcmp(m_cName, "Hauberk(M)") == 0 ||
		strcmp(m_cName, "Hauberk(W)") == 0 ||
		strcmp(m_cName, "Helm(M)") == 0 ||
		strcmp(m_cName, "Helm(W)") == 0 ||
		strcmp(m_cName, "Robe(M)") == 0 ||
		strcmp(m_cName, "Robe(W)") == 0 ||
		strcmp(m_cName, "ChainHose(M)") == 0 ||
		strcmp(m_cName, "ChainHose(W)") == 0 ||
		strcmp(m_cName, "FullHelm(M)") == 0 ||
		strcmp(m_cName, "FullHelm(W)") == 0 ||
		strcmp(m_cName, "ChainMail(M)") == 0 ||
		strcmp(m_cName, "ChainMail(W)") == 0 ||
		strcmp(m_cName, "PlateMail(M)") == 0 ||
		strcmp(m_cName, "PlateMail(W)") == 0 ||
		strcmp(m_cName, "Horned-Helm(M)") == 0 ||
		strcmp(m_cName, "Horned-Helm(W)") == 0 ||
		strcmp(m_cName, "Wings-Helm(M)") == 0 ||
		strcmp(m_cName, "Wings-Helm(W)") == 0 ||
		strcmp(m_cName, "WizardHauberk(M)") == 0 ||
		strcmp(m_cName, "WizardHauberk(W)") == 0 ||
		strcmp(m_cName, "Wizard-Cap(M)") == 0 ||
		strcmp(m_cName, "Wizard-Cap(W)") == 0 ||
		strcmp(m_cName, "Wizard-Hat(M)") == 0 ||
		strcmp(m_cName, "Wizard-Hat(W)") == 0 ||
		strcmp(m_cName, "Cape") == 0 ||
		strcmp(m_cName, "Cape+1") == 0 ||
		strcmp(m_cName, "KnightHauberk(M)") == 0 ||
		strcmp(m_cName, "KnightFullHelm(M)") == 0 ||
		strcmp(m_cName, "KnightPlateMail(M)") == 0 ||
		strcmp(m_cName, "KnightPlateLeg(M)") == 0 ||
		strcmp(m_cName, "Knight-Cap(M)") == 0 ||
		strcmp(m_cName, "KnightMageBerk(M)") == 0 ||
		strcmp(m_cName, "KnightChain(M)") == 0 ||
		strcmp(m_cName, "KnightHauberk(W)") == 0 ||
		strcmp(m_cName, "KnightFullHelm(W)") == 0 ||
		strcmp(m_cName, "KnightPlateMail(W)") == 0 ||
		strcmp(m_cName, "KnightPlateLeg(W)") == 0 ||
		strcmp(m_cName, "Knight-Cap(W)") == 0 ||
		strcmp(m_cName, "KnightMageBerk(W)") == 0 ||
		strcmp(m_cName, "LongBoots") == 0 ||
		strcmp(m_cName, "Shoes") == 0 ||
		strcmp(m_cName, "KnightChain(W)") == 0;
}

bool CItem::PermitedInExp()
{
	return strcmp(m_cName, "Cape") == 0 ||
		strcmp(m_cName, "Shoes") == 0 ||
		strcmp(m_cName, "LongBoots") == 0 ||
		strcmp(m_cName, "Cape+1") == 0;
}


bool CItem::IsShield()
{
	return strcmp(m_cName, "WoodShield") == 0 ||
		strcmp(m_cName, "LeatherShield") == 0 ||
		strcmp(m_cName, "TargeShield") == 0 ||
		strcmp(m_cName, "LagiShield") == 0 ||
		strcmp(m_cName, "TowerShield") == 0;
}

bool CItem::IsWeapon()
{	//swords
	return strcmp(m_cName, "DemonSlayer") == 0 ||
		strcmp(m_cName, "DarExecutor") == 0 ||
		strcmp(m_cName, "LightingBlade") == 0 ||
		strcmp(m_cName, "KlonessBlade") == 0 ||
		strcmp(m_cName, "KlonessHammer") == 0 ||
		strcmp(m_cName, "KlonessAxe") == 0 ||
		strcmp(m_cName, "GiantBattleHammer") == 0 ||
		strcmp(m_cName, "The_Devastator") == 0 ||
		strcmp(m_cName, "StormBringer") == 0 ||
		strcmp(m_cName, "PowerBarbarian") == 0 ||
		strcmp(m_cName, "PowerRapier") == 0 ||
		strcmp(m_cName, "PkssBlade") == 0 ||
		strcmp(m_cName, "StripIceHammer") == 0 ||
		strcmp(m_cName, "PkssBarbarian") == 0 ||
		strcmp(m_cName, "PkssAxe") == 0 ||
		strcmp(m_cName, "PkssEsterk") == 0 ||
		strcmp(m_cName, "BattleHammer") == 0 ||
		strcmp(m_cName, "BarbarianHammer") == 0 ||
		strcmp(m_cName, "PowerHeroSword") == 0 ||
		strcmp(m_cName, "BlackHammer") == 0 ||
		strcmp(m_cName, "GiantSword") == 0 ||


		//wands
		strcmp(m_cName, "MagicWand(MS30-LLF)") == 0 ||
		strcmp(m_cName, "BerserkWand(MS.30)") == 0 ||
		strcmp(m_cName, "BerserkWand(MS.10)") == 0 ||
		strcmp(m_cName, "ResurWand(MS.30)") == 0 ||
		strcmp(m_cName, "ResurWand(MS.10)") == 0 ||
		strcmp(m_cName, "KlonessWand(MS.30)") == 0 ||
		strcmp(m_cName, "WhiteMWand(MS.60)") == 0 ||
		strcmp(m_cName, "PkssWand(MS.20)") == 0 ||
		strcmp(m_cName, "PkssWand(MS.40)") == 0 ||
		strcmp(m_cName, "PowerHeroWand") == 0 ||
		strcmp(m_cName, "KlonessWand(MS.10)") == 0;
}