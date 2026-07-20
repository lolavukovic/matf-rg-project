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
    engine::graphics::OpenGL::enable_depth_testing();
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
    auto resources=engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics=engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model* tree=resources->model("tree");
    //Shader
    engine::resources::Shader* shader=resources->shader("basic");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::mat4 model=glm::mat4(1.0f);
    model=glm::translate(model, glm::vec3(0.0f, -1.0f, -3.0f));
    model=glm::scale(model, glm::vec3(0.15f));
    shader->set_mat4("model", model);
    tree->draw(shader);
}

void MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void MainController::draw() {
    //clear_buffers
    draw_tree();
    //swap_buffers
}

void MainController::end_draw() {
    auto platform=engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}


}