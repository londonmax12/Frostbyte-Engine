#include "frostbyte.h"

using namespace Frostbyte;

int main(int argc, char** argv) {
    Application* app = CreateApplication(argc, argv);

    FrostbyteInstance instance = FrostbyteInstance(app);
    instance.Start();

    while (app->IsRunning())
    {
        instance.Update();
    }

    instance.Shutdown();
}

namespace Frostbyte {
    FrostbyteInstance::FrostbyteInstance(Application* application)
        : m_Application(application) {}

    void FrostbyteInstance::Shutdown()
    {
        if (m_Application)
        {
            m_Application->Shutdown();
            delete m_Application;
            m_Application = nullptr;
        }
    }
    void FrostbyteInstance::Start()
    {
        m_Application->Init();
    }
    void FrostbyteInstance::Update()
    {
        m_Application->Update();
    }
}