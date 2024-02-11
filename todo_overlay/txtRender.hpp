#pragma once
#include <vector>
#include <fstream>
#include <d3d9.h>
#include <json/json.h>
#include <d3dx9core.h>
#include <locale>
#include <codecvt>
#include <vector>
#include "hk_keyboard.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib,"jsoncpp.lib")

struct TextEntry {
    std::wstring sub;
    std::vector<std::wstring> txt;
};



class TextRenderer {
public:
    TextRenderer(LPDIRECT3DDEVICE9 pdevice, int w, int h)
        : pdev(pdevice), width(w), height(h) {}

    void readF(const std::string& path) ;
    void RenderTxts(LPD3DXFONT sub, LPD3DXFONT font,int selected, std::vector<std::vector<vec2>> m) const;
    std::vector<TextEntry> enties;
    inline int itemsCount() {
        int i = 0;
        for (auto& e : this->enties) {
            i += e.txt.size();
        }
        return i;
    }
private:
    LPDIRECT3DDEVICE9 pdev;
    int width;
    int height;
    inline  std::wstring to_wstr(const std::string& str) {
        return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{}.from_bytes(str);
    }
    std::wstring newlines(int count) const {
        std::wstring str{};
        for (count >= 0; count--;) {
            str += L"\n";
        }
        return str;
    }
};
