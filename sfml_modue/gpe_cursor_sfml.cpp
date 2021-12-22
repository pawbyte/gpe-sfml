/*
gpe_cursor_base.cpp
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

#include "gpe_cursor_sfml.h"

namespace gpe
{
    cursor_controller_sfml * cursor_main_sfml_controller = nullptr;

    bool init_sfml_cursor_system()
    {
        if( cursor_main_sfml_controller != nullptr )
        {
            delete cursor_main_sfml_controller;
            cursor_main_sfml_controller = nullptr;
        }
        if( cursor_main_controller !=nullptr )
        {
            delete cursor_main_controller;
            cursor_main_controller = nullptr;
        }
        cursor_main_controller = cursor_main_sfml_controller = new cursor_controller_sfml();
        return true;
    }

    void quit_sfml_cursor_system()
    {
        if( cursor_main_sfml_controller != nullptr )
        {
            if( cursor_main_controller!=nullptr )
            {
                if( cursor_main_controller->equals( cursor_main_sfml_controller) )
                {
                    delete cursor_main_sfml_controller;
                    cursor_main_sfml_controller = nullptr;
                    cursor_main_controller = nullptr;
                }
                else
                {
                    delete cursor_main_sfml_controller;
                    cursor_main_sfml_controller = nullptr;
                }
            }
            else
            {
                delete cursor_main_sfml_controller;
                cursor_main_sfml_controller = nullptr;
            }
        }
    }

    cursor_controller_sfml::cursor_controller_sfml(  int window_id  )
    {
        mouse_vector.x = 0;
        mouse_vector.y = 0;
        cursor_controller_type = "sfml";
        cursor_window_id = 0;


        sf::Cursor cursor_arrow;
        cursor_arrow.loadFromSystem(sf::Cursor::Arrow);
        cursor_sfml_map[ cursor_default_names[ cursor_default_type::arrow ] ] =  &cursor_arrow;

        //Sets the current cursor to the arrow aka default
        cursor_sfml_current = cursor_arrow;

        sf::Cursor cursor_ibeam;
        cursor_ibeam.loadFromSystem(sf::Cursor::Text);
        cursor_sfml_map[ cursor_default_names[cursor_default_type::ibeam ] ] =  cursor_ibeam;

        sf::Cursor cursor_wait;
        cursor_wait.loadFromSystem(sf::Cursor::Wait );
        cursor_sfml_map[ cursor_default_names[cursor_default_type::wait ] ] =  cursor_wait;

        sf::Cursor cursor_crosshair;
        cursor_crosshair.loadFromSystem(sf::Cursor::Cross;
        cursor_sfml_map[ cursor_default_names[cursor_default_type::crosshair ] ] =  cursor_crosshair;

        sf::Cursor cursor_wait_arrow;
        cursor_wait_arrow.loadFromSystem(sf::Cursor::ArrowWait);
        cursor_sfml_map[ cursor_default_names[cursor_default_type::wait_arrow ] ] =  cursor_wait_arrow;

        sf::Cursor cursor_resize_nwse;
        cursor_resize_nwse.loadFromSystem(sf::Cursor:::SizeTopLeftBottomRight);
        cursor_sfml_map[ cursor_default_names[cursor_default_type::sizenwse ] ] =   cursor_resize_nwse;

        sf::Cursor cursor_resize_nesw;
        cursor_resize_nesw.loadFromSystem(sf::Cursor::SizeTopLeftBottomRight);
        cursor_sfml_map[ cursor_default_names[cursor_default_type::sizenesw ] ] =   cursor_resize_nesw;

        sf::Cursor cursor_ew;
        cursor_ew.loadFromSystem(sf::Cursor::SizeHorizontal);
        cursor_sfml_map[ cursor_default_names[cursor_default_type::sizewe ] ] =  cursor_ew;

        sf::Cursor cursor_ns;
        cursor_ns.loadFromSystem(sf::Cursor::SizeVertical);
        cursor_sfml_map[ cursor_default_names[cursor_default_type::sizens ] ] =  cursor_ns;

        sf::Cursor cursor_resize_all;
        cursor_resize_all.loadFromSystem(sf::Cursor::SizeAll);
        cursor_sfml_map[ cursor_default_names[cursor_default_type::sizeall ] ] =  cursor_resize_all;

        sf::Cursor cursor_not_allowed;
        cursor_not_allowed.loadFromSystem(sf::Cursor::NotAllowed);
        cursor_sfml_map[ cursor_default_names[cursor_default_type::no ] ] =  cursor_not_allowed;

        sf::Cursor cursor_poiner_hand;
        cursor_poiner_hand.loadFromSystem(sf::Cursor::Hand);
        cursor_sfml_map[ cursor_default_names[cursor_default_type::hand ] ] =  cursor_poiner_hand;

        sf::Cursor cursor_poiner_help;
        cursor_poiner_help.loadFromSystem(sf::Cursor::Help);
        cursor_sfml_map[ cursor_default_names[cursor_default_type::hand ] ] =  cursor_poiner_help;

        wait_cursor_frame = 0;
        wait_cursor_frame_inc = 0.3;
        wait_cursor_frame_max = 390;
        wait_arrow_cursor_frame = 0;
        wait_arrow_cursor_frame_inc = 0.4;
        wait_arrow_cursor_frame_max = 390;
    }

    cursor_controller_sfml::~cursor_controller_sfml()
    {
        cursor_sfml_map.clear();
    }

    void cursor_controller_sfml::cursor_change( std::string new_cursor )
    {
        cursor_current = new_cursor;
    }

    void cursor_controller_sfml::cursor_change_system( int system_cursor_id )
    {
        if( system_cursor_id >=0 && system_cursor_id < (int)cursor_default_names.size() )
        {
            cursor_change( cursor_system_name( system_cursor_id ) );
        }
    }

    void cursor_controller_sfml::cursor_clear_dynamic()
    {

    }

    bool cursor_controller_sfml::cursor_contains( std::string cursor_name )
    {
        auto cCursor = cursor_sfml_map.find( cursor_name );
        if( cCursor == cursor_sfml_map.end() )
        {
            return false;
        }
        return true;
    }

    bool cursor_controller_sfml::cursor_create_from_image(std::string f_name )
    {
        if( main_file_url_manager->file_exists( f_name) == false )
        {
            gpe::error_log->report("Unable to load cursor from ["+ f_name + "]" );
            return false;
        }


        error_log->report("Unable to load cursor image from ["+ f_name + "[" );
        return false;
    }

    int cursor_controller_sfml::cursor_map_size()
    {
        return (int)cursor_sfml_map.size();
    }

    std::string cursor_controller_sfml::cursor_system_name( int cId )
    {
        if( cId >= 0 && cId <  (int)cursor_default_names.size() )
        {
            return cursor_default_names[ cId ];
        }
        return "";
    }

    void cursor_controller_sfml::hide_cursor()
    {
        cursor_is_hidden  = true;
        window_controller_main_sfml.window_sfml.setMouseCursorVisible( false );
    }

    void cursor_controller_sfml::name_default_cursors()
    {
        cursor_default_names.clear();
        for( int i_cursor = 0; i_cursor < gpe::cursor_default_type::max_default_cursor; i_cursor++ )
        {
            cursor_default_names[ i_cursor] = "";
        }
        cursor_default_names[ cursor_default_type::arrow] = "arrow";
        cursor_default_names[ cursor_default_type::ibeam] = "ibeam";
        cursor_default_names[ cursor_default_type::wait ] = "wait";
        cursor_default_names[ cursor_default_type::crosshair ] = "crosshair";
        cursor_default_names[ cursor_default_type::wait_arrow] = "waitarrow";

        cursor_default_names[ cursor_default_type::sizenwse] = "sizenwse";
        cursor_default_names[ cursor_default_type::sizenesw] = "sizenesw";
        cursor_default_names[ cursor_default_type::sizewe ] = "sizewe";
        cursor_default_names[ cursor_default_type::sizens] = "sizens";
        cursor_default_names[ cursor_default_type::sizeall ] = "sizeall";

        cursor_default_names[ cursor_default_type::no ] = "no";
        cursor_default_names[ cursor_default_type::hand ] = "hand";
        cursor_default_names[ cursor_default_type::help ] = "help";
        cursor_previous = cursor_current = cursor_default_names[ cursor_default_type::arrow ];
    }

    void cursor_controller_sfml::process_cursors()
    {
        if( cursor_current!=cursor_previous )
        {
            cursor_previous = cursor_current;
            if( cursor_contains( cursor_current ) )
            {
                cursor_sfml_id = cursor_sfml_map[cursor_current];

                if( cursor_sfml_id >=0 )
                {
                    show_cusor();
                    window_controller_main_sfml->window_sfml.setMouseCursor( cursor_sfml_current );
                    SetMouseCursor( cursor_sfml_id );
                }
            }
            else
            {
                if( cursor_previous == cursor_default_names[cursor_default_type::wait] )
                {
                    hide_cursor();
                    wait_cursor_frame = 0;
                }
                else if( cursor_previous == cursor_default_names[cursor_default_type::wait_arrow] )
                {
                    hide_cursor();
                    wait_arrow_cursor_frame = 0;
                }
            }
        }
    }

    void cursor_controller_sfml::render()
    {
        if( !window_controller_main->window_has_focus )
        {
            return;
        }

        if( !window_controller_main->window_has_mouse )
        {
            return;
        }
        mouse_vector.x = input->mouse_position_x;
        mouse_vector.y = input->mouse_position_y;


        if( cursor_previous == cursor_default_names[cursor_default_type::wait] )
        {
            wait_cursor_frame += wait_cursor_frame_inc * time_keeper->get_delta_ticks();

            if( wait_cursor_frame > wait_cursor_frame_max )
            {
                wait_cursor_frame = 0;
            }
            DrawRing( mouse_vector, 8,6, 0, 360, 0, BLACK );
            DrawRing( mouse_vector, 8, 6, wait_cursor_frame,360, 0, SKYBLUE );
        }
        else if( cursor_previous == cursor_default_names[cursor_default_type::wait_arrow] )
        {
            wait_arrow_cursor_frame += wait_arrow_cursor_frame_inc * time_keeper->get_delta_ticks();
            if( wait_arrow_cursor_frame > wait_arrow_cursor_frame_max )
            {
                wait_arrow_cursor_frame = 0;
            }

            DrawRing( mouse_vector, 8,6, 0, 360, 0, BLACK );
            DrawRing( mouse_vector, 8,6, wait_arrow_cursor_frame, 360, 0, MAROON );
        }
    }

    void cursor_controller_sfml::show_cusor( )
    {
        window_controller_main_sfml.window_sfml.setMouseCursorVisible( true );
        cursor_is_hidden = false;
    }
}
