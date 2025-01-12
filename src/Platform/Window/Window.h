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

#include <bitset>

#include "Utilities/Time/Time.h"
#include "Core/Config/Config.h"
#include "Core/Events/MouseEvent.h"
#include <VulkanAbstractionLayer/Window.h>

namespace MxEngine
{
    class Window
    {
        using WindowHandle = VulkanAbstractionLayer::Window*;
    private:
        MxString title;
        WindowHandle window = nullptr;
        int width = 0, height = 0;
        EventDispatcherImpl<EventBase>* dispatcher = nullptr;
        mutable std::bitset<350> keyHeld;
        mutable std::bitset<350> keyPressed;
        mutable std::bitset<350> keyReleased;
        mutable std::bitset<8> mouseHeld;
        mutable std::bitset<8> mousePressed;
        mutable std::bitset<8> mouseReleased;
        CursorMode cursorMode = CursorMode::NORMAL;
        bool anyKeyEvent = false;
        bool anyMouseEvent = false;
        Vector2 windowPosition{ 0.0f, 0.0f };

        void Destroy();
        void Move(Window&& window);
    public:
        Window() = default;
        Window(int width, int height, const MxString& title);
        Window(const Window&) = delete;
        Window(Window&& other) noexcept;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&& other) noexcept;
        ~Window();

        int GetHeight() const;
        int GetWidth() const;
        bool IsOpen() const;
        void PullEvents() const;
        void OnUpdate();
        void Present();
        Vector2 GetCursorPosition() const;
        Vector2 GetWindowPosition() const;
        float GetTimeSinceCreation() const;
        void SetTimeSinceCreation(float time);
        CursorMode GetCursorMode() const;
        const MxString& GetTitle() const;
        bool IsKeyHeld(KeyCode key) const;
        bool IsKeyPressed(KeyCode key) const;
        bool IsKeyReleased(KeyCode key) const;
        bool IsMouseHeld(MouseButton button) const;
        bool IsMousePressed(MouseButton button) const;
        bool IsMouseReleased(MouseButton button) const;
        bool IsKeyHeldUnchecked(KeyCode key) const;
        bool IsMouseHeldUnchecked(MouseButton button) const;
        WindowHandle GetNativeHandle();
        EventDispatcherImpl<EventBase>& GetEventDispatcher();
        bool IsCreated() const;
        Window& Create();
        Window& Close();
        Window& SwitchContext();
        Window& UseCursorMode(CursorMode cursor);
        Window& UseCursorPosition(const Vector2& pos);
        Window& UseTitle(const MxString& title);
        Window& UseWindowPosition(int xpos, int ypos);
        Window& UseWindowSize(int width, int height);
        Window& UseEventDispatcher(EventDispatcherImpl<EventBase>* dispatcher);
    };
}