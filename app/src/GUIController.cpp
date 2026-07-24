//
// Created by lola on 7/21/26.
//

#include "../include/GUIController.hpp"

#include "MainController.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "imgui.h"
#include <engine/platform/PlatformController.hpp>

namespace app {

void GUIController::initialize() {
    set_enable(false);
}

void GUIController::draw() {
    auto graphics=engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->begin_gui();

    auto mainController=engine::core::Controller::get<MainController>();
    ImGui::SetNextWindowSize(ImVec2(300, 130), ImGuiCond_Always);
    ImGui::Begin("Lightning");

    ImGui::SliderFloat("Ambient",
                   &mainController->ambientStrength,
                   0.0f, 1.0f);

    ImGui::SliderFloat("Diffuse",
                       &mainController->diffuseStrength,
                       0.0f, 2.0f);

    ImGui::SliderFloat("Specular",
                       &mainController->specularStrength,
                       0.0f, 2.0f);

    ImGui::SliderFloat3("Direction",
                        &mainController->lightDirection.x,
                        -1.0f, 1.0f);

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