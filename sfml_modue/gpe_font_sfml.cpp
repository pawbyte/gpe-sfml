/*
gpe_font_sfml.cpp
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


#include "gpe_font_sfml.h"

namespace gpe
{
    font_pair_sfml::font_pair_sfml(Font fontIn,std::string str_in)
    {
        lastAlphaRendered = 255;
        str = str_in;
        strTexture = nullptr;
        text_width = 0;
        text_height = 0;
        if( (int)str_in.size() > 0)
        {
            //Render text surface
            Image font_img_pair = ImageTextEx( fontIn, str_in.c_str(), fontIn.baseSize, fontIn.charsPadding, WHITE );
            Texture2D font_pair_texture = LoadTextureFromImage( font_img_pair );
            //UnloadImage( font_img_pair );

            text_width = font_pair_texture.width;
            text_height = font_pair_texture.height;

            strTexture = new texture_sfml();
            strTexture->change_texture( font_pair_texture );
        }
    }

    font_pair_sfml::~font_pair_sfml()
    {
        if( strTexture!=nullptr)
        {
            delete strTexture;
            strTexture = nullptr;
        }
    }

    texture_sfml * font_pair_sfml::get_texture()
    {
        return strTexture;
    }

    font_sfml_tff::font_sfml_tff(std::string file_loc, int f_size, bool make_monospaced, std::string f_nickname,  int id_number )
    {
        font_system_type = "sfml_ttf";
        last_used_halign = fa_left;
        last_used_valign = fa_top;
        font_id = id_number;
        mono_width = 7;
        mono_height = 14;
        number_width = 7;
        number_height = 14;
        font_size = f_size;
        font_is_monospaced = make_monospaced;
        sfml_held_font = LoadFontEx( file_loc.c_str(), f_size,0, 0 );
        font_family_name = "";
        font_nickname = f_nickname;



        if( (int)file_loc.size() > 0 )
        {
            if( main_file_url_manager->file_exists( file_loc) )
            {
                gpe::error_log->report("Font-Update: Attempting to load font from ["+file_loc +"] file!" );
            }
            else
            {
                gpe::error_log->report("Font-Error: Unable to locate [" + file_loc + "]!" );
                return;
            }
        }
        else
        {
            return;
        }

        int i = 0;
        if (sfml_held_font.texture.id == GetFontDefault().texture.id && f_size > 0 )
        {
            gpe::error_log->report("Font-Error: [" + file_loc + "] returned with default font values!" );
        }
        else
        {
            GenTextureMipmaps(&sfml_held_font.texture);
        }

        gpe::error_log->report("Setting font values...");
        font_family_name = stg_ex::int_to_string(f_size)+"pt";
        std::string longCharstring = "";
        for( i = 0; i < 256; i++)
        {
            longCharstring+=i;
        }


        gpe::error_log->report("Finding monospace sizes of font");

        Vector2 sfml_text_sizes = MeasureTextEx(sfml_held_font, "W",font_size,sfml_held_font.charsPadding );
        number_width = mono_width = sfml_text_sizes.x;
        number_height = mono_height = sfml_text_sizes.y;

        gpe::error_log->report("Font Mono Sizes = ("+stg_ex::int_to_string(mono_width)+","+stg_ex::int_to_string(mono_height)+")" );
        /*
        font_pair_sfml * monoSpacedPair = find_texture_sfml("W");
        if( monoSpacedPair!=nullptr)
        {
            number_width = mono_width = monoSpacedPair->get_width();
            number_height = mono_height = monoSpacedPair->get_height();
        }

        for( i = 0; i < 10; i++)
        {
            font_character_pairs[ stg_ex::int_to_string(i)] =  new font_pair_sfml( sfml_held_font, stg_ex::int_to_string(i) );
        }

        font_character_pairs["." ] = new font_pair_sfml( sfml_held_font,"." );
        font_character_pairs["," ] =  new font_pair_sfml( sfml_held_font,"," );
        font_character_pairs["-" ] =  new font_pair_sfml( sfml_held_font,"-" );
        font_character_pairs["(" ] =  new font_pair_sfml( sfml_held_font,"(" );
        font_character_pairs[")" ] = new font_pair_sfml( sfml_held_font,")" );

        if( make_monospaced == false)
        {
            std::map<std::string,font_pair_sfml *>::iterator foundIterator;
            foundIterator = font_character_pairs.find("9");
            if( foundIterator!= font_character_pairs.end() )
            {
                font_pair_sfml * tempfont_pair_sfml = foundIterator->second;
                if( tempfont_pair_sfml!=nullptr )
                {
                    if( tempfont_pair_sfml->get_width() > number_width)
                    {
                        number_width = tempfont_pair_sfml->get_width();
                    }
                    if( tempfont_pair_sfml->get_height() > number_height)
                    {
                        number_height = tempfont_pair_sfml->get_height();
                    }
                }
            }
        }
        */
        gpe::error_log->report("Font [" + file_loc + "] loaded...");
    }

    font_sfml_tff::~font_sfml_tff()
    {
        clear_cache();
        font_pair_sfml * tempfont_pair_sfml = nullptr;
        for (std::map<std::string,font_pair_sfml *>::iterator it=font_character_pairs.begin(); it!=font_character_pairs.end(); ++it)
        {
            tempfont_pair_sfml = it->second;
            if( tempfont_pair_sfml!=nullptr)
            {
                delete tempfont_pair_sfml;
                tempfont_pair_sfml = nullptr;
            }
        }
        font_character_pairs.clear();
        UnloadFont( sfml_held_font );
    }

    void font_sfml_tff::clear_cache()
    {
        font_pair_sfml * tempfont_pair_sfml = nullptr;
        for (std::map<std::string,font_pair_sfml *>::iterator it=font_text_pairs.begin(); it!=font_text_pairs.end(); ++it)
        {
            tempfont_pair_sfml = it->second;
            if( tempfont_pair_sfml!=nullptr)
            {
                delete tempfont_pair_sfml;
                tempfont_pair_sfml = nullptr;
            }
        }
        font_text_pairs.clear();
    }

    font_base * font_sfml_tff::create_new(std::string file_loc, int f_size, bool make_monospaced , const std::string f_nickname , int id_number )
    {
        return new font_sfml_tff( file_loc, f_size, make_monospaced, f_nickname, id_number );
    }

    int font_sfml_tff::get_cache_count()
    {
        return (int)font_text_pairs.size();
    }

    int font_sfml_tff::get_font_id()
    {
        return font_id;
    }

    void font_sfml_tff::get_metrics(std::string text_to_render, int * width_value, int * height_value)
    {
        int tSize = (int)text_to_render.size();
        if( tSize > 0)
        {
            if( font_is_monospaced )
            {
                *width_value = mono_width*tSize;
                *height_value = mono_height;
            }
            else
            {
                Vector2 sfml_text_sizes = MeasureTextEx(sfml_held_font, text_to_render.c_str(),font_size,0 );
                *width_value = sfml_text_sizes.x;
                *height_value = sfml_text_sizes.y;

                /*
                font_pair_sfml *  newFoundPair = find_texture_sfml( text_to_render);
                if( newFoundPair!=nullptr)
                {
                    *width_value = newFoundPair->get_width();
                    *height_value = newFoundPair->get_height();
                }
                */
            }
        }
        else
        {
            *width_value = 0;
            *height_value = 0;
        }
    }

    void font_sfml_tff::get_numbered_metrics(std::string text_to_render, int * width_value, int * height_value)
    {
        int tSize = (int)text_to_render.size();
        if( tSize > 0)
        {
            *width_value = number_width*tSize;
            *height_value = number_height;
        }
        else
        {
            *width_value = 0;
            *height_value = 0;
        }
    }

    void font_sfml_tff::get_wrapped_string_metrics( const std::string str_in, int line_width, int linePadding, int * width_value, int *height_value )
    {
        if( (int)str_in.size() == 0 || line_width <=0 )
        {
            return;
        }
        std::vector < std::string > wrappedTextLine;
        stg_ex::wrap_string( str_in, wrappedTextLine, line_width);
        if( (int)wrappedTextLine.size() == 0  )
        {
            *width_value = 0;
            *height_value = 0;
        }
        else if( (int)wrappedTextLine.size() == 1 )
        {

        }
        else
        {

        }
    }

    Font font_sfml_tff::get_sfml_font()
    {
        return sfml_held_font;
    }

    font_pair_base * font_sfml_tff::find_character_texture( const std::string id_number )
    {
        return find_character_texture_sfml( id_number );
    }

    font_pair_sfml * font_sfml_tff::find_character_texture_sfml( const std::string id_number )
    {
        font_pair_sfml * fPair = nullptr;
        texture_sfml * fsfmlTexture = nullptr;
        std::map<std::string,font_pair_sfml *>::iterator foundVal;
        foundVal = font_character_pairs.find( id_number );
        if( foundVal !=  font_character_pairs.end() )
        {
            fPair = foundVal->second;
            if( fPair!=nullptr && fPair->texture_matches(id_number) )
            {
                return fPair;
            }
        }
        else
        {
            if( id_number!=" ")
            {
                fPair = new font_pair_sfml( sfml_held_font,id_number);
                font_character_pairs[id_number] = fPair;
                return fPair;
            }
        }
        return nullptr;
    }

    font_pair_base * font_sfml_tff::find_texture( const std::string text_to_render)
    {
        return find_texture_sfml( text_to_render );
    }

    font_pair_sfml * font_sfml_tff::find_texture_sfml( const std::string text_to_render)
    {
        font_pair_sfml * fPair = nullptr;
        texture_sfml * fsfmlTexture = nullptr;
        std::map< const std::string,font_pair_sfml *>::iterator foundVal;
        foundVal = font_text_pairs.find( text_to_render );
        if( foundVal !=  font_text_pairs.end() )
        {
            return foundVal->second;
        }
        else if( text_to_render!=" ")
        {
            fPair = new font_pair_sfml( sfml_held_font,text_to_render);
            font_text_pairs[text_to_render] = fPair;
            return fPair;
        }
        return nullptr;
    }

    void font_sfml_tff::render_bitmapped_text( int x_pos, int y_pos, std::string number_to_render, color * text_color, int alignment_h,int alignment_v, int render_alpha )
    {
        if( render_alpha < 1 || text_color==nullptr)
        {
            return;
        }
        if( number_to_render.size() == 0 )
        {
            return;
        }

        int numberSize = (int)number_to_render.size();

        switch(alignment_h)
        {
            case gpe::fa_center:
                x_pos=x_pos-(number_width*numberSize)/2;
                //last_used_halign=gpe::fa_center;
            break;

            case gpe::fa_right:
                x_pos=x_pos-(number_width*numberSize);
                // last_used_halign=gpe::fa_right;
            break;

            //rendering left will be the default
            default:
                // last_used_halign=gpe::fa_left;
            break;
        }
        switch(alignment_v)
        {
            case gpe::fa_middle:
                y_pos=y_pos-number_height/2;
                // last_used_valign=gpe::fa_middle;
            break;

            case gpe::fa_bottom:
                y_pos=y_pos-number_height;
                // last_used_valign=gpe::fa_middle;
            break;

            //rendering left will be the default
            default:
                //last_used_valign=gpe::fa_top;
            break;
        }
        font_pair_sfml * strTex = nullptr;
        texture_sfml * fPairTex = nullptr;
        int i = 0;
        std::string currCharacter = "";

        current_font_color.r = text_color->get_r();
        current_font_color.g = text_color->get_g();
        current_font_color.b = text_color->get_b();
        current_font_color.a = render_alpha;

        for( i = 0; i < numberSize; i++)
        {
            currCharacter = stg_ex::get_substring(number_to_render,i,1);

            current_font_position.x = x_pos + renderer_main_sfml->scissor_mode_offset.x;
            current_font_position.y = y_pos + renderer_main_sfml->scissor_mode_offset.y;
            DrawTextEx(sfml_held_font, currCharacter.c_str(), current_font_position, font_size, 0, current_font_color);

            /*
            strTex = find_character_texture_sfml( currCharacter );
            if( strTex!=nullptr)
            {
                fPairTex = strTex->get_texture();
                if( render_alpha > 255)
                {
                    render_alpha = 255;
                }
                if( fPairTex!=nullptr && render_alpha >0)
                {
                    int texWid = 0;
                    int texHeight =0;
                    //sfml_SetTextureColorMod( fPairTex, text_color->get_r(), text_color->get_g(), text_color->get_b() );

                    if( strTex->lastAlphaRendered!=render_alpha )
                    {
                        //sfml_SetTextureAlphaMod(fPairTex,render_alpha);
                        strTex->lastAlphaRendered = render_alpha;
                    }
                    //Get image dimensions
                    texWid = strTex->get_width();
                    texHeight = strTex->get_height();

                    if(alignment_h<gpe::fa_left || alignment_h<0)
                    {
                        alignment_h=last_used_halign;
                    }
                    if(alignment_v <gpe::fa_top || alignment_v<0)
                    {
                        alignment_v=last_used_valign;
                    }
                    //sfml_Rect clip = {x_pos, y_pos, texWid,texHeight};
                    //sfml_RenderCopy( renderer_main_sfml->get_gpe_renderer_sfml(),fPairTex, nullptr, &clip);
                    //sfml_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                }
            }
            */
            x_pos+=number_width;
        }
    }

    void font_sfml_tff::render_text( int x_pos, int y_pos, std::string text_to_render, color * text_color, int alignment_h,int alignment_v,int render_alpha )
    {
        if( render_alpha < 1 || text_color==nullptr)
        {
            //return;
        }
        if( (int)text_to_render.size() == 0 )
        {
            //return;
        }

        current_font_color.r = text_color->get_r();
        current_font_color.g = text_color->get_g();
        current_font_color.b = text_color->get_b();
        current_font_color.a = render_alpha;




        int texWid = 0, texHeight = 0;
        get_metrics( text_to_render, &texWid, &texHeight );

        if(alignment_h<gpe::fa_left || alignment_h > gpe::fa_right )
        {
            alignment_h=last_used_halign;
        }

        if(alignment_v < gpe::fa_top || alignment_v > gpe::fa_bottom )
        {
            alignment_v=last_used_valign;
        }
        switch(alignment_h)
        {
        case gpe::fa_center:
            x_pos=x_pos-texWid/2;
            //last_used_halign=gpe::fa_center;
            break;

        case gpe::fa_right:
            x_pos=x_pos-texWid;
            // last_used_halign=gpe::fa_right;
            break;

        //rendering left will be the default
        default:
            // last_used_halign=gpe::fa_left;
            break;
        }
        switch(alignment_v)
        {
            case gpe::fa_middle:
                y_pos=y_pos-texHeight/2;
                // last_used_valign=gpe::fa_middle;
            break;

            case gpe::fa_bottom:
                y_pos=y_pos-texHeight;
                // last_used_valign=gpe::fa_middle;
            break;

            //rendering left will be the default
            default:
                //last_used_valign=gpe::fa_top;
            break;
        }
        current_font_position.x = x_pos + renderer_main_sfml->scissor_mode_offset.x;
        current_font_position.y = y_pos + renderer_main_sfml->scissor_mode_offset.y;
        DrawTextEx(sfml_held_font, text_to_render.c_str(), current_font_position, font_size, 0, current_font_color);
    }

    void font_sfml_tff::render_text_scaled( int x_pos, int y_pos, std::string text_to_render, color * text_color, float text_scale, int alignment_h,int alignment_v, int render_alpha )
    {
        //Incompleted
    }

    void font_sfml_tff::render_text_resized( int x_pos, int y_pos, std::string text_to_render, color * text_color, int alignment_h,int alignment_v, int render_width, int render_height, int render_alpha )
    {
        if( render_alpha < 1 || text_color==nullptr)
        {
            return;
        }
        if( (int)text_to_render.size() == 0 )
        {
            return;
        }

        current_font_color.r = text_color->get_r();
        current_font_color.g = text_color->get_g();
        current_font_color.b = text_color->get_b();
        current_font_color.a = render_alpha;

        int texWid = 0, texHeight = 0;
        get_metrics( text_to_render, &texWid, &texHeight ); //Get font render dimensions

        if( render_width <= 0 )
        {
            render_width = texWid + mono_width;
        }

        if( render_height <= 0 )
        {
            render_height = texHeight;
        }

        if( texWid <= 0 )
        {
            return;
        }
        else
        {
            texWid = render_width;
        }

        if( texHeight <= 0 )
        {
            return;
        }
        else
        {
            texHeight = render_height;
        }

        if( texWid > render_width )
        {
            texWid = render_width;
        }

        if( texHeight > render_height )
        {
            texHeight = render_height;
        }

        if(alignment_h<gpe::fa_left || alignment_h > gpe::fa_right )
        {
            alignment_h=last_used_halign;
        }

        if(alignment_v < gpe::fa_top || alignment_v > gpe::fa_bottom )
        {
            alignment_v=last_used_valign;
        }
        switch(alignment_h)
        {
            case gpe::fa_center:
                x_pos=x_pos-texWid/2;
                //last_used_halign=gpe::fa_center;
                break;

            case gpe::fa_right:
                x_pos=x_pos-texWid;
                // last_used_halign=gpe::fa_right;
                break;

            //rendering left will be the default
            default:
                // last_used_halign=gpe::fa_left;
                break;
        }
        switch(alignment_v)
        {
            case gpe::fa_middle:
                y_pos=y_pos-texHeight/2;
                // last_used_valign=gpe::fa_middle;
                break;

            case gpe::fa_bottom:
                y_pos=y_pos-texHeight;
                // last_used_valign=gpe::fa_middle;
                break;

            //rendering left will be the default
            default:
                //last_used_valign=gpe::fa_top;
                break;
        }

        current_font_box.x = x_pos + renderer_main_sfml->scissor_mode_offset.x;
        current_font_box.y = y_pos + renderer_main_sfml->scissor_mode_offset.y;
        current_font_box.width = render_width;
        current_font_box.height = render_height;

        DrawTextRec( sfml_held_font, text_to_render.c_str(),current_font_box, font_size, 0, false, current_font_color );
    }

    void font_sfml_tff::render_text_boxed( int x_pos, int y_pos, std::string text_to_render, color * text_color,color * boxColor,int alignment_h,int alignment_v, int render_alpha )
    {
        if( render_alpha < 1 || text_color==nullptr || boxColor == nullptr )
        {
            return;
        }
        if( (int)text_to_render.size() == 0 )
        {
            return;
        }

        current_box_color.r = boxColor->get_r();
        current_box_color.g = boxColor->get_g();
        current_box_color.b = boxColor->get_b();
        current_box_color.a = render_alpha;

        current_font_color.r = text_color->get_r();
        current_font_color.g = text_color->get_g();
        current_font_color.b = text_color->get_b();
        current_font_color.a = render_alpha;

        int texWid = 0, texHeight = 0;
        get_metrics( text_to_render, &texWid, &texHeight ); //Get font render dimensions

        if(alignment_h<gpe::fa_left || alignment_h > gpe::fa_right )
        {
            alignment_h=last_used_halign;
        }

        if(alignment_v < gpe::fa_top || alignment_v > gpe::fa_bottom )
        {
            alignment_v=last_used_valign;
        }
        switch(alignment_h)
        {
            case gpe::fa_center:
                x_pos=x_pos-texWid/2;
                //last_used_halign=gpe::fa_center;
                break;

            case gpe::fa_right:
                x_pos=x_pos-texWid;
                // last_used_halign=gpe::fa_right;
                break;

            //rendering left will be the default
            default:
                // last_used_halign=gpe::fa_left;
                break;
        }
        switch(alignment_v)
        {
            case gpe::fa_middle:
                y_pos=y_pos-texHeight/2;
                // last_used_valign=gpe::fa_middle;
                break;

            case gpe::fa_bottom:
                y_pos=y_pos-texHeight;
                // last_used_valign=gpe::fa_middle;
                break;

            //rendering left will be the default
            default:
                //last_used_valign=gpe::fa_top;
                break;
        }

        current_font_position.x = x_pos;
        current_font_position.y = y_pos;
        DrawRectangle(current_font_position.x - sfml_held_font.charsPadding*2, current_font_position.y - sfml_held_font.charsPadding*2, texWid + sfml_held_font.charsPadding*4, texHeight + sfml_held_font.charsPadding*4, current_box_color );
        DrawTextEx( sfml_held_font, text_to_render.c_str(), current_font_position, font_size, 0, current_font_color );
    }

    void font_sfml_tff::render_text_rotated( int x_pos, int y_pos, std::string text_to_render, color * text_color, float textAngle, int render_alpha )
    {
        if( render_alpha < 1 || text_color==nullptr)
        {
            return;
        }
        if( (int)text_to_render.size() == 0 )
        {
            return;
        }

        current_font_color.r = text_color->get_r();
        current_font_color.g = text_color->get_g();
        current_font_color.b = text_color->get_b();
        current_font_color.a = render_alpha;

        int texWid = 0, texHeight = 0;
        get_metrics( text_to_render, &texWid, &texHeight ); //Get font render dimensions
        rlPushMatrix( );
        rlTranslatef( x_pos + renderer_main_sfml->scissor_mode_offset.x, y_pos + renderer_main_sfml->scissor_mode_offset.y , 0 );
        rlRotatef( -textAngle  , 0,0,1 );

        //draw text normal call
        current_font_position.x = 0;
        current_font_position.y = 0;
        DrawTextEx(sfml_held_font, text_to_render.c_str(), current_font_position, font_size, 0, current_font_color);
        rlPopMatrix();

    }

    bool font_sfml_tff::render_text_special( int x_pos, int y_pos, std::string text_to_render, color * text_color, int alignment_h,int alignment_v, float render_angle , float render_scale, int render_alpha )
    {
        if( render_alpha > 0)
        {
            font_pair_sfml * strTex = find_texture_sfml(text_to_render);
            if( strTex!=nullptr)
            {
                texture_sfml * fPairTex = strTex->get_texture();
                if( fPairTex!=nullptr)
                {
                    int texWid = 0;
                    int texHeight =0;
                    //sfml_SetTextureColorMod( fPairTex, text_color->get_r(), text_color->get_g(), text_color->get_b() );
                    if( strTex->lastAlphaRendered!=render_alpha )
                    {
                        strTex->lastAlphaRendered = render_alpha;
                    }
                    //Get image dimensions
                    texWid = (float)strTex->get_width() * render_scale;
                    texHeight = (float)strTex->get_height() * render_scale;

                    //Get image dimensions
                    if( texWid < 0)
                    {
                        texWid *=-1;
                        render_angle *=-1;
                    }

                    if( texHeight<0 )
                    {
                        texHeight *=-1;
                        render_angle *=-1;
                    }

                    if(alignment_h<gpe::fa_left || alignment_h > gpe::fa_right )
                    {
                        alignment_h=last_used_halign;
                    }

                    if(alignment_v < gpe::fa_top || alignment_v > gpe::fa_bottom )
                    {
                        alignment_v=last_used_valign;
                    }
                    switch(alignment_h)
                    {
                    case gpe::fa_center:
                        x_pos=x_pos-texWid/2;
                        //last_used_halign=gpe::fa_center;
                        break;

                    case gpe::fa_right:
                        x_pos=x_pos-texWid;
                        // last_used_halign=gpe::fa_right;
                        break;

                    //rendering left will be the default
                    default:
                        // last_used_halign=gpe::fa_left;
                        break;
                    }
                    switch(alignment_v)
                    {
                    case gpe::fa_middle:
                        y_pos=y_pos-texHeight/2;
                        // last_used_valign=gpe::fa_middle;
                        break;

                    case gpe::fa_bottom:
                        y_pos=y_pos-texHeight;
                        // last_used_valign=gpe::fa_middle;
                        break;

                    //rendering left will be the default
                    default:
                        //last_used_valign=gpe::fa_top;
                        break;
                    }

                    //Incompleted
                    return true;
                }
            }
        }
        return false;
    }

    bool init_sfml_font_system()
    {
        gpe::error_log->report("Starting sfml_module font system... " );

        //If we already started the font system do nothing.
        if( gfs!=nullptr )
        {
            delete gfs;
            gfs = nullptr;
        }


        gfs = new font_system_controller();
        gfs->font_template = new font_sfml_tff("",0,false,"",-1 );
        gpe::error_log->report("sfml_module font system started...");
        return true;
    }

    void quit_sfml_font_system()
    {
        gpe::error_log->report("Quitting sfml_module font system....");

        if( gfs!=nullptr )
        {
            delete gfs;
            gfs = nullptr;
        }
        gfs = new font_system_controller();
        gpe::error_log->report("sfml_module font system quit...");
    }
}
