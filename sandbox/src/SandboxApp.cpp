#include "Frostbyte.h"
#include "Logging/Logging.h"

#include <stdio.h>

#include "SandboxLayer.h"

class Sandbox : public Frostbyte::Application {
public:
    Sandbox() = default;

    bool OnInit() override {
        Config.WindowConf.Width = 1600;
        Config.WindowConf.Height = 900;
        Config.WindowConf.Name = "Sandbox";

        PushLayer(new SandboxLayer());

        return true;
    }
};

Frostbyte::Application* Frostbyte::CreateApplication(int argc, char** argv) {
    return new Sandbox();
}