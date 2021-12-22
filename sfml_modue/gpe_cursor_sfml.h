/*
gpe_cursor_sfml.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2021 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2021 PawByte LLC.
Copyright (c) 2014-2021 Game Pencil Engine contributors ( Contributors Page )

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#ifndef gpe_cursor_sfml_h
#define gpe_cursor_sfml_h

#include <string>
#include <map>

#include "../gpe/gpe_cursor_base.h"
#include "../gpe/gpe_error_logger.h"
#include "../gpe/gpe_file_system.h"
#include "../gpe/gpe_timer_base.h"
#include "../other_libs/stg_ex.h"

#include <SFML/Window/Cursor.hpp>
#include "gpe_window_controller_sfml.h"

namespace gpe
{
    class cursor_controller_sfml: public gpe::cursor_controller_base
    {
        protected:
            sf::Cursor cursor_sfml_current;
            std::map <std::string, * sf::Cursor > cursor_sfml_map;

            //custom wait animation
            float wait_cursor_frame;
            float wait_cursor_frame_inc;
            float wait_cursor_frame_max;

            //Custom wait arrow animation
            float wait_arrow_cursor_frame;
            float wait_arrow_cursor_frame_inc;
            float wait_arrow_cursor_frame_max;

            sf::Vector2f mouse_vector;
        public:
            cursor_controller_sfml( int window_id = -1 );
            virtual ~cursor_controller_sfml();
            //Cursor logic
            void cursor_change( std::string new_cursor );
            void cursor_change_system( int system_cursor_id );
            void cursor_clear_dynamic();
            bool cursor_contains( std::string cursor_name );
            bool cursor_create_from_image(std::string f_name );
            int cursor_map_size();
            std::string cursor_system_name( int cId );
            void hide_cursor();
            void name_default_cursors();
            void process_cursors();
            void render();
            void show_cusor( );
    };

    extern cursor_controller_sfml * cursor_main_sfml_controller;

    bool init_sfml_cursor_system();
    void quit_sfml_cursor_system();
}

#endif //gpe_cursor_sfml_h
