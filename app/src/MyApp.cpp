//
// Created by lola on 7/19/26.
//

//#include <EnvironmentController.hpp>
//#include <FlashlightController.hpp>
//#include <GUIController.hpp>
//#include <LampController.hpp>
#include "GUIController.hpp"


#include <MainController.hpp>
#include <MyApp.hpp>

#include <spdlog/spdlog.h>

namespace app {
    void app::MyApp::app_setup() {
        spdlog::info("App setup");

        auto main_controller = register_controller<app::MainController>();
        main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());

        auto gui_controller = register_controller<app::GUIController>();
        main_controller->before(gui_controller);
    }
}


