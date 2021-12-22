/*
gpe_audio_sfml.cpp
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

#include "gpe_audio_sfml.h"
#include "../gpe/gpe_error_logger.h"

namespace gpe
{
    bool init_sfml_audio_system()
    {
        error_log->report("-audio_sfml_system...");
        // load support for the OGG and MOD sample/music formats

        //Initialize sfml_audio system
        //SFML doesnt have an actual function so lets just manually assign types

        sound_is_working = true;
        gpe::error_log->report("sfml audio device started successfully....");

        bool at_least_one_type_supported = false;
        int i_format = 0;
        for( i_format = 0; i_format < sound_format_max; i_format++ )
        {
            sound_is_format_supported[ i_format ] = false;
        }

        sound_is_format_supported[ sound_format_acc ] = true;
        sound_is_format_supported[ sound_format_mod ] = true;
        sound_is_format_supported[ sound_format_mp3 ] = false;
        sound_is_format_supported[ sound_format_ogg ] = true;
        sound_is_format_supported[ sound_format_acc ] = true;
        sound_is_format_supported[ sound_format_wav ] = true;
        sound_is_format_supported[ sound_format_xm ] = false;


        for( i_format = 0; i_format < sound_format_max; i_format++ )
        {
            if( sound_is_format_supported[ i_format ] )
            {
                at_least_one_type_supported = true;
            }
        }

        sound_is_working = at_least_one_type_supported;

        if( sound_music_object != nullptr )
        {
            delete sound_music_object;
            sound_music_object = nullptr;
        }

        sound_music_object = new music_sfml("","",-1, -1 );

        if( standard_sound_object != nullptr )
        {
            delete standard_sound_object;
            standard_sound_object = nullptr;
        }
        standard_sound_object = new sound_sfml("","",-1, -1 );
        sound_system_name = "sfml_audio";
        return sound_is_working;
    }

    void quit_sfml_audio_system()
    {
        error_log->report("Quitting audio_sfml_system....");
        if( sound_music_object != nullptr )
        {
            delete sound_music_object;
            sound_music_object = nullptr;
        }
        sound_music_object = new music_base("","",-1, -1 );

        error_log->report("Deleting standard soundobject...");

        if( standard_sound_object != nullptr )
        {
            delete standard_sound_object;
            standard_sound_object = nullptr;
        }
        standard_sound_object = new sound_base("","",-1, -1 );

        error_log->report("Resettting sound formats");

        for( int  i_format = 0; i_format < sound_format_max; i_format++ )
        {
            sound_is_format_supported[ i_format ] = false;
        }

        error_log->report("Closing AudioDevice....");

        sound_is_working = false;
        sound_system_name = "undefined";

        error_log->report("Quitted audio_sfml_system successfully...");
    }

    sound_sfml::sound_sfml( std::string s_name, std::string s_file , int group_id_number, int s_id )
    {
        current_volume = 1.f;
        sound_loaded = false;
        sound_id = s_id;
        sound_name = s_name;
        file_name = s_file;
        load( s_file );
    }

    sound_sfml::~sound_sfml()
    {
        unload();
    }

    sound_base * sound_sfml::create_new( std::string s_name, std::string s_file , int group_id_number, int s_id )
    {
        return new sound_sfml( s_name, s_file, group_id_number, s_id );
    }

    float sound_sfml::get_volume()
    {
        return 100.f * current_volume;
    }

    bool sound_sfml::load( std::string s_file)
    {
        if( main_file_url_manager->file_exists( s_file ) == false )
        {
            return false;
        }

        unload();
        sfml_sound_buffer.loadFromFile( s_file.c_str() );

        return true;
    }

    void sound_sfml::pause()
    {
    }

    int sound_sfml::play( int play_count ,int s_channel )
    {
        return -1;
    }

    void sound_sfml::set_volume( float vol )
    {
        //Since raudio's volume goes from 0 to 1.f, we will do a quick conversion...
        if( vol < 0 )
        {
            vol = 0;
            current_volume = 0;
        }
        else
        {
            current_volume = vol;
            vol =  vol / 100.f;
        }
    }

    void sound_sfml::stop()
    {
    }

    void sound_sfml::unload()
    {
        //if( sound_loaded )
    }

    music_sfml::music_sfml( std::string s_name, std::string s_file , int group_id_number, int s_id )
    {
        sound_id = s_id;
        sound_name = s_name;
        file_name = s_file;
        music_is_playing = false;

        load( s_file );
    }

    music_sfml::~music_sfml()
    {
        unload();
    }

    sound_base * music_sfml::create_new( std::string s_name, std::string s_file , int group_id_number, int s_id  )
    {
        return new music_sfml( s_name, s_file, group_id_number, s_id);
    }

    music_base * music_sfml::create_new_music( std::string s_name, std::string s_file , int group_id_number, int s_id )
    {
        return new music_sfml( s_name, s_file, group_id_number, s_id);
    }

    bool music_sfml::is_playing()
    {
        return false;
    }

    bool music_sfml::load( std::string s_file )
    {
        /*
        if( main_file_url_manager->file_exists( s_file) == false )
        {
            return false;
        }
        unload();

        sfml_music = LoadMusicStream( s_file.c_str() );
        */
        sound_loaded = true;
        sound_error = "";

        return sound_loaded;

    }

    void music_sfml::pause()
    {
    }

    int music_sfml::play( int play_count , int s_channel )
    {
        return play_count;
    }

    void music_sfml::stop()
    {
    }

    void music_sfml::unload()
    {
        //if( sound_loaded )
    }
}
