#include "txtRender.hpp"
#include <iostream>
#define subSize 60
#define txtSize 20
void TextRenderer::readF(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << " arquivo vazio ";
    }

    Json::Value root;
    file >> root;

    std::vector<TextEntry> entries;

    try {
        for (const auto& entry : root)
        {
            TextEntry textEntry;
            if (entry.isMember("sub") && entry["sub"].isString()) {
                textEntry.sub = to_wstr(entry["sub"].asString());
            }
            else {
                throw std::runtime_error("Erro: chave 'sub' ausente ou não é uma string.");
            }

            const auto& txtArray = entry["txt"];
            if (txtArray.isArray()) {
                for (const auto& txt : txtArray)
                {
                    if (txt.isString()) {
                        textEntry.txt.push_back(to_wstr(txt.asString()));
                    }
                    else {
                        throw std::runtime_error("Erro: valor da chave 'txt' não é uma string.");
                    }
                }
            }
            else {
                throw std::runtime_error("Erro: chave 'txt' ausente ou não é um array.");
            }
            file.close();
            entries.push_back(textEntry);
        }
    }
    catch (const std::exception& e) {
        std::cerr  << e.what() << std::endl;
    }
    file.close();
    this->enties = entries;
}
/*
One String
void TextRenderer::RenderTxts(LPD3DXFONT sub, LPD3DXFONT txt) const {
    if (pdev == nullptr)
        return;

    RECT r = { 0, 0, width, height };

    for (size_t i = 0; i < this->enties.size(); ++i)
    {
        const auto& entry = this->enties[i];

        std::wstring str = L"\n========================================================================================================================================================================================================================\n" + entry.sub;

        for (const auto& txtString : entry.txt) {
            str += L"\n" + txtString  ;
        }
        //str += L"\n========================================================================================================================================================================================================================";
        if (i == 0) {
            RECT rect = { 0, static_cast<LONG>(i * (subSize + 20) + 100), 680, static_cast<LONG>((i + 1) * (subSize + 20) + 100) +110 };
            sub->DrawText(nullptr, str.c_str(), -1, &rect, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 255));
        }
        else {
            RECT rect = { 0, static_cast<LONG>(i * (subSize + 20) + 100), 680, static_cast<LONG>((i + 1) * (subSize + 20) + 100) * 2 };
            sub->DrawText(nullptr, str.c_str(), -1, &rect, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 255));
        }
    }
}
*/
void TextRenderer::RenderTxts(LPD3DXFONT sub, LPD3DXFONT txt, int selected, std::vector<std::vector<vec2>> m) const {
    if (pdev == nullptr)
        return;

    RECT r = { 0, 0, width, height };
    int min_h = 180, max_h = 250, max_w = 700;
    for (size_t i = 0; i < this->enties.size(); ++i)
    {
        const auto& entry = this->enties[i];
        int newL_count = entry.txt.size();
        //std::wstring entry.sub = newlines(newL_count)+  L"========================================================================================================================================================================================================================\n"+ entry.sub;

        // Render subtitles
        if (i == 0) {
            RECT subRect = { 0, static_cast<LONG>(i * (subSize + 20) + 100), 680, static_cast<LONG>((i + 1) * (subSize + 20) + 100) + 110 };
            sub->DrawText(nullptr, entry.sub.c_str(), -1, &subRect, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 255));

            // Render first regular text differently
            if (!entry.txt.empty()) {
                const auto& str = entry.txt[0];
                std::wstring txtStr = L"\n" + str;
                for (size_t j = 0; j < entry.txt.size(); ++j) {
                    const auto& str = entry.txt[j];
                    std::wstring txtStr = L"\n" + str;
                    min_h += 20;
                    max_h += 70;
                    D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255);

                    if (str.at(0) == L'✓') {
                        color = D3DCOLOR_XRGB(0, 255, 0);
                    }

                    RECT txtRect = { 0, min_h, 700, max_h };
                    txt->DrawText(nullptr, txtStr.c_str(), -1, &txtRect, DT_CENTER | DT_VCENTER, color);
                }
            }
        }
        else {
            RECT subRect = { 0, min_h += 30, 680, max_h += 30 };
            sub->DrawText(nullptr, entry.sub.c_str(), -1, &subRect, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 255));

            // Render  text
            for (size_t j = 0; j < entry.txt.size(); ++j) {
                const auto& str = entry.txt[j];
                std::wstring txtStr = L"\n" + str;
                RECT txtRect = { 0, min_h += (i * 25), 700, max_h += (i * 25) };
                D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255);

                if (str.at(0) == L'✓') {
                    color = D3DCOLOR_XRGB(0, 255, 0);
                }
                txt->DrawText(nullptr, txtStr.c_str(), -1, &txtRect, DT_CENTER | DT_VCENTER, color);
            }
        }
    }
}
