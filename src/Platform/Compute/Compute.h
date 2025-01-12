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

#include "Platform/GraphicAPI.h"

namespace MxEngine
{
    struct BarrierType
    {
        enum Bits : uint64_t
        {
            VERTEX_ARRAY = 1 << 0,
            INDEX_BUFFER = 1 << 1,
            UNIFORM_BUFFER = 1 << 2,
            TEXTURE_FETCH = 1 << 3,
            IMAGE_ACCESS = 1 << 4,
            COMMAND = 1 << 5,
            PIXEL_BUFFER = 1 << 6,
            TEXTURE_UPDATE = 1 << 7,
            BUFFER_UPDATE = 1 << 8,
            CLIENT_MAPPED_BUFFER = 1 << 9,
            FRAMEBUFFER = 1 << 10,
            TRANSFORM_FEEDBACK = 1 << 11,
            ATOMIC_COUNTER = 1 << 12,
            SHADER_STORAGE_BUFFER = 1 << 13,
            QUERY_BUFFER = 1 << 14,
            ALL = 1 << 15,
        };
    };

    class Compute
    {
    public:
        // static void Dispatch(const ComputeShaderHandle& computeShader, size_t x, size_t y, size_t z);
        static void SetMemoryBarrier(BarrierType::Bits barriers);
    };
}