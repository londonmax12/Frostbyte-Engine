#include "Scene/SceneGraph.h"
#include "Application/IWindow.h"
#include "Application/EventDispatcher.h"
#include "Renderer/Renderer.h"

namespace Frostbyte {
    class Application {
    public:
        Application() = default;
        
        static Application* GetInstance() { return m_Instance; }

        struct ApplicationConfig
        {
            WindowConfig WindowConf;
        };

        bool Init();
        void Update();
        void Shutdown();

        bool IsRunning() { return m_Running; }
        IWindow* GetWindow() { return m_Window; }

        // Virtual functions to be overwritten
        virtual bool OnInit() { return true; };
        
        template <class T>
        void PushLayer(T* layer) {
            m_SceneGraph.GetRoot()->PushChild((Frostbyte::Layer*)layer);
        }

        void OnEvent(IEvent* ev);

    protected:
        ApplicationConfig Config;

    private:
        inline static Application* m_Instance = nullptr;
        
        bool m_Running = true;
        
        IWindow* m_Window = nullptr;
        Renderer* m_Renderer = nullptr;
        SceneGraph m_SceneGraph;
    };
}