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

#include "Utilities/Time/Time.h"
#include "Core/Events/EventBase.h"
#include "Core/Rendering/RenderAdaptor.h"
#include "Core/MxObject/MxObject.h"
#include "Utilities/FileSystem/File.h"
#include "Core/Config/Config.h"
#include "Utilities/Profiler/Profiler.h"
#include "Platform/Window/Window.h"

GENERATE_METHOD_CHECK(OnUpdate, OnUpdate(float()));

namespace MxEngine
{
    class RuntimeEditor;

    class Application
    {
        struct ModuleManager
        {
            ModuleManager(Application* app);
            ~ModuleManager();
        } manager;

        using UpdateCallbackList = MxVector<void(*)(TimeStep)>;
        using CollisionList = MxVector<std::pair<MxObject::Handle, MxObject::Handle>>;
        using CollisionSwapPair = std::pair<CollisionList, CollisionList>;
    private:
        static inline Application* Current = nullptr;
        UniqueRef<Window> window;
        RenderAdaptor renderAdaptor;
        EventDispatcherImpl<EventBase>* dispatcher;
        RuntimeEditor* editor;
        UpdateCallbackList updateCallbacks;
        CollisionSwapPair collisions;
        Config config;
        TimeStep timeDelta = 0.0f;
        size_t counterFPS = 0;
        TimeStep totalElapsedTime = 0.0f;
        bool shouldClose = false;
        bool isRunning = false;

        void InitializeConfig(Config& config);
        void InitializeRuntime(RuntimeEditor& editor);
        void InitializeRenderAdaptor(RenderAdaptor& adaptor);
        void DestroyRenderAdaptor(RenderAdaptor& adaptor);
        void InitializeShaderDebug();
        void UpdateTimeDelta(TimeStep& lastFrameEnd, TimeStep& lastSecondEnd, size_t& framesPerSecond);
        void DrawObjects();
        void UpdateComponents();
        void InvokeUpdate();
        void InvokePhysics();
        void InvokeCreate();
        void CreateContext();
        bool VerifyApplicationState();
    protected:

        Application();

        virtual void OnCreate();
        virtual void OnUpdate();
        virtual void OnRender();
        virtual void OnDestroy();
    public:
        bool IsPaused = false;
        float TimeScale = 1.0f;

        template<typename T>
        void RegisterComponentUpdate();

        void ToggleRuntimeEditor(bool isVisible);
        void ToggleWindowUpdates(bool isPolled);
        void CloseOnKeyPress(KeyCode key);

        void AddCollisionEntry(const MxObject::Handle& object1, const MxObject::Handle& object2);
        EventDispatcherImpl<EventBase>& GetEventDispatcher();
        RenderAdaptor& GetRenderAdaptor();
        RuntimeEditor& GetRuntimeEditor();
        Config& GetConfig();
        Window& GetWindow();
        TimeStep GetTimeDelta() const;
        TimeStep GetUnscaledTimeDelta() const;
        TimeStep GetTotalElapsedTime() const;
        TimeStep GetEngineTime() const;
        void SetTotalElapsedTime(TimeStep time);
        size_t GetCurrentFPS() const;
        void Run();
        bool IsRunning() const;
        void CloseApplication();
        virtual ~Application();

        static void Init();
        static Application* GetImpl();
        static void Clone(Application* application);
    };
    
    template<typename T>
    inline void Application::RegisterComponentUpdate()
    {
        if constexpr (has_method_OnUpdate<T>::value)
        {
            this->updateCallbacks.push_back([](TimeStep dt)
            {
                MAKE_SCOPE_PROFILER(typeid(T).name());
                auto view = ComponentFactory::GetView<T>();
                for (auto& component : view)
                {
                    component.OnUpdate(dt);
                }
            });
        }
    }

    #if defined(MXENGINE_PROJECT_SOURCE_DIRECTORY) && defined(MXENGINE_PROJECT_BINARY_DIRECTORY)
    inline void LaunchFromSourceDirectory()
    {
        FilePath currentWorkingDirectory = std::filesystem::current_path();
        FilePath binaryDirectory = MXENGINE_PROJECT_BINARY_DIRECTORY;
        if (currentWorkingDirectory == binaryDirectory)
        {
            std::filesystem::current_path(MXENGINE_PROJECT_SOURCE_DIRECTORY);
        }
    }
    #endif
}