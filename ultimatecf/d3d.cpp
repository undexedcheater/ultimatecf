#include "includes.h"

#define D3DX_PI (float)(3.14159265358979323846)

std::unique_ptr< CRender2D > pRenderer = nullptr;

struct D3DVERTEX
{
	float x, y, z, rhw;
	DWORD color;
};

void CRender2D::SetDevice(LPDIRECT3DDEVICE9 devIn)
{
	pDevice = devIn;
}

void CRender2D::String(float x, float y, D3DCOLOR colour, ID3DXFont* font, bool outlined, const char* string, ...)
{
	va_list args;
	char cBuffer[256];

	va_start(args, string);
	vsprintf_s(cBuffer, string, args);
	va_end(args);

	RECT rc = { 0 };
	auto len = strlen(cBuffer);

	if (outlined)
	{
		SetRect(&rc, x + 1, y, x, y);
		font->DrawTextA(0, cBuffer, len, &rc, DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
		SetRect(&rc, x, y - 1, x, y);
		font->DrawTextA(0, cBuffer, len, &rc, DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
		SetRect(&rc, x, y, x + 1, y);
		font->DrawTextA(0, cBuffer, len, &rc, DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
		SetRect(&rc, x, y, x, y - 1);
		font->DrawTextA(0, cBuffer, len, &rc, DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

		SetRect(&rc, x, y, x, y);
		font->DrawTextA(0, cBuffer, len, &rc, DT_NOCLIP, colour);
		return;
	}

	SetRect(&rc, x, y, x, y);
	font->DrawTextA(0, cBuffer, len, &rc, DT_NOCLIP, colour);
}

int CRender2D::StringWidth(ID3DXFont* font, const char* string)
{
	RECT rc = { 0 };
	font->DrawTextA(0, string, strlen(string), &rc, DT_CALCRECT, D3DCOLOR_ARGB(0, 0, 0, 0));

	return (int)(rc.right / 2);
}

void CRender2D::Line(float x, float y, float x2, float y2, D3DCOLOR colour)
{
	D3DVERTEX pVertex[2] = { { x, y, 0.0f, 1.0f, colour }, { x2, y2, 0.0f, 1.0f, colour } };
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, &pVertex, sizeof(D3DVERTEX));
}

void CRender2D::FilledBox(float x, float y, float width, float height, D3DCOLOR colour)
{
	D3DVERTEX pVertex[4] = { { x, y + height, 0.0f, 1.0f, colour }, { x, y, 0.0f, 1.0f, colour }, { x + width, y + height, 0.0f, 1.0f, colour }, { x + width, y, 0.0f, 1.0f, colour } };
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pVertex, sizeof(D3DVERTEX));
}

void CRender2D::FilledBoxOutlined(int x, int y, int width, int height, int thickness, D3DCOLOR colour, D3DCOLOR outlinecolor)
{
	this->BorderedBox(x, y, width, height, thickness, outlinecolor);
	this->FilledBox(x + thickness, y + thickness, width - (thickness * 2), height - (thickness * 2), colour);
}

void CRender2D::BorderedBox(int x, int y, int width, int height, int thickness, D3DCOLOR colour)
{
	this->FilledBox(x, y, width, thickness, colour);
	this->FilledBox(x, y, thickness, height, colour);
	this->FilledBox(x + width - thickness, y, thickness, height, colour);
	this->FilledBox(x, y + height - thickness, width, thickness, colour);
}

void CRender2D::BorderedBoxOutlined(int x, int y, int width, int height, int thickness, D3DCOLOR colour, D3DCOLOR outlinecolor)
{
	this->BorderedBox(x, y, width, height, thickness, outlinecolor);
	this->BorderedBox(x + thickness, y + thickness, width - (thickness * 2), height - (thickness * 2), thickness, colour);
	this->BorderedBox(x + (thickness * 2), y + (thickness * 2), width - (thickness * 4), height - (thickness * 4), thickness, outlinecolor);
}

void CRender2D::GradientBox(int x, int y, int width, int height, D3DCOLOR colour, D3DCOLOR color2, bool vertical)
{
	D3DVERTEX pVertex[4] = { { x, y, 0.0f, 1.0f, colour }, { x + width, y, 0.0f, 1.0f, vertical ? colour : color2 }, { x, y + height, 0.0f, 1.0f, vertical ? color2 : colour }, { x + width, y + height, 0.0f, 1.0f, color2 } };
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pVertex, sizeof(D3DVERTEX));
}

void CRender2D::GradientBoxOutlined(int x, int y, int width, int height, int thickness, D3DCOLOR colour, D3DCOLOR color2, D3DCOLOR outlinecolor, bool vertical)
{
	this->BorderedBox(x, y, width, height, thickness, outlinecolor);
	this->GradientBox(x + thickness, y + thickness, width - (thickness * 2), height - (thickness * 2), colour, color2, vertical);
}

void CRender2D::Circle(int x, int y, int radius, int points, D3DCOLOR colour)
{
	D3DVERTEX* pVertex = new D3DVERTEX[points + 1];
	for (int i = 0; i <= points; i++) pVertex[i] = { x + radius * cos(D3DX_PI * (i / (points / 2.0f))), y - radius * sin(D3DX_PI * (i / (points / 2.0f))), 0.0f, 1.0f, colour };
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, points, pVertex, sizeof(D3DVERTEX));
	delete[] pVertex;
}

void CRender2D::FilledCircle(int x, int y, int radius, int points, D3DCOLOR colour)
{
	D3DVERTEX* pVertex = new D3DVERTEX[points + 1];
	for (int i = 0; i <= points; i++) pVertex[i] = { x + radius * cos(D3DX_PI * (i / (points / 2.0f))), y + radius * sin(D3DX_PI * (i / (points / 2.0f))), 0.0f, 1.0f, colour };
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, points, pVertex, sizeof(D3DVERTEX));
	delete[] pVertex;
}