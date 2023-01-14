#ifndef ARRAY_LIST_HPP
#define ARRAY_LIST_HPP

#include <CTRPluginFramework.hpp>
#include "Helpers.hpp"
#include "Unicode.h"

namespace CTRPluginFramework {

    class ArrayList {
        public:

            static std::vector<std::string> Items;
            static Color BackgroundColor;
            static Color ForegroundColor;
            static Color BorderColor;

            /**
             * @brief         Add item to Arraylist item list
             *
             * @param text    Text to add
             * @return true   If the number of items is 24 or less (maximum display)
             * @return false  If the number of items is 25 or more (maximum display)
             */
            static bool Add(const std::string &text);


            /**
             * @brief         Delete item from Arraylist item list
             *
             * @param text    Text of item to delete
             * @return true   If the delete was successful
             * @return false  If the delete was unsuccessful
             */
            static bool Remove(const std::string &text);


            /**
             * @brief         Clear the Arraylist item list
             *
             */
            static void Clear(void);


            /**
             * @brief         Draw Arraylist (pass to Callback of PluginMenu)
             *
             */
            static void Draw(void);


    };

}
#endif
