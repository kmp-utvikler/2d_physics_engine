#ifndef BASE_WINDOW
#define BASE_WINDOW

#define UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <d2d1.h>

class BaseWindow {

    private:
        HWND m_hwnd;
        int width;
        int height;

        ID2D1Factory *pFactory = NULL;
        ID2D1HwndRenderTarget *pRenderTarget = NULL;
        ID2D1SolidColorBrush *pBrush = NULL;

        HRESULT createGraphicsResurce();
        void discardGraphicsResource();
        void onPaint();

    public:
        BaseWindow(int w, int h) : width(w), height(h) {}
        bool create(HINSTANCE hInstance, int nCmdShow);
        LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

#endif