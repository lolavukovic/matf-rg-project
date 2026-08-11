//
// Created by lola on 7/21/26.
//

#include "GUIController.hpp"

#include "MainController.hpp"
#include "engine/graphics/GraphicsController.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <engine/platform/PlatformController.hpp>

namespace app {

void GUIController::initialize() {
    set_enable(false);
}

void GUIController::draw() {

    if (!is_enabled()) {
        return;
    }

    auto graphics=engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->begin_gui();

    auto mainController=engine::core::Controller::get<MainController>();

    if (mainController) {
        ImGui::SetNextWindowSize(ImVec2(340, 160), ImGuiCond_Always);
        ImGui::Begin("Lightning");

        ImGui::SliderFloat("Ambient",
                       &mainController->ambient_strength,
                       0.0f, 1.0f);

        ImGui::SliderFloat("Diffuse",
                           &mainController->diffuse_strength,
                           0.0f, 2.0f);

        ImGui::SliderFloat("Specular",
                           &mainController->specular_strength,
                           0.0f, 2.0f);

        ImGui::SliderFloat3("Direction",
                            &mainController->light_direction.x,
                            -1.0f, 1.0f);

        ImGui::ColorEdit3("Spotlight Color", &mainController->spotlight_color.x);

        ImGui::End();
    }

    graphics->end_gui();
}

void GUIController::poll_events() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if(platform->key(engine::platform::KeyId::KEY_G).state() == engine::platform::Key::State::JustPressed) {
        set_enable(!is_enabled());
    }
}

}