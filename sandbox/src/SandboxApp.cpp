#include "Frostbyte.h"
#include "Logging/Logging.h"

#include <stdio.h>

#include "SandboxLayer.h"

class Sandbox : public Frostbyte::Application {
public:
    bool OnInit() override {
        SandboxLayer* layer = new SandboxLayer();
        PushLayer(layer);
        return true;
    }
};

Frostbyte::Application* Frostbyte::CreateApplication(int argc, char** argv) {
    return new Sandbox();
}