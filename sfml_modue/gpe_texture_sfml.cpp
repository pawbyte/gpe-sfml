/*
gpe_texture_sfml.cpp
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


#include "gpe_texture_sfml.h"
#include "gpe_renderer_sfml.h"

namespace gpe
{
    texture_sfml::texture_sfml()
    {
        textureType = "sfml";
        img_sfml.id = -1;
        img_sfml.width = 0;
        img_sfml.height = 0;
    }

    texture_sfml::~texture_sfml()
    {
        if( img_sfml.id > 0 && img_sfml.id != GetTextureDefault().id )
        {
            UnloadTexture(img_sfml );
        }
    }

    void texture_sfml::change_alpha( uint8_t alpha  )
    {

    }

    void texture_sfml::change_color( color * color_new)
    {
        if( color_new!=nullptr)
        {
            if( color_new->get_r() == currentR && color_new->get_g() == currentG && color_new->get_b() == currentB )
            {
                return;
            }
            //sfml_SetTextureColorMod( texImg, color_new->get_r(), color_new->get_g(), color_new->get_b() );
            currentR = color_new->get_r();
            currentG = color_new->get_g();
            currentB = color_new->get_b();
        }
        else if( currentR != 255 &&  currentG != 255 &&  currentB != 255 )
        {
            //sfml_SetTextureColorMod( texImg, 255,255,255 );
            currentR = 255;
            currentG = 255;
            currentB = 255;
        }
    }

    void texture_sfml::change_color( uint8_t red, uint8_t green, uint8_t blue )
    {
        if( red == currentR && green == currentG && blue == currentB )
        {
            return;
        }
        //sfml_SetTextureColorMod( texImg, red, green, blue );
        currentR = red;
        currentG = green;
        currentB = blue;
    }

    void texture_sfml::change_texture( Texture2D newTexture )
    {
        if( newTexture.id <= 0 || img_sfml.id == GetTextureDefault().id )
        {
            return;
        }

        texWid = 0;
        texHeight = 0;


        if( img_sfml.id != GetTextureDefault().id  && img_sfml.id > 0 )
        {
            UnloadTexture( img_sfml );
        }
        img_sfml = newTexture;
        texWid = newTexture.width;
        texHeight = newTexture.height;
        currentBlendMode = blend_mode_blend;
    }

    bool texture_sfml::copy_image_source(std::string directory_output_name)
    {
        if( get_width()>0 )
        {
            std::string copyDestinationStr = directory_output_name+"/"+ stg_ex::get_short_filename(fileLocation,true);
            return main_file_url_manager->file_copy(fileLocation,copyDestinationStr );
        }
        return false;
    }

    texture_base * texture_sfml::create_new()
    {
        return new texture_sfml();
    }

    renderer_system_sfml * texture_sfml::get_gpe_renderer_sfml(renderer_base * renderer)
    {
        if( renderer == nullptr)
        {
            return nullptr;
        }
        if( renderer->get_renderer_type() == "sfml")
        {
            renderer_system_sfml * sfmlRenderer = (renderer_system_sfml * )renderer;
            return sfmlRenderer;
        }
        return nullptr;
    }

    void texture_sfml::load_new_texture( renderer_base * renderer,std::string file_name, int id, bool transparent, bool useLinearScaling )
    {
        texId=id;
        isTransparent = transparent;
        //The image that's loaded
        if( main_file_url_manager->file_exists(file_name ) == false )
        {
            //error_log->report("[Bad] Unable to load filed  <"+file_name+">. Error: FILE_NOT_FOUND.");
            fileLocation = "-1";
            return;
        }

        if( stg_ex::file_is_image( file_name) == false )
        {
           error_log->report("[NOBUENO] Unable to load filed  <"+file_name+">. Error: Not an Image.");
           fileLocation = "-2";
           return;
        }
        currentBlendMode = blend_mode_blend;


        fileLocation = file_name;

        //Load as an image first since gpu loading maybe delayed
        Image temp_image = LoadImage( file_name.c_str() );

        //If the image loaded
        if( temp_image.width == 0 || temp_image.height == 0 )
        {
            texWid = 0;
            texHeight = 0;
            fileLocation ="notfound.png";
            error_log->report("[SADNESS] Unable to load filed located at <"+file_name+">...");
            return;
        }

        texWid = temp_image.width;
        texHeight = temp_image.height;
        fileLocation = file_name;
        Texture2D temp_texture_img = LoadTextureFromImage( temp_image );

        if( temp_texture_img.id <= 0 || temp_texture_img.id == GetTextureDefault().id )
        {
            error_log->report("[sfml-TEXTURE FAILED] Unable to load filed located at <"+file_name+"> attempting secondary function...");
            temp_texture_img = LoadTexture( file_name.c_str() );
        }

        if( temp_texture_img.id <= 0 || temp_texture_img.id == GetTextureDefault().id )
        {
            error_log->report("[sfml-TEXTURE FAILED] Image <"+file_name+"> failed to load into GPU...");
            return;
        }
        UnloadImage(  temp_image );
        if( img_sfml.id > 0 || img_sfml.id != GetTextureDefault().id )
        {
            UnloadTexture(  img_sfml );
        }
        img_sfml = temp_texture_img;
    }

    Texture2D texture_sfml::get_sfml_texture()
    {
        return img_sfml;
    }

    void texture_sfml::prerender_circle( renderer_base * renderer, int rad, color * circleColor,   uint8_t alpha, int id, bool transparent, bool useLinearScaling, bool isOutline )
    {

    }

    void texture_sfml::prerender_triangle( renderer_base * renderer, shape_triangle2d,  color * circleColor,  uint8_t alpha )
    {

    }

    void texture_sfml::prerender_rectangle( renderer_base * renderer, int w, int h, color * color_new, int id, bool transparent, bool useLinearScaling , bool isOutline)
    {
        if( renderer == nullptr)
        {
            return;
        }
    }

    void texture_sfml::render_align(  int x, int y, int alignment_h, int alignment_v, gpe::shape_rect* clip, color * render_color, int alpha )
    {
        if(alignment_h==gpe::fa_center)
        {
            x-=get_width()/2;
        }
        else if( alignment_h==gpe::fa_right)
        {
            x-= get_width();
        }

        if(alignment_v==gpe::fa_middle)
        {
            y-= get_height()/2;
        }
        else if( alignment_v==gpe::fa_bottom)
        {
            y-= get_height();
        }
        render_tex_colored( x,y, render_color, alpha, clip );
    }

    void texture_sfml::render_align_resized( int x, int y, int new_width, int new_height,  int alignment_h, int alignment_v, gpe::shape_rect* clip,color * render_color, int alpha )
    {
        if(alignment_h==gpe::fa_center)
        {
            x-=new_width/2;
        }
        else if( alignment_h==gpe::fa_right)
        {
            x-= new_width;
        }

        if(alignment_v==gpe::fa_middle)
        {
            y-= new_height/2;
        }
        else if( alignment_v==gpe::fa_bottom)
        {
            y-= new_height;
        }
        render_tex_resized( x,y,new_width, new_height,clip, render_color, alpha );
    }

    void texture_sfml::render_tex(  int x, int y, gpe::shape_rect* clip , int alpha)
    {
        if( alpha < 1 )
        {
            return;
        }

        current_texture_tint.r = currentR;
        current_texture_tint.g = currentG;
        current_texture_tint.b = currentB;
        current_texture_tint.a = alpha;

        current_texture_position.x = x + renderer_main_sfml->scissor_mode_offset.x;
        current_texture_position.y = y+ renderer_main_sfml->scissor_mode_offset.y;
        if( clip !=nullptr )
        {
            DrawTextureRec( img_sfml, current_texture_clip,current_texture_position, current_texture_tint);
        }
        else
        {
            DrawTexture( img_sfml, x + + renderer_main_sfml->scissor_mode_offset.x, y + + renderer_main_sfml->scissor_mode_offset.y,  current_texture_tint );
        }

        if( debug_mode_active && debug_texture_renders )
        {
            std::string render_descripton = "Size:"+stg_ex::int_to_string(texWid)+","+stg_ex::int_to_string(texHeight);
            DrawText( render_descripton.c_str(),x,y,12,GOLD );
        }
    }

    void texture_sfml::render_tex_colored(  int x, int y, color * render_color, int alpha , gpe::shape_rect* clip   )
    {
        if( alpha > 0 )
        {
            change_color(render_color);
            set_alpha( alpha );
            //Set clip rendering dimensions
            render_tex( x,y, clip, alpha);
        }
    }

    void texture_sfml::render_tex_resized(  int x, int y, float new_width, float new_height, gpe::shape_rect* clip, color * render_color, int alpha )
    {
        if( alpha > 0 )
        {
            if( clip != nullptr )
            {
                if( clip->w !=0 && clip->h!= 0 )
                {
                    float foundX_scale = new_width/clip->w;
                    float foundY_scale = new_height/clip->h;
                    render_tex_scaled( x, y, foundX_scale, foundY_scale,clip, render_color,alpha);
                }
            }
            else
            {
                if( texWid !=0 && texHeight!= 0 )
                {
                    float foundX_scale = new_width/(float)texWid;
                    float foundY_scale = new_height/(float)texHeight;
                    render_tex_scaled( x, y, foundX_scale, foundY_scale,clip, render_color,alpha);
                }
            }
            return;
        }
    }

    void texture_sfml::render_tex_scaled(  int x, int y, float x_scale, float y_scale, gpe::shape_rect* clip, color * render_color, int alpha )
    {
        if( x_scale == 0 || y_scale == 0 )
        {
            return;
        }

        if( x_scale < 0 )
        {
            x_scale = 1.f;
        }

        if( y_scale < 0 )
        {
            y_scale = 1.f;
        }

        if( render_color != nullptr )
        {
            current_texture_tint.r = render_color->get_r();
            current_texture_tint.g = render_color->get_g();
            current_texture_tint.b = render_color->get_b();
        }
        else
        {
            current_texture_tint.r = currentR;
            current_texture_tint.g = currentG;
            current_texture_tint.b = currentB;
        }
        current_texture_tint.a = alpha;



        if( clip != nullptr )
        {
            current_texture_clip.x = clip->x;
            current_texture_clip.y = clip->y;
            current_texture_clip.width = clip->w;
            current_texture_clip.height = clip->h;
        }
        else
        {
            current_texture_clip.x = 0;
            current_texture_clip.y = 0;
            current_texture_clip.width = img_sfml.width;
            current_texture_clip.height = img_sfml.height;
        }
        current_texture_dest.x = x + current_texture_clip.width;
        current_texture_dest.y = y + current_texture_clip.height;
        current_texture_dest.width = current_texture_clip.width * x_scale;
        current_texture_dest.height = current_texture_clip.height * y_scale;

        current_texture_rotation_origin.x = current_texture_clip.width;
        current_texture_rotation_origin.y = current_texture_clip.height;

        DrawTexturePro( img_sfml, current_texture_clip, current_texture_dest, current_texture_rotation_origin, 0, current_texture_tint );

        if( debug_mode_active && debug_texture_renders )
        {
            std::string render_descripton = "Size:"+stg_ex::float_to_string(current_texture_dest.x)+","+stg_ex::float_to_string(current_texture_dest.y);
            DrawText( render_descripton.c_str(),x,y,12,WHITE);
        }
    }

    void texture_sfml::render_tex_rotated(  int x, int y, float render_angle, color * render_color, gpe::shape_rect* clip , int alpha )
    {
        if( alpha <= 1 )
        {
            return;
        }

        if( render_color != nullptr )
        {
            current_texture_tint.r = render_color->get_r();
            current_texture_tint.g = render_color->get_g();
            current_texture_tint.b = render_color->get_b();
        }
        else
        {
            current_texture_tint.r = currentR;
            current_texture_tint.g = currentG;
            current_texture_tint.b = currentB;
        }
        current_texture_tint.a = alpha;



        if( clip !=nullptr )
        {
            current_texture_clip.x = clip->x;
            current_texture_clip.y = clip->y;
            current_texture_clip.width = clip->w;
            current_texture_clip.height = clip->h;
        }
        else
        {
            current_texture_clip.x = 0;
            current_texture_clip.y = 0;
            current_texture_clip.width = img_sfml.width;
            current_texture_clip.height = img_sfml.height;
        }

        current_texture_dest.x = x + current_texture_clip.width;
        current_texture_dest.y = y + current_texture_clip.height;
        current_texture_dest.width = current_texture_clip.width;
        current_texture_dest.height = current_texture_clip.height;

        current_texture_rotation_origin.x = current_texture_clip.width;
        current_texture_rotation_origin.y = current_texture_clip.height;

        DrawTexturePro( img_sfml, current_texture_clip, current_texture_dest, current_texture_rotation_origin, render_angle * semath::math_degrees_multiplier, current_texture_tint );
        if( debug_mode_active && debug_texture_renders )
        {
            std::string render_descripton = "Size:"+stg_ex::int_to_string(texWid)+","+stg_ex::int_to_string(texHeight);
            DrawText( render_descripton.c_str(),x,y,12, RED);
        }
    }

    void texture_sfml::render_tex_rotated_at_point(  int x, int y, float render_angle, int point_x, int point_y, color * render_color, gpe::shape_rect* clip , int alpha )
    {
        if( alpha <= 1 )
        {
            return;
        }

        if( render_color != nullptr )
        {
            current_texture_tint.r = render_color->get_r();
            current_texture_tint.g = render_color->get_g();
            current_texture_tint.b = render_color->get_b();
        }
        else
        {
            current_texture_tint.r = currentR;
            current_texture_tint.g = currentG;
            current_texture_tint.b = currentB;
        }
        current_texture_tint.a = alpha;



        if( clip !=nullptr )
        {
            current_texture_clip.x = clip->x;
            current_texture_clip.y = clip->y;
            current_texture_clip.width = clip->w;
            current_texture_clip.height = clip->h;
        }
        else
        {
            current_texture_clip.x = 0;
            current_texture_clip.y = 0;
            current_texture_clip.width = img_sfml.width;
            current_texture_clip.height = img_sfml.height;
        }
        current_texture_dest.x = x + point_x;
        current_texture_dest.y = y + point_y;
        current_texture_dest.width = current_texture_clip.width;
        current_texture_dest.height = current_texture_clip.height;

        current_texture_rotation_origin.x = point_x;
        current_texture_rotation_origin.y = point_y;
        DrawTexturePro( img_sfml, current_texture_clip, current_texture_dest, current_texture_rotation_origin, render_angle * semath::math_degrees_multiplier, current_texture_tint );

        if( debug_mode_active && debug_texture_renders )
        {
            std::string render_descripton = "Size:"+stg_ex::int_to_string(texWid)+","+stg_ex::int_to_string(texHeight);
            DrawText( render_descripton.c_str(),x,y,12,BLUE);
        }
    }

    void texture_sfml::render_tex_special(  int x, int y, float render_angle, int new_width, int new_height, color * render_color, gpe::shape_rect* clip , int alpha )
    {
        if( alpha <= 0 )
        {
            return;
        }

        if( new_width <=0.f)
        {
            new_width = texWid;
        }
        if( new_height<=0.f)
        {
            new_height = texHeight;
        }

        if( render_color != nullptr )
        {
            current_texture_tint.r = render_color->get_r();
            current_texture_tint.g = render_color->get_g();
            current_texture_tint.b = render_color->get_b();
        }
        else
        {
            current_texture_tint.r = currentR;
            current_texture_tint.g = currentG;
            current_texture_tint.b = currentB;
        }
        current_texture_tint.a = alpha;



        if( clip !=nullptr )
        {
            current_texture_clip.x = clip->x;
            current_texture_clip.y = clip->y;
            current_texture_clip.width = clip->w;
            current_texture_clip.height = clip->h;
        }
        else
        {
            current_texture_clip.x = 0;
            current_texture_clip.y = 0;
            current_texture_clip.width = img_sfml.width;
            current_texture_clip.height = img_sfml.height;
        }
        current_texture_dest.x = x + new_width;
        current_texture_dest.y = y + new_height;
        current_texture_dest.width = new_width;
        current_texture_dest.height = new_height;

        current_texture_rotation_origin.x = current_texture_clip.width/2;
        current_texture_rotation_origin.y = current_texture_clip.height/2;
        DrawTexturePro( img_sfml, current_texture_clip, current_texture_dest, current_texture_rotation_origin, render_angle * semath::math_degrees_multiplier, current_texture_tint );

        if( debug_mode_active && debug_texture_renders )
        {
            std::string render_descripton = "Size:"+stg_ex::int_to_string(texWid)+","+stg_ex::int_to_string(texHeight);
            DrawText( render_descripton.c_str(),x,y,12,PINK);
        }
    }

    void texture_sfml::render_tex_special_at_point(  int x, int y, float render_angle, int point_x, int point_y,int new_width, int new_height, color * render_color, gpe::shape_rect* clip , int alpha )
    {
        if(alpha <= 0 )
        {
            return;
        }
        bool flipHori = false,  flipVert = false;
        if( new_width < 0)
        {
            flipHori = true;
            new_width*=-1;
        }
        if( new_height < 0)
        {
            flipVert = true;
            new_height*=-1;
        }
        if( new_width ==0)
        {
            new_width = texWid;
        }
        if( new_height==0)
        {
            new_height = texHeight;
        }

                if( render_color != nullptr )
        {
            current_texture_tint.r = render_color->get_r();
            current_texture_tint.g = render_color->get_g();
            current_texture_tint.b = render_color->get_b();
        }
        else
        {
            current_texture_tint.r = currentR;
            current_texture_tint.g = currentG;
            current_texture_tint.b = currentB;
        }
        current_texture_tint.a = alpha;



        if( clip !=nullptr )
        {
            current_texture_clip.x = clip->x;
            current_texture_clip.y = clip->y;
            current_texture_clip.width = clip->w;
            current_texture_clip.height = clip->h;
        }
        else
        {
            current_texture_clip.x = 0;
            current_texture_clip.y = 0;
            current_texture_clip.width = img_sfml.width;
            current_texture_clip.height = img_sfml.height;
        }
        current_texture_dest.x = x + point_x;
        current_texture_dest.y = y + point_y;
        current_texture_dest.width = new_width;
        current_texture_dest.height = new_height;

        current_texture_rotation_origin.x = point_x;
        current_texture_rotation_origin.y = point_y;
        DrawTexturePro( img_sfml, current_texture_clip, current_texture_dest, current_texture_rotation_origin, render_angle * semath::math_degrees_multiplier, current_texture_tint );

        if( debug_mode_active && debug_texture_renders )
        {
            std::string render_descripton = "Size:"+stg_ex::int_to_string(texWid)+","+stg_ex::int_to_string(texHeight);
            DrawText( render_descripton.c_str(),x,y,12,ORANGE );
        }
    }

    void texture_sfml::set_alpha( int alpha )
    {
        if( alpha < 0 )
        {
            alpha = 0;
        }
        else if( alpha > 255)
        {
            alpha = 255;
        }
        if( lastAlphaRendered!=alpha )
        {
            lastAlphaRendered = alpha;
        }
    }

    void texture_sfml::set_blend_mode( int blend_mode_new)
    {
        if( currentBlendMode == blend_mode_new )
        {
            return;
        }
        currentBlendMode = blend_mode_new;
    }
}
