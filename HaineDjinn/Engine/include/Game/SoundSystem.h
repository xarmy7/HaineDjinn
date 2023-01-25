#pragma once

#include "Core/Maths.h"

#include <IrrKlang/irrKlang.h>
#include <iostream>
#include <string>

namespace SoundSystem
{
    static irrklang::ISoundEngine* engine = {};

    void Init();
    void PlaySound2D(const char* path, bool loop = false);
    void PlaySound3D(const char* path, Vector3 position, bool loop = false);
}