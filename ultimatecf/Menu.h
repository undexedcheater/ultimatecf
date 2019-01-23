/*********************************************************/
/* CA Base D3D Menu Selection V5
/* Creditos:
/*	Solify's	-> CA Base D3D Menu Selection.
/*  WE11ington  -> Disponibilizou Para Combat Arms Brasil.
/*  luizimloko  -> Menu Selection V5.
/*********************************************************/
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

extern ID3DXFont* g_pMainFont;

struct CMenu
{
	INT MenuSelection,
		matrixPos[ 3 ],
		Current;

	BOOL ShowMenu;

	CMenu( VOID )
	{
		ShowMenu = 1;
		Current = 1;

		matrixPos[ 0 ] = 30; //x1
		matrixPos[ 1 ] = 70; //y1
		matrixPos[ 2 ] = 60; //y2
		matrixPos[ 3 ] = 00; //x2
	}
};

extern CMenu menu;

namespace Menu
{
	VOID Additem(const CHAR *String, INT MaxValue, INT &Variable, INT Folder, const CHAR **Option);
	void Update(IDirect3DDevice9* pDevice);
}