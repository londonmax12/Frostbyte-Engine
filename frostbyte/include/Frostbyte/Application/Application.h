#include "Scene/SceneGraph.h"
#include "Interfaces/IWindow.h"
#include "Application/EventDispatcher.h"

namespace Frostbyte {
    class Application {
    public:
        Application() = default;

        struct ApplicationConfig
        {
            WindowConfig WindowConf;
        };

        bool Init();
        void Update();
        void Shutdown();

        bool IsRunning() { return m_Running; }

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
        bool m_Running = true;

        IWindow* m_Window = nullptr;
        SceneGraph m_SceneGraph;
    };
}