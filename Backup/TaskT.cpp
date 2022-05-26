#include <Windows.h>
#include <math.h>

static int squareXCords[2];
static int squareYCords[2];


static int lineXCords[2];
static int lineYCords[2];


struct Point {
    int x, y;

    Point() {}

    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

static int CircleRadius;


int Round(double x) {
    return (int) (x + 0.5);
}

int DistBet2Points(int x1, int x2, int y1, int y2) {
    int res = 0;
    res = Round(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
    return res;
}

void swap(int &x1, int &y1, int &x2, int &y2) {
    int tmp = x1;
    x1 = x2;
    x2 = tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
}

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


/////////////////////////////////
union OutCode {
    unsigned All : 4;
    struct {
        unsigned left : 1, top : 1, right : 1, bottom : 1;
    };
};

OutCode GetOutCode(double x, double y, int xleft, int ytop, int xright, int ybottom) {
    OutCode out;
    out.All = 0;
    if (x < xleft)out.left = 1; else if (x > xright)out.right = 1;
    if (y < ytop)out.top = 1; else if (y > ybottom)out.bottom = 1;
    return out;
}

void VIntersect(double xs, double ys, double xe, double ye, int x, double* xi, double* yi) {
    *xi = x;
    *yi = ys + (x - xs) * (ye - ys) / (xe - xs);
}

void HIntersect(double xs, double ys, double xe, double ye, int y, double* xi, double* yi) {
    *yi = y;
    *xi = xs + (y - ys) * (xe - xs) / (ye - ys);
}

void CohenSuth(HDC hdc, int xs, int ys, int xe, int ye, int xleft, int ytop, int xright, int ybottom) {
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
        }
        else {
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
        MoveToEx(hdc, Round(x1), Round(y1), NULL);
        LineTo(hdc, Round(x2), Round(y2));
    }
}
////////////////////////////////////////////

void sqPointClip(HDC hdc,int x, int y, COLORREF color) {
    if (x >= squareXCords[0] && x <= squareXCords[1] && y >= squareYCords[0] && y <= squareYCords[1]) {
        SetPixel(hdc, x, y, color);
    }
}


LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp) {
    static HDC hdc;
    static int leftClicks;
    static bool squareDrawn = false;
    static Point p1;
    static Point p2;
    static int index = 0;


    switch (mcode) {

        case WM_CREATE:
            hdc = GetDC(hwnd);
            break;
        case WM_LBUTTONDOWN:
            if (squareDrawn == false) {
                if (leftClicks == 0) {
                    POINT p;
                    GetCursorPos(&p);
                    p1.x = p.x;
                    p1.y = p.y;
                    squareXCords[0] = p.x;
                    squareYCords[0] = p.y;
                    leftClicks++;
                    break;
                } else if (leftClicks == 1) {
                    POINT p;
                    GetCursorPos(&p);
                    p2.x = p.x;
                    p2.y = p.y;
                    squareXCords[1] = p.x;
                    squareYCords[1] = p.y;
                    DrawSquare(hdc, p1, p2, RGB(0, 0, 69));
                    squareDrawn = true;
                }
            } else {

                POINT p;
                GetCursorPos(&p);
                lineXCords[index] = p.x;
                lineYCords[index] = p.y;
                sqPointClip(hdc, p.x, p.y, RGB(255, 0, 0));
                if (index == 1) {
                    CohenSuth(hdc, lineXCords[0], lineYCords[0], lineXCords[1], lineYCords[1], squareXCords[0], squareYCords[0], squareXCords[1], squareYCords[1]);
                    index = 0;
                } else {
                    index++;
                }

            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, mcode, wp, lp);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh)
{
    WNDCLASS wc;
    wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hinst;
    wc.lpfnWndProc = MyWndProc;
    wc.lpszClassName = L"MyClass";
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow(L"MyClass", L"My First Window", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 800, 600, NULL, NULL, hinst, 0);
    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
