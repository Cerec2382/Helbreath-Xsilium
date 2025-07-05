// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_

#include <windows.h>
#include <winbase.h>
#include "..\\..\\Shared\\netmsg.h"
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
										   
#define DEF_MAXITEMEQUIPPOS		15
#define DEF_EQUIPPOS_NONE		0		// ������ �Ұ����ϴ�.
#define DEF_EQUIPPOS_HEAD		1
#define DEF_EQUIPPOS_BODY		2
#define DEF_EQUIPPOS_ARMS		3
#define DEF_EQUIPPOS_PANTS		4
#define DEF_EQUIPPOS_LEGGINGS	5
#define DEF_EQUIPPOS_NECK		6
#define DEF_EQUIPPOS_LHAND		7
#define DEF_EQUIPPOS_RHAND		8
#define DEF_EQUIPPOS_TWOHAND	9
#define DEF_EQUIPPOS_RFINGER	10
#define DEF_EQUIPPOS_LFINGER	11
#define DEF_EQUIPPOS_BACK		12
#define DEF_EQUIPPOS_FULLBODY	13			//��� �ٵ� �� �Ź� ���� ����
#define DEF_ITEMTYPE_NOTUSED	-1			// v1.4 ������ ���Ǿ����� ����μ��� ������ �ʴ� ������: ��������� ��ü�ȴ�.
#define DEF_ITEMTYPE_NONE		 0
#define DEF_ITEMTYPE_EQUIP		 1
#define DEF_ITEMTYPE_APPLY		 2
#define DEF_ITEMTYPE_USE_DEPLETE 3       
#define DEF_ITEMTYPE_INSTALL	 4
#define DEF_ITEMTYPE_CONSUME	 5
#define DEF_ITEMTYPE_ARROW		 6
#define DEF_ITEMTYPE_EAT		 7
#define DEF_ITEMTYPE_USE_SKILL   8
#define DEF_ITEMTYPE_USE_PERM    9
#define DEF_ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX	10
#define DEF_ITEMTYPE_USE_DEPLETE_DEST			11
#define DEF_ITEMTYPE_MATERIAL					12


#define DEF_ITEMEFFECTTYPE_NONE				0
#define DEF_ITEMEFFECTTYPE_ATTACK			1		// ����ġ: value1 D value2 + value3
#define DEF_ITEMEFFECTTYPE_DEFENSE			2		// ���ɷ� 
#define DEF_ITEMEFFECTTYPE_ATTACK_ARROW		3		// ȭ���� ����ϴ� ���ݹ���. ���� ��ü�� ���ݷ��� ����.		
#define DEF_ITEMEFFECTTYPE_HP   		    4		// HP���� ȿ�� 
#define DEF_ITEMEFFECTTYPE_MP   		    5		// MP���� ȿ�� 
#define DEF_ITEMEFFECTTYPE_SP   		    6		// SP���� ȿ�� 
#define DEF_ITEMEFFECTTYPE_HPSTOCK 		    7		// HP���� ȿ��. �ﰢ������ ������ ��Ÿ������ �ʴ´�. 
#define DEF_ITEMEFFECTTYPE_GET			    8		// ��´�. ����⳪ ������ 
#define DEF_ITEMEFFECTTYPE_STUDYSKILL		9		// ����� ���� �������̴�.
#define DEF_ITEMEFFECTTYPE_SHOWLOCATION		10		// ��ġ�� ǥ���ϴ� ������. 
#define DEF_ITEMEFFECTTYPE_MAGIC			11		// ����ϸ� ���� ȿ���� ��� ������ 
#define DEF_ITEMEFFECTTYPE_CHANGEATTR		12		// �÷��̾��� �Ӽ��� ��ȯ��Ų��. ����, �Ӹ����, �Ǻλ�, ��Ÿ�� 
#define DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE	13		// ����ȿ���� ���Ҿ� ������ ���� ȿ���� �ִ�.(�������)
#define DEF_ITEMEFFECTTYPE_ADDEFFECT	    14		// �߰� ȿ��
#define DEF_ITEMEFFECTTYPE_MAGICDAMAGESAVE	15		// �������� ���� ������ ������� �����Ѵ�. ������ ����̷� 
#define DEF_ITEMEFFECTTYPE_OCCUPYFLAG		16		// ���� ��� 
#define DEF_ITEMEFFECTTYPE_DYE				17		// ������. 
#define DEF_ITEMEFFECTTYPE_STUDYMAGIC		18		// ������ ���� �������̴�.
#define DEF_ITEMEFFECTTYPE_ATTACK_MAXHPDOWN	19		// ���� ȿ���� �Բ� �ִ�  HP �� HP ȸ������ �������� ������ 
#define DEF_ITEMEFFECTTYPE_ATTACK_DEFENSE	20		// ���� ȿ���� �Բ� ���ʰ� ���� ���� �� ȿ���� �ִ� ������ 
#define DEF_ITEMEFFECTTYPE_MATERIAL_ATTR	21		// ���� ���� ��� �Ӽ� 
#define DEF_ITEMEFFECTTYPE_FIRMSTAMINAR		22		// ���¹̳� ���� ������ 
#define DEF_ITEMEFFECTTYPE_LOTTERY			23		// ���� ������

#define DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY		24	// ���� ������ Ư�� ȿ�� 
#define DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY	25	// �� ������ Ư�� ȿ�� 
#define DEF_ITEMEFFECTTYPE_ALTERITEMDROP		26	// �ٸ� ������ ������ �� ��� �� �������� ��������.

#define DEF_ITEMEFFECTTYPE_CONSTRUCTIONKIT		27	// ���� ŰƮ
#define DEF_ITEMEFFECTTYPE_WARM					28	// �� ���� ���δ�. v2.172 2002-7-5
#define DEF_ITEMEFFECTTYPE_DEFENSE_ANTIMINE		29	// ���ɷ� v2.172 2002-7-5
#define DEF_ITEMEFFECTTYPE_ITEMFARM				30  //v2.19 2002-12-16 ��� ��ų ����
#define DEF_ITEMEFFECTTYPE_SLATES				31
#define DEF_ITEMEFFECTTYPE_ARMORDYE				32

#define DEF_ITEMEFFECTTYPE_ADDREPUTATION	35
#define DEF_ITEMEFFECTTYPE_SWEAPONATTRIBUTE_STONE	36  //by ogeid Atributos secundarios
#define DEF_ITEMEFFECTTYPE_PWEAPONATTRIBUTE_STONE	37 //by ogeid Atributos primarios
#define DEF_ITEMEFFECTTYPE_ADDCOINS			38
#define DEF_ITEMEFFECTTYPE_SUMMONBALL		39

#define DEF_ITEMEFFECTTYPE_REMOVEITEMID		40 // ZeroEoyPnk - Remove Item ID
#define DEF_ITEMEFFECTTYPE_MasterItemID		41 // ZeroEoyPnk - Remove Item ID
#define DEF_ITEMEFFECTTYPE_RANKUPDIEZ		42
#define DEF_ITEMEFFECTTYPE_RANKUPVEINTE		43
#define DEF_ITEMEFFECTTYPE_RANKUPMAX		44

//#define DEF_ITEMEFFECTTYPE_SEXITEMCHANGE	80 // ZeroEoyPnk - Remove Item ID

#define DEF_ITEMEFFECTTYPE_CONTRIBUTION		45

#define ITEMEFFECTTYPE_BALL 62

#define DEF_ITEMEFFECTTYPE_COIN_BALL		47
#define DEF_ITEMEFFECTTYPE_REP_BALL		48
#define DEF_ITEMEFFECTTYPE_MAJ_BALL		49

#define DEF_ITEMEFFECTTYPE_ADDCRITICALS		55
#define DEF_ITEMEFFECTTYPE_DEDUCTREPUTATION	60

#define DEF_ITEMEFFECTTYPE_CANDYINSERT	65
#define DEF_ITEMEFFECTTYPE_CANDYSUPRIMIR	66

#define DEF_ITEMEFFECTTYPE_SacoItemID	67

#define DEF_ITET_UNIQUE_OWNER				1		// ItemTouchEffect: ������ ���� ������ 
#define DEF_ITET_ID							2		// �׳� ������ ���̵�
#define DEF_ITET_DATE						3		// ��밡���� ��¥�� ������ ������ 


class CItem  
{
public:

	CItem();
	virtual ~CItem();

	char  m_cName[21];
	
	short m_sIDnum;					// �������� ���� ��ȣ 
	char  m_cItemType;
	char  m_cEquipPos;
	short m_sItemEffectType;
	short m_sItemEffectValue1, m_sItemEffectValue2, m_sItemEffectValue3;
	short m_sItemEffectValue4, m_sItemEffectValue5, m_sItemEffectValue6;

	//Fix Endu 	
//	WORD  m_wMaxLifeSpan;
	int   m_wMaxLifeSpan;
	short m_sSpecialEffect;
	
	//short m_sSM_HitRatio, m_sL_HitRatio;
	//v1.432 ���߷� ���� ��� ���Ѵ�. ��� Ư�� �ɷ� ��ġ�� ����.
	short m_sSpecialEffectValue1, m_sSpecialEffectValue2;

	short m_sSprite;
	short m_sSpriteFrame;

	char  m_cApprValue;
	char  m_cSpeed;

	DWORD m_wPrice;
	WORD  m_wWeight;
	short m_sLevelLimit;
	char  m_cGenderLimit;

	short m_sRelatedSkill;
	
	char  m_cCategory;
	BOOL  m_bIsForSale;
	// 

	DWORD m_dwCount;
	short m_sTouchEffectType;
	short m_sTouchEffectValue1, m_sTouchEffectValue2;
	int m_sTouchEffectValue3;
	char  m_cItemColor; // v1.4 ���� ������ ������ ����Ǿ���. 
//	short m_sItemSpecEffectValue1, m_sItemSpecEffectValue2, m_sItemSpecEffectValue3;
	//Fix Endurance
	int m_sItemSpecEffectValue1, m_sItemSpecEffectValue2, m_sItemSpecEffectValue3;
	int  m_wCurLifeSpan;
	DWORD m_dwAttribute;				// aaaa bbbb cccc dddd eeee ffff xxxx xxx1 
										// 1: Custom-Made Item flag 
										// a: �߰� �����
										// b: RESERVED
										// c: Ư�� ������ ���� Flag 
										// d: Ư�� ������ ���� ���� 
										// e: Ư�� ������ �߰� ���� Flag
	int m_iCoinVal;
	DWORD m_dwTimeDrop;
	short sIndex;

private:
    short m_bagPositionX;
    short m_bagPositionY;
	short m_sBankPosition;

public:

    short GetBagPositionX();
    short GetBagPositionY();
	short GetBankPosition();

	//heaton 2024
	bool IsArmor();
	bool PermitedInExp();
	bool IsWeapon();
	bool IsShield();

    void SetBagPosition(short positionX, short positionY);
	void SetBankPosition(short BankPosition);

	DWORD GetBallValue()
	{
		return m_dwCount;
	}

	bool isBall()
	{
		return m_sIDnum == (short)BItemID::ContributionBall || m_sIDnum == (short)BItemID::MajesticBall || m_sIDnum == (short)BItemID::CoinBall || m_sIDnum == (short)BItemID::RepBall;
	}
};



#endif // !defined(AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_)
