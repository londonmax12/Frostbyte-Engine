#include "Frostbyte.h"
#include "Logging/Logging.h"
#include <stdio.h>

class Sandbox : public Frostbyte::Application {
public:
    bool OnInit() override {
        LOG_INFO("Print {}", "World!");
        return true;
    }
    void OnUpdate() override {
    }
};

Frostbyte::Application* Frostbyte::CreateApplication(int argc, char** argv) {
    return new Sandbox();
}