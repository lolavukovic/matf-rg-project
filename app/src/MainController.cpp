//
// Created by lola on 7/19/26.
//

#include "../include/MainController.hpp"

#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>

namespace app {
class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};

void MainController::initialize() {
}

bool MainController::loop() {
    const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KEY_ESCAPE).state() == engine::platform::Key::State::JustPressed) {
        return false;
    }
    return true;
}

void MainController::draw_tree() {
    //Model

    //Shader
}


void MainController::draw() {
    draw_tree();
}

}