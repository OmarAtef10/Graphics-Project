#pragma once

#include <Windows.h>
#include <iostream>
#include <stack>
#include <math.h>
#include <vector>
#include <list>
#include "UJtils.h"

using namespace std;


/********************************** TASK T *************************************/
static int squareXCords[2];
static int squareYCords[2];
static int squareXcenter;
static int squareYcenter;


static int lineXCords[2];
static int lineYCords[2];

void DrawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (abs(dy) <= abs(dx)) {
        if (x1 > x2)swap(x1, y1, x2, y2);
        SetPixel(hdc, x1, y1, c);
        int x = x1;
        while (x < x2) {
            x++;
            double y = y1 + (double) (x - x1) * dy / dx;
            SetPixel(hdc, x, ceil(y), c);
            SetPixel(hdc, x, floor(y), c);

        }
    } else {
        if (y1 > y2)swap(x1, y1, x2, y2);
        SetPixel(hdc, x1, y1, c);
        int y = y1;
        while (y < y2) {
            y++;
            double x = x1 + (double) (y - y1) * dx / dy;
            SetPixel(hdc, ceil(x), y, c);
            SetPixel(hdc, floor(x), y, c);

        }
    }
}


void DrawSquare(HDC hdc, Point p1, Point p2, COLORREF c) {
    if (p1.x > p2.x) {
        int temp = p1.x;
        p1.x = p2.x;
        p2.x = temp;
    }
    DrawLine(hdc, p1.x, p1.y, p1.x, p2.y, c);
    DrawLine(hdc, p2.x, p1.y, p2.x, p2.y, c);
    DrawLine(hdc, p1.x, p2.y, p2.x, p2.y, c);
    DrawLine(hdc, p1.x, p1.y, p2.x, p1.y, c);
}

void DrawSqClip(HDC hdc, Point &p1, Point &p2, COLORREF c) {
    if (p1.x > p2.x) {
        int temp = p1.x;
        p1.x = p2.x;
        p2.x = temp;
    }

    double distance = (p2.x - p1.x);

    p2.x = p1.x + distance;
    p2.y = p1.y;
    Point p3;
    p3.x = p1.x;
    p3.y = p1.y - distance;
    Point p4;
    p4.x = p2.x;
    p4.y = p2.y - distance;
    DrawLineSimpleDDA(hdc, p1.x, p1.y, p1.x + distance, p1.y, c);

    DrawLineSimpleDDA(hdc, p1.x, p1.y - distance, p1.x + distance, p1.y - distance, c);

    DrawLineSimpleDDA(hdc, p1.x, p1.y, p1.x, p1.y - distance, c);

    DrawLineSimpleDDA(hdc, p1.x + distance, p1.y, p1.x + distance, p1.y - distance, c);

}

/////////////////////////////////
union OutCode {
    unsigned All: 4;
    struct {
        unsigned left: 1, top: 1, right: 1, bottom: 1;
    };
};

OutCode GetOutCode(double x, double y, int xleft, int ytop, int xright, int ybottom) {
    OutCode out;
    out.All = 0;
    if (x < xleft)out.left = 1; else if (x > xright)out.right = 1;
    if (y < ytop)out.top = 1; else if (y > ybottom)out.bottom = 1;
    return out;
}

void VIntersect(double xs, double ys, double xe, double ye, int x, double *xi, double *yi) {
    *xi = x;
    *yi = ys + (x - xs) * (ye - ys) / (xe - xs);
}

void HIntersect(double xs, double ys, double xe, double ye, int y, double *xi, double *yi) {
    *yi = y;
    *xi = xs + (y - ys) * (xe - xs) / (ye - ys);
}

void CohenSuth(HDC hdc, int xs, int ys, int xe, int ye, int xleft, int ytop, int xright, int ybottom, COLORREF c) {
    double x1 = xs, y1 = ys, x2 = xe, y2 = ye;
    OutCode out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
    OutCode out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
    while ((out1.All || out2.All) && !(out1.All & out2.All)) {
        double xi, yi;
        if (out1.All) {
            if (out1.left)VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
            else if (out1.top)HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
            else if (out1.right)VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
            else HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
            x1 = xi;
            y1 = yi;
            out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
        } else {
            if (out2.left)VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
            else if (out2.top)HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
            else if (out2.right)VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
            else HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
            x2 = xi;
            y2 = yi;
            out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
        }
    }
    if (!out1.All && !out2.All) {
        DrawLineSimpleDDA(hdc, round(x1), round(y1), round(x2), round(y2), c);
    }
}
////////////////////////////////////////////

void sqPointClip(HDC hdc, int x, int y, COLORREF color) {
    if (x >= squareXCords[0] && x <= squareXCords[1] && y >= squareYCords[0] && y <= squareYCords[1]) {
        SetPixel(hdc, x, y, color);
    }
}


/*                                 TASK                             U                     */



static int CircleCenter[2];
static int CircleRadius;

void DrawLineClip(HDC hdc, int x1, int y1, int x2, int y2, int radius, COLORREF c) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (abs(dy) <= abs(dx)) {
        if (x1 > x2)swap(x1, y1, x2, y2);
        SetPixel(hdc, x1, y1, c);
        int x = x1;
        while (x < x2) {
            x++;
            double y = y1 + (double) (x - x1) * dy / dx;
            int dist = DistBet2Points(x, CircleCenter[0], y, CircleCenter[1]);
            if (dist <= radius) {
                SetPixel(hdc, x, ceil(y), c);
                SetPixel(hdc, x, floor(y), c);
            } else {
                continue;
            }

        }
    } else {
        if (y1 > y2)swap(x1, y1, x2, y2);
        SetPixel(hdc, x1, y1, c);
        int y = y1;
        while (y < y2) {
            y++;
            double x = x1 + (double) (y - y1) * dx / dy;
            int dist = DistBet2Points(x, CircleCenter[0], y, CircleCenter[1]);

            if (dist <= radius) {
                SetPixel(hdc, ceil(x), y, c);
                SetPixel(hdc, floor(x), y, c);
            } else {
                continue;
            }


        }
    }
}

void circlePointClip(HDC hdc, Point p, int radius, COLORREF c) {
    Point center;
    center.x = CircleCenter[0];
    center.y = CircleCenter[1];
    if (distanceBetweenPoints(p, center) < radius) {
        SetPixel(hdc, p.x, p.y, c);
    }
}

////////// Circle Stuff ///////////////////
/************************************** TASK S ***************************************************/

//void DrawRec(HDC hdc, Point p1, Point p2, COLORREF c)
//{
//    if (p1.x > p2.x) {
//        int temp = p1.x;
//        p1.x = p2.x;
//        p2.x = temp;
//    }
//
//    double distance = (p2.x - p1.x);
//    double width = 0.5 * distance;
//
//    DrawLineSimpleDDA(hdc, p1.x, p1.y, p1.x + distance, p1.y, c);
//
//    DrawLineSimpleDDA(hdc, p1.x, p1.y - width, p1.x + distance, p1.y - width, c);
//
//    DrawLineSimpleDDA(hdc, p1.x, p1.y, p1.x, p1.y - width, c);
//
//    DrawLineSimpleDDA(hdc, p1.x + distance, p1.y, p1.x + distance, p1.y - width, c);
//}

void DrawRec(HDC hdc, Point p1, Point p2, COLORREF c) {
    Point p3, p4;
    if (p1.x > p2.x) {
        int temp = p1.x;
        p1.x = p2.x;
        p2.x = temp;
    }
    p3.x = p2.x;
    p3.y = p1.y;
    p4.x = p1.x;
    p4.y = p2.y;
    DrawLine(hdc, p1.x, p1.y, p3.x, p3.y, c);
    DrawLine(hdc, p2.x, p2.y, p4.x, p4.y, c);
    DrawLine(hdc, p3.x, p3.y, p2.x, p2.y, c);
    DrawLine(hdc, p4.x, p4.y, p1.x, p1.y, c);
}

/////////////////////////////////
void recPointClip(HDC hdc, int x, int y, int xleft, int ytop, int xright, int ybottom, COLORREF color) {
    if (x >= xleft && x <= xright && y >= ytop && y <= ybottom) {
        SetPixel(hdc, x, y, color);
    }
}
/////////////////////////////////

bool isInLeft(Point x, int XL) {
    return x.x >= XL;
}

vector<Point> ClipLeft(vector<Point> p, int n, int XL) {
    vector<Point> outList;
    Point v1 = p[n - 1];
    bool in1 = isInLeft(v1, XL);
    for (int i = 0; i < n; i++) {
        Point v2 = p[i];
        bool in2 = isInLeft(v2, XL);
        if (in1 && in2) {
            outList.push_back(v2);
        } else if (in1) {
            double intersectX, intersectY;
            VIntersect(v1.x, v1.y, v2.x, v2.y, XL, &intersectX, &intersectY);
            outList.push_back(Point(intersectX, intersectY));
        } else if (in2) {
            double intersectX, intersectY;
            VIntersect(v1.x, v1.y, v2.x, v2.y, XL, &intersectX, &intersectY);
            outList.push_back(Point(intersectX, intersectY));
            outList.push_back(v2);
        }
        v1 = v2;
        in1 = in2;
    }
    return outList;
}
/////////////////////////////////

/////////////////////////////////
bool isInRight(Point x, int XR) {
    return x.x <= XR;
}

vector<Point> ClipRight(vector<Point> p, int n, int XR) {
    vector<Point> outList;
    Point v1 = p[n - 1];
    bool in1 = isInRight(v1, XR);
    for (int i = 0; i < n; i++) {
        Point v2 = p[i];
        bool in2 = isInRight(v2, XR);
        if (in1 && in2) {
            outList.push_back(v2);
        } else if (in1) {
            double intersectX, intersectY;
            VIntersect(v1.x, v1.y, v2.x, v2.y, XR, &intersectX, &intersectY);
            outList.push_back(Point(intersectX, intersectY));
        } else if (in2) {
            double intersectX, intersectY;
            VIntersect(v1.x, v1.y, v2.x, v2.y, XR, &intersectX, &intersectY);
            outList.push_back(Point(intersectX, intersectY));
            outList.push_back(v2);
        }
        v1 = v2;
        in1 = in2;
    }
    return outList;
}
//////////////////////////

/////////////////////////////////
bool isInTop(Point x, int YT) {
    return x.y >= YT;
}

vector<Point> ClipTop(vector<Point> p, int n, int YT) {
    vector<Point> outList;
    Point v1 = p[n - 1];
    bool in1 = isInTop(v1, YT);
    for (int i = 0; i < n; i++) {
        Point v2 = p[i];
        bool in2 = isInTop(v2, YT);
        if (in1 && in2) {
            outList.push_back(v2);
        } else if (in1) {
            double intersectX, intersectY;
            HIntersect(v1.x, v1.y, v2.x, v2.y, YT, &intersectX, &intersectY);
            outList.push_back(Point(intersectX, intersectY));
        } else if (in2) {
            double intersectX, intersectY;
            HIntersect(v1.x, v1.y, v2.x, v2.y, YT, &intersectX, &intersectY);
            outList.push_back(Point(intersectX, intersectY));
            outList.push_back(v2);
        }
        v1 = v2;
        in1 = in2;
    }
    return outList;
}
/////////////////////////////////

/////////////////////////////////
bool isInBottom(Point x, int YB) {
    return x.y <= YB;
}

vector<Point> ClipBottom(vector<Point> p, int n, int YB) {
    vector<Point> outList;
    Point v1 = p[n - 1];
    bool in1 = isInBottom(v1, YB);
    for (int i = 0; i < n; i++) {
        Point v2 = p[i];
        bool in2 = isInBottom(v2, YB);
        if (in1 && in2) {
            outList.push_back(v2);
        } else if (in1) {
            double intersectX, intersectY;
            HIntersect(v1.x, v1.y, v2.x, v2.y, YB, &intersectX, &intersectY);
            outList.push_back(Point(intersectX, intersectY));
        } else if (in2) {
            double intersectX, intersectY;
            HIntersect(v1.x, v1.y, v2.x, v2.y, YB, &intersectX, &intersectY);
            outList.push_back(Point(intersectX, intersectY));
            outList.push_back(v2);
        }
        v1 = v2;
        in1 = in2;
    }
    return outList;
}
/////////////////////////////////

void polygonClip(HDC hdc, vector<Point> p, int n, int xL, int yTop, int xR, int yBottom, COLORREF color) {
    vector<Point> p1, p2, p3, p4, p5;
    p1 = p;
    if (!p1.empty()) {
        p2 = ClipLeft(p1, n, xL);
    }
    if (!p2.empty()) {
        p3 = ClipRight(p2, p2.size(), xR);
    }
    if (!p3.empty()) {
        p4 = ClipTop(p3, p3.size(), yTop);
    }
    if (!p4.empty()) {
        p5 = ClipBottom(p4, p4.size(), yBottom);
    }

    if (!p5.empty()) {
        Point v1 = p5[p5.size() - 1];
        for (int i = 0; i < p5.size(); i++) {
            DrawLine(hdc, v1.x, v1.y, p5[i].x, p5[i].y, color);
            v1 = p5[i];
        }
    }
}