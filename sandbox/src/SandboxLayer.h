#include "Frostbyte.h"
#include "Logging/Logging.h"
#include <stdio.h>

class SandboxLayer : public Frostbyte::Layer {
public:
    void OnInit() override {
        LOG_INFO("Main layer init");
    };
};