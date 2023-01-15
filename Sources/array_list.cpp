#include "array_list.hpp"
#include "CTRPluginFramework/System/Time.hpp"
#include <vector>
#include <math.h>


namespace CTRPluginFramework {


    Color ArrayList::BackgroundColor = Color::Black;
    Color ArrayList::ForegroundColor = Color::White;
    Color ArrayList::BorderColor = Color::White;
    std::vector<std::string> ArrayList::Items = {};
    int ArrayList::AnimatingIndex = -1;


    void ArrayList::_Wait() {
        while (ArrayList::AnimatingIndex != -1) {}
    }


    // Thanks: https://easings.net/ja#easeOutSine
    float ArrayList::_EaseOutSine(float x) {
        return sin((x * M_PI) / 2);
    }


    // Thanks: https://easings.net/ja#easeInSine
    float ArrayList::_EaseInSine(float x) {
        return (1 - cos((x * M_PI) / 2));
    }


    void ArrayList::_EaseOut(const std::string &text, int index) {
        const Screen &scr = OSD::GetTopScreen();
        int width = OSD::GetTextWidth(false, text) + 3; // 1(left border) + 2(CTRPF evil)
        int x = 400;
        int y = index*10;
        const int step = 10;

        for (int i=0; i<step; i++) {
            float p = i / static_cast<float>(step); // Current progress (0 ~ 1)
            int sub = ArrayList::_EaseOutSine(p)*width; // amount to sub
            scr.Draw(text, 400 - sub, y);
            if (i != step - 1) { // Not needed for normal drawing
                Sleep(Milliseconds((1000*((float)1 / (float)3)) / step)); // Do not think
            }
        }
    }


    void ArrayList::_EaseIn(const std::string &text, int index) {
        const Screen &scr = OSD::GetTopScreen();
        int width = OSD::GetTextWidth(false, text) + 3; // 1(left border) + 2(CTRPF evil)
        int x = 400 - width;
        int y = index*10;
        const int step = 10;

        for (int i=0; i<step; i++) {
            float p = i / static_cast<float>(step); // Current progress (0 ~ 1)
            int add = ArrayList::_EaseInSine(p)*width; // amount to add
            scr.Draw(text, x + add, y);
            if (i != step - 1) { // Not needed for normal drawing
                Sleep(Milliseconds((1000*((float)1 / (float)3)) / step)); // Do not think
            }
        }
    }


    bool ArrayList::Add(const std::string &text) {
        int item_count = ArrayList::Items.size();
        bool ret = 24 < item_count ? false : true; // 24 = 240(Top window height) / 10(Item height)

        if (item_count == 0) {
            ArrayList::_EaseOut(text, 0);
            ArrayList::Items.push_back(text);
        }
        else {
            // If the item to be added is the shortest
            if (text.length() < ArrayList::Items[item_count - 1].length()) {
                ArrayList::AnimatingIndex = item_count;
                ArrayList::Items.insert(ArrayList::Items.end(), text);
                Task([](void *) {
                    ArrayList::_Wait();
                    ArrayList::_EaseOut(ArrayList::Items[ArrayList::AnimatingIndex], ArrayList::AnimatingIndex);
                    ArrayList::AnimatingIndex = -1;
                    return (s32)0;
                }).Start();
            }
            else {
                for (int i=0; i<item_count; i++) {
                    std::string item = ArrayList::Items[i];
                    if (item.length() <= text.length()) {
                        ArrayList::AnimatingIndex = i;
                        ArrayList::Items.insert(ArrayList::Items.begin() + i, text);
                        Task([](void *) {
                            ArrayList::_Wait();
                            ArrayList::_EaseOut(ArrayList::Items[ArrayList::AnimatingIndex], ArrayList::AnimatingIndex);
                            ArrayList::AnimatingIndex = -1;
                            return (s32)0;
                        }).Start();
                        break;
                    }
                }
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
                ArrayList::AnimatingIndex = i;
                Task([](void *) {
                    ArrayList::_Wait();
                    ArrayList::_EaseIn(ArrayList::Items[ArrayList::AnimatingIndex], ArrayList::AnimatingIndex);
                    ArrayList::Items.erase(ArrayList::Items.begin() + ArrayList::AnimatingIndex);
                    ArrayList::AnimatingIndex = -1;
                    return (s32)0;
                }).Start();
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
            // If an item is animating, do not draw
            if (i == ArrayList::AnimatingIndex) { continue; };

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
