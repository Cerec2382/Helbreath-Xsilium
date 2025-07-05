#include "../Headers/Game.h"
#include "../Headers/Winmain.h"
#include "../Language/lan_eng.h"
//added resolution
#include "..\Resolution\Resolution.h"
#include <algorithm>
#include <thread>

#include <string>
#include <cctype>
#include <map>

extern class cResolution * c_reso;
extern char G_cSpriteAlphaDegree;

extern char G_cCmdLine[256], G_cCmdLineTokenA[120], G_cCmdLineTokenA_Lowercase[120], G_cCmdLineTokenB[120], G_cCmdLineTokenC[120], G_cCmdLineTokenD[120], G_cCmdLineTokenE[120];
extern class XSocket * G_pCalcSocket;
extern BOOL G_bIsCalcSocketConnected;
extern DWORD G_dwCalcSocketTime, G_dwCalcSocketSendTime;
extern HWND	G_hWnd, G_hEditWnd;
extern HINSTANCE G_hInstance;

extern bool _tmp_bSpriteOmit;

char _cDrawingOrder2[] = { 0, 1, 0, 0, 0, 0, 0, 1, 1 };
char _cMantleDrawingOrder2[] = { 0, 1, 1, 1, 0, 0, 0, 2, 2 };
char _cMantleDrawingOrderOnRun2[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1 };

static char __cSpace[] = { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 8, 7, 8, 8, 9, 10, 9, 7, 8, 8, 8, 8, 8, 8, 8,
15, 16, 12, 17, 14, 15, 14, 16, 10, 13, 19, 10, 17, 17, 15, 14, 15, 16, 13, 17, 16, 16, 20, 17, 16, 14,
8, 8, 8, 8, 8, 8, 8, 6, 7, 8, 7, 7, 7, 7, 4, 7, 7, 4, 11, 7, 8, 8, 7, 8, 6, 5, 8, 9, 14, 8, 9, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8 };

void CGame::DrawShadowMenu(short x, short y, short x2, short y2)
{
	m_DDraw.DrawShadowBox(x, y, x2, y2);
	m_DDraw.DrawShadowBox(x, y, x2, y2);
}

void CGame::DrawDialogShadowPixels(int mode, int count, short x, short y)
{
	switch (mode)
	{
	case 1:
	{
		for (int d = 0; d < count; d++)
		{
			m_DDraw.PutPixel(x + (d), y, 73, 58, 42);
		}
		break;
	}

	case 2:
	{
		for (int d = 0; d < count; d++)
		{
			m_DDraw.PutPixel(x, y + (d), /*152, 123, 54);*/73, 58, 42);
		}
		break;
	}
	}
}

void CGame::DrawShadowMenu_DialogBorders(short sX, short sY, short szX, short szY)
{
	DrawDialogShadowPixels(1, szX, sX, sY);
	DrawDialogShadowPixels(1, szX, sX, sY + szY);
	DrawDialogShadowPixels(1, szX, sX, sY + 23);
	DrawDialogShadowPixels(2, szY, sX, sY);
	DrawDialogShadowPixels(2, szY + 1, sX + szX, sY);
}

void CGame::DrawShadowMenu_DialogBordersNoTitle(short sX, short sY, short szX, short szY)
{
	DrawDialogShadowPixels(1, szX, sX, sY);
	DrawDialogShadowPixels(1, szX, sX, sY + szY);
	//DrawDialogShadowPixels(1, szX, sX, sY + 23);
	DrawDialogShadowPixels(2, szY, sX, sY);
	DrawDialogShadowPixels(2, szY + 1, sX + szX, sY);
}

void CGame::DrawNewDialogBox2(int cType, int sX, int sY, int iFrame, BOOL bIsNoColorKey, BOOL bIsTrans)
{
	DWORD dwTime = G_dwGlobalTime;

	if (m_pSprite[cType] == NULL) return;
	if (bIsNoColorKey == FALSE)
	{
		if (bIsTrans == TRUE)
			m_pSprite[cType]->PutTransSprite2(sX, sY, iFrame, dwTime);
		else m_pSprite[cType]->PutSpriteFast(sX, sY, iFrame, dwTime);
	}
	else m_pSprite[cType]->PutSpriteFastNoColorKey(sX, sY, iFrame, dwTime);
}

void CGame::PutCenterString_SpriteFont(int iX1, int iX2, int iY, const char * pStr, short sR, short sG, short sB)
{
	int iXpos, iR, iG, iB;
	DWORD iCnt;
	DWORD dwTime = G_dwGlobalTime;
	char  cTmpStr[200];

	m_DDraw.ColorTransferRGB(RGB(sR, sG, sB), &iR, &iG, &iB);

	ZeroMemory(cTmpStr, sizeof(cTmpStr));
	strcpy(cTmpStr, pStr);

	int textWidth = 0;
	for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++) {
		if ((cTmpStr[iCnt] >= 33) && (cTmpStr[iCnt] <= 122)) {
			textWidth += __cSpace[cTmpStr[iCnt] - 33];
		}
		else {
			textWidth += 5;
		}
	}

	int areaWidth = iX2 - iX1;
	int initialOffset = (areaWidth - textWidth) / 2;

	iXpos = iX1 + initialOffset;

	for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++) {
		if ((cTmpStr[iCnt] >= 33) && (cTmpStr[iCnt] <= 122)) {
			if ((sR == 0) && (sG == 0) && (sB == 0)) {
				m_pSprite[DEF_SPRID_INTERFACE_FONT1]->PutSpriteFast(iXpos, iY, cTmpStr[iCnt] - 33, dwTime);
			}
			else {
				m_pSprite[DEF_SPRID_INTERFACE_FONT1]->PutSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 33, iR, iG, iB, dwTime);
			}
			iXpos += __cSpace[cTmpStr[iCnt] - 33];
		}
		else {
			iXpos += 5;
		}
	}
}

void CGame::DrawEnhancement_Tab(short sX, short sY, short msX, short msY, char * text, bool active)
{
	int isize = 128;

	//if (!active) DrawNewDialogBox2(SPRID_ENHANCEMENT_TAB, sX, sY, 0, FALSE, FALSE);

	auto mouse_over = (msX >= sX) && (msX <= sX + isize) && (msY >= sY) && (msY <= sY + 23);

	if (active)
		PutCenterString_SpriteFont(sX, sX + isize, sY + 2, text, 37, 199, 47);
	else if (mouse_over)
		PutCenterString_SpriteFont(sX, sX + isize, sY + 2, text, 255, 200, 0);
	else
		PutCenterString_SpriteFont(sX, sX + isize, sY + 2, text, 255, 0, 0);
}

void CGame::DrawMenu_EnhBox(short sX, short sY, short width, short height)
{
	DrawDialogShadowPixels(1, width, sX, sY);
	DrawDialogShadowPixels(1, width, sX, sY + height);
	DrawDialogShadowPixels(2, height, sX, sY);
	DrawDialogShadowPixels(2, height + 1, sX + width, sY);

	for (short y = 1; y < height; ++y) {
		for (short x = 1; x < width; ++x) {
			m_DDraw.PutTransparentPixel(sX + x, sY + y, 128, 128, 128);
		}
	}
}

void CGame::DrawDialogShadowPixelsEnc(int mode, int count, short x, short y)
{
	switch (mode)
	{
	case 1:
	{
		for (int d = 0; d < count; d++)
		{
			m_DDraw.PutPixel(x + (d), y, 73, 58, 42);
		}
		break;
	}

	case 2:
	{
		for (int d = 0; d < count; d++)
		{
			m_DDraw.PutPixel(x, y + (d), 73, 58, 42);
		}
		break;
	}
	}
}

void CGame::DrawShadowMenu_ExtMenuBorders(short sX, short sY, short szX, short szY)
{
	DrawDialogShadowPixelsEnc(1, szX, sX, sY);
	DrawDialogShadowPixelsEnc(1, szX, sX, sY + szY);
	DrawDialogShadowPixelsEnc(2, szY, sX, sY);
	DrawDialogShadowPixelsEnc(2, szY + 1, sX + szX, sY);
}
void CGame::DrawComboBoxEnc(short x, short y, short width, short height, const std::vector<std::string>& options, int selectedIndex, bool isOpen, short msX, short msY, bool isEnabled) {
	// Dibujar el menú principal con bordes reducidos
	DrawShadowMenu(x - 1, y - 1, x + width + 1, y + height + 1);

	// Determinar la opción seleccionada o mensaje a mostrar
	std::string selectedOption;
	if (!isEnabled) {
		selectedOption = m_sLang == LANG_ESP ? "Por favor, introduce un ítem" : "Please enter an item";
	}
	else if (options.empty()) {
		selectedOption = m_sLang == LANG_ESP ? "Nada para seleccionar" : "Nothing to select";
	}
	else {
		selectedOption = (selectedIndex == -1) ?
			(m_sLang == LANG_ESP ? "Selecciona una estadística" : "Select a statistic") :
			options[selectedIndex];
	}

	// Dibujar la opción seleccionada
	PutString2(x + 5, y + 3, (char*)selectedOption.c_str(), 255, 255, 255);

	if (isEnabled && !options.empty()) {
		// Dibujar la flecha del ComboBox
		if (isOpen) {
			DrawNewDialogBox2(SPRID_ARROWS + 1, x + width - 15, y + 5, 0);
		}
		else {
			DrawNewDialogBox2(SPRID_ARROWS, x + width - 15, y + 5, 0);
		}

		// Dibujar el menú extendido si está abierto
		if (isOpen) {
			// Calcular el alto del menú desplegable para abarcar todas las opciones
			short extMenuHeight = 20 * (options.size() - (selectedIndex == -1 ? 0 : 1));

			// Dibujar el shadow menu para todo el menú extendido
			DrawShadowMenu(x - 1, y + height - 1, x + width + 1, y + height + extMenuHeight + 1);

			// Dibujar cada opción en el menú desplegable
			int optionIndex = 0;
			for (size_t i = 0; i < options.size(); ++i) {
				if (i == selectedIndex) continue;

				// Calcular la posición Y de cada opción
				short optionY = y + height + (optionIndex * 20);

				// Cambiar color si el mouse está sobre la opción
				if (msX >= x && msX <= x + width && msY >= optionY && msY <= optionY + 20) {
					PutString2(x + 5, optionY, (char*)options[i].c_str(), 255, 165, 0); // Hover
				}
				else {
					PutString2(x + 5, optionY, (char*)options[i].c_str(), 200, 200, 200); // Normal
				}
				optionIndex++;
			}
		}
	}
}


void CGame::HandleComboBoxClick(short msX, short msY, short comboX, short comboY, short comboWidth, short comboHeight, const std::vector<std::string>& options, char cLB, int dnumber, bool isEnabled) {
	if (!isEnabled || options.empty()) return;

	if (cLB != 0 && iGetTopDialogBoxIndex() == dnumber && !isClickProcessed) {
		if (msX >= comboX && msX <= comboX + comboWidth && msY >= comboY && msY <= comboY + comboHeight) {
			isComboBoxOpen = !isComboBoxOpen;
			PlaySound('E', 14, 5);
			isClickProcessed = true;
		}
		else if (isComboBoxOpen) {
			int optionIndex = 0;
			for (size_t i = 0; i < options.size(); ++i) {
				if (i == selectedOptionIndex) continue;
				short optionY = comboY + comboHeight + (optionIndex * 20);
				if (msX >= comboX && msX <= comboX + comboWidth && msY >= optionY && msY <= optionY + 20) {
					selectedOptionIndex = i;
					isComboBoxOpen = false;
					PlaySound('E', 14, 5);
					isClickProcessed = true;
					selectedType = (selectedOptionIndex == 0) ? 1 : 2;
					break;
				}
				optionIndex++;
			}
		}
	}
	else if (cLB == 0) {
		isClickProcessed = false;
	}
}

void CGame::DrawMenu_SmallButton(short sX, short sY, short msX, short msY, short x, short y, char * text)
{
	if ((msX >= sX + x) && (msX <= sX + x + 76) && (msY >= sY + y) && (msY <= sY + y + 22)){
		DrawNewDialogBox2(SPRID_SMALLBUTTONS + 1, sX + x, sY + y, 0, FALSE, FALSE);
		PutAlignedString2(sX + x, sX + x + 76, sY + y + 2, text, 255, 240, 0);
	}
	else
	{
		DrawNewDialogBox2(SPRID_SMALLBUTTONS, sX + x, sY + y, 0, FALSE, FALSE);
		PutAlignedString2(sX + x, sX + x + 76, sY + y + 2, text, 255, 240, 255);
	}
}


void CGame::DrawNewPanelButton(short sX, short sY, short msX, short msY, short x, short y, char * text, bool istrue)
{
	if ((msX >= sX + x) && (msX <= sX + x + 86) && (msY >= sY + y) && (msY <= sY + y + 24) || istrue){
		DrawNewDialogBox2(SPRID_NEWPANELBUTS + 1, sX + x, sY + y, 0, FALSE, FALSE);
		PutAlignedString2(sX + x, sX + x + 86, sY + y + 3, text, 255,255,0);
	}
	else
	{
		DrawNewDialogBox2(SPRID_NEWPANELBUTS, sX + x, sY + y, 0, FALSE, FALSE);
		PutAlignedString2(sX + x, sX + x + 86, sY + y + 3, text, 255, 200, 0);
	}
}

void CGame::draw_enchanting(short msX, short msY, short msZ, char cLB) {
	short sX, sY, sizeX, sizeY;
	int dnumber = 72;
	int iValue, i;
	char cItemColor, cStr1[120], cStr2[120], cStr3[120], cStr4[120], cStr5[120], cStr6[120];
	DWORD dwTime = timeGetTime();
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;

	std::string selectedOptionText;
/*	DrawShadowMenu(sX, sY, sX + sizeX, sY + sizeY);
	DrawShadowMenu_DialogBorders(sX, sY, sizeX, sizeY);*/

	DrawShadowDialog2(sX, sY, dnumber);
//	PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Rebirth", 255, 255, 255);
	// title


	DrawNewDialogBox2(SPRID_FLAGSLANG, sX + 195 + 80, sY + 13, 0, FALSE, true);
	DrawNewDialogBox2(SPRID_FLAGSLANG + 1, sX + 195 + 28 + 80, sY + 13, 0, FALSE, true);

	//PutCenterString_SpriteFont(sX, sX + sizeX, sY + 13, "Enchanting", 255, 200, 0);

	bool tab1 = mode == 0;
	bool tab2 = mode == 1;

	if (mode == 0 || mode == 1)
	{
		DrawEnhancement_Tab(sX + 44 + 2, sY + 24 + 6, msX, msY, m_sLang == LANG_ESP ? "Extraer" : "Extract", tab1);
		DrawEnhancement_Tab(sX + 44 + 128 + 1, sY + 24 + 6, msX, msY, m_sLang == LANG_ESP ? "Mejorar" : "Upgrade", tab2);
	}

	if (mode == 2)
	{
		PutAlignedString2(sX, sX + sizeX, sY + 50, m_sLang == LANG_ESP ? "Confirmar Extraccion" : "Confirm Extraction", 255, 0, 0);
		PutAlignedString2(sX + 20, sX + sizeX - 20, sY + 75, m_sLang == LANG_ESP ? "¡ADVERTENCIA: Los item seleccionados se perderan!" : "WARNING: The selected items will be lost!", 255, 0, 0);
	}
	else if (mode == 3)
	{
		PutAlignedString2(sX, sX + sizeX, sY + 55, m_sLang == LANG_ESP ? "Mejorar estadistica del item" : "Upgrade item stat", 255, 0, 0);
	}

	switch (mode)
	{
	case 0:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Enchanting", 240, 240, 240);

		int iTotalLines = 0;
		for (int b = 0; b < 20; b++)
		{
			if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] != -1)
			{
				iTotalLines++;
			}
		}

		if (iTotalLines == 0)
		{
			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 95, m_sLang == LANG_ESP ? "Por favor ingresa un item o mas." : "Please enter one or more items.", 255, 255, 255);
		}

		int iPointerLoc;
		double d1, d2, d3;
		int maxlines = 13;

		if (iTotalLines > maxlines)
		{
			d1 = static_cast<double>(m_stDialogBoxInfo[72].sView);
			d2 = static_cast<double>(iTotalLines - maxlines);
			d3 = (275.0f * d1) / d2;
			iPointerLoc = static_cast<int>(d3 + 0.5);
			//DrawNewDialogBox2(SPRID_NEWSCROLL, sX + sizeX - 15, sY + 44, 0);
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + sizeX - 13, sY + 44 + iPointerLoc, 7);
		}
		else
		{
			iPointerLoc = 0;
		}

		int tempindex = -1;

		for (int i = 0; i < maxlines; i++)
		{
			if ((i + m_stDialogBoxInfo[72].sView) < iTotalLines)
			{
				int enchantingItemIndex = m_stDialogBoxInfo[dnumber].m_iEnchantingItem[i + m_stDialogBoxInfo[72].sView];
				if (enchantingItemIndex != -1)
				{
					char cTemp[512] = {};
					strcpy(cTemp, m_pItemList[enchantingItemIndex]->m_cName);
					if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
					{
						PutAlignedString(sX + 10, sX + sizeX - 10, sY + i * 18 + 75, cTemp, 255, 255, 255);
						tempindex = i + m_stDialogBoxInfo[72].sView;
					}
					else
					{
						PutAlignedString(sX + 10, sX + sizeX - 10, sY + i * 18 + 75, cTemp, 255, 200, 0);
					}
				}
			}
		}

		if (tempindex != -1 && (msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + (tempindex - m_stDialogBoxInfo[72].sView) * 18 + 75) && (msY <= sY + (tempindex - m_stDialogBoxInfo[72].sView) * 18 + 89))
		{
			DrawMarketItem(m_pItemList[m_stDialogBoxInfo[dnumber].m_iEnchantingItem[tempindex]], msX + 50, msY);
		}

		// Manejo de clic del botón izquierdo
		if (cLB != 0 && iTotalLines > maxlines)
		{
			if (iGetTopDialogBoxIndex() == 72)
			{
				if ((msX >= sX + sizeX - 13) && (msX <= sX + sizeX + 12) && (msY >= sY + 44) && (msY <= sY + 374))
				{
					d1 = static_cast<double>(msY - (sY + 44));
					d2 = static_cast<double>(iTotalLines - maxlines);
					d3 = (d1 * d2) / 275.0f;
					m_stDialogBoxInfo[72].sView = static_cast<int>(d3 + 0.5);
				}
			}
		}
		else
		{
			m_stDialogBoxInfo[72].bIsScrollSelected = FALSE;
		}

		// Manejo del scroll del mouse
		if (iGetTopDialogBoxIndex() == 72 && msZ != 0)
		{
			m_stDialogBoxInfo[72].sView -= msZ / 120;  // Ajustar este valor para un desplazamiento más controlado
			if (m_stDialogBoxInfo[72].sView < 0)
			{
				m_stDialogBoxInfo[72].sView = 0;
			}
			if (m_stDialogBoxInfo[72].sView > iTotalLines - maxlines)
			{
				m_stDialogBoxInfo[72].sView = iTotalLines - maxlines;
			}
			m_DInput.m_sZ = 0;  // Reiniciar el valor del scroll
		}

		// Limitar la vista dentro de los valores válidos
		if (iTotalLines > maxlines && m_stDialogBoxInfo[72].sView > iTotalLines - maxlines)
		{
			m_stDialogBoxInfo[72].sView = iTotalLines - maxlines;
		}
		if (m_stDialogBoxInfo[72].sView < 0 || iTotalLines <= maxlines)
		{
			m_stDialogBoxInfo[72].sView = 0;
		}

		DrawMenu_SmallButton(sX + 44, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY + 65, m_sLang == LANG_ESP ? "Extraer" : "Extract");
		DrawMenu_SmallButton(sX + 44, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY + 65, m_sLang == LANG_ESP ? "Cerrar" : "Close");
		
		break;
	}
	case 1:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Enchanting", 240, 240, 240);
		DrawMenu_EnhBox(sX + 44 + 94, sY + 132 + 20, 80, 114);
		std::vector<std::string> enhancementOptions;

		bool isEnabled = false;
		bool optionSelected = false;
		std::string selectedStatText;
		std::string showselectedStatText;
		int selectedValue = 0;
		int istype = -1;

		// Llenar el combo box con las variables globales
		char optionBuffer[200];

		sprintf(optionBuffer, "Light: %d shards", shard_light);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "Strong: %d shards", shard_strong);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "Hit Prob: %d fragments", fragment_hp);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "Defense Ratio: %d fragments", fragment_dr);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "HP Recovery: %d fragments", fragment_hprec);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "SP Recovery: %d fragments", fragment_sprec);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "MP Recovery: %d fragments", fragment_mprec);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "Magic Resist: %d fragments", fragment_mr);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "Physical Absorption: %d fragments", fragment_pa);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "Magic Absorption: %d fragments", fragment_ma);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "Exp: %d fragments", fragment_exp);
		enhancementOptions.push_back(optionBuffer);

		// Dibujar el sprite del ítem primero
		if (m_stDialogBoxInfo[dnumber].sV1 != -1) {
			short type1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00F00000) >> 20;
			short val1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x000F0000) >> 16;
			short type2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x0000F000) >> 12;
			short val2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00000F00) >> 8;

			char ctype1[200] = { 0 };
			char ctype2[200] = { 0 };
			char tmpctype1[200] = { 0 };
			char tmpctype2[200] = { 0 };
			int multiplier = 0;

			i = m_stDialogBoxInfo[dnumber].sV1;
			cItemColor = m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_cItemColor;
			if ((m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_cEquipPos == DEF_EQUIPPOS_LHAND)
				|| (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_cEquipPos == DEF_EQUIPPOS_RHAND)
				|| (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND)) {
				m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_sSprite]->PutSpriteRGB(sX + 44 + 134, sY + 182 + 20, m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_sSpriteFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
			}
			else {
				m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_sSprite]->PutSpriteRGB(sX + 44 + 134, sY + 182 + 20, m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_sSpriteFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
			}

			ZeroMemory(cStr1, sizeof(cStr1));
			ZeroMemory(cStr2, sizeof(cStr2));
			ZeroMemory(cStr3, sizeof(cStr3));
			GetItemName(m_pItemList[m_stDialogBoxInfo[dnumber].sV1], cStr1, cStr2, cStr3);
			PutAlignedString(sX + 44 + 24, sX + 44 + 248, sY + 230 + 20 + 20, cStr1, 255, 165, 0);
			PutAlignedString(sX + 44 + 24, sX + 44 + 248, sY + 245 + 20 + 20, cStr2, 255, 255, 255);
			PutAlignedString(sX + 44 + 24, sX + 44 + 248, sY + 260 + 20 + 20, cStr3, 255, 255, 255);

			isEnabled = true;
		}

		// Mostrar el combo box después del sprite
		DrawComboBoxEnc(sX + 17, sY + 28 + 25, sizeX - 36, 20, enhancementOptions, selectedOptionIndex, isComboBoxOpen, msX, msY, !enhancementOptions.empty());
		if (!enhancementOptions.empty()) {
			HandleComboBoxClick(msX, msY, sX + 44 + 3, sY + 28 + 25, sizeX - 6, 20, enhancementOptions, cLB, dnumber, !enhancementOptions.empty());
		}

		if (m_stDialogBoxInfo[dnumber].sV1 != -1) {
			short type1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00F00000) >> 20;
			short val1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x000F0000) >> 16;
			short type2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x0000F000) >> 12;
			short val2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00000F00) >> 8;

			char ctype1[200] = { 0 };
			char ctype2[200] = { 0 };
			char tmpctype1[200] = { 0 };
			char tmpctype2[200] = { 0 };
			int multiplier = 0;

			// Dibujar el texto del combo box sobre el sprite
			if (selectedOptionIndex == -1 && !isComboBoxOpen)
				PutAlignedString(sX + 10, sX + sizeX, sY + 120, m_sLang == LANG_ESP ? "Por favor, selecciona una estadistica valida." : "Please select valid stat.", 255, 0, 0);
			else if (m_stDialogBoxInfo[dnumber].sV1 != -1 && selectedOptionIndex != -1 && !isComboBoxOpen) {


				seltype = 0;
				SelectedType1 = -1;
				SelectedType2 = -1;

				// Verificar la opción seleccionada
				if (selectedOptionIndex == 0) {
					seltype = 1;
					SelectedType1 = 6;
				}
				else if (selectedOptionIndex == 1) {
					seltype = 1;
					SelectedType1 = 8;
				}
				else if (selectedOptionIndex == 2) {
					seltype = 2;
					SelectedType2 = 2;
				}
				else if (selectedOptionIndex == 3) {
					seltype = 2;
					SelectedType2 = 3;
				}
				else if (selectedOptionIndex == 4) {
					seltype = 2;
					SelectedType2 = 4;
				}
				else if (selectedOptionIndex == 5) {
					seltype = 2;
					SelectedType2 = 5;
				}
				else if (selectedOptionIndex == 6) {
					seltype = 2;
					SelectedType2 = 6;
				}
				else if (selectedOptionIndex == 7) {
					seltype = 2;
					SelectedType2 = 7;
				}
				else if (selectedOptionIndex == 8) {
					seltype = 2;
					SelectedType2 = 8;
				}
				else if (selectedOptionIndex == 9) {
					seltype = 2;
					SelectedType2 = 9;
				}
				else if (selectedOptionIndex == 10) {
					seltype = 2;
					SelectedType2 = 11;
				}

				int costPoints = 0;

				if (seltype != 0)
				{
					if (seltype == 1) {
						if (SelectedType1 == type1 || type1 == 0) {
							costPoints = (val1 + 1) * 150;
						}
					}
					else if (seltype == 2) {
						if (SelectedType2 == type2 || type2 == 0) {
							costPoints = (val2 + 1) * 150;
						}
					}
				}


				ZeroMemory(costMessage, sizeof(costMessage));

				if (costPoints == 0)
					sprintf(costMessage, m_sLang == LANG_ESP ? "No se puede mejorar este item con la estadística seleccionada." : "Can't upgrade this item with the selected stat.");
				else
					sprintf(costMessage, m_sLang == LANG_ESP ? "Costara %d puntos subir el item al siguiente nivel." : "It will cost %d points to enhance to the next level.", costPoints);

				PutAlignedString(sX + 10, sX + sizeX, sY + 120, costMessage, 255, 165, 0);
			}
		}

		DrawMenu_SmallButton(sX + 44, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY + 65, m_sLang == LANG_ESP ? "Mejorar" : "Upgrade");
		DrawMenu_SmallButton(sX + 44, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY + 65, m_sLang == LANG_ESP ? "Cerrar" : "Close");

		break;
	}

	case 2:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, m_sLang == LANG_ESP ? "Confirmar extrac." : "Confirm Extract", 240, 240, 240);
		
		// Mapa para almacenar las estadísticas acumuladas
		std::map<std::string, int> statMap;

		for (int i = 0; i < 20; i++) {
			if (m_stDialogBoxInfo[72].m_iEnchantingItem[i] != -1) {
				auto item = m_pItemList[m_stDialogBoxInfo[72].m_iEnchantingItem[i]];
				if (!item) continue;

				short type1 = (item->m_dwAttribute & 0x00F00000) >> 20;
				short val1 = (item->m_dwAttribute & 0x000F0000) >> 16;
				short type2 = (item->m_dwAttribute & 0x0000F000) >> 12;
				short val2 = (item->m_dwAttribute & 0x00000F00) >> 8;

				if (PermitedType1(type1) && val1 != 0) {
					std::string statName = GetStatNameType1(type1);
					statMap[statName] += val1 * 10;
				}

				if (PermitedType2(type2) && val2 != 0) {
					std::string statName = GetStatNameType2(type2);
					statMap[statName] += val2 * 10;
				}
			}
		}

		PutAlignedString(sX + 10, sX + sizeX, sY + 100, m_sLang == LANG_ESP ? "Recibiras: " : "You will get: ", 255, 200, 0);

		int offsetY = sY + 115;
		for (const auto& stat : statMap) {
			char temp[100];
			wsprintf(temp, m_sLang == LANG_ESP ? "%d %s fragmentos" : "%d %s fragments", stat.second, (char*)stat.first.c_str());
			PutAlignedString(sX + 10, sX + sizeX, offsetY, temp, 255, 255, 255);
			offsetY += 15; 
		}
			
		DrawMenu_SmallButton(sX + 44, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY + 65, m_sLang == LANG_ESP ? "Si" : "Yes");
		DrawMenu_SmallButton(sX + 44, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY + 65, m_sLang == LANG_ESP ? "No" : "No");

		break;
	}

	case 3:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, m_sLang == LANG_ESP ? "Confirmar mejora" : "Confirm Upgrade", 240, 240, 240);

		char texting[500];
		if (selectedOptionIndex == 0)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Light Shards: %d" : "Your Light Shards: %d", shard_light);
		}
		else if (selectedOptionIndex == 1)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Strong Shards: %d" : "Your Strong Shards: %d", shard_strong);
		}
		else if (selectedOptionIndex == 2)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de HP: %d" : "Your HP Fragments: %d", fragment_hp);
		}
		else if (selectedOptionIndex == 3)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de DR: %d" : "Your DR Fragments: %d", fragment_dr);
		}
		else if (selectedOptionIndex == 4)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de HP Rec: %d" : "Your HP Rec Fragments: %d", fragment_hprec);
		}
		else if (selectedOptionIndex == 5)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de SP Rec: %d" : "Your SP Rec Fragments: %d", fragment_sprec);
		}
		else if (selectedOptionIndex == 6)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de MP Rec: %d" : "Your MP Rec Fragments: %d", fragment_mprec);
		}
		else if (selectedOptionIndex == 7)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de MR: %d" : "Your MR Fragments: %d", fragment_mr);
		}
		else if (selectedOptionIndex == 8)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de Absorcion Fisica: %d" : "Your PA Fragments: %d", fragment_pa);
		}
		else if (selectedOptionIndex == 9)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de Absorcion Magica: %d" : "Your MA Fragments: %d", fragment_ma);
		}
		else if (selectedOptionIndex == 10)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de Exp: %d" : "Your EXP Fragments: %d", fragment_exp);
		}

		PutAlignedString(sX + 15, sX + sizeX - 15, sY + 83, texting, 255, 255, 20);

		PutAlignedString(sX + 15, sX + sizeX - 15, sY + 110, costMessage, 240, 240, 240);

		if (m_stDialogBoxInfo[dnumber].sV1 != -1) {
			short type1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00F00000) >> 20;
			short val1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x000F0000) >> 16;
			short type2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x0000F000) >> 12;
			short val2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00000F00) >> 8;

			char ctype1[200] = { 0 };
			char ctype2[200] = { 0 };
			char tmpctype1[200] = { 0 };
			char tmpctype2[200] = { 0 };
			int multiplier = 0;

			i = m_stDialogBoxInfo[dnumber].sV1;
			cItemColor = m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_cItemColor;
			if ((m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_cEquipPos == DEF_EQUIPPOS_LHAND)
				|| (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_cEquipPos == DEF_EQUIPPOS_RHAND)
				|| (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND)) {
				m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_sSprite]->PutSpriteRGB(sX + 44 + 134, sY + 182 + 20, m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_sSpriteFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
			}
			else {
				m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_sSprite]->PutSpriteRGB(sX + 44 + 134, sY + 182 + 20, m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_sSpriteFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
			}

			ZeroMemory(cStr1, sizeof(cStr1));
			ZeroMemory(cStr2, sizeof(cStr2));
			ZeroMemory(cStr3, sizeof(cStr3));
			GetItemName(m_pItemList[m_stDialogBoxInfo[dnumber].sV1], cStr1, cStr2, cStr3);
			PutAlignedString(sX + 44 + 24, sX + 44 + 248, sY + 230 + 20 + 20, cStr1, 255, 165, 0);
			PutAlignedString(sX + 44 + 24, sX + 44 + 248, sY + 245 + 20 + 20, cStr2, 255, 255, 255);
			PutAlignedString(sX + 44 + 24, sX + 44 + 248, sY + 260 + 20 + 20, cStr3, 255, 255, 255);

		}

		DrawMenu_SmallButton(sX + 44, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY + 65, m_sLang == LANG_ESP ? "Si" : "Yes");
		DrawMenu_SmallButton(sX + 44, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY + 65, m_sLang == LANG_ESP ? "No" : "No");
		break;
	}

	default:
		break;
	}

	//DrawDialogCord(sX, sY, msX, msY);
}


// Función para obtener el nombre de la estadística
string CGame::GetStatNameType1(short type) {
	switch (type) {
	case 6: return "Light";
	case 8: return "Strong";
	default: return "Unknown";
	}
}

string CGame::GetStatNameType2(short type) {
	switch (type) {
	case 2: return "Hitting Probability";
	case 3: return "Defense Ratio";
	case 4: return "HP Recovery";
	case 5: return "SP Recovery";
	case 6: return "MP Recovery";
	case 7: return "Magic Resistance";
	case 8: return "Physical Absorption";
	case 9: return "Magic Absorption";
	case 11: return "Exp";
	default: return "Unknown";
	}
}


bool CGame::PermitedType1(short type)
{
	switch (type)
	{
	case 6: //Light
	case 8: //Strong
		return true;
		break;

	default: return false; break;
	}

	return false;
}

bool CGame::PermitedType2(short type)
{
	switch (type)
	{
	case 2:	//hitting probability
	case 3:	//defense ratio
	case 4: //HP Recovery
	case 5: //SP Recovery
	case 6: //MP Recovery
	case 7: //Magic Resistance
	case 8: //Physical Absorption
	case 9: //Magic Absorption
	case 11: //Exp
		return true;
		break;
	default: return false; break;
	}

	return false;
}

void CGame::click_enchanting(short msX, short msY)
{
	short sX, sY, szX;
	int dnumber = 72;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	szX = m_stDialogBoxInfo[dnumber].sSizeX;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;

	auto tab1 = (msX >= sX + 44 + 2) && (msX <= sX + 44 + 2 + 128) && (msY >= sY + 24) && (msY <= sY + 24 + 25);
	auto tab2 = (msX >= sX + 44 + 2 + 128 + 1) && (msX <= sX + 44 + 2 + 128 + 1 + 128) && (msY >= sY + 24) && (msY <= sY + 24 + 25);

	auto eng = (msX >= sX + 276) && (msX <= sX + 299) && (msY >= sY + 13) && (msY <= sY + 29);
	auto esp = (msX >= sX + 304) && (msX <= sX + 326) && (msY >= sY + 13) && (msY <= sY + 29);

	if (eng) {
		PlaySound('E', 14, 5); m_sLang = LANG_ENG;
	}
	if (esp) {
		PlaySound('E', 14, 5); m_sLang = LANG_ESP;
	}


	if (tab1)
	{
		if (m_stDialogBoxInfo[dnumber].sV1 != -1) {
			m_bIsItemDisabled[m_stDialogBoxInfo[dnumber].sV1] = FALSE;
			m_stDialogBoxInfo[dnumber].sV1 = -1;
		}

		if (m_stDialogBoxInfo[dnumber].sV2 != -1) {
			m_bIsItemDisabled[m_stDialogBoxInfo[dnumber].sV2] = FALSE;
			m_stDialogBoxInfo[dnumber].sV2 = -1;
		}
		
		for (int b = 0; b < 20; b++)
		{
			if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] != -1)
			{
				m_bIsItemDisabled[m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b]] = FALSE;
				m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] = -1;
			}
		}

		isComboBoxOpen = false;
		selectedOptionIndex = -1;
		isClickProcessed = false;
		selectedType = 0;

		m_stDialogBoxInfo[dnumber].cMode = 0;
		PlaySound('E', 14, 5);
		return;
	}

	if (tab2)
	{

		if (m_stDialogBoxInfo[dnumber].sV1 != -1) {
			m_bIsItemDisabled[m_stDialogBoxInfo[dnumber].sV1] = FALSE;
			m_stDialogBoxInfo[dnumber].sV1 = -1;
		}

		if (m_stDialogBoxInfo[dnumber].sV2 != -1) {
			m_bIsItemDisabled[m_stDialogBoxInfo[dnumber].sV2] = FALSE;
			m_stDialogBoxInfo[dnumber].sV2 = -1;
		}
		
		for (int b = 0; b < 20; b++)
		{
			if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] != -1)
			{
				m_bIsItemDisabled[m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b]] = FALSE;
				m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] = -1;
			}
		}

		isComboBoxOpen = false;
		selectedOptionIndex = -1;
		isClickProcessed = false;
		selectedType = 0;

		m_stDialogBoxInfo[dnumber].cMode = 1;
		PlaySound('E', 14, 5);
		return;
	}

	switch (mode)
	{
	case 0:
	{
		auto extract = (msX >= sX + 44 + 30) && (msX <= sX + 44 + 105) && (msY >= sY + 293 + 65) && (msY <= sY + 145 + 313 + 65);
		auto cancel = (msX >= sX + 44 + 153) && (msX <= sX + 44 + 230) && (msY >= sY + 293 + 65) && (msY <= sY + 145 + 313 + 65);

		if (cancel)
		{
			DisableDialogBox(72);
			PlaySound('E', 14, 5);
			return;
		}

		if (extract)
		{
			// Verificar si todos los ítems están en estado -1
			bool allItemsInvalid = true;
			for (int b = 0; b < 20; b++)
			{
				if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] != -1 && m_pItemList[m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b]])
				{
					allItemsInvalid = false;
					break;
				}
			}

			// Si todos los ítems son inválidos, no continuar
			if (allItemsInvalid) return;


			m_stDialogBoxInfo[dnumber].cMode = 2;
			PlaySound('E', 14, 5);
		}
		break;
	}
	case 1:
	{
		auto fusion = (msX >= sX + 44 + 30) && (msX <= sX + 44 + 105) && (msY >= sY + 293 + 65) && (msY <= sY + 145 + 313 + 65);
		auto cancel = (msX >= sX + 44 + 153) && (msX <= sX + 44 + 230) && (msY >= sY + 293 + 65) && (msY <= sY + 145 + 313 + 65);

		if (cancel)
		{
			DisableDialogBox(72);
			PlaySound('E', 14, 5);
			return;
		}

		if (!m_pItemList[m_stDialogBoxInfo[dnumber].sV1] || m_stDialogBoxInfo[dnumber].sV1 == -1) return;
		short destItem = m_stDialogBoxInfo[dnumber].sV1;

		if (fusion)
		{
			if (m_stDialogBoxInfo[dnumber].sV1 != -1 && seltype != 0)
			{

				short type1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00F00000) >> 20;
				short val1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x000F0000) >> 16;
				short type2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x0000F000) >> 12;
				short val2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00000F00) >> 8;

				if (seltype == 1)
				{
					if (SelectedType1 == type1 || type1 == 0) {
						m_stDialogBoxInfo[dnumber].cMode = 3;
					}
				}
				else if (seltype == 2)
				{
					if (SelectedType2 == type2 || type2 == 0) {
						m_stDialogBoxInfo[dnumber].cMode = 3;
					}
				}

			}

			PlaySound('E', 14, 5);

			return;
		}
		break;
	}

	case 2:
	{
		auto yes = (msX >= sX + 44 + 30) && (msX <= sX + 44 + 105) && (msY >= sY + 293 + 65) && (msY <= sY + 145 + 313 + 65);
		auto no = (msX >= sX + 44 + 153) && (msX <= sX + 44 + 230) && (msY >= sY + 293 + 65) && (msY <= sY + 145 + 313 + 65);


		if (yes)
		{
			// Verificar si todos los ítems están en estado -1
			int count = 0;
			for (int b = 0; b < 20; b++)
			{
				if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] != -1 && m_pItemList[m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b]])
				{					
					count++;
				//	break;
				}
			}

			// Si todos los ítems son inválidos, no continuar
			if (count == 0) return;


			char data[512];
			auto * cp = data;

			Push(cp, (u32)ENHANCEMENT_EXTRACT);
			Push(cp, (u16)0);

			Push(cp, count);

			for (int b = 0; b < 20; b++)
			{
				if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] != -1 && m_pItemList[m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b]])
				{
					Push(cp, m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b]);
				}
			}
			
			m_pGSock->iSendMsg(data, cp - data);

			PlaySound('E', 14, 5);
			m_stDialogBoxInfo[dnumber].cMode = 0;
			return;
		}

		if (no)
		{
			PlaySound('E', 14, 5);
			m_stDialogBoxInfo[dnumber].cMode = 0;
			return;
		}

		break;
	}

	case 3:
	{
		auto yes = (msX >= sX + 44 + 30) && (msX <= sX + 44 + 105) && (msY >= sY + 293 + 65) && (msY <= sY + 145 + 313 + 65);
		auto no = (msX >= sX + 44 + 153) && (msX <= sX + 44 + 230) && (msY >= sY + 293 + 65) && (msY <= sY + 145 + 313 + 65);

		if (yes)
		{
			if (!m_pItemList[m_stDialogBoxInfo[dnumber].sV1] || m_stDialogBoxInfo[dnumber].sV1 == -1) return;
			short destItem = m_stDialogBoxInfo[dnumber].sV1;

			short type1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00F00000) >> 20;
			short val1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x000F0000) >> 16;
			short type2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x0000F000) >> 12;
			short val2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00000F00) >> 8;

			if (seltype == 1)
			{
				if (SelectedType1 == type1 || type1 == 0) {
					char data[50];
					auto * cp = data;

					Push(cp, (u32)ENHANCEMENT_FUSION);
					Push(cp, (u16)0);

					Push(cp, destItem);
					Push(cp, seltype);
					Push(cp, SelectedType1);

					m_pGSock->iSendMsg(data, cp - data);
					m_stDialogBoxInfo[dnumber].sV1 = -1;
					for (int i = 0; i < 20; i++)
					{
						m_stDialogBoxInfo[dnumber].m_iEnchantingItem[i] = -1;
					}
					seltype = 0;
					m_bIsItemDisabled[destItem] = FALSE;

					isComboBoxOpen = false;
					selectedOptionIndex = -1;
					isClickProcessed = false;
					selectedType = 0;

					seltype = 0;
					SelectedType1 = -1;
					SelectedType2 = -1;

					PlaySound('E', 14, 5);
					m_stDialogBoxInfo[dnumber].cMode = 1;
				}
			}
			else if (seltype == 2)
			{
				if (SelectedType2 == type2 || type2 == 0) {
					char data[50];
					auto * cp = data;

					Push(cp, (u32)ENHANCEMENT_FUSION);
					Push(cp, (u16)0);

					Push(cp, destItem);
					Push(cp, seltype);
					Push(cp, SelectedType2);

					m_pGSock->iSendMsg(data, cp - data);
					m_stDialogBoxInfo[dnumber].sV1 = -1;
					for (int i = 0; i < 20; i++)
					{
						m_stDialogBoxInfo[dnumber].m_iEnchantingItem[i] = -1;
					}
					seltype = 0;
					m_bIsItemDisabled[destItem] = FALSE;

					isComboBoxOpen = false;
					selectedOptionIndex = -1;
					isClickProcessed = false;
					selectedType = 0;

					seltype = 0;
					SelectedType1 = -1;
					SelectedType2 = -1;

					PlaySound('E', 14, 5);
					m_stDialogBoxInfo[dnumber].cMode = 1;
				}
			}
			return;
		}

		if (no)
		{
			PlaySound('E', 14, 5);
			m_stDialogBoxInfo[dnumber].cMode = 1;
			return;
		}

		break;
	}

	default:
		break;
	}
}

void CGame::bItemDrop_enchanting()
{
	int dnumber = 72;
	char cItemID;
	cItemID = (char)m_stMCursor.sSelectedObjectID;

	if (m_bIsItemDisabled[cItemID] == TRUE) return;
	if (m_cCommand < 0) return;

	short type1 = (m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) >> 20;
	short val1 = (m_pItemList[cItemID]->m_dwAttribute & 0x000F0000) >> 16;
	short type2 = (m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) >> 12;
	short val2 = (m_pItemList[cItemID]->m_dwAttribute & 0x00000F00) >> 8;

	switch (m_stDialogBoxInfo[dnumber].cMode) {
	case 0:
	{
		if (m_pItemList[cItemID]->m_cEquipPos == DEF_EQUIPPOS_NONE) return;

		if (val1 == 0 && val2 == 0) return;

		int itemCount = 0;
		for (int i = 0; i < 20; i++)
		{
			if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[i] != -1)
			{
				itemCount++;
			}
		}

		if (itemCount >= 20)
		{
			AddEventList("No hay espacio para más items.");
			return;
		}

		for (int i = 0; i < 20; i++)
		{
			if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[i] == -1)
			{
				m_stDialogBoxInfo[dnumber].m_iEnchantingItem[i] = cItemID;
				m_bIsItemDisabled[cItemID] = TRUE;
				PlaySound('E', 29, 0);
				return;
			}
		}
		break;
	}
	case 1:

		if (m_pItemList[cItemID]->m_cEquipPos == DEF_EQUIPPOS_NONE) return;
		m_bIsItemDisabled[m_stDialogBoxInfo[dnumber].sV1] = FALSE;
		m_stDialogBoxInfo[dnumber].sV1 = cItemID;
		m_bIsItemDisabled[cItemID] = TRUE;
		PlaySound('E', 29, 0);

		break;
	}
}

void CGame::getCommandHandler(char * data)
{
	char command[30] = {};
	int val1 = 0;
	int val2 = 0;

	ZeroMemory(command, sizeof(command));
	Pop(data, command, 30);
	Pop(data, val1);
	Pop(data, val2);

	procCommandHandler(command, val1, val2);
}

void CGame::startReceiver()
{
	// Crear un hilo para ejecutar receiveMessage en paralelo
	std::thread receiverThread(&CGame::receiveMessageFromServer, this);

	// Opcional: Separar el hilo para que funcione de forma independiente
	receiverThread.detach(); // Permite que el hilo funcione sin necesidad de unirse
}

void CGame::procCommandHandler(char * command, int val1, int val2)
{
	//HeatoN rebirth
	if (strcmp(command, "/rebirthlevel") == 0)
	{
		m_iRebirthLevel = val1;
	}//heaton fast teleport free command for fast moving when teleport
	else if (strcmp(command, "/freecmd") == 0)
	{
		/*std::thread timerThread([this]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(180));
			this->m_bCommandAvailable = true;
			this->m_dwCommandTime = 0;
		});
		timerThread.detach();*/
		reqfree = true;
		return;
	}
	/*else if (strcmp(command, "/eventstatus") == 0)
	{
		bool status = val2 ? 1 : 0;
		g_ev.NotifyEventStatus((EventID)val1, status, 1);
		PlaySound('E', 25, 0, 0);
	}
	else if (strcmp(command, "/logineventstatus") == 0)
	{
		bool status = val2 ? 1 : 0;
		g_ev.NotifyEventStatus((EventID)val1, status, 2);
	}*/
	else if (strcmp(command, "/coins") == 0)
	{
		m_iCoins = val1;
	}
	else if (strcmp(command, "/rep") == 0)
	{
		m_iReputation = val1;
	}
	else if (strcmp(command, "/majs") == 0)
	{
		m_iGizonItemUpgradeLeft = val1;
	}
	else if (strcmp(command, "/rebirthstatus") == 0)
	{
		m_sRebirthStatus = val1;
	}
	else if (strcmp(command, "/rebirthenabled") == 0)
	{
		m_sRebirthEnabled = val1;
	}
	else if (strcmp(command, "/handle") == 0)
	{
		/*std::thread timerThread([this]() {
			UDPrequest_ping();
		});
		timerThread.detach();*/
		
		/*udpPort = 8888 + val1;

		const std::string SERVER_IP = "127.0.0.1"; // Replace with your server's IP address
		const int SERVER_PORT = udpPort;

		startUDPClient(SERVER_PORT);
		startReceiver();
		sendMessageToServer(SOCKMSG_WELCOME);*/
	}
	else if (strcmp(command, "/hp") == 0)
	{
		m_iHP = val1;
	}
	else if (strcmp(command, "/mp") == 0)
	{
		m_iMP = val1;
	}
	else if (strcmp(command, "/sp") == 0)
	{
		m_iSP = val1;
	}
	else if (strcmp(command, "/playerexp") == 0)
	{
		m_iExp = val1;
	}
	else if (strcmp(command, "/statspoints") == 0)
	{
		m_iLU_Point = val1;
	}
	else if (strcmp(command, "/playerlevel") == 0)
	{
		m_iLevel = val1;
	}
	else if (strcmp(command, "/playerexp") == 0)
	{
		m_iExp = val1;
	}
	else if (strcmp(command, "/str") == 0)
	{
		m_iStr = val1;
	}
	else if (strcmp(command, "/dex") == 0)
	{
		m_iDex = val1;
	}
	else if (strcmp(command, "/vit") == 0)
	{
		m_iVit = val1;
	}
	else if (strcmp(command, "/int") == 0)
	{
		m_iInt = val1;
	}
	else if (strcmp(command, "/mag") == 0)
	{
		m_iMag = val1;
	}
	else if (strcmp(command, "/chr") == 0)
	{
		m_iCharisma = val1;
	}
	else if (string(command) == "/contrib") {
		m_iContribution = val1;
	}
	else if (string(command) == "/enableitem") {
		for (int i = 0; i < 20; i++)
		{
			if (m_stDialogBoxInfo[72].m_iEnchantingItem[i] != -1 && val1 == i)
			{
				m_bIsItemDisabled[m_stDialogBoxInfo[72].m_iEnchantingItem[i]] = FALSE;
				m_stDialogBoxInfo[72].m_iEnchantingItem[i] = -1;
			}
		}		
	}
	else if (string(command) == "/closeenchantingpanel")
	{
		m_bIsItemDisabled[m_stDialogBoxInfo[72].sV1] = FALSE;
		m_stDialogBoxInfo[72].sV1 = -1;
		for (int i = 0; i < 20; i++)
		{
			if (m_stDialogBoxInfo[72].m_iEnchantingItem[i] != -1)
			{
				m_bIsItemDisabled[m_stDialogBoxInfo[72].m_iEnchantingItem[i]] = FALSE;
				m_stDialogBoxInfo[72].m_iEnchantingItem[i] = -1;
			}
		}
		m_stDialogBoxInfo[72].dwV1 = NULL;
		isComboBoxOpen = false; // Bandera para controlar si el combobox está abierto o cerrado
		selectedOptionIndex = -1; // Índice de la opción seleccionada
		 // Bandera para controlar si el clic ya fue procesado
		selectedType = 0;
		isClickProcessed = false;
		//DisableDialogBox(72);
	}
	else if (string(command) == "/shards") {
		shard_light = val1;
		shard_strong = val2;
	}
	else if (string(command) == "/fragment_hp") {
		fragment_hp = val1;
	}
	else if (string(command) == "/fragment_dr") {
		fragment_dr = val1;
	}
	else if (string(command) == "/fragment_hprec") {
		fragment_hprec = val1;
	}
	else if (string(command) == "/fragment_sprec") {
		fragment_sprec = val1;
	}
	else if (string(command) == "/fragment_mprec") {
		fragment_mprec = val1;
	}
	else if (string(command) == "/fragment_mr") {
		fragment_mr = val1;
	}
	else if (string(command) == "/fragment_pa") {
		fragment_pa = val1;
	}
	else if (string(command) == "/fragment_ma") {
		fragment_ma = val1;
	}
	else if (string(command) == "/fragment_exp") {
		fragment_exp = val1;
	}
	else if (strcmp(command, "/eks") == 0)
	{
		m_iEnemyKillCount = val1;
	}
	else if (strcmp(command, "/responseping") == 0)
	{
		endTime = std::chrono::high_resolution_clock::now();
	}
	else if (strcmp(command, "/revive") == 0)
	{
		if (!m_bIsDialogEnabled[74]) {
			EnableDialogBox(74, NULL, NULL, NULL);
		}
		return;
	}
}

// Heaton Rebirth
void CGame::draw_rebirth(short msX, short msY) {
	short sX, sY, sizeX, sizeY;
	int dnumber = 73;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;
	int fix_x = 142;


	DrawShadowDialog2(sX, sY, dnumber);
	PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Rebirth", 255, 255, 255);


	DrawNewDialogBox2(SPRID_FLAGSLANG, sX + 195 + 80, sY + 13, 0, FALSE, true);
	DrawNewDialogBox2(SPRID_FLAGSLANG + 1, sX + 195 + 28 + 80, sY + 13, 0, FALSE, true);

	/*PutString_SprFont2(sX + fix_x, sY + 13, "Rebirth", 255, 200, 0);*/
	
	// Textos dinámicos según el idioma
	const char* txtRebirth = "Rebirth";
	const char* txtSystemDesc = (m_sLang == LANG_ESP) ?
		"El sistema de rebirth te otorga 3 puntos adicionales de estadísticas por cada renacer." :
		"The rebirth system grants you 3 additional stat points for every rebirth.";
	const char* txtModeDesc = (m_sLang == LANG_ESP) ?
		"Iniciar un nuevo rebirth te pondrá en Modo Rebirth y te llevará al Nivel 1." :
		"Starting a new rebirth will enter Rebirth Mode, granting you one Rebirth Level and resetting you to Level 1.";
	const char* txtSkillsDesc = (m_sLang == LANG_ESP) ?
		"No perderás tus habilidades, pero no podrás usarlas hasta tener suficientes estadísticas." :
		"You will not lose your skills or spells, but you will be unable to use them until you have enough stats.";
	const char* txtPenalty = (m_sLang == LANG_ESP) ?
		"En Modo Rebirth, tendrás una penalización del -20%% de experiencia hasta alcanzar el Nivel %d nuevamente." :
		"While in Rebirth mode, you will have a -20%% experience penalty for each Rebirth Level until you reach Level %d again.";
	const char* txtToggle = (m_sLang == LANG_ESP) ?
		"Puedes alternar entre Modo Rebirth y Modo Normal (Nivel %d) en cualquier momento por un pequeño precio." :
		"You can toggle between Rebirth Mode and Normal Mode (Level %d) at any time for a small price.";
	const char* txtRequirements = (m_sLang == LANG_ESP) ? "Requisitos:" : "Requirements:";
	const char* txtStartRebirth = (m_sLang == LANG_ESP) ? "Iniciar Nuevo Rebirth" : "Start New Rebirth";
	const char* txtSwitchMode = (m_sLang == LANG_ESP) ? "Cambiar Modo Rebirth" : "Switch Rebirth Mode";
	const char* txtContribution = (m_sLang == LANG_ESP) ? "%d Contribución" : "%d Contribution";
	const char* txtGold = (m_sLang == LANG_ESP) ? "%d Oro" : "%d Gold";
	const char* txtfree = (m_sLang == LANG_ESP) ? "El primer Rebirth es gratis!" : "First rebirth is free!";

	const char* txt1 = (m_sLang == LANG_ESP) ?
		"Estas seguro que deseas realizar esta operacion?" :
		"you sure want to perform this operation?";

	switch (mode)
	{
	case 0:
	{
		char temp[5000];
		int maxlevel = m_iPlayerMaxLevel;

		// Descripciones
		PutAlignedString(sX + 20, sX + sizeX - 20, sY + 40, (char*)txtSystemDesc, 240, 240, 240);
		PutAlignedString(sX + 20, sX + sizeX - 20, sY + 75, (char*)txtModeDesc, 240, 240, 240);
		PutAlignedString(sX + 20, sX + sizeX - 20, sY + 110, (char*)txtSkillsDesc, 240, 240, 240);

		wsprintf(temp, txtPenalty, maxlevel);
		PutAlignedString(sX + 10, sX + sizeX - 20, sY + 145, temp, 240, 240, 240);

		ZeroMemory(temp, sizeof(temp));
		wsprintf(temp, txtToggle, maxlevel);
		PutAlignedString(sX + 20, sX + sizeX - 20, sY + 198, temp, 240, 240, 240);

		// Requisitos
		PutAlignedString(sX + 20, sX + sizeX - 20, sY + 250, (char*)txtRequirements, 255, 200, 0);

		if (m_iLevel == m_iPlayerMaxLevel && m_sRebirthStatus == 0 && m_sRebirthEnabled == 0) {
			// Mostrar Nivel y Contribución requerida

			int cost = m_iRebirthLevel * 2500;
			if (cost != 0)
			{
				wsprintf(G_cTxt, txtContribution, m_iRebirthLevel * 2500);
				PutAlignedString(sX + 10, sX + sizeX - 10, sY + 270, G_cTxt, 2, 220, 5);
			}
			else
			{
				wsprintf(G_cTxt, txtContribution, m_iRebirthLevel * 2500);
				PutAlignedString(sX + 20, sX + sizeX - 20, sY + 270, (char*)txtfree, 25, 203, 12);
			}

			// Botón para iniciar renacer
			DrawMenu_LongBut(sX, sY, msX, msY, 36, 360, (char*)txtStartRebirth);
		}
		else {
			// Mostrar costo en oro para cambiar de modo
			wsprintf(G_cTxt, txtGold, 5000);
			PutAlignedString(sX + 20, sX + sizeX - 20, sY + 270, G_cTxt, 25, 203, 12);

			// Botón para cambiar de modo
			DrawMenu_LongBut(sX, sY, msX, msY, 36, 360, (char*)txtSwitchMode);
		}
		break;
	}
	case 1:
	{
		PutAlignedString(sX + 20, sX + sizeX - 20, sY + 75, (char*)txt1, 240, 240, 240);
		DrawMenu_LongBut(sX, sY, msX, msY, 36, 360, (char*)txtStartRebirth);
		break;
	}
	case 2:
	{
		PutAlignedString(sX + 20, sX + sizeX - 20, sY + 75, (char*)txt1, 240, 240, 240);
		DrawMenu_LongBut(sX, sY, msX, msY, 36, 360, (char*)txtSwitchMode);
		break;
	}
	default:
		break;
	}

	DrawDialogCord(sX, sY, msX, msY);
}



void CGame::DrawDialogCord(short sX, short sY, short msX, short msY)
{
	return;
	
	string cords = to_string(msX - sX) + "/" + to_string(msY - sY);
	PutString2(msX + 10, msY + 25, (char*)cords.c_str(), 255, 255, 255);
	
}

void CGame::click_rebirth(short msX, short msY)
{
	short sX, sY, szX;
	int dnumber = 73;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	szX = m_stDialogBoxInfo[dnumber].sSizeX;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;
	
	auto eng = (msX >= sX + 276) && (msX <= sX + 299) && (msY >= sY + 13) && (msY <= sY + 29);
	auto esp = (msX >= sX + 304) && (msX <= sX + 326) && (msY >= sY + 13) && (msY <= sY + 29);

	if (eng) {
		PlaySound('E', 14, 5); m_sLang = LANG_ENG;
	}
	if (esp) {
		PlaySound('E', 14, 5); m_sLang = LANG_ESP;
	}


	int fix_y = 360;

	auto rebirth_click = (msX >= sX + 36) && (msX <= sX + 36 + 278) && (msY >= sY + fix_y) && (msY <= sY + fix_y + 22);

	switch (mode)
	{
	case 0:
	{
		if (m_iLevel == m_iPlayerMaxLevel && m_sRebirthStatus == 0 && m_sRebirthEnabled == 0)
		{
			if (rebirth_click)
			{
				m_stDialogBoxInfo[dnumber].cMode = 1;
				PlaySound('E', 14, 5);
			}
		}
		else
		{
			if (rebirth_click)
			{
				m_stDialogBoxInfo[dnumber].cMode = 2;
				PlaySound('E', 14, 5);
			}
		}
		break;
	}
	case 1: 
	{
		if (rebirth_click)
		{
			if (m_iLevel != m_iPlayerMaxLevel)
			{
				wsprintf(G_cTxt, "You need to be level %d to be reborn!", m_iPlayerMaxLevel);
				AddEventList(G_cTxt);
				PlaySound('E', 14, 5);
				return;
			}

			SendChat("/reqrebirth");
			PlaySound('E', 14, 5);
			DisableDialogBox(73);
		}
		break; 
	}
	case 2:
	{ 
		if (rebirth_click)
		{
			SendChat("/reqswitch");
			PlaySound('E', 14, 5);
			DisableDialogBox(73);
		}
		break; 
	}
	default:
		break;
	}
}


void CGame::DrawMenu_LongBut(short sX, short sY, short msX, short msY, short x, short y, const char* text)
{
	if ((msX >= sX + x) && (msX <= sX + x + 250) && (msY >= sY + y) && (msY <= sY + y + 22)) {
		DrawNewDialogBox2(SPRID_LONGBUTTONS + 1, sX + x, sY + y, 0, FALSE, FALSE);
		PutAlignedString2(sX + x, sX + x + 278, sY + y + 2, (char*)text, 255, 240, 0);
	}
	else
	{
		DrawNewDialogBox2(SPRID_LONGBUTTONS, sX + x, sY + y, 0, FALSE, FALSE);
		PutAlignedString2(sX + x, sX + x + 278, sY + y + 2, (char*)text, 255, 255, 255);
	}
}

// heaton send chat
void CGame::SendChat(char * str)
{
	bSendCommand(CLIENT_REQUEST_COMMAND_CHATMSG, NULL, NULL, NULL, NULL, NULL, str);
}

// draw enable events names heaton
void CGame::DrawActiveEvents()
{
	string s;

	s.append("");

	for (int i = 0; i < (int)EventID::Max; i++)
	{

		auto is = g_ev.Is((EventID)i);

		if (!is)
			continue;

		eventcount++;

		s.append(" - ");
		s.append(getEventName((EventID)i));
	}

	if (eventcount >= 1)
	{
		s.append(" -");
		eventcount = 0;
	}
	if (c_reso->IsResolution() == C1024x768)
	{
		PutAlignedString(1, 1023, 40, (char*)s.c_str(), 255, 255, 255);
	}
	else if (c_reso->IsResolution() == C800x600)
	{
		PutAlignedString(1, 799, 40, (char*)s.c_str(), 255, 255, 255);
	}
	else if (c_reso->IsResolution() == C640x480)
	{
		PutAlignedString(1, 639, 40, (char*)s.c_str(), 255, 255, 255);
	}
}

void CGame::click_requestrevive(short msX, short msY)
{
	short sX, sY;
	int dnumber = 74;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;

	auto yes = (msX >= sX + 15) && (msX <= sX + 15 + 76) && (msY >= sY + 50) && (msY <= sY + 50 + 22);
	auto no = (msX >= sX + 163) && (msX <= sX + 163 + 76) && (msY >= sY + 50) && (msY <= sY + 50 + 22);

	if (yes)
	{
		SendChat("/reviveyes");
		PlaySound('E', 14, 5);
		DisableDialogBox(74);
		return;
	}

	if (no)
	{
		SendChat("/reviveno");
		PlaySound('E', 14, 5);
		DisableDialogBox(74);
		return;
	}
}

void CGame::draw_requestrevive(short msX, short msY)
{
	short sX, sY, sizeX;
	int dnumber = 74;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;

	
	DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 6);
	

	PutAlignedString(sX, sX + sizeX, sY + 10, "Do you want revive in event?", 255, 255, 255);

	DrawMenu_SmallButton(sX, sY, msX, msY, 15, 50, "Yes");
	DrawMenu_SmallButton(sX, sY, msX, msY, 163, 50, "No");
}

//heaton
void CGame::request_ranking(short mode)
{
	switch (mode)
	{
	case DEF_TOPDM: vec_fighters.clear(); SendChat("/reqtopdm"); break;
	default: break;
	}
}

void CGame::click_top10(short msX, short msY)
{
	short sX, sY;
	int dnumber = 75;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;

}

void CGame::draw_top10(short msX, short msY)
{
	short sX, sY, sizeX;
	int dnumber = 75;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;

	DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 0);

	PutAlignedString(sX, sX + sizeX, sY + 10, "Deathmatch Top 10", 255, 255, 255);
	PutAlignedString(sX, sX + sizeX + 1, sY + 10, "Deathmatch Top 10", 255, 255, 255);

	PutString(sX + 20, sY + 52, "Pos", RGB(255, 200, 0));
	PutString(sX + 50, sY + 52, "Name", RGB(255, 200, 0));
	PutString(sX + 140, sY + 52, "Kills", RGB(255, 200, 0));
	PutString(sX + 200, sY + 52, "Deaths", RGB(255, 200, 0));

	for (int i = 0; i < vec_fighters.size(); i++)
	{
		auto y = sY + 75 + (i * 15);
		string pos = to_string(i + 1);
		pos.append(")");

		PutString(sX + 20, y, (char*)pos.c_str(), RGB(255, 200, 0));
		PutString(sX + 50, y, vec_fighters[i].fighter, RGB(255, 255, 255));
		PutString(sX + 140, y, (char*)to_string(vec_fighters[i].kills).c_str(), RGB(255, 255, 255));
		PutString(sX + 200, y, (char*)to_string(vec_fighters[i].deaths).c_str(), RGB(255, 255, 255));
	}
}

void CGame::getNewItemWeight(char* data)
{
	short destItem;
	WORD destItemweight;
	Pop(data, destItem);
	Pop(data, destItemweight);

	if (!destItem) return;
	m_pItemList[destItem]->m_wWeight = destItemweight;
}

void CGame::getNewItemEndurance(char* data)
{
	short destItem;
	int destItemEndu;
	int destItemMaxEndu;

	Pop(data, destItem);
	Pop(data, destItemEndu);
	Pop(data, destItemMaxEndu);

	if (!destItem) return;
	//alrevez no es? nook porq aca lo hace 
	m_pItemList[destItem]->m_wCurLifeSpan = destItemEndu;
	m_pItemList[destItem]->m_wMaxLifeSpan = destItemMaxEndu;
}

void CGame::get_top_deathmatch(char * data)
{
	short fighters = 0;
	char name[12] = {};
	int kills = 0;
	int deaths = 0;

	Pop(data, fighters);

	for (int i = 0; i < fighters; i++)
	{
		Pop(data, name, 12);
		Pop(data, kills);
		Pop(data, deaths);

		st_fighters u;

		ZeroMemory(u.fighter, sizeof(u.fighter));
		strcpy(u.fighter, name);
		u.kills = kills;
		u.deaths = deaths;

		vec_fighters.push_back(u);
	}
}

// draw cords on game
void CGame::DrawCords(short x, short y)
{
	return;
	DWORD dwTime = timeGetTime();
	WORD  wR, wG, wB;
	string s = std::to_string(x) + "/" + std::to_string(y);
	PutString2(x + 10, y + 25, (char*)s.c_str(), 255, 255, 255);
}

void CGame::DrawMarketItem(CItem* item, int x, int y, int mx, int my)
{
	short msX, msY, msZ;
	char cLB, cRB;// , cMB;
	auto mousex = mx;
	char cStr1[64], cStr2[64], cStr3[64], cStr4[64], cStr5[64], cStr6[64], cItemsTotal[64], cStr7[64];
	int  iLoc, iLocStr2 = 0, iLocStr3 = 0, iLocStr4 = 0, iLocStr5 = 0, iLocStr6 = 0, iLocStr7 = 0, ItemsTotal = 0;
	int  iStrLen1 = 0, iStrLen2 = 0, iStrLen3 = 0, iStrLen4 = 0, iStrLen5 = 0, iStrLen6 = 0;
	short msY1 = 0, msX1 = 0, msK = 0;

	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);

	int posx = x == 0 ? msX - m_stMCursor.sDistX : x;
	int posy = y == 0 ? msY - m_stMCursor.sDistY : y;
	auto cItemColor = item->m_cItemColor;

	int magicspr = DEF_SPRID_ITEMPACK_PIVOTPOINT;

	if (cItemColor != 0) {
		if ((item->m_cEquipPos == DEF_EQUIPPOS_LHAND) ||
			(item->m_cEquipPos == DEF_EQUIPPOS_RHAND) ||
			(item->m_cEquipPos == DEF_EQUIPPOS_TWOHAND))
		{
			m_pSprite[magicspr + item->m_sSprite]->PutSpriteRGB(posx, posy,
				item->m_sSpriteFrame,
				m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], 1);
		}
		else
		{
			m_pSprite[magicspr + item->m_sSprite]->PutSpriteRGB(posx, posy,
				item->m_sSpriteFrame,
				m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], 1);
		}
	}
	else
	{
		m_pSprite[magicspr + item->m_sSprite]->PutSpriteFast(posx, posy, item->m_sSpriteFrame, 1);
	}


	GetMarketItemName(item, cStr1, cStr2, cStr3, cStr7);

	//AddEventList(cStr7);

	if (mx != 0 && my != 0)
	{
		msX = mx;
		msY = my;
	}

	iLoc = 0;
	if (strlen(cStr1) != 0)
	{
		if (m_bIsSpecial)
		{
			if (!ShadowsBox)
				PutString(msX, msY + 25, cStr1, RGB(0, 255, 50), FALSE, 1);
		}
		else {
			if (!ShadowsBox)
				PutString(msX, msY + 25, cStr1, RGB(255, 255, 255), FALSE, 1);
		}
		iLoc += 15;
	}
	if (strlen(cStr2) != 0) 
	{
		if (!ShadowsBox)
			PutString(msX, msY + 25 + iLoc, cStr2, RGB(150, 150, 150), FALSE, 1);
		iLoc += 15;
		iLocStr2 += 15;
	}
	if (strlen(cStr3) != 0)
	{
		if (!ShadowsBox)
			PutString(msX, msY + 25 + iLoc, cStr3, RGB(150, 150, 150), FALSE, 1);
		iLoc += 15;
		iLocStr3 += 15;
	}
	/**/
	if ((item->m_sLevelLimit != 0) && ((item->m_dwAttribute & 0x00000001) == 0))
	{
		wsprintf(cStr4, "%s: %d", DRAW_DIALOGBOX_SHOP24, item->m_sLevelLimit);//"���� ����: %d"
		if (!ShadowsBox)
			PutString(msX, msY + 25 + iLoc, cStr4, RGB(150, 150, 150), FALSE, 1);
		iLoc += 15;
		iLocStr4 += 15;
	}
	if ((item->m_cEquipPos != DEF_EQUIPPOS_NONE) && (item->m_wWeight >= 1100))
	{
		int	_wWeight = 0;
		if (item->m_wWeight % 100) _wWeight = 1;
		wsprintf(cStr5, DRAW_DIALOGBOX_SHOP15, item->m_wWeight / 100 + _wWeight);
		if (!ShadowsBox)
			PutString(msX, msY + 25 + iLoc, cStr5, RGB(150, 150, 150), FALSE, 1);
		iLoc += 15;
		iLocStr5 += 15;
	}
	if (item->m_cEquipPos != DEF_EQUIPPOS_NONE || !strcmp(item->m_cName, "ZemstoneofSacrifice"))
	{
		wsprintf(cStr6, UPDATE_SCREEN_ONGAME10, item->m_wCurLifeSpan);
		if (!ShadowsBox)
			PutString(msX, msY + 25 + iLoc, cStr6, RGB(150, 150, 150), FALSE, 1);
		iLoc += 15;
		iLocStr6 += 15;
	}

	if (strlen(cStr7) != 0)
	{
		if (!ShadowsBox)
			PutString(msX, msY + 25 + iLoc, cStr7, RGB(150, 150, 150), FALSE, 1);
		iLoc += 15;
		iLocStr7 += 15;
	}

	if (iLoc == 15)
	{
		iLoc = 0;
		for (int iTmp = 0; iTmp < DEF_MAXITEMS; iTmp++)
		{
			if (item != NULL)
			{
				if (strcmp(item->m_cName, item->m_cName) == 0) iLoc++;
				ItemsTotal = 1;
			}
		}
		if (iLoc > 1)
		{
		}
	}
	//Coloco el recuadro - LaloRamos
	if (ShadowsBox)
	{
		if (ItemsTotal > 1)
			iLoc = 30;
		else if (ItemsTotal == 1)
			iLoc = 13;

		//msY1 = msY;
		if (c_reso->IsResolution() == C800x600)
		{
			if ((msY + iLoc + 28) > 599)
				msY1 = 599;
			else msY1 = msY + iLoc + 28;
		}
		else if (c_reso->IsResolution() == C1024x768)
		{
			if ((msY + iLoc + 28) > 767)
				msY1 = 767;
			else msY1 = msY + iLoc + 28;
		}
		else if (c_reso->IsResolution() == C640x480)
		{
			if ((msY + iLoc + 28) > 479)
				msY1 = 479;
			else msY1 = msY + iLoc + 28;
		}

		msK = SpaceChar(cStr1);
		if (msK < SpaceChar(cStr2)) msK = SpaceChar(cStr2);
		if (msK < SpaceChar(cStr3)) msK = SpaceChar(cStr3);
		if (msK < SpaceChar(cStr4)) msK = SpaceChar(cStr4);
		if (msK < SpaceChar(cStr5)) msK = SpaceChar(cStr5);
		if (msK < SpaceChar(cStr6)) msK = SpaceChar(cStr6);

		msX1 = msX + msK;

		if (c_reso->IsResolution() == C800x600)
		{
			if (msX1 > 799)
				msX1 = 799;
		}
		else if (c_reso->IsResolution() == C1024x768)
		{
			if (msX1 > 1023)
				msX1 = 1023;
		}
		else if (c_reso->IsResolution() == C640x480)
		{
			if (msX1 > 639)
				msX1 = 639;
		}

		m_DDraw.DrawShadowBox(msX, msY + 25, msX1, msY1);
		PutAlignedString(msX, msX + msK, msY + 25, cStr1, 234, 176, 51);

		if (ItemsTotal > 1)
			PutAlignedString(msX, msX + msK, msY + 25 + 15, cItemsTotal, 255, 255, 255);
		if (iLocStr2 > 0)
		{	//Purity
			if (memcmp(GET_ITEM_NAME1, cStr2, 6) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 255, 255, 255); // Blanco
			//Completion
			else if (memcmp(GET_ITEM_NAME2, cStr2, 10) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 255, 255, 255); // Blanco
			//Critical
			else if (memcmp(GET_ITEM_NAME3, cStr2, 8) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 221, 203, 49); // Amarillo
			// Poisoning
			else if (memcmp(GET_ITEM_NAME4, cStr2, 9) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 145, 209, 61); // Verde
			// Righteous
			else if (memcmp(GET_ITEM_NAME5, cStr2, 9) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 119, 119, 119); // 
			// Agile
			else if (memcmp(GET_ITEM_NAME6, cStr2, 5) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 137, 185, 195); // 
			// Light
			else if (memcmp(GET_ITEM_NAME7, cStr2, 5) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 255, 235, 104); // 
			// Sharp
			else if (memcmp(GET_ITEM_NAME8, cStr2, 5) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 41, 70, 218); // 
			// Strong
			else if (memcmp(GET_ITEM_NAME9, cStr2, 6) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 113, 113, 113); //
			// Ancient
			else if (memcmp(GET_ITEM_NAME10, cStr2, 7) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 195, 77, 147); //
			// Special
			else if (memcmp(GET_ITEM_NAME11, cStr2, 7) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 223, 148, 3); //
			// Mana Converting
			else if (memcmp(GET_ITEM_NAME12, cStr2, 15) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 113, 169, 226); //
			// Critical
			else if (memcmp(GET_ITEM_NAME13, cStr2, 8) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 221, 203, 49); //
			// Critical Hit Damage
			else if (memcmp(GET_ITEM_NAME14, cStr2, 19) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 221, 203, 49); //
			// Poison Damage
			else if (memcmp(GET_ITEM_NAME15, cStr2, 13) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 145, 209, 61); // Verde
			// Attack Speed -1
			else if (memcmp(GET_ITEM_NAME16, cStr2, 15) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 137, 185, 195); //
			// light
			else if (memcmp(GET_ITEM_NAME17, cStr2, 5) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 255, 235, 104); //
			// Damage added
			else if (memcmp(GET_ITEM_NAME18, cStr2, 12) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 206, 18, 115); //
			// Endurance
			else if (memcmp(GET_ITEM_NAME19, cStr2, 9) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 139, 139, 139); //
			// Extra Damage added
			else if (memcmp(GET_ITEM_NAME20, cStr2, 18) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 228, 184, 67); //
			// Magic Casting Probability
			else if (memcmp(GET_ITEM_NAME21, cStr2, 25) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 214, 72, 72); //
			// Replace %d%% damage to mana
			else if (memcmp(GET_ITEM_NAME22, cStr2, 7) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 113, 169, 226); //
			// Crit Increase Chance
			else if (memcmp(GET_ITEM_NAME23, cStr2, 20) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 221, 203, 49); //
			// Poison Resistance
			else if (memcmp(GET_ITEM_NAME24, cStr2, 17) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 145, 209, 61); // Verde
			// Hitting Probability
			else if (memcmp(GET_ITEM_NAME25, cStr2, 19) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 200, 165, 3); //
			// Defense Ratio
			else if (memcmp(GET_ITEM_NAME26, cStr2, 13) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 170, 127, 174); //
			// HP recovery
			else if (memcmp(GET_ITEM_NAME27, cStr2, 11) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 216, 38, 38); // Rojo
			// SP recovery
			else if (memcmp(GET_ITEM_NAME28, cStr2, 11) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 145, 209, 61); // Verde
			// MP recovery
			else if (memcmp(GET_ITEM_NAME29, cStr2, 11) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 113, 169, 226); // Azul
			// Magic Resistance
			else if (memcmp(GET_ITEM_NAME30, cStr2, 16) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 145, 187, 87); // 
			// Physical Absorption
			else if (memcmp(GET_ITEM_NAME31, cStr2, 19) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 38, 122, 118); // 
			// Magic Absorption
			else if (memcmp(GET_ITEM_NAME32, cStr2, 16) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 175, 26, 100); //
			// Consecutive Attack Damage
			else if (memcmp(GET_ITEM_NAME33, cStr2, 25) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 228, 184, 67); //
			// Experience
			else if (memcmp(GET_ITEM_NAME34, cStr2, 10) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 168, 74, 140); //
			// Gold
			else if (memcmp(GET_ITEM_NAME35, cStr2, 4) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 223, 148, 3); //
			else
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 255, 255, 255);
			if (iLocStr3 > 0)
				iLocStr3 += iLocStr2;
			else if (iLocStr4 > 0)
				iLocStr4 += iLocStr2;
			else if (iLocStr5 > 0)
				iLocStr5 += iLocStr2;
			else if (iLocStr6 > 0)
				iLocStr6 += iLocStr2;
			else if (iLocStr7 > 0)
				iLocStr7 += iLocStr2;
		}
		if (iLocStr3 > 0)
		{
			//Purity
			if (memcmp(GET_ITEM_NAME1, cStr3, 6) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 255, 255, 255); // Blanco
			//Completion
			else if (memcmp(GET_ITEM_NAME2, cStr3, 10) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 255, 255, 255); // Blanco
			// Critical Hit Damage
			else if (memcmp(GET_ITEM_NAME14, cStr3, 19) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 221, 203, 49); //
			// Poison Damage
			else if (memcmp(GET_ITEM_NAME15, cStr3, 13) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 145, 209, 61); // Verde
			// Attack Speed -1
			else if (memcmp(GET_ITEM_NAME16, cStr3, 15) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 137, 185, 195); //
			// light
			else if (memcmp(GET_ITEM_NAME17, cStr3, 5) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 255, 235, 104); //
			// Damage added
			else if (memcmp(GET_ITEM_NAME18, cStr3, 12) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 206, 18, 115); //
			// Endurance
			else if (memcmp(GET_ITEM_NAME19, cStr3, 9) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 139, 139, 139); //
			// Extra Damage added
			else if (memcmp(GET_ITEM_NAME20, cStr3, 18) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 228, 184, 67); //
			// Magic Casting Probability
			else if (memcmp(GET_ITEM_NAME21, cStr3, 25) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 214, 72, 72); //
			// Replace %d%% damage to mana
			else if (memcmp(GET_ITEM_NAME22, cStr3, 7) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 113, 169, 226); //
			// Crit Increase Chance
			else if (memcmp(GET_ITEM_NAME23, cStr3, 20) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 221, 203, 49); //
			// Poison Resistance
			else if (memcmp(GET_ITEM_NAME24, cStr3, 17) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 145, 209, 61); // Verde
			// Hitting Probability
			else if (memcmp(GET_ITEM_NAME25, cStr3, 19) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 200, 165, 3); //
			// Defense Ratio
			else if (memcmp(GET_ITEM_NAME26, cStr3, 13) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 170, 127, 174); //
			// HP recovery
			else if (memcmp(GET_ITEM_NAME27, cStr3, 11) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 216, 38, 38); // Rojo
			// SP recovery
			else if (memcmp(GET_ITEM_NAME28, cStr3, 11) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 145, 209, 61); // Verde
			// MP recovery
			else if (memcmp(GET_ITEM_NAME29, cStr3, 11) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 113, 169, 226); // Azul
			// Magic Resistance
			else if (memcmp(GET_ITEM_NAME30, cStr3, 16) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 145, 187, 87); // 
			// Physical Absorption
			else if (memcmp(GET_ITEM_NAME31, cStr3, 19) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 38, 122, 118); // 
			// Magic Absorption
			else if (memcmp(GET_ITEM_NAME32, cStr3, 16) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 175, 26, 100); //
			// Consecutive Attack Damage
			else if (memcmp(GET_ITEM_NAME33, cStr3, 25) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 228, 184, 67); //
			// Experience
			else if (memcmp(GET_ITEM_NAME34, cStr3, 10) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 168, 74, 140); //
			// Gold
			else if (memcmp(GET_ITEM_NAME35, cStr3, 4) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 223, 148, 3); //
			else
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 250, 250, 250);
			if (iLocStr4 > 0)
				iLocStr4 += iLocStr3;
			else if (iLocStr5 > 0)
				iLocStr5 += iLocStr3;
			else if (iLocStr6 > 0)
				iLocStr6 += iLocStr3;
			else if (iLocStr7 > 0)
				iLocStr7 += iLocStr3;
		}
		if (iLocStr4 > 0)
		{
			PutAlignedString(msX, msX + msK, msY + 25 + iLocStr4, cStr4, 255, 255, 255);
			if (iLocStr5 > 0)
				iLocStr5 += iLocStr4;
			else if (iLocStr6 > 0)
				iLocStr6 += iLocStr4;
			else if (iLocStr7 > 0)
				iLocStr7 += iLocStr4;
		}
		if (iLocStr5 > 0)
		{
			PutAlignedString(msX, msX + msK, msY + 25 + iLocStr5, cStr5, 255, 255, 255);
			if (iLocStr6 > 0)
				iLocStr6 += iLocStr5;
			else if (iLocStr7 > 0)
				iLocStr7 += iLocStr5;
		}
		if (iLocStr6 > 0)
		{
			PutAlignedString(msX, msX + msK, msY + 25 + iLocStr6, cStr6, 255, 255, 255);

			if (iLocStr7 > 0)
				iLocStr7 += iLocStr6;
		}	
		if (iLocStr7 > 0)
			PutAlignedString(msX, msX + msK, msY + 25 + iLocStr7, cStr7, 255, 255, 255);
	}
}

// draw market dialog
void CGame::DrawDialogBox_Market(short msX, short msY, short msZ, char cLB)
{
	auto sX = m_stDialogBoxInfo[76].sX;
	auto sY = m_stDialogBoxInfo[76].sY;
	auto szx = m_stDialogBoxInfo[76].sSizeX;
	int r = 0;
	int g = 0;
	int b = 0;

	int r2 = 0;
	int g2 = 0;
	int b2 = 0;

	r = 255; g = 255; b = 0;

	int addy = 35;

	DrawShadowDialog2(sX, sY, 76);

	if (m_sLang == LANG_ENG)
	{
		PutCenterString_SpriteFont(sX, sX + szx, sY + 3, "Market Place", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + szx, sY + 4, "Market Place", 255, 200, 0);
	}
	else if (m_sLang == LANG_ESP)
	{
		PutCenterString_SpriteFont(sX, sX + szx, sY + 3, "Mercado", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + szx, sY + 4, "Mercado", 255, 200, 0);
	}


	if (m_sLang == LANG_ENG)
	{

	}
	else if (m_sLang == LANG_ESP)
	{

	}
	string s;

	DrawNewDialogBox2(SPRID_FLAGSLANG, sX + 195 + 80 - 80, sY + 13, 0, FALSE, true);
	DrawNewDialogBox2(SPRID_FLAGSLANG + 1, sX + 195 + 28 + 80 - 80, sY + 13, 0, FALSE, true);

	switch (m_stDialogBoxInfo[76].cMode)
	{
	case 0:

		if (m_sLang == LANG_ENG)
		{
			if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 70) && (msY < sY + 95))
			{
				PutAlignedString(sX, sX + szx, sY + 70, "Post an Item", 255, 200, 0);
			}
			else
			{
				PutAlignedString(sX, sX + szx, sY + 70, "Post an Item", 255, 255, 255);
			}

			if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 95) && (msY < sY + 120))
			{
				PutAlignedString(sX, sX + szx, sY + 95, "Search for an Item", 255, 200, 0);
			}
			else
			{
				PutAlignedString(sX, sX + szx, sY + 95, "Search for an Item", 255, 255, 255);
			}

			if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 120) && (msY < sY + 145))
			{
				PutAlignedString(sX, sX + szx, sY + 120, "My posted Items", 255, 200, 0);
			}
			else
			{
				PutAlignedString(sX, sX + szx, sY + 120, "My posted Items", 255, 255, 255);
			}

			if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
				PutString_SprFont(sX + 195, sY + 290 + addy, "Close", 250, 250, 250);
			else PutString_SprFont(sX + 195, sY + 290 + addy, "Close", 255, 200, 0);
		}
		else if (m_sLang == LANG_ESP)
		{
			if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 70) && (msY < sY + 95))
			{
				PutAlignedString(sX, sX + szx, sY + 70, "Publicar un item", 255, 200, 0);
			}
			else
			{
				PutAlignedString(sX, sX + szx, sY + 70, "Publicar un item", 255, 255, 255);
			}

			if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 95) && (msY < sY + 120))
			{
				PutAlignedString(sX, sX + szx, sY + 95, "Buscar un item", 255, 200, 0);
			}
			else
			{
				PutAlignedString(sX, sX + szx, sY + 95, "Buscar un item", 255, 255, 255);
			}

			if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 120) && (msY < sY + 145))
			{
				PutAlignedString(sX, sX + szx, sY + 120, "Mis items publicados", 255, 200, 0);
			}
			else
			{
				PutAlignedString(sX, sX + szx, sY + 120, "Mis items publicados", 255, 255, 255);
			}

			if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
				PutString_SprFont(sX + 195, sY + 290 + addy, "Cerrar", 250, 250, 250);
			else PutString_SprFont(sX + 195, sY + 290 + addy, "Cerrar", 255, 200, 0);
		}

		break;

	case 1:
		if (!market_item)
		{
			if (m_sLang == LANG_ENG)
			{
				PutAlignedString(sX + 30, sX + szx - 30, sY + 175, "Drag an Item from your bag and set the price in Coins", 255, 255, 255);
			}
			else if (m_sLang == LANG_ESP)
			{
				PutAlignedString(sX + 30, sX + szx - 30, sY + 175, "Arrastra un item desde tu bolsa y establece el precio en monedas", 255, 255, 255);
			}
		}
		else
		{
			DrawMarketItem(market_item, sX + 60, sY + 70, sX + 60, sY + 70);
			//DrawItemInfo(market_item, sX + 60, sY + 70, sX + 60, sY + 70);

			if (market_coins > 0)
			{
				wsprintf(G_cTxt, "Coins: %d", market_coins);
				PutAlignedString(sX, sX + szx, sY + 200, G_cTxt, 255, 255, 255);


				if (m_sLang == LANG_ENG)
				{
					DrawMenu_SmallButton(sX, sY, msX, msY, 25, 296, "Yes");
					DrawMenu_SmallButton(sX, sY, msX, msY, 150 + 15, 296, "No");

				}
				else if (m_sLang == LANG_ESP)
				{
					DrawMenu_SmallButton(sX, sY, msX, msY, 25, 296, "Si");
					DrawMenu_SmallButton(sX, sY, msX, msY, 150 + 15, 296, "No");

				}

				break;
			}
		}

		if (m_sLang == LANG_ENG)
		{
			if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
				PutString_SprFont(sX + 195, sY + 290 + addy, "Back", 250, 250, 250);
			else PutString_SprFont(sX + 195, sY + 290 + addy, "Back", 255, 200, 0);

		}
		else if (m_sLang == LANG_ESP)
		{
			if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
				PutString_SprFont(sX + 195, sY + 290 + addy, "Atras", 250, 250, 250);
			else PutString_SprFont(sX + 195, sY + 290 + addy, "Atras", 255, 200, 0);

		}
		break;

		case 2:
		{
			int iTotalLines = static_cast<int>(market_names.size());

			int iPointerLoc;
			double d1, d2, d3;
			int maxlines = 13;

			if (iTotalLines > maxlines)
			{
				d1 = static_cast<double>(m_stDialogBoxInfo[76].sView);
				d2 = static_cast<double>(iTotalLines - maxlines);
				d3 = (275.0f * d1) / d2;
				iPointerLoc = static_cast<int>(d3 + 0.5);
				//DrawNewDialogBox2(SPRID_NEWSCROLL, sX + szx - 15, sY + 44, 0);
				DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + szx - 13, sY + 44 + iPointerLoc, 7);
			}
			else
			{
				iPointerLoc = 0;
			}

			// Mostrar texto según el idioma
			if (m_sLang == LANG_ENG) {
				PutAlignedString(sX + 19, sX + 237, sY + 55, "Character Name", 255, 255, 255);//60
			}
			else if (m_sLang == LANG_ESP) {
				PutAlignedString(sX + 19, sX + 237, sY + 55, "Nombre del personaje", 255, 255, 255);
			}

			// Dibujar nombres de mercado
			for (int i = 0; i < 13; i++) {
				if ((i + m_stDialogBoxInfo[76].sView) < market_names.size()) {
					char cTemp[512] = {};
					strcpy(cTemp, market_names[i + m_stDialogBoxInfo[76].sView].c_str());
					if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89)) {
						PutAlignedString(sX + 19, sX + 237, sY + i * 18 + 75, cTemp, 255, 255, 255);
					}
					else {
						PutAlignedString(sX + 19, sX + 237, sY + i * 18 + 75, cTemp, r, g, b);
					}
				}
			}

			// Manejo de clic del botón izquierdo
			if (cLB != 0 && iTotalLines > maxlines)
			{
				if (iGetTopDialogBoxIndex() == 76)
				{
					if ((msX >= sX + szx - 13) && (msX <= sX + szx + 12) && (msY >= sY + 44) && (msY <= sY + 374))
					{
						d1 = static_cast<double>(msY - (sY + 44));
						d2 = static_cast<double>(iTotalLines - maxlines);
						d3 = (d1 * d2) / 275.0f;
						m_stDialogBoxInfo[76].sView = static_cast<int>(d3 + 0.5);
					}
				}
			}
			else
			{
				m_stDialogBoxInfo[76].bIsScrollSelected = FALSE;
			}

			// Manejo del scroll del mouse
			if (iGetTopDialogBoxIndex() == 76 && msZ != 0)
			{
				m_stDialogBoxInfo[76].sView -= msZ / 120;  // Ajustar este valor para un desplazamiento más controlado
				if (m_stDialogBoxInfo[76].sView < 0)
				{
					m_stDialogBoxInfo[76].sView = 0;
				}
				if (m_stDialogBoxInfo[76].sView > iTotalLines - maxlines)
				{
					m_stDialogBoxInfo[76].sView = iTotalLines - maxlines;
				}
				m_DInput.m_sZ = 0;  // Reiniciar el valor del scroll
			}

			// Limitar la vista dentro de los valores válidos
			if (iTotalLines > maxlines && m_stDialogBoxInfo[76].sView > iTotalLines - maxlines)
			{
				m_stDialogBoxInfo[76].sView = iTotalLines - maxlines;
			}
			if (m_stDialogBoxInfo[76].sView < 0 || iTotalLines <= maxlines)
			{
				m_stDialogBoxInfo[76].sView = 0;
			}

			// Mostrar mensajes adicionales según el idioma
			if (m_sLang == LANG_ENG) {
				if (iTotalLines == 0) {
					PutAlignedString(sX + 19, sX + 237, sY + 165, "No operations available in Market Place", 255, 255, 255);
				}

				if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy)) {
					PutString_SprFont(sX + 195, sY + 290 + addy, "Back", 250, 250, 250);
				}
				else {
					PutString_SprFont(sX + 195, sY + 290 + addy, "Back", 255, 200, 0);
				}
			}
			else if (m_sLang == LANG_ESP) {
				if (iTotalLines == 0) {
					PutAlignedString(sX + 19, sX + 237, sY + 165, "No hay operaciones disponibles en el mercado.", 255, 255, 255);
				}

				if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy)) {
					PutString_SprFont(sX + 195, sY + 290 + addy, "Atras", 250, 250, 250);
				}
				else {
					PutString_SprFont(sX + 195, sY + 290 + addy, "Atras", 255, 200, 0);
				}
			}

			break;
		}


	case 3:
	{
		int iTotalLines = static_cast<int>(market_items.size());

		int iPointerLoc;
		double d1, d2, d3;
		int maxlines = 13;

		if (m_sLang == LANG_ENG)
		{
			PutAlignedString(sX + 22, sX + 165, sY + 55, "Item Name", 255, 255, 255);
			PutAlignedString(sX + 23, sX + 166, sY + 55, "Item Name", 255, 255, 255);
			PutAlignedString(sX + 153, sX + 250, sY + 55, "Coins", 255, 255, 255);
			PutAlignedString(sX + 154, sX + 251, sY + 55, "Coins", 255, 255, 255);
		}
		else if (m_sLang == LANG_ESP)
		{
			PutAlignedString(sX + 22, sX + 165, sY + 55, "Nombre del item", 255, 255, 255);
			PutAlignedString(sX + 23, sX + 166, sY + 55, "Nombre del item", 255, 255, 255);
			PutAlignedString(sX + 153, sX + 250, sY + 55, "Monedas", 255, 255, 255);
			PutAlignedString(sX + 154, sX + 251, sY + 55, "Monedas", 255, 255, 255);
		}

		if (iTotalLines > maxlines)
		{
			d1 = static_cast<double>(m_stDialogBoxInfo[76].sView);
			d2 = static_cast<double>(iTotalLines - maxlines);
			d3 = (275.0f * d1) / d2;
			iPointerLoc = static_cast<int>(d3 + 0.5);
			//DrawNewDialogBox2(SPRID_NEWSCROLL, sX + szx - 15, sY + 44, 0);
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + szx - 13, sY + 44 + iPointerLoc, 7);
		}
		else
		{
			iPointerLoc = 0;
		}


		int tempindex = -1;

		for (int i = 0; i < maxlines; i++)
		{
			if ((i + m_stDialogBoxInfo[76].sView) < iTotalLines)
			{
				char cTemp[512] = {};
				strcpy(cTemp, market_items[i + m_stDialogBoxInfo[76].sView].item.m_cName);
				if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
				{
					PutAlignedString(sX + 10, sX + 190, sY + i * 18 + 75, cTemp, 255, 255, 255);
					PutAlignedString(sX + 148, sX + 260, sY + i * 18 + 75, (char*)std::to_string(market_items[i + m_stDialogBoxInfo[76].sView].coins).c_str(), 255, 255, 255);
					tempindex = i + m_stDialogBoxInfo[76].sView;
				}
				else
				{
					PutAlignedString(sX + 10, sX + 190, sY + i * 18 + 75, cTemp, r, g, b);
					PutAlignedString(sX + 148, sX + 260, sY + i * 18 + 75, (char*)std::to_string(market_items[i + m_stDialogBoxInfo[76].sView].coins).c_str(), r, g, b);
				}
			}
		}//para mira esto me olvide

		if (tempindex != -1)
		{
			DrawMarketItem(&market_items[tempindex].item, msX + 50, msY);
		}

		// Manejo de clic del botón izquierdo
		if (cLB != 0 && iTotalLines > maxlines)
		{
			if (iGetTopDialogBoxIndex() == 76)
			{
				if ((msX >= sX + szx - 13) && (msX <= sX + szx + 12) && (msY >= sY + 44) && (msY <= sY + 374))
				{
					d1 = static_cast<double>(msY - (sY + 44));
					d2 = static_cast<double>(iTotalLines - maxlines);
					d3 = (d1 * d2) / 275.0f;
					m_stDialogBoxInfo[76].sView = static_cast<int>(d3 + 0.5);
				}
			}
		}
		else
		{
			m_stDialogBoxInfo[76].bIsScrollSelected = FALSE;
		}

		// Manejo del scroll del mouse
		if (iGetTopDialogBoxIndex() == 76 && msZ != 0)
		{
			m_stDialogBoxInfo[76].sView -= msZ / 120;  // Ajustar este valor para un desplazamiento más controlado
			if (m_stDialogBoxInfo[76].sView < 0)
			{
				m_stDialogBoxInfo[76].sView = 0;
			}
			if (m_stDialogBoxInfo[76].sView > iTotalLines - maxlines)
			{
				m_stDialogBoxInfo[76].sView = iTotalLines - maxlines;
			}
			m_DInput.m_sZ = 0;  // Reiniciar el valor del scroll
		}

		// Limitar la vista dentro de los valores válidos
		if (iTotalLines > maxlines && m_stDialogBoxInfo[76].sView > iTotalLines - maxlines)
		{
			m_stDialogBoxInfo[76].sView = iTotalLines - maxlines;
		}
		if (m_stDialogBoxInfo[76].sView < 0 || iTotalLines <= maxlines)
		{
			m_stDialogBoxInfo[76].sView = 0;
		}

		if (m_sLang == LANG_ENG)
		{
			if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
				PutString_SprFont(sX + 195, sY + 290 + addy, "Back", 250, 250, 250);
			else
				PutString_SprFont(sX + 195, sY + 290 + addy, "Back", 255, 200, 0);
		}
		else if (m_sLang == LANG_ESP)
		{
			if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
				PutString_SprFont(sX + 195, sY + 290 + addy, "Atras", 250, 250, 250);
			else
				PutString_SprFont(sX + 195, sY + 290 + addy, "Atras", 255, 200, 0);
		}

		break;
	}


	case 4:
		DrawMarketItem(&market_item_ptr->item, sX + 60, sY + 70, sX + 60, sY + 70);


		if (m_sLang == LANG_ENG)
		{
			if (string(m_cPlayerName) == market_name)
			{

				wsprintf(G_cTxt, "Withdraw item from market?", market_coins);
				PutAlignedString(sX, sX + szx, sY + 200, G_cTxt, 255, 255, 255);

			}
			else
			{

				wsprintf(G_cTxt, "Coins: %d", market_item_ptr->coins);
				PutAlignedString(sX, sX + szx, sY + 200, G_cTxt, 255, 255, 255);

			}


			DrawMenu_SmallButton(sX, sY, msX, msY, 25, 296, "Yes");
			DrawMenu_SmallButton(sX, sY, msX, msY, 150 + 15, 296, "No");
		}
		else if (m_sLang == LANG_ESP)
		{
			if (string(m_cPlayerName) == market_name)
			{

				wsprintf(G_cTxt, "¿Retirar item del mercado?", market_coins);
				PutAlignedString(sX, sX + szx, sY + 200, G_cTxt, 255, 255, 255);

			}
			else
			{

				wsprintf(G_cTxt, "Coins: %d", market_item_ptr->coins);
				PutAlignedString(sX, sX + szx, sY + 200, G_cTxt, 255, 255, 255);

			}


			DrawMenu_SmallButton(sX, sY, msX, msY, 25, 296, "si");
			DrawMenu_SmallButton(sX, sY, msX, msY, 150 + 15, 296, "No");
		}

		break;

	case 5:
	{

		int iTotalLines = static_cast<int>(market_items.size());

		int iPointerLoc;
		double d1, d2, d3;
		int maxlines = 13;

		if (iTotalLines > maxlines)
		{
			d1 = static_cast<double>(m_stDialogBoxInfo[76].sView);
			d2 = static_cast<double>(iTotalLines - maxlines);
			d3 = (275.0f * d1) / d2;
			iPointerLoc = static_cast<int>(d3 + 0.5);
			//DrawNewDialogBox2(SPRID_NEWSCROLL, sX + szx - 15, sY + 44, 0);
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + szx - 13, sY + 44 + iPointerLoc, 7);
		}
		else
		{
			iPointerLoc = 0;
		}

		int tempindex = -1;

		if (m_sLang == LANG_ENG)
		{
			PutAlignedString(sX + 22, sX + 165, sY + 55, "Item Name", 255, 255, 255);
			PutAlignedString(sX + 23, sX + 166, sY + 55, "Item Name", 255, 255, 255);
			PutAlignedString(sX + 153, sX + 250, sY + 55, "Coins", 255, 255, 255);
			PutAlignedString(sX + 154, sX + 251, sY + 55, "Coins", 255, 255, 255);
		}
		else if (m_sLang == LANG_ESP)
		{
			PutAlignedString(sX + 22, sX + 165, sY + 55, "Nombre del item", 255, 255, 255);
			PutAlignedString(sX + 23, sX + 166, sY + 55, "Nombre del item", 255, 255, 255);
			PutAlignedString(sX + 153, sX + 250, sY + 55, "Monedas", 255, 255, 255);
			PutAlignedString(sX + 154, sX + 251, sY + 55, "Monedas", 255, 255, 255);
		}

		for (int i = 0; i < 13; i++)
			if (((i + m_stDialogBoxInfo[76].sView) < market_items.size()))
			{
				char cTemp[512] = {};
				strcpy(cTemp, market_items[i + m_stDialogBoxInfo[76].sView].item.m_cName);

				if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
				{
					PutAlignedString(sX + 10, sX + 190, sY + i * 18 + 75, cTemp, 255, 255, 255);
					PutAlignedString(sX + 148, sX + 260, sY + i * 18 + 75, (char*)std::to_string(market_items[i + m_stDialogBoxInfo[76].sView].coins).c_str(), 255, 255, 255);
					//DrawMarketItem(&market_items[i + m_stDialogBoxInfo[76].sView].item, msX + 50, msY);
					tempindex = i + m_stDialogBoxInfo[76].sView;
				}
				else
				{
					PutAlignedString(sX + 10, sX + 190, sY + i * 18 + 75, cTemp, r, g, b);
					PutAlignedString(sX + 148, sX + 260, sY + i * 18 + 75, (char*)std::to_string(market_items[i + m_stDialogBoxInfo[76].sView].coins).c_str(), r, g, b);
				}
			}

		if (tempindex != -1)
		{
			DrawMarketItem(&market_items[tempindex].item, msX + 50, msY);
		}

		// Manejo de clic del botón izquierdo
		if (cLB != 0 && iTotalLines > maxlines)
		{
			if (iGetTopDialogBoxIndex() == 76)
			{
				if ((msX >= sX + szx - 13) && (msX <= sX + szx + 12) && (msY >= sY + 44) && (msY <= sY + 374))
				{
					d1 = static_cast<double>(msY - (sY + 44));
					d2 = static_cast<double>(iTotalLines - maxlines);
					d3 = (d1 * d2) / 275.0f;
					m_stDialogBoxInfo[76].sView = static_cast<int>(d3 + 0.5);
				}
			}
		}
		else
		{
			m_stDialogBoxInfo[76].bIsScrollSelected = FALSE;
		}

		// Manejo del scroll del mouse
		if (iGetTopDialogBoxIndex() == 76 && msZ != 0)
		{
			m_stDialogBoxInfo[76].sView -= msZ / 120;  // Ajustar este valor para un desplazamiento más controlado
			if (m_stDialogBoxInfo[76].sView < 0)
			{
				m_stDialogBoxInfo[76].sView = 0;
			}
			if (m_stDialogBoxInfo[76].sView > iTotalLines - maxlines)
			{
				m_stDialogBoxInfo[76].sView = iTotalLines - maxlines;
			}
			m_DInput.m_sZ = 0;  // Reiniciar el valor del scroll
		}

		// Limitar la vista dentro de los valores válidos
		if (iTotalLines > maxlines && m_stDialogBoxInfo[76].sView > iTotalLines - maxlines)
		{
			m_stDialogBoxInfo[76].sView = iTotalLines - maxlines;
		}
		if (m_stDialogBoxInfo[76].sView < 0 || iTotalLines <= maxlines)
		{
			m_stDialogBoxInfo[76].sView = 0;
		}

		if (m_sLang == LANG_ENG)
		{
			if (iTotalLines == 0)
			{
				PutAlignedString(sX + 19, sX + 237, sY + 165, "No have items in Market Place", 255, 255, 255);
			}


			if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
				PutString_SprFont(sX + 195, sY + 290 + addy, "Back", 250, 250, 250);
			else PutString_SprFont(sX + 195, sY + 290 + addy, "Back", 255, 200, 0);
		}
		else if (m_sLang == LANG_ESP)
		{
			if (iTotalLines == 0)
			{
				PutAlignedString(sX + 19, sX + 237, sY + 165, "No hay items en el mercado.", 255, 255, 255);
			}


			if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
				PutString_SprFont(sX + 195, sY + 290 + addy, "Atras", 250, 250, 250);
			else PutString_SprFont(sX + 195, sY + 290 + addy, "Atras", 255, 200, 0);
		}

		break;
	}

	case 6:

		DrawMarketItem(&market_item_ptr->item, sX + 60, sY + 70, sX + 60, sY + 70);

		if (m_sLang == LANG_ENG)
		{
			if (string(m_cPlayerName) == market_name)
			{

				wsprintf(G_cTxt, "Delete you item from market?");
				PutAlignedString(sX, sX + szx, sY + 200, G_cTxt, 255, 255, 255);

			}


			DrawMenu_SmallButton(sX, sY, msX, msY, 25, 296, "Yes");
			DrawMenu_SmallButton(sX, sY, msX, msY, 150 + 15, 296, "No");
		}
		else if (m_sLang == LANG_ESP)
		{
			if (string(m_cPlayerName) == market_name)
			{

				wsprintf(G_cTxt, "¿Deseas eliminar este item del mercado?");
				PutAlignedString(sX, sX + szx, sY + 200, G_cTxt, 255, 255, 255);

			}


			DrawMenu_SmallButton(sX, sY, msX, msY, 25, 296, "Si");
			DrawMenu_SmallButton(sX, sY, msX, msY, 150 + 15, 296, "No");
		}



		break;

	}

	DrawDialogCord(sX, sY, msX, msY);
}

// drop item in market
bool CGame::bItemDrop_Market()
{
	auto sX = m_stDialogBoxInfo[76].sX;
	auto sY = m_stDialogBoxInfo[76].sY;
	auto szx = m_stDialogBoxInfo[76].sSizeX;

	short msX, msY, msZ;
	char cLB, cRB, cMB;
	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);

	int item_index = m_stMCursor.sSelectedObjectID;
	if (!m_pItemList[item_index])
		return false;

	if (m_pItemList[item_index]->m_cItemType == DEF_ITEMTYPE_CONSUME || m_pItemList[item_index]->m_cItemType == DEF_ITEMTYPE_ARROW)
	{
		AddEventList("Cannot place a countable item");
		return false;
	}

	if (m_stDialogBoxInfo[76].cMode != 1)
		return false;

	market_item = m_pItemList[item_index];
	m_bIsItemDisabled[item_index] = TRUE;
	market_item_index = item_index;
	EnableDialogBox(17, 1, NULL, NULL);
	m_stDialogBoxInfo[17].sV11 = m_stMCursor.sSelectedObjectID + 1;
	return true;
}



// click market dialog
void CGame::DlgBoxClick_Market(short msX, short msY)
{
	auto sX = m_stDialogBoxInfo[76].sX;
	auto sY = m_stDialogBoxInfo[76].sY;
	auto szx = m_stDialogBoxInfo[76].sSizeX;

	int addy = 35;

	if (m_stDialogBoxInfo[76].cMode == 0 && (msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
	{
		PlaySound('E', 14, 5);
		DisableDialogBox(76);
		return;
	}
	else if (m_stDialogBoxInfo[76].cMode == 1 || m_stDialogBoxInfo[76].cMode == 2 || m_stDialogBoxInfo[76].cMode == 3 || m_stDialogBoxInfo[76].cMode == 5)
	{
		if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
		{
			if (m_stDialogBoxInfo[76].cMode == 1) m_stDialogBoxInfo[76].cMode = 0;
			else if (m_stDialogBoxInfo[76].cMode == 2) m_stDialogBoxInfo[76].cMode = 0;
			else if (m_stDialogBoxInfo[76].cMode == 3) m_stDialogBoxInfo[76].cMode = 2;
			else if (m_stDialogBoxInfo[76].cMode == 5) m_stDialogBoxInfo[76].cMode = 0;

			PlaySound('E', 14, 5);
			return;
		}
	}

	auto btneng = (msX > sX + 196) && (msX < sX + 219) && (msY > sY + 13) && (msY < sY + 29);
	auto btnesp = (msX > sX + 225) && (msX < sX + 246) && (msY > sY + 13) && (msY < sY + 29);

	if (btneng)
	{
		m_sLang = LANG_ENG;
		PlaySound('E', 14, 5);
		return;
	}

	if (btnesp)
	{
		m_sLang = LANG_ESP;
		PlaySound('E', 14, 5);
		return;
	}

	switch (m_stDialogBoxInfo[76].cMode)
	{
	case 0:

		if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 70) && (msY < sY + 95))
		{
			market_coins = 0;
			market_item = nullptr;
			m_stDialogBoxInfo[76].cMode = 1;
			PlaySound('E', 14, 5);
		}

		if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 95) && (msY < sY + 120))
		{
			bSendCommand(REQUEST_MARKETNAMES, NULL, NULL, NULL, NULL, NULL, NULL);
			market_names.clear();
			m_stDialogBoxInfo[76].cMode = 2;
			PlaySound('E', 14, 5);
		}

		if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 120) && (msY < sY + 145))
		{
			market_name = m_cPlayerName;
			market_items.clear();
			bSendCommand(REQUEST_MARKETITEMS, NULL, NULL, NULL, NULL, NULL, NULL);

			m_stDialogBoxInfo[76].cMode = 5;
			PlaySound('E', 14, 5);
		}

		break;

	case 1:

		if (!market_item)
		{

		}

		if (market_coins == 0)
			break;


		if ((msX >= sX + 25) && (msX <= sX + 25 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
		{
			bSendCommand(REQUEST_SETMARKETITEM, NULL, NULL, NULL, NULL, NULL, NULL);
			PlaySound('E', 14, 5);
			m_stDialogBoxInfo[76].cMode = 0;
			/*DisableDialogBox(76);*/

		}


		if ((msX >= sX + 150 + 15) && (msX <= sX + 150 + DEF_BTNSZX + 15) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
		{
			//DisableDialogBox(76);
			m_bIsItemDisabled[market_item_index] = FALSE;
			m_stDialogBoxInfo[76].cMode = 0;
			PlaySound('E', 14, 5);
		}
		break;

	case 2:
		for (int i = 0; i < 13; i++)
			if (((i + m_stDialogBoxInfo[76].sView) < market_names.size()))
			{
				char cTemp[512] = {};
				strcpy(cTemp, market_names[i + m_stDialogBoxInfo[76].sView].c_str());
				if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
				{
					market_name = cTemp;
					market_items.clear();
					m_stDialogBoxInfo[76].cMode = 3;
					bSendCommand(REQUEST_MARKETITEMS, NULL, NULL, NULL, NULL, NULL, NULL);
					PlaySound('E', 14, 5);
				}
			}

		break;

	case 3:
		for (int i = 0; i < 13; i++)
			if (((i + m_stDialogBoxInfo[76].sView) < market_items.size()))
			{
				auto ptr = &market_items[i + m_stDialogBoxInfo[76].sView];
				if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
				{
					m_stDialogBoxInfo[76].cMode = 4;
					market_item_ptr = ptr;

					PlaySound('E', 14, 5);
				}
			}


		break;

	case 4:
		if ((msX >= sX + 25) && (msX <= sX + 25 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
		{
			bSendCommand(REQUEST_GETMARKETITEM, NULL, NULL, NULL, NULL, NULL, NULL);
			PlaySound('E', 14, 5);
			//DisableDialogBox(76);
			m_stDialogBoxInfo[76].cMode = 0;
		}


		if ((msX >= sX + 150 + 15) && (msX <= sX + 150 + DEF_BTNSZX + 15) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
		{
			m_stDialogBoxInfo[76].cMode = 0;
			//DisableDialogBox(76);
			PlaySound('E', 14, 5);
		}
		break;

	case 5:
		for (int i = 0; i < 13; i++)
			if (((i + m_stDialogBoxInfo[76].sView) < market_items.size()))
			{
				auto ptr = &market_items[i + m_stDialogBoxInfo[76].sView];
				if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
				{
					m_stDialogBoxInfo[76].cMode = 6;
					market_item_ptr = ptr;
					PlaySound('E', 14, 5);
				}
			}


		break;

	case 6:
		if ((msX >= sX + 25) && (msX <= sX + 25 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
		{
			bSendCommand(REQUEST_GETMARKETITEM, NULL, NULL, NULL, NULL, NULL, NULL);
			PlaySound('E', 14, 5);
			m_stDialogBoxInfo[76].cMode = 5;
			market_items.clear();
			bSendCommand(REQUEST_MARKETITEMS, NULL, NULL, NULL, NULL, NULL, NULL);
		}


		if ((msX >= sX + 150 + 15) && (msX <= sX + 150 + DEF_BTNSZX + 15) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
		{
			m_stDialogBoxInfo[76].cMode = 5;
			PlaySound('E', 14, 5);
			market_items.clear();
			bSendCommand(REQUEST_MARKETITEMS, NULL, NULL, NULL, NULL, NULL, NULL);
		}
		break;
	}
}

void CGame::draw_questlist(short msX, short msY, short msZ, char cLB)
{
	short sX, sY, sizeX, sizeY;
	int dnumber = 77;
	int maxlines = 16;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;
	int tmpindex = -1;
	//DrawNewDialogBox2(SPRID_QUESTLIST, sX, sY, 0);

	DrawShadowDialog2(sX, sY, 77);

	PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Quest List", 0, 0, 0);
	PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Quest List", 250, 250, 250);
	
	
	switch (mode)
	{
	case 0:
	{
		PutString2(sX + 23, sY + 33, "Quest list", 228, 161, 6);
		PutString2(sX + 23, sY + 55, "Mob", 228, 161, 6);
		PutString2(sX + 145, sY + 55, "Kills", 228, 161, 6);
		PutString2(sX + 220, sY + 55, "Map", 228, 161, 6);

		int iTotalLines, iPointerLoc;
		double d1, d2, d3;

		iTotalLines = 0;
		for (int i = 0; i < vec_questlist.size(); i++)
		{
			iTotalLines++;
		}

		if (iTotalLines > maxlines)
		{			
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 100, sY, 3);			
		}
		if (iGetTopDialogBoxIndex() == dnumber && msZ != 0)
		{
			m_stDialogBoxInfo[dnumber].sView = m_stDialogBoxInfo[dnumber].sView - msZ / 60;
			m_DInput.m_sZ = 0;
		}
		if (m_stDialogBoxInfo[dnumber].sView < 0) m_stDialogBoxInfo[dnumber].sView = 0;
		if (iTotalLines > maxlines && m_stDialogBoxInfo[dnumber].sView > iTotalLines - maxlines) m_stDialogBoxInfo[dnumber].sView = iTotalLines - maxlines;

		if (iTotalLines > maxlines) {
			d1 = (double)m_stDialogBoxInfo[dnumber].sView;
			d2 = (double)(iTotalLines - maxlines);
			d3 = (274.0f * d1) / d2;
			iPointerLoc = (int)(d3 + 0.5);			
			//DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 100, sY, 3);			
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 242 + 100, sY + 35 + iPointerLoc, 7);
		}
		else iPointerLoc = 0;

		for (int i = 0; i < maxlines; i++)
			if (i + m_stDialogBoxInfo[dnumber].sView < iTotalLines) {

				auto & quest = vec_questlist[i + m_stDialogBoxInfo[dnumber].sView];
				int y = sY + 75 + (15 * i);

				if ((msX > sX + 20) && (msX < sX + 327) && (msY > y) && (msY < y + 15))
				{
					PutString2(sX + 23, y, quest.m_cNpcName, 228, 161, 6);
					PutString2(sX + 145, y, (char*)to_string(quest.m_iMaxKills).c_str(), 228, 161, 6);
					PutString2(sX + 220, y, quest.m_cRealMapName, 228, 161, 6);
					tmpindex = i + m_stDialogBoxInfo[dnumber].sView;

				}
				else
				{
					PutString2(sX + 23, y, quest.m_cNpcName, 255, 255, 255);
					PutString2(sX + 145, y, (char*)to_string(quest.m_iMaxKills).c_str(), 255, 255, 255);
					PutString2(sX + 220, y, quest.m_cRealMapName, 255, 255, 255);
				}
			}

		if (tmpindex != -1)
		{
			auto & quest = vec_questlist[tmpindex];
			int iAdd = 0;
			bool show1 = false;
			bool show2 = false;
			bool show3 = false;

			if (string(quest.m_cQuestReward1) != "None")
			{
				show1 = true;
				iAdd = iAdd + 20;
			}

			if (string(quest.m_cQuestReward2) != "None")
			{
				show2 = true;
				iAdd = iAdd + 20;
			}

			if (string(quest.m_cQuestReward3) != "None")
			{
				show3 = true;
				iAdd = iAdd + 20;
			}

			int startY = msY - 21;

			DrawShadowMenu(msX, startY, msX + 200, startY + iAdd);
			DrawMenu_Box(msX, startY, 200, iAdd);

			if (show1)
			{
				PutAlignedString(msX, msX + 200, startY + 2, quest.m_cQuestReward1, 240, 240, 240);
			}

			if (show2)
			{
				PutAlignedString(msX, msX + 200, startY + 2 + 20, quest.m_cQuestReward2, 240, 240, 240);
			}

			if (show3)
			{
				PutAlignedString(msX, msX + 200, startY + 2 + 40, quest.m_cQuestReward3, 240, 240, 240);
			}
		}

		if (cLB != 0 && iTotalLines > maxlines) {
			if ((iGetTopDialogBoxIndex() == dnumber)) {
				if ((msX >= sX + 334) && (msX <= sX + 346) && (msY >= sY + 40) && (msY <= sY + 320)) {
					d1 = (double)(msY - (sY + 35));
					d2 = (double)(iTotalLines - maxlines);
					d3 = (d1 * d2) / 274.0f;
					iPointerLoc = (int)d3;
					if (iPointerLoc > iTotalLines - maxlines) iPointerLoc = iTotalLines - maxlines;
					m_stDialogBoxInfo[dnumber].sView = iPointerLoc;
				}
			}
		}
		else m_stDialogBoxInfo[dnumber].bIsScrollSelected = FALSE;

		break;
	}
	case 1:
	{
		//HeatoN PRO CODE no dummy (kazin)
		wsprintf(G_cTxt, "Rewards");
		PutAlignedString(sX, sX + sizeX, sY + 50, G_cTxt, 228, 161, 6);

		string rews;
		rews.append(vec_questlist[tmpquest].m_cQuestReward1);
		rews.append(" - ");
		rews.append(vec_questlist[tmpquest].m_cQuestReward2);
		rews.append(" - ");
		rews.append(vec_questlist[tmpquest].m_cQuestReward3);

		wsprintf(G_cTxt, "%s", (char*)rews.c_str());
		PutAlignedString(sX, sX + sizeX, sY + 75, G_cTxt, 240, 240, 240);

		PutAlignedString(sX, sX + sizeX, sY + 100, "(At the end of the Quest, you can choose only one reward)", 240, 240, 240);

		wsprintf(G_cTxt, "Mob: %s", vec_questlist[tmpquest].m_cNpcName);
		PutAlignedString(sX, sX + sizeX, sY + 150, G_cTxt, 240, 240, 240);

		wsprintf(G_cTxt, "Map: %s", vec_questlist[tmpquest].m_cRealMapName);
		PutAlignedString(sX, sX + sizeX, sY + 175, G_cTxt, 240, 240, 240);

		wsprintf(G_cTxt, "Kills: %d", vec_questlist[tmpquest].m_iMaxKills);
		PutAlignedString(sX, sX + sizeX, sY + 200, G_cTxt, 240, 240, 240);

		DrawMenu_SmallButton(sX, sY, msX, msY, 30, 290, "Accept");
		DrawMenu_SmallButton(sX, sY, msX, msY, 246, 290, "Cancel");
		break;
	}

	default:
		break;
	}

	DrawDialogCord(sX, sY, msX, msY);
}


void CGame::click_questlist(short msX, short msY)
{
	short sX, sY, sizeX;
	int dnumber = 77;
	int maxlines = 16;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;

	switch (mode)
	{
	case 0:
	{
		int cnt = 0;
		for (int i = 0; i < vec_questlist.size(); i++)
		{
			cnt++;
		}

		for (int i = 0; i < maxlines; i++)
		{
			if (i + m_stDialogBoxInfo[dnumber].sView < cnt)
			{
				auto & quest = vec_questlist[i + m_stDialogBoxInfo[dnumber].sView];
				int y = sY + 75 + (15 * i);
				if ((msX > sX + 20) && (msX < sX + 327) && (msY > y) && (msY < y + 15))
				{
					tmpquest = i + m_stDialogBoxInfo[dnumber].sView;
					m_stDialogBoxInfo[dnumber].cMode = 1;
					PlaySound('E', 14, 5);
				}
			}
		}
		break;
	}

	case 1:
	{
		auto quest = vec_questlist[tmpquest];

		auto cancel = (msX >= sX + 246) && (msX <= sX + 321) && (msY >= sY + 290) && (msY <= sY + 311);
		auto accept = (msX >= sX + 30) && (msX <= sX + 105) && (msY >= sY + 290) && (msY <= sY + 311);

		if (cancel)
		{
			m_stDialogBoxInfo[dnumber].cMode = 0;
			PlaySound('E', 14, 5);
		}

		if (accept)
		{
			char data[256];
			auto * cp = data;

			Push(cp, (u32)START_QUEST);
			Push(cp, (u16)0);

			Push(cp, quest.m_sQuestID);
			m_pGSock->iSendMsg(data, cp - data);

			PlaySound('E', 14, 5);
			m_stDialogBoxInfo[dnumber].cMode = 0;
		}
		break;
	}
	default:break;
	}
}


void CGame::draw_charquests(short msX, short msY, short msZ, char cLB)
{
	short sX, sY, sizeX, sizeY;
	int dnumber = 78;
	int maxlines = 16;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;
		
	DrawShadowDialog2(sX, sY, 78);

	PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Character Quest's", 0, 0, 0);
	PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Character Quest's", 250, 250, 250);
	int tmpindex = -1;
	switch (mode)
	{
	case 0:
	{
		PutString2(sX + 23, sY + 55, "Mob", 228, 161, 6);
		PutString2(sX + 145, sY + 55, "Kills", 228, 161, 6);
		PutString2(sX + 220, sY + 55, "Map", 228, 161, 6);

		int iTotalLines, iPointerLoc;
		double d1, d2, d3;

		iTotalLines = 0;
		for (int i = 0; i < vec_charquest.size(); i++)
		{
			iTotalLines++;
		}

		if (iTotalLines > maxlines){
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 100, sY, 3);			
		}
		if (iGetTopDialogBoxIndex() == dnumber && msZ != 0)
		{
			m_stDialogBoxInfo[dnumber].sView = m_stDialogBoxInfo[dnumber].sView - msZ / 60;
			m_DInput.m_sZ = 0;
		}
		if (m_stDialogBoxInfo[dnumber].sView < 0) m_stDialogBoxInfo[dnumber].sView = 0;
		if (iTotalLines > maxlines && m_stDialogBoxInfo[dnumber].sView > iTotalLines - maxlines) m_stDialogBoxInfo[dnumber].sView = iTotalLines - maxlines;

		if (iTotalLines > maxlines) {
			d1 = (double)m_stDialogBoxInfo[dnumber].sView;
			d2 = (double)(iTotalLines - maxlines);
			d3 = (274.0f * d1) / d2;
			iPointerLoc = (int)(d3 + 0.5);			
			//DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 100, sY, 3);			
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 242 + 100, sY + 35 + iPointerLoc, 7);
		}
		else iPointerLoc = 0;

		for (int i = 0; i < maxlines; i++)
			if (i + m_stDialogBoxInfo[dnumber].sView < iTotalLines) {

				auto & quest = vec_charquest[i + m_stDialogBoxInfo[dnumber].sView];
				int y = sY + 80 + (15 * i);

				if ((msX > sX + 20) && (msX < sX + 327) && (msY > y) && (msY < y + 15))
				{
					PutString2(sX + 23, y, quest.m_cNpcName, 228, 161, 6);

					string skills;
					skills.append(to_string(quest.m_iKills));
					skills.append("/");
					skills.append(to_string(quest.m_iMaxKills));
					PutString2(sX + 145, y, (char*)skills.c_str(), 228, 161, 6);

					PutString2(sX + 220, y, quest.m_cRealMapName, 228, 161, 6);


					tmpindex = i + m_stDialogBoxInfo[dnumber].sView;
				}
				else
				{
					PutString2(sX + 23, y, quest.m_cNpcName, 255, 255, 255);

					string skills;
					skills.append(to_string(quest.m_iKills));
					skills.append("/");
					skills.append(to_string(quest.m_iMaxKills));
					PutString2(sX + 145, y, (char*)skills.c_str(), 255, 255, 255);

					PutString2(sX + 220, y, quest.m_cRealMapName, 255, 255, 255);
				}
			}

		if (tmpindex != -1)
		{
			auto & quest = vec_charquest[tmpindex];
			int iAdd = 0;
			bool show1 = false;
			bool show2 = false;
			bool show3 = false;

			if (string(quest.m_cQuestReward1) != "None")
			{
				show1 = true;
				iAdd = iAdd + 20;
			}

			if (string(quest.m_cQuestReward2) != "None")
			{
				show2 = true;
				iAdd = iAdd + 20;
			}

			if (string(quest.m_cQuestReward3) != "None")
			{
				show3 = true;
				iAdd = iAdd + 20;
			}

			int startY = msY - 21;

			DrawShadowMenu(msX, startY, msX + 200, startY + iAdd);
			DrawMenu_Box(msX, startY, 200, iAdd);

			if (show1)
			{
				PutAlignedString(msX, msX + 200, startY + 2, quest.m_cQuestReward1, 240, 240, 240);
			}

			if (show2)
			{
				PutAlignedString(msX, msX + 200, startY + 2 + 20, quest.m_cQuestReward2, 240, 240, 240);
			}

			if (show3)
			{
				PutAlignedString(msX, msX + 200, startY + 2 + 40, quest.m_cQuestReward3, 240, 240, 240);
			}
		}

		if (cLB != 0 && iTotalLines > maxlines) {
			if ((iGetTopDialogBoxIndex() == dnumber)) {
				if ((msX >= sX + 334) && (msX <= sX + 346) && (msY >= sY + 40) && (msY <= sY + 320)) {
					d1 = (double)(msY - (sY + 35));
					d2 = (double)(iTotalLines - maxlines);
					d3 = (d1 * d2) / 274.0f;
					iPointerLoc = (int)d3;
					if (iPointerLoc > iTotalLines - maxlines) iPointerLoc = iTotalLines - maxlines;
					m_stDialogBoxInfo[dnumber].sView = iPointerLoc;
				}
			}
		}
		else m_stDialogBoxInfo[dnumber].bIsScrollSelected = FALSE;

		break;
	}
	case 1:
	{
		//HeatoN PRO CODE no dummy (kazin)
		wsprintf(G_cTxt, "Rewards");
		PutAlignedString(sX, sX + sizeX, sY + 50, G_cTxt, 228, 161, 6);

		string rews;
		rews.append(vec_charquest[tmpquest].m_cQuestReward1);
		rews.append(" - ");
		rews.append(vec_charquest[tmpquest].m_cQuestReward2);
		rews.append(" - ");
		rews.append(vec_charquest[tmpquest].m_cQuestReward3);

		wsprintf(G_cTxt, "%s", (char*)rews.c_str());
		PutAlignedString(sX, sX + sizeX, sY + 75, G_cTxt, 240, 240, 240);

		PutAlignedString(sX, sX + sizeX, sY + 100, "(At the end of the Quest, you can choose only one reward)", 240, 240, 240);

		wsprintf(G_cTxt, "Mob: %s", vec_charquest[tmpquest].m_cNpcName);
		PutAlignedString(sX, sX + sizeX, sY + 125, G_cTxt, 240, 240, 240);

		wsprintf(G_cTxt, "Map: %s", vec_charquest[tmpquest].m_cRealMapName);
		PutAlignedString(sX, sX + sizeX, sY + 150, G_cTxt, 240, 240, 240);

		wsprintf(G_cTxt, "Kills: %d", vec_charquest[tmpquest].m_iMaxKills);
		PutAlignedString(sX, sX + sizeX, sY + 175, G_cTxt, 240, 240, 240);

		string skills = "Current Kills: ";
		skills.append(to_string(vec_charquest[tmpquest].m_iKills));
		skills.append("/");
		skills.append(to_string(vec_charquest[tmpquest].m_iMaxKills));

		PutAlignedString(sX, sX + sizeX, sY + 200, (char*)skills.c_str(), 255, 255, 255);

		string s_status = "Status: ";
		if (vec_charquest[tmpquest].m_sCompleted == 1)
		{
			s_status.append("Completed");
			PutAlignedString(sX, sX + sizeX, sY + 225, (char*)s_status.c_str(), 255, 255, 255);
		}
		else if (vec_charquest[tmpquest].m_sStatus == 1){
			s_status.append("Active");
			PutAlignedString(sX, sX + sizeX, sY + 225, (char*)s_status.c_str(), 255, 255, 255);
		}
		else {
			s_status.append("Paused");
			PutAlignedString(sX, sX + sizeX, sY + 225, (char*)s_status.c_str(), 255, 255, 255);
		}

		if (vec_charquest[tmpquest].m_sCompleted == 0)
			DrawMenu_SmallButton(sX, sY, msX, msY, 246, 33, "Dismiss");

		if (vec_charquest[tmpquest].m_sCompleted == 1)
			DrawMenu_SmallButton(sX, sY, msX, msY, 30, 290, "Get reward");
		else if (vec_charquest[tmpquest].m_sStatus == 1)
			DrawMenu_SmallButton(sX, sY, msX, msY, 30, 290, "Disable");
		else
			DrawMenu_SmallButton(sX, sY, msX, msY, 30, 290, "Enable");

		DrawMenu_SmallButton(sX, sY, msX, msY, 246, 290, "Back");
		break;
	}

	case 2:
	{
		PutAlignedString(sX, sX + sizeX, sY + 35, "Dismiss quest", 228, 161, 6);

		wsprintf(G_cTxt, "Mob: %s", vec_charquest[tmpquest].m_cNpcName);
		PutAlignedString(sX, sX + sizeX, sY + 50 + 15, G_cTxt, 255, 255, 255);

		wsprintf(G_cTxt, "Map: %s", vec_charquest[tmpquest].m_cRealMapName);
		PutAlignedString(sX, sX + sizeX, sY + 75 + 15, G_cTxt, 255, 255, 255);

		string skills = "Current Kills: ";
		skills.append(to_string(vec_charquest[tmpquest].m_iKills));
		skills.append("/");
		skills.append(to_string(vec_charquest[tmpquest].m_iMaxKills));

		PutAlignedString(sX, sX + sizeX, sY + 100 + 15, (char*)skills.c_str(), 255, 255, 255);

		wsprintf(G_cTxt, "Rewards");
		PutAlignedString(sX, sX + sizeX, sY + 125 + 15, G_cTxt, 228, 161, 6);

		string rews;
		rews.append(vec_charquest[tmpquest].m_cQuestReward1);
		rews.append(" - ");
		rews.append(vec_charquest[tmpquest].m_cQuestReward2);
		rews.append(" - ");
		rews.append(vec_charquest[tmpquest].m_cQuestReward3);

		wsprintf(G_cTxt, "%s", (char*)rews.c_str());
		PutAlignedString(sX, sX + sizeX, sY + 150 + 15, G_cTxt, 240, 240, 240);

		PutAlignedString(sX, sX + sizeX, sY + 135 + 75, "Are you sure to cancel the quest?", 255, 255, 255);
		PutAlignedString(sX, sX + sizeX, sY + 135 + 90, "All data from this quest will be lost.", 255, 255, 255);

		DrawMenu_SmallButton(sX, sY, msX, msY, 30, 290, "Yes");
		DrawMenu_SmallButton(sX, sY, msX, msY, 246, 290, "No");
		break;
	}

	case 3:
	{
		PutAlignedString(sX, sX + sizeX, sY + 35, "Select one Reward", 228, 161, 6);

		if (string(vec_charquest[tmpquest].m_cQuestReward1) != "None")
		{
			if ((msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 80) && (msY <= sY + 80 + 12))
				PutAlignedString(sX, sX + sizeX, sY + 80, vec_charquest[tmpquest].m_cQuestReward1, 228, 161, 6);
			else
				PutAlignedString(sX, sX + sizeX, sY + 80, vec_charquest[tmpquest].m_cQuestReward1, 255, 255, 255);
		}

		if (string(vec_charquest[tmpquest].m_cQuestReward2) != "None")
		{
			if ((msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 100) && (msY <= sY + 100 + 12))
				PutAlignedString(sX, sX + sizeX, sY + 100, vec_charquest[tmpquest].m_cQuestReward2, 228, 161, 6);
			else PutAlignedString(sX, sX + sizeX, sY + 100, vec_charquest[tmpquest].m_cQuestReward2, 255, 255, 255);

		}

		if (string(vec_charquest[tmpquest].m_cQuestReward3) != "None")
		{
			if ((msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 120) && (msY <= sY + 120 + 12))
				PutAlignedString(sX, sX + sizeX, sY + 120, vec_charquest[tmpquest].m_cQuestReward3, 228, 161, 6);
			else PutAlignedString(sX, sX + sizeX, sY + 120, vec_charquest[tmpquest].m_cQuestReward3, 255, 255, 255);
		}

		switch (selectedoption)
		{
		case 1:
			wsprintf(G_cTxt, "Selected: %s", vec_charquest[tmpquest].m_cQuestReward1);
			PutAlignedString(sX, sX + sizeX, sY + 150, G_cTxt, 228, 161, 6);
			break;
		case 2:
			wsprintf(G_cTxt, "Selected: %s", vec_charquest[tmpquest].m_cQuestReward2);
			PutAlignedString(sX, sX + sizeX, sY + 150, G_cTxt, 228, 161, 6);
			break;
		case 3:
			wsprintf(G_cTxt, "Selected: %s", vec_charquest[tmpquest].m_cQuestReward3);
			PutAlignedString(sX, sX + sizeX, sY + 150, G_cTxt, 228, 161, 6);
			break;
		default:

			break;
		}



		DrawMenu_SmallButton(sX, sY, msX, msY, 30, 290, "Obtain");
		DrawMenu_SmallButton(sX, sY, msX, msY, 246, 290, "Back");
		break;
	}

	default:
		break;
	}

	DrawDialogCord(sX, sY, msX, msY);
}


void CGame::click_charquests(short msX, short msY)
{
	short sX, sY, sizeX;
	int dnumber = 78;
	int maxlines = 16;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;

	switch (mode)
	{
	case 0:
	{
		int cnt = 0;
		for (int i = 0; i < vec_charquest.size(); i++)
		{
			cnt++;
		}

		for (int i = 0; i < maxlines; i++)
		{
			if (i + m_stDialogBoxInfo[dnumber].sView < cnt)
			{
				auto & quest = vec_charquest[i + m_stDialogBoxInfo[dnumber].sView];
				int y = sY + 80 + (15 * i);
				if ((msX > sX + 20) && (msX < sX + 327) && (msY > y) && (msY < y + 15))
				{
					tmpquest = i + m_stDialogBoxInfo[dnumber].sView;
					m_stDialogBoxInfo[dnumber].cMode = 1;
					PlaySound('E', 14, 5);
				}
			}
		}
		break;
	}

	case 1:
	{
		auto quest = vec_charquest[tmpquest];

		auto back = (msX >= sX + 246) && (msX <= sX + 321) && (msY >= sY + 290) && (msY <= sY + 311);
		auto dismiss = (msX >= sX + 246) && (msX <= sX + 321) && (msY >= sY + 33) && (msY <= sY + 33 + 21);
		auto button = (msX >= sX + 30) && (msX <= sX + 105) && (msY >= sY + 290) && (msY <= sY + 311);

		if (button)
		{
			if (vec_charquest[tmpquest].m_sCompleted == 1)
			{
				selectedoption = 0;
				m_stDialogBoxInfo[dnumber].cMode = 3;
				PlaySound('E', 14, 5);
			}

			else if (vec_charquest[tmpquest].m_sStatus == 1)
			{
				char data[256];
				auto * cp = data;
				Push(cp, (u32)PAUSE_QUEST);
				Push(cp, (u16)0);
				Push(cp, tmpquest);
				m_pGSock->iSendMsg(data, cp - data);
				PlaySound('E', 14, 5);
			}
			else
			{
				char data[256];
				auto * cp = data;
				Push(cp, (u32)ACTIVE_QUEST);
				Push(cp, (u16)0);
				Push(cp, tmpquest);
				m_pGSock->iSendMsg(data, cp - data);
				PlaySound('E', 14, 5);
			}
		}

		if (back)
		{
			m_stDialogBoxInfo[dnumber].cMode = 0;
			PlaySound('E', 14, 5);
		}

		if (dismiss)
		{
			if (vec_charquest[tmpquest].m_sCompleted == 0){
				m_stDialogBoxInfo[dnumber].cMode = 2;
				PlaySound('E', 14, 5);
			}
		}
		break;
	}

	case 2:
	{
		auto quest = vec_charquest[tmpquest];

		auto no = (msX >= sX + 246) && (msX <= sX + 321) && (msY >= sY + 290) && (msY <= sY + 311);
		auto yes = (msX >= sX + 30) && (msX <= sX + 105) && (msY >= sY + 290) && (msY <= sY + 311);


		if (no)
		{
			m_stDialogBoxInfo[dnumber].cMode = 1;
			PlaySound('E', 14, 5);
		}

		if (yes)
		{
			if (vec_charquest[tmpquest].m_sCompleted == 0){
				char data[256];
				auto * cp = data;
				Push(cp, (u32)DISMISS_QUEST);
				Push(cp, (u16)0);
				Push(cp, tmpquest);
				m_pGSock->iSendMsg(data, cp - data);
				m_stDialogBoxInfo[dnumber].cMode = 0;
				PlaySound('E', 14, 5);
			}
		}
		break;
	}

	case 3:
	{

		auto quest = vec_charquest[tmpquest];

		auto no = (msX >= sX + 246) && (msX <= sX + 321) && (msY >= sY + 290) && (msY <= sY + 311);
		auto yes = (msX >= sX + 30) && (msX <= sX + 105) && (msY >= sY + 290) && (msY <= sY + 311);



		{
			if ((msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 80) && (msY <= sY + 80 + 12))
				PutAlignedString(sX, sX + sizeX, sY + 80, vec_charquest[tmpquest].m_cQuestReward1, 228, 161, 6);
			else
				PutAlignedString(sX, sX + sizeX, sY + 80, vec_charquest[tmpquest].m_cQuestReward1, 255, 255, 255);
		}

		if (string(vec_charquest[tmpquest].m_cQuestReward2) != "None")
		{
			if ((msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 100) && (msY <= sY + 100 + 12))
				PutAlignedString(sX, sX + sizeX, sY + 100, vec_charquest[tmpquest].m_cQuestReward2, 228, 161, 6);
			else PutAlignedString(sX, sX + sizeX, sY + 100, vec_charquest[tmpquest].m_cQuestReward2, 255, 255, 255);

		}

		if (string(vec_charquest[tmpquest].m_cQuestReward3) != "None")
		{
			if ((msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 120) && (msY <= sY + 120 + 12))
				PutAlignedString(sX, sX + sizeX, sY + 120, vec_charquest[tmpquest].m_cQuestReward3, 228, 161, 6);
			else PutAlignedString(sX, sX + sizeX, sY + 120, vec_charquest[tmpquest].m_cQuestReward3, 255, 255, 255);
		}

		auto option1 = (msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 80) && (msY <= sY + 80 + 12);
		auto option2 = (msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 100) && (msY <= sY + 100 + 12);
		auto option3 = (msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 120) && (msY <= sY + 120 + 12);

		if (string(vec_charquest[tmpquest].m_cQuestReward1) != "None")
		{
			if (option1)
			{
				selectedoption = 1;
				PlaySound('E', 14, 5);
			}
		}

		if (string(vec_charquest[tmpquest].m_cQuestReward2) != "None")
		{
			if (option2)
			{
				selectedoption = 2;
				PlaySound('E', 14, 5);
			}
		}


		if (string(vec_charquest[tmpquest].m_cQuestReward3) != "None")
		{
			if (option3)
			{
				selectedoption = 3;
				PlaySound('E', 14, 5);
			}
		}


		if (no)
		{
			m_stDialogBoxInfo[dnumber].cMode = 1;
			PlaySound('E', 14, 5);
		}

		if (yes)
		{
			if (vec_charquest[tmpquest].m_sCompleted == 1){

				if (selectedoption == 0)
				{
					AddEventList("Por favor no seas estupido y selecciona una opcion. Gracias.");
					PlaySound('E', 14, 5);
					return;
				}

				char data[260];
				auto * cp = data;
				Push(cp, (u32)REWARD_QUEST);
				Push(cp, (u16)0);
				Push(cp, tmpquest);
				Push(cp, selectedoption);
				m_pGSock->iSendMsg(data, cp - data);
				m_stDialogBoxInfo[dnumber].cMode = 0;
				PlaySound('E', 14, 5);
			}
		}
		break;
	}

	default:break;
	}
}
/*

void CGame::Draw_NetGraph()
{
	int fix_x, fix_y; // Coordenadas iniciales que ajustarás según la resolución

	if (c_reso->IsResolution() == C1024x768)
	{
		fix_x = 1024 - 100;
		fix_y = 768 - 30;
	}
	else if (c_reso->IsResolution() == C800x600)
	{
		fix_x = 800 - 100;
		fix_y = 600 - 30;
	}
	else if (c_reso->IsResolution() == C640x480)
	{
		fix_x = 640 - 100;
		fix_y = 480 - 30;
	}

	if (BigBar == 0)
	{
		fix_y = fix_y - 54;
	}

	m_dPing = calculatePing();
	char pingMessage[50]; // Buffer suficiente para almacenar el mensaje
	if (m_dPing > 0.0)
		sprintf(pingMessage, "Ping: %.2f ms", m_dPing);
	else
		sprintf(pingMessage, "Ping: calculating...");

	// Mostrar el mensaje en la posición calculada
	PutString2(fix_x, fix_y, pingMessage, 55, 255, 255);
}
*/
void CGame::Draw_NetGraph()
{
	const int CHAR_WIDTH = 5; // Ajusta según el tamaño real de tu fuente

	int fix_x, fix_y; // Coordenadas iniciales que ajustarás según la resolución

	if (c_reso->IsResolution() == C1024x768)
	{
		fix_x = 1024 - 100;
		fix_y = 768 - 55;
	}
	else if (c_reso->IsResolution() == C800x600)
	{
		fix_x = 800 - 100;
		fix_y = 600 - 55;
	}
	else if (c_reso->IsResolution() == C640x480)
	{
		fix_x = 640 - 100;
		fix_y = 480 - 55;
	}

	if (BigBar == 0)
	{
		fix_y = fix_y - 54;
	}

	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);


	//time
	char timeLabel[10] = "Time:";
	char timeValue[50];
	wsprintf(timeValue, "  %d", SysTime.wSecond);
	PutString2(fix_x, fix_y - 30, timeLabel, 255, 200, 0);
	int timelabelWidth = strlen(timeLabel) * CHAR_WIDTH;
	PutString2(fix_x + timelabelWidth, fix_y - 30, timeValue, 255, 255, 255);

	//fps
	char fpsLabel[10] = "Fps:";
	char fpsValue[50];         
	wsprintf(fpsValue, " %d", m_sFPS + 713);
	PutString2(fix_x, fix_y - 15, fpsLabel, 255, 200, 0);
	int fpslabelWidth = strlen(fpsLabel) * CHAR_WIDTH;
	PutString2(fix_x + fpslabelWidth, fix_y - 15, fpsValue, 255, 255, 255);

	//ping
	//m_dPing = calculatePing();
	char pingLabel[10] = "Ping:";
	char pingValue[50];

	/*if (m_dPing > 130.0) {
		m_dPing -= 60.0;  // Descontamos 60 si el ping es mayor a 130
	}*/

	if (m_dPing > 0.0)
		sprintf(pingValue, " %.2f ms", m_dPing);
	else
		sprintf(pingValue, " request...");

	PutString2(fix_x, fix_y, pingLabel, 255, 200, 0);
	int labelWidth = strlen(pingLabel) * CHAR_WIDTH;
	PutString2(fix_x + labelWidth, fix_y, pingValue, 255, 255, 255);
}


void CGame::DrawQuest()
{
	if (vec_charquest.size() == 0) return;
	int questindex = -1;

	int fix_x = 0;
	int less = 15;
	if (c_reso->IsResolution() == C640x480)
	{
		fix_x = 540 - less;
	}
	else if (c_reso->IsResolution() == C800x600)
	{
		fix_x = 700 - less;
	}
	else if (c_reso->IsResolution() == C1024x768)
	{
		fix_x = 924 - less;
	}

	for (int i = 0; i < vec_charquest.size(); i++)
	{
		auto quest = vec_charquest[i];
		if (quest.m_sStatus == 1)
		{
			questindex = i;
		}
	}
		
	// {
		// Convertir a char[]
	
	//}

	// Ahora puedes usar pingMessage como un char[]

	if (questindex != -1)
	{
		if (true)
		{
			auto quest = vec_charquest[questindex];
			//if (string(quest.m_cMapName) != m_cMapName) return;
			if (quest.m_sStatus == 1){
				PutString2(fix_x - 10, 200 - 28 + 70, "Quest:", 255, 200, 0);
				wsprintf(G_cTxt, "%s: %d/%d", quest.m_cNpcName, quest.m_iKills, quest.m_iMaxKills);
				PutString2(fix_x - 10, 215 - 28 + 70, G_cTxt, 255,255,255);
			}
		}
	}
}

void CGame::DrawMenu_Box(short sX, short sY, short x, short y)
{
	DrawDialogShadowPixels(1, x, sX, sY);
	DrawDialogShadowPixels(1, x, sX, sY + y);
	DrawDialogShadowPixels(2, y, sX, sY);
	DrawDialogShadowPixels(2, y + 1, sX + x, sY);
}

void CGame::get_questlist(char * data)
{
	vec_questlist.clear();
	short size = 0;

	Pop(data, size);

	for (int i = 0; i < size; i++)
	{
		st_quest u;
		Pop(data, u.m_sQuestID);
		ZeroMemory(u.m_cNpcName, sizeof(u.m_cNpcName));
		Pop(data, u.m_cNpcName, 21);
		ZeroMemory(u.m_cMapName, sizeof(u.m_cMapName));
		Pop(data, u.m_cMapName, 12);
		ZeroMemory(u.m_cRealMapName, sizeof(u.m_cRealMapName));
		Pop(data, u.m_cRealMapName, 12);
		Pop(data, u.m_iMaxKills);
		Pop(data, u.m_iRewardContribution);


		ZeroMemory(u.m_cQuestReward1, sizeof(u.m_cQuestReward1));
		Pop(data, u.m_cQuestReward1, 21);
		ZeroMemory(u.m_cQuestReward2, sizeof(u.m_cQuestReward2));
		Pop(data, u.m_cQuestReward2, 21);
		ZeroMemory(u.m_cQuestReward3, sizeof(u.m_cQuestReward3));
		Pop(data, u.m_cQuestReward3, 21);


		vec_questlist.push_back(u);
	}
}

void CGame::get_charquests(char * data)
{
	vec_charquest.clear();
	short size = 0;

	Pop(data, size);

	for (int i = 0; i < size; i++)
	{
		st_charquest u;

		Pop(data, u.m_sParentQuestID);
		ZeroMemory(u.m_cNpcName, sizeof(u.m_cNpcName));
		Pop(data, u.m_cNpcName, 21);
		ZeroMemory(u.m_cMapName, sizeof(u.m_cMapName));
		Pop(data, u.m_cMapName, 12);
		ZeroMemory(u.m_cRealMapName, sizeof(u.m_cRealMapName));
		Pop(data, u.m_cRealMapName, 12);
		Pop(data, u.m_iKills);
		Pop(data, u.m_iMaxKills);
		Pop(data, u.m_iRewardContribution);

		ZeroMemory(u.m_cQuestReward1, sizeof(u.m_cQuestReward1));
		Pop(data, u.m_cQuestReward1, 21);
		ZeroMemory(u.m_cQuestReward2, sizeof(u.m_cQuestReward2));
		Pop(data, u.m_cQuestReward2, 21);
		ZeroMemory(u.m_cQuestReward3, sizeof(u.m_cQuestReward3));
		Pop(data, u.m_cQuestReward3, 21);

		Pop(data, u.m_sStatus);
		Pop(data, u.m_sCompleted);

		vec_charquest.push_back(u);
	}
}

void CGame::get_charquests_byindex(char * data)
{
	int index = -1;

	Pop(data, index);

	for (int i = 0; i < vec_charquest.size(); i++)
	{
		if (index != i) continue;

		Pop(data, vec_charquest[i].m_iKills);
		Pop(data, vec_charquest[i].m_sStatus);
		Pop(data, vec_charquest[i].m_sCompleted);

		AddEventList((char*)to_string(vec_charquest[i].m_sCompleted).c_str());
	}
}


static bool startsWith(const std::string& str, const std::string& prefix) {
	return str.rfind(prefix, 0) == 0; // Verifica si `prefix` es el inicio de `str`
}


void CGame::draw_trades(short msX, short msY, short msZ, char cLB)
{
	short sX, sY, sizeX, sizeY;
	int dnumber = 79;
	int maxlines = 18;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;	

	DrawShadowDialog2(sX, sY, 79);

	switch (trade_mode)
	{
	case DEF_NOTSELECTED:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Items for sale", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Items for sale", 250, 250, 250);
		//255, 200, 0

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 60) && (msY <= sY + 60 + 15))
		PutAlignedString(sX, sX + sizeX, sY + 60, "Trade ek", 255, 255, 255); 
		else PutAlignedString(sX, sX + sizeX, sY + 60, "Trade ek", 255, 200, 0);


		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 80) && (msY <= sY + 80 + 15))
		PutAlignedString(sX, sX + sizeX, sY + 80, "Trade coins", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 80, "Trade coins", 255, 200, 0);


		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 100) && (msY <= sY + 100 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 100, "Trade contribution", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 100, "Trade contribution", 255, 200, 0);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 120) && (msY <= sY + 120 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 120, "Trade majestics", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 120, "Trade majestics", 255, 200, 0);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 140) && (msY <= sY + 140 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 140, "Trade balls", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 140, "Trade balls", 255, 200, 0);

		return;
		break;
	}
	case DEF_TRADEEK:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Trade Ek", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Trade Ek", 250, 250, 250);
		char temp[50];
		wsprintf(temp, "Ek Count: %d", m_iEnemyKillCount);
		PutAlignedString(sX, sX + sizeX, sY + 33, temp, 255, 255, 255);
		break;
	}
	case DEF_TRADEBALLS:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Trade Balls", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Trade Balls", 250, 250, 250);
		//255, 200, 0

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 60) && (msY <= sY + 60 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 60, "Contribution", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 60, "Contribution", 255, 200, 0);


		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 80) && (msY <= sY + 80 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 80, "Coins", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 80, "Coins", 255, 200, 0);


		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 100) && (msY <= sY + 100 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 100, "Reputation", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 100, "Reputation", 255, 200, 0);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 120) && (msY <= sY + 120 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 120, "Majestics", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 120, "Majestics", 255, 200, 0);
		

		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Back");
		return;
		break;
	}
	case DEF_TRADECOIN:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Trade Coin", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Trade Coin", 250, 250, 250);
		char temp[50];
		wsprintf(temp, "Coin count: %d", m_iCoins);
		PutAlignedString(sX, sX + sizeX, sY + 33, temp, 255, 255, 255);
		break;
	}
	case DEF_TRADECONTRIB:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Trade Contrib", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Trade Contrib", 250, 250, 250);
		char temp[50];
		wsprintf(temp, "Contrib Count: %d", m_iContribution);
		PutAlignedString(sX, sX + sizeX, sY + 33, temp, 255, 255, 255);
		break;
	}
	case DEF_TRADEMAJS:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Trade Majestics", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Trade Majestics", 250, 250, 250);
		char temp[50];
		wsprintf(temp, "Majestics Count: %d", m_iGizonItemUpgradeLeft);
		PutAlignedString(sX, sX + sizeX, sY + 33, temp, 255, 255, 255);
		break;
	}
	default:
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Trades", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Trades", 250, 250, 250);
		break;
	}

	switch (mode)
	{
	case 0:
	{
		int iTotalLines = static_cast<int>(vec_trades.size());

		int iPointerLoc;
		double d1, d2, d3;
		int maxlines = 11;

		if (m_sLang == LANG_ENG)
		{
			PutAlignedString(sX + 22, sX + 165, sY + 55, "Item Name", 255, 255, 255);
			PutAlignedString(sX + 23, sX + 166, sY + 55, "Item Name", 255, 255, 255);
			PutAlignedString(sX + 153, sX + 250, sY + 55, "Cost", 255, 255, 255);
			PutAlignedString(sX + 154, sX + 251, sY + 55, "Cost", 255, 255, 255);
		}
		else if (m_sLang == LANG_ESP)
		{
			PutAlignedString(sX + 22, sX + 165, sY + 55, "Nombre del item", 255, 255, 255);
			PutAlignedString(sX + 23, sX + 166, sY + 55, "Nombre del item", 255, 255, 255);
			PutAlignedString(sX + 153, sX + 250, sY + 55, "Valor", 255, 255, 255);
			PutAlignedString(sX + 154, sX + 251, sY + 55, "Valor", 255, 255, 255);
		}

		if (iTotalLines > maxlines)
		{
			d1 = static_cast<double>(m_stDialogBoxInfo[dnumber].sView);
			d2 = static_cast<double>(iTotalLines - maxlines);
			d3 = (275.0f * d1) / d2;
			iPointerLoc = static_cast<int>(d3 + 0.5);
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + sizeX - 13, sY + 44 + iPointerLoc, 7);
		}
		else
		{
			iPointerLoc = 0;
		}


		int tempindex = -1;

		for (int i = 0; i < maxlines; i++)
		{
			if ((i + m_stDialogBoxInfo[dnumber].sView) < iTotalLines)
			{
				char cTemp[512] = {};
				strcpy(cTemp, vec_trades[i + m_stDialogBoxInfo[dnumber].sView].itemname);


				char cTxt[120], cTxt3[50];
				char cNameStr[120], cSubStr1[120], cSubStr2[120];
				GetItemName(cTemp, 0, cNameStr, cSubStr1, cSubStr2, 0);
				wsprintf(cTxt, "%s", cNameStr);


				if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
				{
					PutAlignedString(sX + 10, sX + 190, sY + i * 18 + 75, cNameStr, 255, 255, 255);
					PutAlignedString(sX + 148, sX + 260, sY + i * 18 + 75, (char*)std::to_string(vec_trades[i + m_stDialogBoxInfo[dnumber].sView].cost).c_str(), 255, 255, 255);
				}
				else
				{
					PutAlignedString(sX + 10, sX + 190, sY + i * 18 + 75, cNameStr, 255, 200, 0);
					PutAlignedString(sX + 148, sX + 260, sY + i * 18 + 75, (char*)std::to_string(vec_trades[i + m_stDialogBoxInfo[dnumber].sView].cost).c_str(), 255, 200, 0);
				}
			}
		}

		if (cLB != 0 && iTotalLines > maxlines)
		{
			if (iGetTopDialogBoxIndex() == dnumber)
			{
				if ((msX >= sX + sizeX - 13) && (msX <= sX + sizeX + 12) && (msY >= sY + 44) && (msY <= sY + 374))
				{
					d1 = static_cast<double>(msY - (sY + 44));
					d2 = static_cast<double>(iTotalLines - maxlines);
					d3 = (d1 * d2) / 275.0f;
					m_stDialogBoxInfo[dnumber].sView = static_cast<int>(d3 + 0.5);
				}
			}
		}
		else
		{
			m_stDialogBoxInfo[dnumber].bIsScrollSelected = FALSE;
		}

		if (iGetTopDialogBoxIndex() == dnumber && msZ != 0)
		{
			m_stDialogBoxInfo[dnumber].sView -= msZ / 120; 
			if (m_stDialogBoxInfo[dnumber].sView < 0)
			{
				m_stDialogBoxInfo[dnumber].sView = 0;
			}
			if (m_stDialogBoxInfo[dnumber].sView > iTotalLines - maxlines)
			{
				m_stDialogBoxInfo[dnumber].sView = iTotalLines - maxlines;
			}
			m_DInput.m_sZ = 0; 
		}

		if (iTotalLines > maxlines && m_stDialogBoxInfo[dnumber].sView > iTotalLines - maxlines)
		{
			m_stDialogBoxInfo[dnumber].sView = iTotalLines - maxlines;
		}
		if (m_stDialogBoxInfo[dnumber].sView < 0 || iTotalLines <= maxlines)
		{
			m_stDialogBoxInfo[dnumber].sView = 0;
		}

		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Back");

		break;
	}
	case 1:
	{	
		char cTemp[255];
		auto dwTime = timeGetTime();		
		
		auto index = tmptrade;

		string reward = vec_trades[index].itemname;
		string contrib_text = "Contrib+";
		string maj_text = "Majestic+";
		string rep_text = "Rep+";

		if (startsWith(reward, contrib_text))
		{
			m_stDialogBoxInfo[dnumber].sV3 = 1;
		}
		else if (startsWith(reward, maj_text))
		{
				m_stDialogBoxInfo[dnumber].sV3 = 1;
		}
		else if (startsWith(reward, rep_text))
		{
			m_stDialogBoxInfo[dnumber].sV3 = 1;
		}
		else
		{
			m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->PutSpriteFast(sX + 156, sY + 219, 19, dwTime);
			m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->PutSpriteFast(sX + 170, sY + 219, 19, dwTime);
			PutString2(sX + 80, sY + 235 - 10, DRAW_DIALOGBOX_SHOP27, 228, 161, 6); // "Quantity:"

			if (iGetTopDialogBoxIndex() == dnumber && msZ != 0)
			{
				if (msZ > 0) {
					m_stDialogBoxInfo[dnumber].sV3 += 1;
				}
				else if (msZ < 0) {
					m_stDialogBoxInfo[dnumber].sV3 -= 1;
				}

			}

			if (m_stDialogBoxInfo[dnumber].sV3 > (50 - _iGetTotalItemNum()))
				m_stDialogBoxInfo[dnumber].sV3 = (50 - _iGetTotalItemNum());

			if (m_stDialogBoxInfo[dnumber].sV3 < 1)
				m_stDialogBoxInfo[dnumber].sV3 = 1;

			if (m_stDialogBoxInfo[dnumber].sV3 >= 10)
			{
				ZeroMemory(cTemp, sizeof(cTemp));
				_itoa(m_stDialogBoxInfo[dnumber].sV3, cTemp, 10);
				cTemp[1] = NULL;
				PutString2(sX - 35 + 186, sY - 10 + 235, cTemp, 228, 161, 6);
				ZeroMemory(cTemp, sizeof(cTemp));
				_itoa(m_stDialogBoxInfo[dnumber].sV3, cTemp, 10);
				PutString2(sX - 35 + 200, sY - 10 + 235, (cTemp + 1), 228, 161, 6);
			}
			else
			{
				PutString2(sX - 35 + 186, sY - 10 + 235, "0", 228, 161, 6);
				ZeroMemory(cTemp, sizeof(cTemp));
				_itoa(m_stDialogBoxInfo[dnumber].sV3, cTemp, 10);
				PutString2(sX - 35 + 200, sY - 10 + 235, (cTemp), 228, 161, 6);
			}
			m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->PutSpriteFast(sX + 156, sY + 244, 20, dwTime);
			m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->PutSpriteFast(sX + 170, sY + 244, 20, dwTime);
		}
		
		auto quant = m_stDialogBoxInfo[dnumber].sV3;
		auto icost = vec_trades[index].cost * quant;
		string sitem = vec_trades[index].itemname;
		char * item = (char*)sitem.c_str();
		string scost = "Value: ";

		scost.append(to_string(icost));
		
		switch (trade_mode)
		{
		case DEF_TRADEEK:
		{			
			scost.append(" (Enemy Kills)");
			break;
		}
		case DEF_TRADECOIN:
		{			
			scost.append(" (Coins)");
			
			break;
		}
		case DEF_TRADECONTRIB:
		{
			scost.append(" (Contribution)");
			break;
		}
		case DEF_TRADEMAJS:
		{
			scost.append(" (Majestics)");
			break;
		}
		default:

			break;
		}

		char * cost = (char*)scost.c_str();		

		PutAlignedString(sX + 60, sX + 200, sY + 60, item, 255, 255, 255);
		PutAlignedString(sX + 60, sX + 200, sY + 180, cost, 255, 255, 255);

		if (startsWith(reward, contrib_text))
		{
		}
		else if (startsWith(reward, maj_text))
		{
		}
		else
		{
			if (vec_trades[index].itemcolor != 0)
				m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + vec_trades[index].sprite]->PutSpriteRGB(sX + 130, sY + 133, vec_trades[index].spriteframe, m_wWR[vec_trades[index].itemcolor] - m_wR[0], m_wWG[vec_trades[index].itemcolor] - m_wG[0], m_wWB[vec_trades[index].itemcolor] - m_wB[0], dwTime);
			else {
				if (string(vec_trades[index].itemname) == "PandoraBox(Small)" ||
					string(vec_trades[index].itemname) == "PandoraBox(Medium)" ||
					string(vec_trades[index].itemname) == "PandoraBox(Big)")
					m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + vec_trades[index].sprite]->PutSpriteFast(sX + 130 - 30, sY + 133 - 40, vec_trades[index].spriteframe, dwTime);
				else m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + vec_trades[index].sprite]->PutSpriteFast(sX + 130, sY + 133, vec_trades[index].spriteframe, dwTime);
			}
		}
		
		PutAlignedString(sX, sX + sizeX, sY + 262, DRAW_DIALOGBOX_CITYHALL_MENU46A, 255, 255, 255);
		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY, "Yes");
		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "No");
		DrawDialogCord(sX, sY, msX, msY);

		break;
	}

	default:
		break;
	}
}

void CGame::click_trades(short msX, short msY)
{
	short sX, sY, sizeX;
	int dnumber = 79;
	int maxlines = 18;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;

	switch (mode)
	{
	case 0:
	{

		if (trade_mode == DEF_NOTSELECTED)
		{
			if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 60) && (msY <= sY + 60 + 15))
			{
				bSendCommand(CLIENT_REQUEST_FULLDATACHAR, NULL, NULL, NULL, NULL, NULL, NULL);
				trade_mode = DEF_TRADEEK;
				SendChat("/tradeek");
				PlaySound('E', 14, 5);
				return;
			}

			if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 80) && (msY <= sY + 80 + 15))
			{
				bSendCommand(CLIENT_REQUEST_FULLDATACHAR, NULL, NULL, NULL, NULL, NULL, NULL);
				trade_mode = DEF_TRADECOIN;
				SendChat("/tradecoin");
				PlaySound('E', 14, 5);
				return;
			}


			if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 100) && (msY <= sY + 100 + 15))
			{
				bSendCommand(CLIENT_REQUEST_FULLDATACHAR, NULL, NULL, NULL, NULL, NULL, NULL);
				trade_mode = DEF_TRADECONTRIB;
				SendChat("/tradecontrib");
				PlaySound('E', 14, 5);
				return;
			}

			//add
			if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 120) && (msY <= sY + 120 + 15))
			{
				bSendCommand(CLIENT_REQUEST_FULLDATACHAR, NULL, NULL, NULL, NULL, NULL, NULL);
				trade_mode = DEF_TRADEMAJS;
				SendChat("/trademajs");
				PlaySound('E', 14, 5);
				return;
			}

			if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 140) && (msY <= sY + 140 + 15))
			{
				trade_mode = DEF_TRADEBALLS;
				PlaySound('E', 14, 5);
				return;
			}

			return;
		}


	if (trade_mode == DEF_TRADEBALLS)
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Trade Balls", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Trade Balls", 250, 250, 250);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 60) && (msY <= sY + 60 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 1;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}
			
		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 80) && (msY <= sY + 80 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 2;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}
			
		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 100) && (msY <= sY + 100 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 3;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}
			
		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 120) && (msY <= sY + 120 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 4;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}

		auto back = (msX >= sX + 153) && (msX <= sX + 230) && (msY >= sY + 293) && (msY <= sY + 145 + 313);
		if (back)
		{
			trade_mode = DEF_NOTSELECTED;
			PlaySound('E', 14, 5);
		}
	}

		for (int i = 0; i < 11; i++)
		if (((i + m_stDialogBoxInfo[79].sView) < vec_trades.size()))
		{
			auto ptr = &vec_trades[i + m_stDialogBoxInfo[79].sView];
			if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
			{
				tmptrade = i + m_stDialogBoxInfo[79].sView;
				m_stDialogBoxInfo[79].cMode = 1;
				PlaySound('E', 14, 5);
			}
		}

		if (trade_mode != DEF_NOTSELECTED)
		{
			auto back = (msX >= sX + 153) && (msX <= sX + 230) && (msY >= sY + 293) && (msY <= sY + 145 + 313);
			if (back)
			{
				trade_mode = DEF_NOTSELECTED;
				PlaySound('E', 14, 5);
			}
		}
		
		break;
	}

	case 1:
	{

		auto yes = (msX >= sX + 30) && (msX <= sX + 105) && (msY >= sY + 293) && (msY <= sY + 145 + 313);
		auto no = (msX >= sX + 153) && (msX <= sX + 230) && (msY >= sY + 293) && (msY <= sY + 145 + 313);

		if (yes)
		{
			tmpquant = m_stDialogBoxInfo[dnumber].sV3;

			char data[256];
			auto * cp = data;
			
			Push(cp, (u32)REQ_TRADE);
			Push(cp, (u16)0);

			Push(cp, trade_mode);
			Push(cp, tmptrade);
			Push(cp, tmpquant);

			m_pGSock->iSendMsg(data, cp - data);

			DisableDialogBox(79);
			PlaySound('E', 14, 5);
		}

		if (no)
		{
			m_stDialogBoxInfo[79].cMode = 0;
			PlaySound('E', 14, 5);
		}

		break;
	}
	default:
		break;
	}
}


void CGame::DrawShadowMenu_Borders2(short sX, short sY, short szX, short szY)
{
	return;
	DrawShadowPixels2(1, szX, sX, sY);
	DrawShadowPixels2(1, szX, sX, sY + szY);
	DrawShadowPixels2(2, szY, sX, sY);
	DrawShadowPixels2(2, szY + 1, sX + szX, sY);
}


// draw pixels
void CGame::DrawShadowPixels2(int mode, int count, short x, short y)
{
	switch (mode)
	{
	case 1:
	{
		for (int d = 0; d < count; d++)
		{
			m_DDraw.PutPixel(x + (d), y, 78, 45, 14);
		}
		break;
	}

	case 2:
	{
		for (int d = 0; d < count; d++)
		{
			m_DDraw.PutPixel(x, y + (d), 78, 45, 14);
		}
		break;
	}
	}
}


void CGame::get_trade_ek(char * data)
{
	vec_trades.clear();
	int size = 0;
	Pop(data, size);

	for (int i = 0; i < size; i++)
	{
		st_trade_ek u;
		ZeroMemory(u.itemname, sizeof(u.itemname));
		Pop(data, u.itemname, 21);
		Pop(data, u.cost);
		Pop(data, u.sprite);
		Pop(data, u.spriteframe);
		Pop(data, u.itemcolor);
		vec_trades.push_back(u);
	}
}

void CGame::get_trade_contrib(char * data)
{
	vec_trades.clear();
	int size = 0;
	Pop(data, size);

	for (int i = 0; i < size; i++)
	{
		st_trade_ek u;
		ZeroMemory(u.itemname, sizeof(u.itemname));
		Pop(data, u.itemname, 21);
		Pop(data, u.cost);
		Pop(data, u.sprite);
		Pop(data, u.spriteframe);
		Pop(data, u.itemcolor);
		vec_trades.push_back(u);
	}
}


void CGame::get_trade_coin(char * data)
{
	vec_trades.clear();
	int size = 0;
	Pop(data, size);

	for (int i = 0; i < size; i++)
	{
		st_trade_ek u;
		ZeroMemory(u.itemname, sizeof(u.itemname));
		Pop(data, u.itemname, 21);
		Pop(data, u.cost);
		Pop(data, u.sprite);
		Pop(data, u.spriteframe);
		Pop(data, u.itemcolor);
		vec_trades.push_back(u);
	}
}

void CGame::get_trade_majs(char * data)
{
	vec_trades.clear();
	int size = 0;
	Pop(data, size);

	for (int i = 0; i < size; i++)
	{
		st_trade_ek u;
		ZeroMemory(u.itemname, sizeof(u.itemname));
		Pop(data, u.itemname, 21);
		Pop(data, u.cost);
		Pop(data, u.sprite);
		Pop(data, u.spriteframe);
		Pop(data, u.itemcolor);
		vec_trades.push_back(u);
	}
}

void CGame::DrawShadowPixels(int mode, int count, short x, short y)
{
	switch (mode)
	{
	case 1:
	{
		for (int d = 0; d < count; d++)
		{
			m_DDraw.PutPixel(x + (d), y, 73, 58, 42);
		}
		break;
	}

	case 2:
	{
		for (int d = 0; d < count; d++)
		{
			m_DDraw.PutPixel(x, y + (d), 73, 58, 42);
		}
		break;
	}
	}
}

void CGame::DrawMenu_TextBox(short sX, short sY, short x, short y)
{
	DrawShadowPixels(1, x, sX, sY);
	DrawShadowPixels(1, x, sX, sY + y);
	DrawShadowPixels(2, y, sX, sY);
	DrawShadowPixels(2, y + 1, sX + x, sY);
}

void CGame::draw_confirm(short msX, short msY, char cLB)
{
	short sX, sY, sizeX, sizeY;
	int dnumber = 80;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;

	DrawShadow_Box(sX, sY, sX + sizeX, sY + sizeY);
	DrawShadowMenu_DialogBorders(sX, sY, sizeX, sizeY);

	PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Confirm Operation", 230, 230, 255);
	
	switch (confirmoperation)
	{

	case 1:
	{
		DrawMenu_TextBox(sX + 65, sY + 45, 120, 22);
		PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Contribution Ball", 230, 230, 255);

		PutString2(sX + 70, sY + 50, m_cAmmount, 230, 230, 255);

		DrawMenu_SmallButton(sX, sY, msX, msY, 15, 90, "Get");
		DrawMenu_SmallButton(sX, sY, msX, msY, 163, 90, "Cancel");
		break;
	}
	case 2:
	{
		DrawMenu_TextBox(sX + 65, sY + 45, 120, 22);
		PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Coin Ball", 230, 230, 255);

		PutString2(sX + 70, sY + 50, m_cAmmount, 230, 230, 255);

		DrawMenu_SmallButton(sX, sY, msX, msY, 15, 90, "Get");
		DrawMenu_SmallButton(sX, sY, msX, msY, 163, 90, "Cancel");
		break;
	}
	case 3:
	{
		DrawMenu_TextBox(sX + 65, sY + 45, 120, 22);
		PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Reputation Ball", 230, 230, 255);

		PutString2(sX + 70, sY + 50, m_cAmmount, 230, 230, 255);

		DrawMenu_SmallButton(sX, sY, msX, msY, 15, 90, "Get");
		DrawMenu_SmallButton(sX, sY, msX, msY, 163, 90, "Cancel");
		break;
	}
	case 4:
	{
		DrawMenu_TextBox(sX + 65, sY + 45, 120, 22);
		PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Majestic Ball", 230, 230, 255);

		PutString2(sX + 70, sY + 50, m_cAmmount, 230, 230, 255);

		DrawMenu_SmallButton(sX, sY, msX, msY, 15, 90, "Get");
		DrawMenu_SmallButton(sX, sY, msX, msY, 163, 90, "Cancel");
		break;
	}
	default: break;
	}

	DrawDialogCord(sX, sY, msX, msY);

}

void CGame::click_confirm(short msX, short msY)
{
	short sX, sY, sizeX, sizeY;
	int dnumber = 80;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;

	switch (confirmoperation)
	{
	case 1:
	{
		auto strinput = (msX >= sX + 65) && (msX <= sX + 65 + 120) && (msY >= sY + 45) && (msY <= sY + 65);
		if (strinput)
		{
			StartInputString(m_stDialogBoxInfo[dnumber].sX + 70, m_stDialogBoxInfo[dnumber].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
			return;
		}

		auto give = (msX >= sX + 15) && (msX <= sX + 90) && (msY >= sY + 90) && (msY <= sY + 110);
		auto cancel = (msX >= sX + 163) && (msX <= sX + 239) && (msY >= sY + 90) && (msY <= sY + 110);

		if (give)
		{
			EndInputString();

			if (!isValidAmount(m_cAmmount))
			{
				AddEventList("Error. Invalid Contribution amount.");
				return;
			}

			int quant = atoi(m_cAmmount);

			char data[256];
			auto * cp = data;

			Push(cp, (u32)REQ_TRADE_BALL);
			Push(cp, (u16)0);

			Push(cp, (short)BALL_CONTRIB);
			Push(cp, quant);

			m_pGSock->iSendMsg(data, cp - data);

			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}

		if (cancel)
		{
			EndInputString();
			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}
		break;
	}
	case 2:
	{
		auto strinput = (msX >= sX + 65) && (msX <= sX + 65 + 120) && (msY >= sY + 45) && (msY <= sY + 65);
		if (strinput)
		{
			StartInputString(m_stDialogBoxInfo[dnumber].sX + 70, m_stDialogBoxInfo[dnumber].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
			return;
		}

		auto give = (msX >= sX + 15) && (msX <= sX + 90) && (msY >= sY + 90) && (msY <= sY + 110);
		auto cancel = (msX >= sX + 163) && (msX <= sX + 239) && (msY >= sY + 90) && (msY <= sY + 110);

		if (give)
		{
			EndInputString();

			if (!isValidAmount(m_cAmmount))
			{
				AddEventList("Error. Invalid Contribution amount.");
				return;
			}

			int quant = atoi(m_cAmmount);

			char data[256];
			auto * cp = data;

			Push(cp, (u32)REQ_TRADE_BALL);
			Push(cp, (u16)0);

			Push(cp, (short)BALL_COIN);
			Push(cp, quant);

			m_pGSock->iSendMsg(data, cp - data);

			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}

		if (cancel)
		{
			EndInputString();
			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}
		break;
	}

	case 3:
	{
		auto strinput = (msX >= sX + 65) && (msX <= sX + 65 + 120) && (msY >= sY + 45) && (msY <= sY + 65);
		if (strinput)
		{
			StartInputString(m_stDialogBoxInfo[dnumber].sX + 70, m_stDialogBoxInfo[dnumber].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
			return;
		}

		auto give = (msX >= sX + 15) && (msX <= sX + 90) && (msY >= sY + 90) && (msY <= sY + 110);
		auto cancel = (msX >= sX + 163) && (msX <= sX + 239) && (msY >= sY + 90) && (msY <= sY + 110);

		if (give)
		{
			EndInputString();

			if (!isValidAmount(m_cAmmount))
			{
				AddEventList("Error. Invalid Contribution amount.");
				return;
			}

			int quant = atoi(m_cAmmount);

			char data[256];
			auto * cp = data;

			Push(cp, (u32)REQ_TRADE_BALL);
			Push(cp, (u16)0);

			Push(cp, (short)BALL_REP);
			Push(cp, quant);

			m_pGSock->iSendMsg(data, cp - data);

			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}

		if (cancel)
		{
			EndInputString();
			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}
		break;
	}
	case 4:
	{
		auto strinput = (msX >= sX + 65) && (msX <= sX + 65 + 120) && (msY >= sY + 45) && (msY <= sY + 65);
		if (strinput)
		{
			StartInputString(m_stDialogBoxInfo[dnumber].sX + 70, m_stDialogBoxInfo[dnumber].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
			return;
		}

		auto give = (msX >= sX + 15) && (msX <= sX + 90) && (msY >= sY + 90) && (msY <= sY + 110);
		auto cancel = (msX >= sX + 163) && (msX <= sX + 239) && (msY >= sY + 90) && (msY <= sY + 110);

		if (give)
		{
			EndInputString();

			if (!isValidAmount(m_cAmmount))
			{
				AddEventList("Error. Invalid Contribution amount.");
				return;
			}

			int quant = atoi(m_cAmmount);

			char data[256];
			auto * cp = data;

			Push(cp, (u32)REQ_TRADE_BALL);
			Push(cp, (u16)0);

			Push(cp, (short)BALL_MAJS);
			Push(cp, quant);

			m_pGSock->iSendMsg(data, cp - data);

			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}

		if (cancel)
		{
			EndInputString();
			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}
		break;
	}

	default: break;
	}
}

bool CGame::isValidAmount(const std::string& str) {
	// La cadena vacía no es un número válido
	if (str.empty()) {
		return false;
	}

	if (string(str) == "0") return false;

	// Comprobar que la longitud no sea mayor a 8
	if (str.length() > 8) {
		return false;
	}

	// Comprobar que todos los caracteres son dígitos
	for (char c : str) {
		if (!std::isdigit(c)) {
			return false;
		}
	}

	// Comprobar que no tenga ceros a la izquierda
	if (str.size() > 1 && str[0] == '0') {
		return false;
	}

	return true;
}

double CGame::calculatePing() {
	std::chrono::duration<double, std::milli> elapsed = endTime - startTime;
	return elapsed.count(); // Retorna el tiempo en milisegundos
}

void CGame::startUDPClient(int port) {
	initializeWinsock();
	createSocket();
	setupServerAddress(DEF_SERVER_IP, port);
}

void CGame::destroyUDPClient() {
	closesocket(clientSocket);
	WSACleanup();
}

void CGame::initializeWinsock() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "Failed to initialize Winsock." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void CGame::setupServerAddress(const std::string& serverIp, int port) {
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
	serverAddr.sin_port = htons(port);
}

void CGame::createSocket() {
	clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "Failed to create socket." << std::endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
}

void CGame::sendMessageToServer(int msgid) {
	return;
	char data[500];
	char* cp = data;

	bool bsend = false;

	Push(cp, msgid);
	switch (msgid)
	{
	case SOCKMSG_WELCOME:
	{
		char message[5] = "hola";
		Push(cp, message, 5); // Pasa el tamaño real de la cadena
		bsend = true;
		break;
	}
	case SOCKMSG_PING:
	{
		startTime = std::chrono::high_resolution_clock::now();
		bsend = true;
		break;
	}
	}

	if (!bsend) return;

	// Calcular el tamaño total del mensaje
	int totalSize = cp - data;

	// Enviar el mensaje
	int bytesSent = sendto(clientSocket, data, totalSize, 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if (bytesSent == SOCKET_ERROR) {
		std::cerr << "Error sending custom message to client." << std::endl;
	}
	else {
		std::cout << "Custom message sent to client: Size = " << totalSize << std::endl;
	}
}

void CGame::receiveMessageFromServer()
{
	while (true) {
		char buffer[500];
		sockaddr_in fromAddr;
		int fromAddrLen = sizeof(fromAddr);
		
		int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (sockaddr*)&fromAddr, &fromAddrLen);
		if (bytesReceived == SOCKET_ERROR) {
			continue;
		}

		char* cp = buffer;

		int msgid;
		Pop(cp, msgid);

		switch (msgid)
		{
		case SOCKMSG_WELCOME:
		{
			char test3[4];
			Pop(cp, test3, sizeof(test3));
			break;
		}
		case SOCKMSG_PING:
		{
			endTime = std::chrono::high_resolution_clock::now();
			break;
		}
		case SOCKMSG_OBJECTCOMMAND:
		{/*
			short owner_type;
			short command;
			Pop(cp, owner_type);
			Pop(cp, command);
			
			if (owner_type == DEF_OWNERTYPE_NPC && command == ACTION_OBJECTMOVE)
			{
				int owner;
				short sx;
				short sy;
				short type;
				char dir;
				char cName[5];
				short Appr2;
				int status;
				char killed;
				short sAppr1, sAppr3, sAppr4, sV1, sV2, sV3, sPrevAppr2;
				int iApprColor, iApprColor2, iLoc;
				WORD wEventType;
				char AdminLevel;

				Pop(cp, owner);
				Pop(cp, sx);
				Pop(cp, sy);
				Pop(cp, type);
				Pop(cp, dir);
				Pop(cp, cName, 5);
				Pop(cp, Appr2);
				Pop(cp, status);
				Pop(cp, killed);


				m_pMapData->bSetOwner(owner, sx, sy, type, dir, sAppr1, Appr2, sAppr3, sAppr4, iApprColor, iApprColor2, status, cName, (char)wEventType, sV1, sV2, sV3, iLoc, 0, AdminLevel);

			}*/

			break;
		}
		default:
			break;
		}
	}
}

void CGame::request_ping()
{
	char data[10];
	auto * cp = data;

	Push(cp, (u32)NOTIFY_PING);
	Push(cp, (u16)0);

	m_pGSock->iSendMsg(data, cp - data);
}

/*


int CGame::UDPrequest_ping() {
	while (running) {

		char buffer[UDP_BUFFER_SIZE];
		int serverAddrSize = sizeof(serverAddr);

		// Inicializar Winsock
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			std::cerr << "Error al iniciar Winsock" << std::endl;
			return -1;
		}

		// Crear socket
		clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if (clientSocket == INVALID_SOCKET) {
			std::cerr << "Error al crear socket" << std::endl;
			WSACleanup();
			return -1;
		}

		// Configurar dirección del servidor
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
		serverAddr.sin_port = htons(SERVER_PORT);

		// Preparar mensaje
		buffer[0] = 'P'; // Mensaje mínimo (1 byte)

		// Registrar tiempo de envío
		auto startTime = std::chrono::high_resolution_clock::now();

		// Enviar mensaje
		if (sendto(clientSocket, buffer, UDP_BUFFER_SIZE, 0, (sockaddr*)&serverAddr, serverAddrSize) == SOCKET_ERROR) {
			std::cerr << "Error al enviar mensaje" << std::endl;
			closesocket(clientSocket);
			WSACleanup();
			return -1;
		}

		// Recibir respuesta
		int receivedBytes = recvfrom(clientSocket, buffer, UDP_BUFFER_SIZE, 0, (sockaddr*)&serverAddr, &serverAddrSize);
		if (receivedBytes == SOCKET_ERROR) {
			std::cerr << "Error al recibir respuesta" << std::endl;
		}
		else {
			auto endTime = std::chrono::high_resolution_clock::now();
			// Calcular ping
			auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime); // Usar 'double' para precisión decimal
			m_dPingSocket = duration.count() * 1000.0; // Convertir a milisegundos

			// Mostrar ping con 2 decimales
			std::cout << "Ping: " << std::fixed << std::setprecision(2) << m_dPingSocket << " ms" << std::endl;
		}

		closesocket(clientSocket);
		WSACleanup();

		if (!running) break;
		std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Esperar antes de enviar de nuevo
	}

	return 0;
}
*/
