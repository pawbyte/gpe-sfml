/*
gpe_renderer_sfml.cpp
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

#include "gpe_renderer_sfml.h"
#include "../gpe/gpe_constants.h"
#include "../gpe/gpe_globals.h"
#include "../gpe/gpe_settings.h"
#include "../other_libs/stg_ex.h"


namespace gpe
{
    renderer_system_sfml * renderer_main_sfml = nullptr;

    renderer_system_sfml::renderer_system_sfml( int rId, int wWidth, int wHeight )
    {
        for( int i_cam = 0; i_cam < max_cameras_allowed; i_cam++ )
        {
            sfml_2d_cameras[ i_cam] = { 0 };

            sfml_3d_cameras[ i_cam] = { 0 };
        }
        r_name = sfml_VERSION;
        r_name = "sfml_renderer " + r_name;
        r_name += "Opengl V" + stg_ex::int_to_string( rlGetVersion() );
        r_type = "sfml";

        render_blend_mode = blend_mode_blend;
        last_rendered_width = 0;
        last_rendered_height = 0;
        rendered_once = false;

        cleared_this_frame = false;
        r_width = wWidth;
        r_height = wHeight;
        last_screenshot_id = 0;
        supports_render_mode[ rmode_2d ] = true;
        supports_render_mode[ rmode_25d ] = true;
        supports_render_mode[ rmode_3d ] = true;
        supports_render_mode[ rmode_vr ] = true;
        scissor_mode_target = LoadRenderTexture( r_width, r_height );
        in_scissor_mode = false;

        scissor_mode_offset.x = 0;
        scissor_mode_offset.y = 0;

    }

    renderer_system_sfml::~renderer_system_sfml()
    {

    }

    bool renderer_system_sfml::begin_mode_2d()
    {
        EndMode3D();
        return true;
    }

    bool renderer_system_sfml::begin_mode_25d()
    {
        BeginMode3D( sfml_3d_cameras[0] );
        return true;
    }

    bool renderer_system_sfml::begin_mode_3d()
    {
        BeginMode3D( sfml_3d_cameras[0] );
        return true;
    }

    bool renderer_system_sfml::begin_mode_vr()
    {
        return IsVrSimulatorReady();
    }

    bool renderer_system_sfml::disable_scaling()
    {
        renderer_scaling = false;
        return false;
    }

    bool renderer_system_sfml::enable_scaling()
    {
        return true;
    }


    bool renderer_system_sfml::end_mode_2d()
    {
        //To us this means we jump back into 3D mode....
        BeginMode3D( sfml_3d_cameras[0] );
        return true;
    }

    bool renderer_system_sfml::end_mode_25d()
    {
        //2.5D to us is 3D
        EndMode3D();
        return true;
    }

    bool renderer_system_sfml::end_mode_3d()
    {
        EndMode3D();
        return true;
    }

    bool renderer_system_sfml::end_mode_vr()
    {
        return false; //We'll get to this later
    }

    int renderer_system_sfml::get_blend_mode()
    {
        return render_blend_mode;
    }



    void renderer_system_sfml::reset_input()
    {
        cleared_this_frame = false;
    }

    void renderer_system_sfml::reset_viewpoint()
    {
        if( in_scissor_mode )
        {
            rlPopMatrix();
            EndScissorMode();
        }
        render_sub_rectangle->x = 0;
        render_sub_rectangle->y = 0;
        render_sub_rectangle->w = 0;
        render_sub_rectangle->h = 0;
        scissor_mode_offset.x = 0;
        scissor_mode_offset.y = 0;
        in_scissor_mode = false;
    }

    void renderer_system_sfml::resize_renderer(int newW, int newH )
    {
        if( newW == r_width || newH == r_height)
        {
            return;
        }
        r_width = newW;
        r_height = newH;

        if( r_width != scissor_mode_target.texture.width || r_height != scissor_mode_target.texture.height )
        {
            if( scissor_mode_target.texture.id >= 0 || scissor_mode_target.texture.id != GetTextureDefault().id )
            {
                //UnloadRenderTexture( scissor_mode_target );
            }
            scissor_mode_target = LoadRenderTexture( r_width, r_height );
            error_log->report("Updating RenderTexture to "+stg_ex::int_to_string(r_width)+","+stg_ex::int_to_string(r_height)+"....");
            in_scissor_mode = false;
        }
    }

    void renderer_system_sfml::set_viewpoint( shape_rect * newViewPoint)
    {
        reset_viewpoint();

        if( newViewPoint!=nullptr)
        {
            render_sub_rectangle->x = newViewPoint->x;
            render_sub_rectangle->y = newViewPoint->y;
            render_sub_rectangle->w = newViewPoint->w;
            render_sub_rectangle->h = newViewPoint->h;

            scissor_mode_offset.x = 0; //newViewPoint->x;
            scissor_mode_offset.y = 0; //newViewPoint->y;
            if( render_sub_rectangle->w != 0 || render_sub_rectangle->h != 0 )
            {
                EndScissorMode();
                BeginScissorMode( render_sub_rectangle->x, render_sub_rectangle->y, render_sub_rectangle->w, render_sub_rectangle->h );
                in_scissor_mode = true;
                rlPushMatrix( );
                rlTranslatef(newViewPoint->x ,newViewPoint->y, 0 );

                return;
            }
        }
        in_scissor_mode = false;
        reset_viewpoint();
        render_sub_rectangle->x = 0;
        render_sub_rectangle->y = 0;
        render_sub_rectangle->w = 0;
        render_sub_rectangle->h = 0;
    }

    std::string renderer_system_sfml::get_renderer_name()
    {
        return r_name;
    }

    std::string renderer_system_sfml::get_renderer_type()
    {
        return r_type;
    }


    void renderer_system_sfml::clear_renderer( bool windowIsMinimized )
    {
        if( windowIsMinimized )
        {
            return;
        }
        uint32_t sTicks = time_keeper->get_ticks();
        set_render_blend_mode( blend_mode_blend );


        BeginDrawing();
        EndScissorMode();
        in_scissor_mode = false;
        ClearBackground( WHITE );
        BeginBlendMode( BLEND_ALPHA );

        uint32_t eTicks = time_keeper->get_ticks();
        error_log->log_ms_action("renderer_system_sfml::clear_renderer()",eTicks - sTicks,10 );
        return;

    }

    bool renderer_system_sfml::render_circle_color( int16_t x, int16_t y, int16_t rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        if( rad < 1)
        {
            return false;
        }
        render_current_color.r = r;
        render_current_color.g = g;
        render_current_color.b = b;
        render_current_color.a = a;
        DrawCircle( x + scissor_mode_offset.x , y + scissor_mode_offset.y, rad, render_current_color);
        return true;
    }

    void renderer_system_sfml::render_horizontal_line(int y, int x1, int x2)
    {
        DrawLine( x1 + scissor_mode_offset.x, y + scissor_mode_offset.y, x2 + scissor_mode_offset.x, y + scissor_mode_offset.y, render_current_color );
    }

    void renderer_system_sfml::render_horizontal_line_color( int y, int x1, int x2,  uint8_t r, uint8_t g, uint8_t b, uint8_t a )
    {
        render_current_color.r = r;
        render_current_color.g = g;
        render_current_color.b = b;
        render_current_color.a = a;
        DrawLine( x1 + scissor_mode_offset.x, y + scissor_mode_offset.y, x2 + scissor_mode_offset.x, y + scissor_mode_offset.y, render_current_color );

    }

    std::string renderer_system_sfml::save_screenshot(std::string file_location)
    {
        TakeScreenshot( file_location.c_str() );
        return file_location;
    }

    bool renderer_system_sfml::scale_renderer( int s_width, int s_height, bool scale_int )
    {
        return false;
    }

    bool renderer_system_sfml::scale_renderer_factor( float s_width, float s_height, bool scale_int )
    {
        return false;
    }

    bool renderer_system_sfml::screen_was_cleared()
    {
        return cleared_this_frame;
    }

    void renderer_system_sfml::set_render_blend_mode( int blend_mode_new )
    {
        if( render_blend_mode!=blend_mode_new)
        {
            render_blend_mode = blend_mode_new;
            EndBlendMode();
            switch( blend_mode_new)
            {
                case blend_mode_add:
                    BeginBlendMode( BLEND_ADDITIVE );
                break;

                case blend_mode_mod:
                    BeginBlendMode( BLEND_MULTIPLIED );
                break;

                case blend_mode_mul:
                    BeginBlendMode( BLEND_SUBTRACT_COLORS );
                break;

                case blend_mode_none:
                    BeginBlendMode( BLEND_ADD_COLORS );
                break;

                default:
                    BeginBlendMode( BLEND_ALPHA );
                    render_blend_mode = blend_mode_blend;
                break;
            }

        }
    }

    void renderer_system_sfml::update_renderer( bool windowIsMinimized )
    {
        uint32_t sTicks = time_keeper->get_ticks();
        last_rendered_width = r_width;
        last_rendered_height = r_height;
        if( windowIsMinimized == false )
        {
            set_render_blend_mode( blend_mode_blend );

            reset_viewpoint();

            cursor_main_controller->render();
            EndDrawing();

            cleared_this_frame = false;
            rendered_once = true;
        }
        uint32_t eTicks =  time_keeper->get_ticks();
        //error_log->log_ms_action("renderer_system_sfml::update_renderer()",eTicks - sTicks,10 );
    }
}
