#pragma once

#include "Platform/PlatformDetection.h"
#include "Platform/Interfaces/IWindow.h"
#include "Application/Application.h"

int main(int argc, char** argv);

namespace Frostbyte {    
    // Define in client
    Application* CreateApplication(int argc, char** argv);

    class FrostbyteInstance {
        public:
            FrostbyteInstance(Application* application);

            void Shutdown();
            void Start();
            void Update();

        private:
            IWindow* m_Window;
            Application* m_Application;
    };
}