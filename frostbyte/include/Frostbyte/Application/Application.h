#include "Scene/SceneGraph.h"

namespace Frostbyte {
    class Application {
    public:
        bool Init();
        void Update();
        void Shutdown();

        bool IsRunning() { return m_Running; }

        // Pure virtual functions to be overwritten
        virtual bool OnInit() = 0;
        
        template <class T>
        void PushLayer(T layer) {
            m_SceneGraph.GetRoot()->PushChild((Frostbyte::Layer*)layer);
        }
    private:
        bool m_Running = true;
        SceneGraph m_SceneGraph;
    };
}