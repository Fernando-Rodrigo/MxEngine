// Copyright(c) 2019 - 2020, #Momo
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
// 
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and /or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once
#include <string>

#include "Utilities/Memory/Memory.h"
#include "Platform/GraphicAPI.h"
#include "Utilities/Math/Math.h"

namespace MxEngine
{
    enum class AlphaModeGroup : uint8_t
    {
        OPAQUE = 0,
        MASKED,
        TRANSPARENT,
    };

    class Material
    {
    public:
        // TextureHandle AlbedoMap;
        // TextureHandle EmissiveMap;
        // TextureHandle NormalMap;
        // TextureHandle HeightMap;
        // TextureHandle AmbientOcclusionMap;
        // TextureHandle MetallicMap;
        // TextureHandle RoughnessMap;

        float Transparency = 1.0f;
        float Emission = 0.0f;
        float Displacement = 0.025f;
        float RoughnessFactor = 0.75f;
        float MetallicFactor = 0.0f;

        Vector3 BaseColor{ 1.0f };
        Vector2 UVMultipliers{ 1.0f };
        AlphaModeGroup AlphaMode = AlphaModeGroup::OPAQUE;
        MxString Name = "DefaultMaterial";

        constexpr static size_t TextureCount = 7;
        bool IsInternalEngineResource() const { return false; }
    };

}

MXENGINE_FORCE_REFLECTION(Material);