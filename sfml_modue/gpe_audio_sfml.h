/*
gpe_audio_sfml.h
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

#ifndef gpe_audio_sfml_h
#define gpe_audio_sfml_h

#include "../gpe/gpe_audio_base.h"
#include <SFML/Audio.hpp>

namespace gpe
{
    class sound_sfml: public sound_base
    {
        private:
            sf::SoundBuffer sfml_sound_buffer;
            int sound_plays_remaining;
            float current_volume; //In sfml this is between 0.f and 1.f.
            //The base class uses between 0 to 100, so conversions will be made on getters/setters
        public:
            sound_sfml( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1 );
            ~sound_sfml();
            sound_base * create_new( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1 );
            float get_volume();

            bool load( std::string s_file );
            void pause();
            int play( int play_count = 0, int s_channel = -1 );
            void set_volume( float vol );
            void stop();
            void unload();
    };

    class music_sfml: public music_base
    {
        private:
            sf::Music sfml_music;
        public:
            music_sfml( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1 );
            ~music_sfml();
            sound_base * create_new( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1);
            music_base * create_new_music( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1 );
            bool is_playing();
            bool load( std::string s_file );
            void pause();
            int play( int play_count = 0, int s_channel = -1 );
            void stop();
            void unload();
    };

    bool init_sfml_audio_system();
    void quit_sfml_audio_system();
}

#endif //gpe_audio_sfml_h
