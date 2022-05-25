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

void DrawLineClip(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (abs(dy) <= abs(dx)) {
        if (x1 > x2)swap(x1, y1, x2, y2);
        if (x1 >= squareXCords[0] && x1 <= squareXCords[1]) {
            SetPixel(hdc, x1, y1, c);
        }
        int x = x1;
        while (x < x2) {
            x++;
            double y = y1 + (double) (x - x1) * dy / dx;
            if (x >= squareXCords[0] && x <= squareXCords[1] && y >= squareYCords[0] && y <= squareYCords[1]) {
                SetPixel(hdc, x, ceil(y), c);
                SetPixel(hdc, x, floor(y), c);
            } else {
                continue;
            }

        }
    } else {
        if (y1 > y2)swap(x1, y1, x2, y2);
        if (y1 >= squareYCords[0] && y1 <= squareYCords[1]) {
            SetPixel(hdc, x1, y1, c);
        }
        int y = y1;
        while (y < y2) {
            y++;
            double x = x1 + (double) (y - y1) * dx / dy;
            if (x >= squareXCords[0] && x <= squareXCords[1] && y >= squareYCords[0] && y <= squareYCords[1]) {
                SetPixel(hdc, ceil(x), y, c);
                SetPixel(hdc, floor(x), y, c);
            } else {
                continue;
            }

        }
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
                if (index == 1) {
                    DrawLineClip(hdc, lineXCords[0], lineYCords[0], lineXCords[1], lineYCords[1], RGB(0, 0, 69));
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