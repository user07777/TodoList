#include <iostream>
#include "txtRender.hpp"
#include "hk_keyboard.h"
LPDIRECT3D9 d3d = nullptr;
LPDIRECT3DDEVICE9 pdev = nullptr;
LPD3DXFONT Bannerfont = nullptr, font = nullptr, sub = nullptr;
D3DDISPLAYMODE displayMode;
int width{}, height{};
RECT rect;
TextRenderer* txt =nullptr;
Hk keyboard_hook;
void DrawRect(LPDIRECT3DDEVICE9 pDevice, RECT rect, D3DCOLOR color)
{
    struct Vertex
    {
        float x, y, z, rhw;
        D3DCOLOR color;
    };

    Vertex v[4] = {
        { (float)rect.left, (float)rect.top, 0.0f, 1.0f, color },
        { (float)rect.right, (float)rect.top, 0.0f, 1.0f, color },
        { (float)rect.left, (float)rect.bottom, 0.0f, 1.0f, color },
        { (float)rect.right, (float)rect.bottom, 0.0f, 1.0f, color }
    };
    pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex));
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
            PostQuitMessage(0);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void InitD3D(HWND hwnd) {
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hwnd;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    d3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hwnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &pdev
    );

    // Alfa
    pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    D3DXCreateFont(pdev, 20, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &font);
    D3DXCreateFont(pdev, 40, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Cascadia Code", &sub);
    D3DXCreateFont(pdev, 80, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Cascadia Code", &Bannerfont);
}

void Render(int selected) {
    if (pdev == nullptr) return;
    txt->readF("data.json");

    pdev->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 0.0f, 0);
    pdev->BeginScene();
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        exit(0);

    RECT r = { 0, 0, width, height };
    RECT txt_r = { 0  , 0,525, 160 };
    keyboard_hook.update(txt->itemsCount());
    DrawRect(pdev, r, D3DCOLOR_XRGB(17, 17, 17));
    Bannerfont->DrawText(nullptr, L"Todo List", -1, &txt_r, DT_RIGHT, D3DCOLOR_XRGB(40, 170, 255));

    txt->RenderTxts(sub, font,selected,keyboard_hook.marked);
    
    /*
    sub->DrawText(nullptr, L"Projeckt", -1, &txt_r2, DT_RIGHT, D3DCOLOR_XRGB(255, 255, 255));
    font->DrawText(nullptr, L"do this auto from a file", -1, &txt_r3, DT_RIGHT, D3DCOLOR_XRGB(255, 255, 255));
    font->DrawText(nullptr, L"Pipe to check tasks", -1, &txt_r4, DT_RIGHT, D3DCOLOR_XRGB(255, 255, 255));
    */

    pdev->EndScene();
    pdev->Present(nullptr, nullptr, nullptr, nullptr);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Configurando a janela
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"todo";
    RegisterClass(&wc);
    int sWidth = GetSystemMetrics(SM_CXSCREEN);
    int sHeight = GetSystemMetrics(SM_CYSCREEN);
    RECT r = { sWidth - sWidth / 4, 0, sWidth, sHeight };
    int rectWidth = r.right - r.left;
    int rectHeight = r.bottom - r.top;
    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
        L"todo",
        L"todo",
        WS_POPUP,
        sWidth - rectWidth, 0, rectWidth, rectHeight,
        nullptr, nullptr, hInstance, nullptr);

    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & ~WS_EX_TOPMOST);
    SetLayeredWindowAttributes(hwnd, 0, 220, ULW_ALPHA); //<--
    ShowWindow(hwnd, SW_SHOWNORMAL);

    // Inicializando o DirectX
    InitD3D(hwnd);
    d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);
    width = displayMode.Width;
    height = displayMode.Height;
    rect = { 10, 10, width - 10, height - 10 };
    txt = new TextRenderer(pdev, rectWidth, rectHeight);
    txt->readF("data.json");

    // hookando o teclado
    keyboard_hook.init(&Render);

    // Loop de mensagem da janela
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        Render(keyboard_hook.i);

    }
    // Liberando recursos do DirectX
    if (Bannerfont != nullptr) {
        Bannerfont->Release();
    }
    if (sub != nullptr) {
        font->Release();
    }
    if (font != nullptr) {
        font->Release();
    }
    if (pdev != nullptr) {
        pdev->Release();
    }
    if (d3d != nullptr) {
        d3d->Release();
    }
    delete txt;
    return 0;
}
