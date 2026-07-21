//
// Created by lola on 7/21/26.
//

#include "../include/GUIController.hpp"

#include "engine/graphics/GraphicsController.hpp"
#include <engine/platform/PlatformController.hpp>
#include "imgui.h"

namespace app {

void GUIController::initialize() {
    set_enable(false);
}

void GUIController::draw() {
    auto graphics=engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->begin_gui();

    ImGui::Begin("Camera info");

    ImGui::End();

    graphics->end_gui();
}

void GUIController::poll_events() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if(platform->key(engine::platform::KeyId::KEY_G).state() == engine::platform::Key::State::JustPressed) {
        set_enable(!is_enabled());
    }
}


}