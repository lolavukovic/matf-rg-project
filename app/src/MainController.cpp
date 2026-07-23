//
// Created by lola on 7/19/26.
//

#include "../include/MainController.hpp"

#include "GUIController.hpp"
#include "spdlog/spdlog.h"
#include <engine/graphics/Camera.hpp>

#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>

namespace app {
class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};


void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto gui_controller=engine::core::Controller::get<GUIController>();
    if (!gui_controller->is_enabled()) {
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        camera->rotate_camera(position.dx*0.2, position.dy*0.2);
    }
}


void MainController::initialize() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
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
    model=glm::translate(model, glm::vec3(0.0f, -1.0f, -4.0f));
    model=glm::scale(model, glm::vec3(0.15f));
    shader->set_mat4("model", model);
    tree->draw(shader);
}


void MainController::draw_house() {
    //Model
    auto resources=engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics=engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model* house=resources->model("house");
    //Shader
    engine::resources::Shader* shader=resources->shader("house");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::mat4 model=glm::mat4(1.0f);
    model=glm::translate(model, glm::vec3(2.5f, -1.0f, -4.0f));
    model=glm::scale(model, glm::vec3(0.083f));
    shader->set_mat4("model", model);
    house->draw(shader);
}

void MainController::draw_bee() {
    //Model
    auto resources=engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics=engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model* bee=resources->model("bee");
    //Shader
    engine::resources::Shader* shader=resources->shader("bee");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::mat4 model=glm::mat4(1.0f);
    model=glm::translate(model, glm::vec3(1.5f, 1.3f, -4.0f));
    model=glm::scale(model, glm::vec3(0.033f));
    shader->set_mat4("model", model);
    bee->draw(shader);
}

void MainController::draw_lamp() {
    //Model
    auto resources=engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics=engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model* lamp=resources->model("lamp");
    //Shader
    engine::resources::Shader* shader=resources->shader("lamp");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::mat4 model=glm::mat4(1.0f);
    model=glm::translate(model, glm::vec3(1.3f, -1.0f, -2.8f));
    model=glm::scale(model, glm::vec3(0.823f));
    shader->set_mat4("model", model);
    lamp->draw(shader);
}


void MainController::update_camera() {

    auto platform=engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics=engine::core::Controller::get<engine::graphics::GraphicsController>();

    float dt = platform->dt();
    auto camera = graphics->camera();
    if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
    }

    if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
    }

    if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
    }

    if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
    }

}


void MainController::update() {
    update_camera();
}


void MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void MainController::draw_skybox() {
    auto resouces= engine::core::Controller::get<engine::resources::ResourcesController>();
    auto skybox= resouces->skybox("skybox");
    auto shader=resouces->shader("skybox");
    auto graphics=engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->draw_skybox(shader, skybox);
}


void MainController::draw() {
    //clear_buffers
    draw_tree();
    draw_house();
    draw_bee();
    draw_lamp();
    draw_skybox();
    //swap_buffers
}

void MainController::end_draw() {
    auto platform=engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}


}