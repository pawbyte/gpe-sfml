/*
gpe_sfml_basic.cpp
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

#include "gpe_sfml_basic.h"

namespace gpe
{
    bool using_sfml_system_underneath;

    //initialize sfml
    bool init_sfml_main_system()
    {
        //Initialize all sfml subsystems
        error_log->report(" ");
        error_log->report("Starting sfml_module...");
        error_log->report("--sfml_module systems started...");

        //sfml is so simple it appears we don't need to do anything here just yet.
        using_sfml_system_underneath = true;

        int main_module_version_major = SFML_VERSION_MAJOR;
        int main_module_version_minor = SFML_VERSION_MINOR;
        int main_module_version_patch_level = SFML_VERSION_PATCH;
        return true;
    }

    bool sfml_is_initted()
    {
        return using_sfml_system_underneath;
    }

    void quit_sfml_main_system()
    {
        error_log->report("Exiting sfml module...");

        using_sfml_system_underneath = false;
        error_log->report("Exited sfml module successfully...");

    }
}
