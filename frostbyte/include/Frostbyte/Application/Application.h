#include "Scene/SceneGraph.h"
#include "Platform/Interfaces/IWindow.h"

namespace Frostbyte {
    class Application {
    public:
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

    protected:
        ApplicationConfig Config;

    private:
        bool m_Running = true;

        IWindow* m_Window = nullptr;
        SceneGraph m_SceneGraph;
    };
}