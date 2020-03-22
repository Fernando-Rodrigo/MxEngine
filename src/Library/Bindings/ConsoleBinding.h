// Copyright(c) 2019 - 2020, #Momo
// All rights reserved.
// 
// Redistributionand use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
// 
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditionsand the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditionsand the following disclaimer in the documentation
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

#include "Core/Application/Application.h"
#include "Core/Event/Events/KeyEvent.h"
#include "Core/Event/Events/UpdateEvent.h"

namespace MxEngine
{
	class ConsoleBinding
	{
		std::string handle;
	public:
		inline ConsoleBinding(const std::string& eventHandle) noexcept
			: handle(eventHandle) { }

		inline ConsoleBinding& Bind(KeyCode activateKey)
		{
			Logger::Instance().Debug("MxEngine::ConsoleBinding", Format("bound console to keycode: {0}", int(activateKey)));
			Application::Get()->GetEventDispatcher().AddEventListener<UpdateEvent>(this->handle,
				[key = activateKey](UpdateEvent& event)
				{
					auto& window = Application::Get()->GetWindow();
					if (window.IsKeyPressed(key))
					{
						// save cursor position to static variable, restore it when return to default cursor mode
						static Vector2 cursorPos = window.GetCursorPos();
						if (window.GetCursorMode() == CursorMode::NORMAL)
						{
							window.UseCursorMode(CursorMode::DISABLED);
							Application::Get()->ToggleDeveloperConsole(false);
							window.UseCursorPos(cursorPos);
						}
						else
						{
							cursorPos = window.GetCursorPos();
							window.UseCursorMode(CursorMode::NORMAL);
							Application::Get()->ToggleDeveloperConsole(true);
							window.UseCursorPos({ window.GetWidth() / 4.0f, window.GetHeight() / 2.0f });
						}
					}
				});
			return *this;
		}
	};
}