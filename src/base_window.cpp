#include "base_window.h"

bool BaseWindow::create(HINSTANCE hInstance, int nCmdShow) {

    WNDCLASS wc = {};

    wc.hInstance = hInstance;
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = L"Base Window";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        L"2d Physics Engine",
        WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL,
        NULL,
        hInstance,
        this
    );

    if (hwnd == NULL) return false;

    ShowWindow(hwnd, nCmdShow);
    m_hwnd = hwnd;

    return true;
}

HRESULT BaseWindow::createGraphicsResource() {
    HRESULT hr = S_OK;

    if (pRenderTarget == NULL) {

        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, D2D1::SizeU(width,height)),
            &pRenderTarget
        );

        if (SUCCEEDED(hr)) {
            D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::White);
            hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
        }

    }

    return hr;
}

void BaseWindow::discardGraphicsResource() {
    if (pRenderTarget) {
        pRenderTarget->Release();
        pRenderTarget = nullptr;
    }
    if (pBrush) {
        pBrush->Release();
        pBrush = nullptr;
    }
}

void BaseWindow::onPaint() {

    HRESULT hr = createGraphicsResource();
    
    if (SUCCEEDED(hr)) {

        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);
        pRenderTarget->BeginDraw();

        pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
        pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(100,100),50,50), pBrush);

        hr = pRenderTarget->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) discardGraphicsResource();
        EndPaint(m_hwnd, &ps);
    }
}

LRESULT BaseWindow::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {
        case WM_PAINT:
            onPaint();
            return 0;

        case WM_CREATE:
            if (FAILED(
                D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)
            )) return -1;
            return 0;

        case WM_DESTROY:
            discardGraphicsResource();
            pFactory->Release();
            pFactory = NULL;
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK BaseWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    BaseWindow *self = nullptr;

    if (uMsg == WM_NCCREATE) {

        CREATESTRUCTW *cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
        self = static_cast<BaseWindow*>(cs->lpCreateParams);

        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));

    } else {
        self = reinterpret_cast<BaseWindow*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    }

    if (self) return self->HandleMessage(hwnd, uMsg, wParam, lParam);

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}