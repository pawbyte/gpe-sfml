/*
gpe_font_sfml.h
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

#ifndef gpe_font_sfml_h
#define gpe_font_sfml_h

#include "../gpe/gpe_artist_base.h"
#include "../gpe/gpe_branch.h"
#include "../gpe/gpe_common_includes.h"
#include "../gpe/gpe_color.h"
#include "../gpe/gpe_font_base.h"
#include "../gpe/gpe_globals.h"
#include "../gpe/gpe_runtime.h"
#include "../gpe/gpe_shared_resources.h"
#include "../other_libs/stg_ex.h"
#include "gpe_renderer_sfml.h"
#include "gpe_texture_sfml.h"

#include <SFML/Window.hpp>


namespace gpe
{
    class font_pair_sfml: public font_pair_base
    {
        private:
            std::string str;
            sf::Texture * strTexture;
        public:
            int lastAlphaRendered;
            font_pair_sfml(sf::Font  fontIn, std::string str_in);
            ~font_pair_sfml();
            texture_sfml * get_texture();
    };

    class font_sfml_tff: public font_base
    {
        private:
            std::map < const std::string, font_pair_sfml * > font_text_pairs;
            std::map <const std::string, font_pair_sfml * > font_character_pairs;
            sf::Font sfml_held_font;
            sf::Color current_box_color;
            sf::Color current_font_color;
            sf::Vector2f current_font_position;
            sf::RectangleShape current_font_box;
        public:
            font_sfml_tff( const std::string file_loc, int f_size, bool make_monospaced = false, const std::string f_nickname = "", int id_number =-1);
            ~font_sfml_tff();
            void clear_cache();
            font_base * create_new(std::string file_loc, int f_size, bool make_monospaced = false, const std::string f_nickname = "", int id_number =-1);
            void get_metrics(std::string text_to_render, int * width_value, int *height_value);
            void get_numbered_metrics(std::string text_to_render, int * width_value, int *height_value);
            void get_wrapped_string_metrics( const std::string str_in, int line_width, int linePadding, int * width_value, int *height_value);
            sf::Font get_sfml_font();
            int get_cache_count();
            int get_font_id();
            font_pair_base * find_character_texture( const std::string id_number);
            font_pair_sfml * find_character_texture_sfml( const std::string id_number);

            font_pair_base * find_texture( const std::string text_to_render);
            font_pair_sfml * find_texture_sfml( const std::string text_to_render);
            void render_bitmapped_text( int x_pos, int y_pos, std::string number_to_render, color * text_color, int alignment_h=gpe::fa_top,int alignment_v=gpe::fa_top,int render_alpha = 255);
            void render_text( int x_pos, int y_pos, std::string text_to_render, color * text_color, int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top,int render_alpha = 255);
            void render_text_boxed( int x_pos, int y_pos, std::string text_to_render, color * text_color,color * boxColor,int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top,int render_alpha = 255);
            void render_text_scaled( int x_pos, int y_pos, std::string text_to_render, color * text_color, float text_scale, int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top, int render_alpha = 255);
            void render_text_resized( int x_pos, int y_pos, std::string text_to_render, color * text_color, int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top, int render_width = -1, int render_height = -1,int render_alpha = 255);
            void render_text_rotated( int x_pos, int y_pos, std::string text_to_render, color * text_color, float textAngle = 0,int render_alpha = 255);
            bool render_text_special( int x_pos, int y_pos, std::string text_to_render, color * text_color, int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top, float render_angle = 0, float render_scale = 1.f, int render_alpha = 255);

    };

    bool init_sfml_font_system();
    void quit_sfml_font_system();
}

#endif // gpe_font_sfml_h
