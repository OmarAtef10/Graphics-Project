#pragma once

#include <Windows.h>
#include <iostream>
#include <stack>
#include <math.h>
#include <vector>
#include <list>

using namespace std;

struct Vector {
	double v[2];
	Vector(double x = 0, double y = 0)
	{
		v[0] = x; v[1] = y;
	}
	double& operator[](int i) {
		return v[i];
	}
};

static void DrawHermiteCurve(HDC hdc, Vector& p1, Vector& T1, Vector& p2, Vector& T2, COLORREF c)
{
	double a0 = p1[0], a1 = T1[0],
		a2 = -3 * p1[0] - 2 * T1[0] + 3 * p2[0] - T2[0],
		a3 = 2 * p1[0] + T1[0] - 2 * p2[0] + T2[0];
	double b0 = p1[1], b1 = T1[1],
		b2 = -3 * p1[1] - 2 * T1[1] + 3 * p2[1] - T2[1],
		b3 = 2 * p1[1] + T1[1] - 2 * p2[1] + T2[1];
	for (double t = 0; t <= 1; t += 0.001)
	{
		double t2 = t * t, t3 = t2 * t;
		double x = a0 + a1 * t + a2 * t2 + a3 * t3;
		double y = b0 + b1 * t + b2 * t2 + b3 * t3;
		SetPixel(hdc, round(x), round(y), c);
	}
}

static void DrawBezierCurve(HDC hdc, Vector& P0, Vector& P1, Vector& P2, Vector& P3, COLORREF c)
{
	Vector T0(3 * (P1.v[0] - P0.v[0]), 3 * (P1.v[1] - P0.v[1]));
	Vector T1(3 * (P3.v[0] - P2.v[0]), 3 * (P3.v[1] - P2.v[1]));
	DrawHermiteCurve(hdc, P0, T0, P3, T1, c);
}


static void DrawCardinalSpline(HDC hdc, Vector P[], int n, double c, COLORREF color)
{
	double c1 = 1 - c;
	Vector T0(c1 * (P[2].v[0] - P[0].v[0]), c1 * (P[2].v[1] - P[0].v[1]));
	for (int i = 2; i < n - 1; i++)
	{
		Vector T1(c1 * (P[i + 1].v[0] - P[i - 1].v[0]), c1 * (P[i + 1].v[1] - P[i - 1].v[1]));
		DrawHermiteCurve(hdc, P[i - 1], T0, P[i], T1, color);
		T0 = T1;
	}
}
