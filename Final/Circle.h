#pragma once
#include "Line.h"
using namespace std;
/* CIRCLES */

static void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
	SetPixel(hdc, xc + x, yc + y, c);
	SetPixel(hdc, xc + x, yc - y, c);
	SetPixel(hdc, xc - x, yc - y, c);
	SetPixel(hdc, xc - x, yc + y, c);

	SetPixel(hdc, xc + y, yc + x, c);
	SetPixel(hdc, xc + y, yc - x, c);
	SetPixel(hdc, xc - y, yc - x, c);
	SetPixel(hdc, xc - y, yc + x, c);
}

static void DrawCircleNaive(HDC hdc, int xc, int yc, int R, COLORREF c)
{
	int x = 0;
	double y = R;
	Draw8Points(hdc, xc, yc, 0, R, c);
	while (x < y)
	{
		x++;
		y = sqrt((double)R * R - x * x);
		Draw8Points(hdc, xc, yc, x, Round(y), c);
	}

}

static void DrawCirclePolar(HDC hdc, int xc, int yc, int R, COLORREF color)
{
	int x = R, y = 0;
	double theta = 0, dtheta = 1.0 / R;
	Draw8Points(hdc, xc, yc, x, y, color);
	while (x > y)
	{
		theta += dtheta;
		x = round(R * cos(theta));
		y = round(R * sin(theta));
		Draw8Points(hdc, xc, yc, x, y, color);
	}
}

static void DrawCircleIterativePolar(HDC hdc, int xc, int yc, int R, COLORREF color)
{
	double x = R, y = 0;
	double dtheta = 1.0 / R;
	double cdtheta = cos(dtheta), sdtheta = sin(dtheta);
	Draw8Points(hdc, xc, yc, R, 0, color);
	while (x > y)
	{
		double x1 = x * cdtheta - y * sdtheta;
		y = x * sdtheta + y * cdtheta;
		x = x1;
		Draw8Points(hdc, xc, yc, round(x), round(y), color);
	}
}

static void DrawCircleMidpoint(HDC hdc, int xc, int yc, int R, COLORREF color)
{
	int x = 0, y = R;
	int d = 1 - R;
	Draw8Points(hdc, xc, yc, x, y, color);
	while (x < y)
	{
		if (d < 0)
			d += 2 * x + 2;
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		Draw8Points(hdc, xc, yc, x, y, color);
	}
}


static void DrawCircleModifiedMidpoint(HDC hdc, int xc, int yc, int R, COLORREF color)
{
	int x = 0, y = R;
	int d = 1 - R;
	int c1 = 3, c2 = 5 - 2 * R;
	Draw8Points(hdc, xc, yc, x, y, color);
	while (x < y)
	{
		if (d < 0)
		{
			d += c1;
			c2 += 2;
		}
		else
		{
			d += c2;
			c2 += 4;
			y--;
		}
		c1 += 2;
		x++;
		Draw8Points(hdc, xc, yc, x, y, color);
	}
}
