/*
gpe_input_sfml.h
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

#ifndef gpe_input_sfml_h
#define gpe_input_sfml_h

#include "../gpe/gpe_input_base.h"
#include "../gpe/gpe_render_package_handler.h"
#include "../gpe/gpe_shared_resources.h"
#include "../gpe/gpe_timer_base.h"
#include <string>
#include <SFML/Window.hpp>


namespace gpe
{
    class gamepad_sfml: public gamepad_base
    {
        public:
            gamepad_sfml();
            ~gamepad_sfml();
            void handle_input();
            void pure_reset();
            void reset_gamepad();
            void reset_temp_input();
            void setup_default_mapping( bool map_buttons = true , bool mapAxis = true );
    };

    /**
     * class input_manager_sfml
     *
     * Handles keyboard, gamepad and mouse states
     */

    class input_manager_sfml: public input_manager_base
    {
        private:
            gamepad_sfml * game_pads_sfml[gp_max_devices];

        public:
            input_manager_sfml();
            ~input_manager_sfml();

            bool clipboard_empty();
            bool clipboard_set( std::string new_clipboard_string);
            std::string clipboard_string();

            //void handle_modifers( sfmlMod mod );
            bool gamepad_detect_all();
            bool gamepad_disconnect( int gamepad_id);
            bool gamepad_setup(int gamepad_id );

            void handle_input(bool dump_event = false, bool is_embedded=false );

            void key_bind_load();
            void key_bind_qwerty();
            void key_bind_save();

             bool load_input_settings(std::string file_path = "");
            //void reset_all_input();
            //void reset_temp_input();
    };

    bool init_sfml_input_system();
    void quit_sfml_input_system();
}

#endif //gpe_input_sfml_h
