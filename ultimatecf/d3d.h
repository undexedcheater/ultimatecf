#pragma once

#include "d3dfont9.h"

class CRender2D
{
private: 
	LPDIRECT3DDEVICE9 pDevice;

public:
	CRender2D() { pDevice = nullptr; }
	~CRender2D() = default;

	void SetDevice(LPDIRECT3DDEVICE9 devIn);
	
	void String(float x, float y, D3DCOLOR colour, CD3DFont* font, bool outlined, const char* string, ...);
	int StringWidth(CD3DFont* font, const char* string);

	void Line(float x, float y, float x2, float y2, D3DCOLOR colour);
	void FilledBox(float x, float y, float width, float height, D3DCOLOR colour);
	void FilledBoxOutlined(int x, int y, int width, int height, int thickness, D3DCOLOR colour, D3DCOLOR outlinecolor);
	void BorderedBox(int x, int y, int width, int height, int thickness, D3DCOLOR colour);
	void BorderedBoxOutlined(int x, int y, int width, int height, int thickness, D3DCOLOR colour, D3DCOLOR outlinecolor);
	void GradientBox(int x, int y, int width, int height, D3DCOLOR colour, D3DCOLOR color2, bool vertical);
	void GradientBoxOutlined(int x, int y, int width, int height, int thickness, D3DCOLOR colour, D3DCOLOR color2, D3DCOLOR outlinecolor, bool vertical);

	void Circle(int x, int y, int radius, int points, D3DCOLOR colour);
	void FilledCircle(int x, int y, int radius, int points, D3DCOLOR colour);
};

extern std::unique_ptr< CRender2D > pRenderer;