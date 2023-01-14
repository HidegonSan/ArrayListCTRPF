#include "array_list.hpp"
#include <vector>

namespace CTRPluginFramework {


    Color ArrayList::BackgroundColor = Color::Black;
    Color ArrayList::ForegroundColor = Color::White;
    Color ArrayList::BorderColor = Color::White;
    std::vector<std::string> ArrayList::Items = {};


    bool ArrayList::Add(const std::string &text) {
        int item_count = ArrayList::Items.size();
        bool ret = 24 < item_count ? false : true; // 24 = 240(Top window height) / 10(Item height)

        if (item_count == 0) ArrayList::Items.push_back(text);
        for (int i=0; i<item_count + 1; i++) {
            std::string item = ArrayList::Items[i];
            if (item.length() <= text.length()) {
                ArrayList::Items.insert(ArrayList::Items.begin() + i, text);
                break;
            }
        }

        return ret;
    }


    bool ArrayList::Remove(const std::string &text) {
        int item_count = ArrayList::Items.size();
        bool ret = false;

        for (int i=0; i<item_count; i++) {
            std::string item = ArrayList::Items[i];
            if (item == text) {
                ArrayList::Items.erase(ArrayList::Items.begin() + i);
                ret = true;
                break;
            }
        }

        return ret;
    }


    void ArrayList::Clear(void) {
        ArrayList::Items.clear();
    }


    void ArrayList::Draw(void) {
        int item_count = ArrayList::Items.size();
        const Screen &scr = OSD::GetTopScreen();

        for (int i=0; i<item_count; i++) {
            std::string item = ArrayList::Items[i];
            int width = OSD::GetTextWidth(false, item);

            // Draw left border (2 = Evil implementations of CTRPF)
            // Cf. https://gitlab.com/thepixellizeross/ctrpluginframework/-/blob/develop/Library/source/CTRPluginFrameworkImpl/Graphics/Renderer_DrawStrings.cpp#L141
            for (int yy=0; yy<10; yy++) {
                scr.DrawPixel(400 - (width + 1 + 2), i*10 + yy, ArrayList::BorderColor);
            }

            // Draw text
            scr.Draw(item, 400 - (width + 2), i*10, ArrayList::ForegroundColor, ArrayList::BackgroundColor);
        }
    }


}
