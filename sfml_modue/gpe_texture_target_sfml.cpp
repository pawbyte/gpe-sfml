/*
gpe_texture_target_sfml.cpp
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

#include "gpe_texture_target_sfml.h"

namespace gpe
{
    texture_target_sfml::texture_target_sfml()
    {
        textureType = "target-sfml";
        sfml_texture_target = LoadRenderTexture( screen_width, screen_height );
        texWid = screen_width;
        texHeight = screen_height;
        current_render_color.r = 255;
        current_render_color.g = 255;
        current_render_color.b = 255;
        current_render_color.a = 255;
    }

    texture_target_sfml::~texture_target_sfml()
    {
        if( sfml_texture_target.texture.id >= 0 || sfml_texture_target.texture.id != GetTextureDefault().id )
        {
            UnloadRenderTexture( sfml_texture_target );
        }
    }

    void texture_target_sfml::change_alpha( uint8_t alpha  )
    {
        lastAlphaRendered = alpha;
        current_render_color.a = alpha;
    }

    void texture_target_sfml::change_color( color * color_new)
    {
        if( color_new == nullptr)
        {
            return;
        }
        if( color_new->get_r() == currentR && color_new->get_g() == currentG && color_new->get_b() == currentB )
        {
                return;
        }
        currentR = color_new->get_r();
        currentG = color_new->get_g();
        currentB = color_new->get_b();

        current_render_color.r = currentR;
        current_render_color.g = currentG;
        current_render_color.b = currentB;
    }

    void texture_target_sfml::change_color( uint8_t red, uint8_t green, uint8_t blue )
    {
        if( red == currentR && green == currentG && blue == currentB )
        {
            return;
        }
        currentR = red;
        currentG = green;
        currentB = blue;

        current_render_color.r = red;
        current_render_color.g = green;
        current_render_color.b = blue;
    }

    renderer_system_sfml * texture_target_sfml::get_gpe_renderer_sfml(renderer_base * renderer)
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

    RenderTexture2D  texture_target_sfml::get_sfml_render_texture()
    {
        return sfml_texture_target;
    }

    Texture  texture_target_sfml::get_sfml_texture()
    {
        return sfml_texture_target.texture;
    }

    texture_base * texture_target_sfml::create_new()
    {
        return new texture_target_sfml();
    }

    void texture_target_sfml::render_overlay(  renderer_base * renderer,int x, int y, gpe::shape_rect* clip , int alpha  )
    {
        if( alpha == 0 ||  renderer == nullptr)
        {
            return;
        }
        renderer_system_sfml * sfmlRenderer = get_gpe_renderer_sfml(renderer);

        if( sfmlRenderer == nullptr)
        {
            return;
        }

        change_color( 255,255,255 );
        set_alpha( alpha );
        //Set clip rendering dimensions
        if( clip != nullptr )
        {
            sfml_target_source.x = clip->x;
            sfml_target_source.y = clip->y;
            sfml_target_source.width = clip->get_width();
            sfml_target_source.height = -clip->get_height();
        }
        else
        {
            sfml_target_source.x = 0;
            sfml_target_source.y = 0;
            sfml_target_source.width = sfml_texture_target.texture.width;
            sfml_target_source.height = -sfml_texture_target.texture.height;
        }

        int temp_blend = renderer_main->get_blend_mode();

        renderer_main->set_render_blend_mode( currentBlendMode );

        DrawTextureRec(sfml_texture_target.texture, sfml_target_source, (Vector2) { x, y }, current_render_color);
        EndBlendMode();

        //renderer_main->set_render_blend_mode( temp_blend );
    }

    void texture_target_sfml::render_overlay_scaled( renderer_base * renderer, int x, int y,float x_scale, float y_scale, gpe::shape_rect* clip, color * render_color, int alpha )
    {
        if( x_scale <= 0 || y_scale <= 0 || alpha == 0 || renderer == nullptr)
        {
            return;
        }
        renderer_system_sfml * sfmlRenderer = get_gpe_renderer_sfml(renderer);

        if( sfmlRenderer == nullptr)
        {
            return;
        }
        //sfml_Rect render_rect = { x, y, texWid, texHeight };

        change_color(render_color);
        set_alpha( alpha );

        int new_width = 0;
        int new_height = 0;
        bool flipHori = false,  flipVert = false;
        if( clip != nullptr )
        {
            sfml_target_source.x = clip->x;
            sfml_target_source.y = clip->y;
            sfml_target_source.width = clip->get_width();
            sfml_target_source.height = -clip->get_height();
        }
        else
        {
            sfml_target_source.x = 0;
            sfml_target_source.y = 0;
            sfml_target_source.width = sfml_texture_target.texture.width;
            sfml_target_source.height = -sfml_texture_target.texture.height;
        }

        sfml_target_dest.x = x;
        sfml_target_dest.y = y;
        sfml_target_dest.width = sfml_target_source.width * x_scale;
        sfml_target_dest.height = sfml_target_source.height * y_scale;

        sfml_target_origin.x = sfml_target_source.width;
        sfml_target_origin.y = sfml_target_source.height;

        int temp_blend = renderer_main->get_blend_mode();

        renderer_main->set_render_blend_mode( blend_mode_mod );

        DrawTextureRec(sfml_texture_target.texture, sfml_target_source, (Vector2) { x, y }, WHITE);

        DrawTexturePro(sfml_texture_target.texture, sfml_target_source, sfml_target_dest, sfml_target_origin, 0, current_render_color);
        EndBlendMode();

        //renderer_main->set_render_blend_mode( temp_blend );
    }

    void texture_target_sfml::resize_target(renderer_base * renderer, int w, int h, int id, bool useLinearScaling  )
    {
        if( w > 0 &&  h > 0)
        {
            UnloadRenderTexture( sfml_texture_target );
            sfml_texture_target = LoadRenderTexture( w, h);
            texWid = sfml_texture_target.texture.width;
            texHeight = sfml_texture_target.texture.height;
        }
    }


    void texture_target_sfml::set_blend_mode( int blend_mode_new)
    {
        if( currentBlendMode == blend_mode_new )
        {
            return;
        }
        currentBlendMode = blend_mode_new;
    }
}
