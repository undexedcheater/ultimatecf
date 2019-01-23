/*********************************************************/
/* CA Base D3D Menu Selection V5
/* Creditos:
/*	Solify's	-> CA Base D3D Menu Selection.
/*  WE11ington  -> Disponibilizou Para Combat Arms Brasil.
/*  luizimloko  -> Menu Selection V5.
/*********************************************************/
#include "includes.h"

//Chaves do menu
const CHAR *OnOff[] = { "0", "1" };
const CHAR *Folder[] = { ">>", "<<" };

//Menu
CHAR MenuTitle[MAX_PATH] = { "Menu Selection V5" }; //Título do menu aqui.
CHAR Timestruct[MAX_PATH] = { "hh':'mm':'ss tt" }; //Estrutura do relógio.
CHAR TimeString[MAX_PATH];

int Variable[99] = { 0 };
int VFolder[10] = { 0 };

#define Red       D3DCOLOR_ARGB(255, 255, 000, 000)
#define Green     D3DCOLOR_ARGB(255, 127, 255, 000)
#define Orange    D3DCOLOR_ARGB(255, 255, 140, 000)
#define Blue      D3DCOLOR_ARGB(255, 000, 000, 255)
#define Yellow    D3DCOLOR_ARGB(255, 255, 255,  51)
#define Black     D3DCOLOR_ARGB(255, 000, 000, 000)
#define Grey      D3DCOLOR_ARGB(255, 112, 112, 112)
#define Gold      D3DCOLOR_ARGB(255, 255, 215, 000)
#define Pink      D3DCOLOR_ARGB(255, 255, 192, 203)
#define Purple    D3DCOLOR_ARGB(255, 128, 000, 128)
#define White     D3DCOLOR_ARGB(255, 255, 255, 249)
#define Cyan      D3DCOLOR_ARGB(255, 000, 255, 255)
#define Magenta   D3DCOLOR_ARGB(255, 255, 000, 255)
#define TBlack    D3DCOLOR_ARGB(128, 000, 000, 000)

CMenu menu;

namespace Menu
{
	VOID Additem(const CHAR *String, INT MaxValue, INT &Variable, INT Folder, const CHAR **Option)
	{
		if (menu.ShowMenu)
		{
			if (menu.MenuSelection == menu.Current)
			{
				if (GetAsyncKeyState(VK_RIGHT) & 1)
				{
					if (Variable < MaxValue)
						Variable++;
				}
				else if (GetAsyncKeyState(VK_LEFT) & 1)
				{
					if (Variable > 0)
						Variable--;
				}
			}

			if (menu.MenuSelection == menu.Current)
			{
				pRenderer->String(menu.matrixPos[0] + 2, menu.matrixPos[2] + (menu.Current * 15), D3DCOLOR_XRGB(200, 200, 200), g_pMainFont, true, String);
				pRenderer->String(menu.matrixPos[0] + 125, menu.matrixPos[2] + (menu.Current * 15), D3DCOLOR_XRGB(200, 200, 200), g_pMainFont, true, Option[Variable]);
			}
			else if (Variable > 0)
			{
				if (Folder == 1)
				{
					pRenderer->String(menu.matrixPos[0] - 3, menu.matrixPos[2] + (menu.Current * 15), White, g_pMainFont, true, String);
					pRenderer->String(menu.matrixPos[0] + 125, menu.matrixPos[2] + (menu.Current * 15), Red, g_pMainFont, true, Option[Variable]);
				}
				else
				{
					pRenderer->String(menu.matrixPos[0] + 5, menu.matrixPos[2] + (menu.Current * 15), Red, g_pMainFont, true, String);
					pRenderer->String(menu.matrixPos[0] + 125, menu.matrixPos[2] + (menu.Current * 15), White, g_pMainFont, true, Option[Variable]);
				}
			}
			else
			{
				if (Folder == 1)
				{
					pRenderer->String(menu.matrixPos[0] - 3, menu.matrixPos[2] + (menu.Current * 15), White, g_pMainFont, true, String);
					pRenderer->String(menu.matrixPos[0] + 125, menu.matrixPos[2] + (menu.Current * 15), Red, g_pMainFont, true, Option[Variable]);
				}
				else
				{
					pRenderer->String(menu.matrixPos[0] + 5, menu.matrixPos[2] + (menu.Current * 15), Red, g_pMainFont, true, String);
					pRenderer->String(menu.matrixPos[0] + 125, menu.matrixPos[2] + (menu.Current * 15), White, g_pMainFont, true, Option[Variable]);
				}
			}

			menu.Current++;
		}
	}

	void Update(IDirect3DDevice9* pDevice)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1)
			menu.ShowMenu = !menu.ShowMenu;

		if (menu.ShowMenu)
		{
			//main box
			pRenderer->FilledBox(menu.matrixPos[0] - 10, menu.matrixPos[1] - 22, 162, ((menu.Current * 15) + 15), TBlack);
			//menu title
			pRenderer->String((menu.matrixPos[0]), menu.matrixPos[1] - 20, White, g_pMainFont, true, MenuTitle);

			//border around the menu
			pRenderer->BorderedBox(menu.matrixPos[0] - 10, menu.matrixPos[1] - 23, 162, ((menu.Current * 15) - 5), 1, Black); 
			//selection border
			pRenderer->BorderedBox(menu.matrixPos[0] - 10, ((menu.matrixPos[2] + 2) + (menu.MenuSelection * 15)) - 1, 162, 13, 1, D3DCOLOR_XRGB(200, 200, 200));

			menu.Current = 1;

			if (GetAsyncKeyState(VK_UP) & 1)
				menu.MenuSelection--;

			if (GetAsyncKeyState(VK_DOWN) & 1)
				menu.MenuSelection++;

			Additem("WEAPON", 1, VFolder[0], 1, Folder);
			if (VFolder[0])
			{
				Additem("less recoil", 1, Variable[0], 0, OnOff);
				Additem("no reload", 1, Variable[1], 0, OnOff);
				Additem("no wep switch delay", 1, Variable[2], 0, OnOff);
				Additem("infinte range", 1, Variable[3], 0, OnOff);
			}

			if (menu.MenuSelection >= menu.Current)
				menu.MenuSelection = 1;
			else if (menu.MenuSelection <= 0)
				menu.MenuSelection = (menu.Current - 1);
		}
	}
};