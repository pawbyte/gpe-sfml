/*
gpe_window_controller_sfml.h
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

#ifndef gpe_window_sfml_h
#define gpe_window_sfml_h

//The headers

#include "../gpe/gpe_globals.h"
#include "../gpe/gpe_shared_resources.h"
#include "../gpe/gpe_window_controller_base.h"
#include <SFML/Window.hpp>

namespace gpe
{
    /*
    Our sfml_module window wrapper
    Since sfml is simple it only supports 1 window by default.
    So declaring this won't create multiple windows, so bear this in mind please
    "Please understand" - legend
    */

    class window_controller_sfml: public window_controller_base
    {
        public:
            sf::Window window_sfml;
            window_controller_sfml(std::string windowTitle,int wWidth, int wHeight,bool showBorder, bool fullScreen, bool maximized, bool isResizable );
            ~window_controller_sfml();

            bool disable_scaling();
            bool enable_scaling();

            //Handle window events
            bool hide_window();
            void process_event( input_event_container * event_holder );

            //sfml_SysWMinfo * get_info();
            void reset_input();
            void resize_window();

            bool scale_window( int s_width, int s_height , bool scale_int );
            bool scale_window_factor( float s_width, float s_height, bool scale_int );

            void set_renderer( renderer_base * new_renderer, bool remove_current );
            void set_window_position( int new_x, int new_y );
            bool set_window_size( int n_width, int n_height );
            //std::string save_screenshot(std::string file_location = "");
            void set_window_title(std::string new_title);
            bool show_window();
            void start_loop();

            //Turn fullscreen on/off
            void toggle_fullscreen();

            bool window_changed();
    };

    extern window_controller_sfml * window_controller_main_sfml;

    bool init_sfml_window_system();
    void quit_sfml_window_system();
}
#endif //gpe_window_sfml_h
