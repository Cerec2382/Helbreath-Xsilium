#include "../Header/Map.h"

CMap::CMap(int iClientH, char *pData)
{
	ZeroMemory(m_cMapName, sizeof(m_cMapName));
	iIndex = -1; // default if not registered
	memcpy(m_cMapName, pData, 30);
	iIndex = iClientH;						// 0BE1Ch->10h
}

CMap::~CMap()
{

}