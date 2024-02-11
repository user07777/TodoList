#pragma once
#include <Windows.h>
#include <vector>
struct vec2 {
    int x, y;
    //vec2(int x_val, int y_val) : x(x_val), y(y_val) {}
    void set(int new_x, int new_y) {
        x = new_x;
        y = new_y;
    }
};
class Hk {
public:
    Hk() : hook(nullptr) {}

    bool init(void* addr) {
        render = reinterpret_cast<uintptr_t>(addr);
        if (hook == nullptr) {
            hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, nullptr, 0);
            return hook != nullptr;
        }
        return false;
    }

    void update(int sz) {
        this->sz = sz;
    }

    ~Hk() {
        if (hook != nullptr) {
            UnhookWindowsHookEx(hook);
            hook = nullptr;
        }
    }

    static int i;
    static std::vector<std::vector<vec2>> marked;

private:
    static int sz;
    HHOOK hook;
    using Render_t = void(_cdecl*)(int);
    static uintptr_t render;

    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (render != NULL)
            reinterpret_cast<Render_t>(render)(i);
        if (nCode >= 0) {
            KBDLLHOOKSTRUCT* pKbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

            if (wParam == WM_KEYDOWN) {
                if (pKbStruct->vkCode == VK_ESCAPE && GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                    exit(0);
                }
            }
            else if (pKbStruct->vkCode == VK_DOWN && (GetAsyncKeyState(VK_CONTROL) & 0x8000)) {
                if (i <= sz) {
                    i++;
                }
            }
            else if (pKbStruct->vkCode == VK_UP && (GetAsyncKeyState(VK_CONTROL) & 0x8000)) {
                if (i > 0) {
                    i--;
                }
            }
            /*
            else if (pKbStruct->vkCode == VK_F1 && (GetAsyncKeyState(VK_CONTROL) & 0x8000)) {
                vec2 v{ 0,i };
                marked[0].push_back(v);
            }
            */
        }

        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    }
};
