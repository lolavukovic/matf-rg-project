//
// Created by lola on 7/19/26.
//

#include "../include/MainController.hpp"

#include "../../engine/libs/glad/include/glad/glad.h"
#include "GUIController.hpp"
#include "spdlog/spdlog.h"
#include <engine/graphics/Camera.hpp>

#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>



float beeAngle=0.0f;

namespace app {
class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
    void on_window_resize(int width, int height) override;
};


void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto gui_controller=engine::core::Controller::get<GUIController>();
    if (!gui_controller->is_enabled()) {
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        camera->rotate_camera(position.dx*0.2, position.dy*0.2);
    }
}
void MainPlatformEventObserver::on_window_resize(int width, int height) {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    if (!graphics) return;

    graphics->perspective_params().Width = static_cast<float>(width);
    graphics->perspective_params().Height = static_cast<float>(height);

    glViewport(0,0,width,height);
}


void MainController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();

    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->initialize_bloom(platform->window()->width(), platform->window()->height());

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
    engine::resources::Shader* shader=resources->shader("light");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());

    shader->set_vec3("viewPos", graphics->camera()->Position);
    shader->set_float("material.ambient", 0.3f);
    shader->set_float("material.diffuse", 1.0f);
    shader->set_float("material.specular", 0.0f);
    shader->set_float("material.shiness", 8.0f);


    shader->set_vec3("dirlight.direction", lightDirection);
    shader->set_vec3("dirlight.ambient", glm::vec3(ambientStrength));
    shader->set_vec3("dirlight.diffuse", glm::vec3(diffuseStrength));
    shader->set_vec3("dirlight.specular", glm::vec3(specularStrength));


    shader->set_vec3("spotlight.position", glm::vec3(1.3f, 2.5f, -2.8f));
    shader->set_vec3("spotlight.direction", glm::vec3(0.0f, -1.0f, 0.0f));

    shader->set_float("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
    shader->set_float("spotlight.outerCutOff", glm::cos(glm::radians(17.5f)));

    shader->set_float("spotlight.constant", 1.0f);
    shader->set_float("spotlight.linear", 0.07f);
    shader->set_float("spotlight.quadratic", 0.032f);

    if (spotlightEnabled) {
        shader->set_vec3("spotlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader->set_vec3("spotlight.diffuse", glm::vec3(3.0f, 3.0f, 3.0f));
        shader->set_vec3("spotlight.specular", glm::vec3(3.0f, 3.0f, 3.0f));

        shader->set_vec3 ("spotlight.color",glm::vec3(1.0f, 0.8f, 0.3f));
    }
    else {
        shader->set_vec3("spotlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader->set_vec3("spotlight.diffuse", glm::vec3(0.0f));
        shader->set_vec3("spotlight.specular", glm::vec3(0.0f));
    }

    glm::mat4 model=glm::mat4(1.0f);
    model=glm::translate(model, glm::vec3(0.0f, -1.0f, -4.0f));
    model=glm::scale(model, glm::vec3(0.65f));
    shader->set_mat4("model", model);

    tree->draw(shader);
}



void MainController::draw_house() {
    //Model
    auto resources=engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics=engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model* house=resources->model("house");
    //Shader
    engine::resources::Shader* shader=resources->shader("light");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());

    shader->set_vec3("viewPos", graphics->camera()->Position);
    shader->set_float("material.ambient", 0.3f);
    shader->set_float("material.diffuse", 1.0f);
    shader->set_float("material.specular", 0.5f);
    shader->set_float("material.shiness", 32.0f);

    shader->set_vec3("dirlight.direction", lightDirection);
    shader->set_vec3("dirlight.ambient", glm::vec3(ambientStrength));
    shader->set_vec3("dirlight.diffuse", glm::vec3(diffuseStrength));
    shader->set_vec3("dirlight.specular", glm::vec3(specularStrength));


    shader->set_vec3("spotlight.position", glm::vec3(1.3f, 2.5f, -2.8f));
    shader->set_vec3("spotlight.direction", glm::vec3(0.0f, -1.0f, 0.0f));

    shader->set_float("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
    shader->set_float("spotlight.outerCutOff", glm::cos(glm::radians(17.5f)));

    shader->set_float("spotlight.constant", 1.0f);
    shader->set_float("spotlight.linear", 0.07f);
    shader->set_float("spotlight.quadratic", 0.032f);

    if (spotlightEnabled) {
        shader->set_vec3("spotlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader->set_vec3("spotlight.diffuse", glm::vec3(3.0f, 3.0f, 3.0f));
        shader->set_vec3("spotlight.specular", glm::vec3(3.0f, 3.0f, 3.0f));

        shader->set_vec3 ("spotlight.color",glm::vec3(1.0f, 0.8f, 0.3f));
    }
    else {
        shader->set_vec3("spotlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader->set_vec3("spotlight.diffuse", glm::vec3(0.0f));
        shader->set_vec3("spotlight.specular", glm::vec3(0.0f));
    }
    glm::mat4 model=glm::mat4(1.0f);
    model=glm::translate(model, glm::vec3(2.5f, -1.0f, -4.0f));
    model=glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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
    engine::resources::Shader* shader=resources->shader("light");

    float radius = 0.8f;

    float x = radius * cos(beeAngle);
    float z = -3.0f + radius * sin(beeAngle);
    float y = 1.3f + 0.1f * sin(beeAngle * 4.0f);

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());

    shader->set_vec3("viewPos", graphics->camera()->Position);
    shader->set_float("material.ambient", 0.3f);
    shader->set_float("material.diffuse", 1.0f);
    shader->set_float("material.specular", 0.5f);
    shader->set_float("material.shiness", 32.0f);

    shader->set_vec3("dirlight.direction", lightDirection);
    shader->set_vec3("dirlight.ambient", glm::vec3(ambientStrength));
    shader->set_vec3("dirlight.diffuse", glm::vec3(diffuseStrength));
    shader->set_vec3("dirlight.specular", glm::vec3(specularStrength));


    shader->set_vec3("spotlight.position", glm::vec3(1.3f, 2.5f, -2.8f));
    shader->set_vec3("spotlight.direction", glm::vec3(0.0f, -1.0f, 0.0f));

    shader->set_float("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
    shader->set_float("spotlight.outerCutOff", glm::cos(glm::radians(17.5f)));

    shader->set_float("spotlight.constant", 1.0f);
    shader->set_float("spotlight.linear", 0.07f);
    shader->set_float("spotlight.quadratic", 0.032f);

    if (spotlightEnabled) {
        shader->set_vec3("spotlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader->set_vec3("spotlight.diffuse", glm::vec3(3.0f, 3.0f, 3.0f));
        shader->set_vec3("spotlight.specular", glm::vec3(3.0f, 3.0f, 3.0f));

        shader->set_vec3 ("spotlight.color",glm::vec3(1.0f, 0.8f, 0.3f));
    }
    else {
        shader->set_vec3("spotlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader->set_vec3("spotlight.diffuse", glm::vec3(0.0f));
        shader->set_vec3("spotlight.specular", glm::vec3(0.0f));
    }

    glm::mat4 model=glm::mat4(1.0f);
    model=glm::translate(model, glm::vec3(x,y,z));
    model=glm::rotate(model, glm::radians(-70.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model=glm::scale(model, glm::vec3(0.025f));
    shader->set_mat4("model", model);
    bee->draw(shader);
}

void MainController::draw_lamp() {
    //Model
    auto resources=engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics=engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model* lamp=resources->model("lamp");
    //Shader
    engine::resources::Shader* shader=resources->shader("light");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());

    shader->set_vec3("viewPos", graphics->camera()->Position);
    shader->set_float("material.ambient", 0.3f);
    shader->set_float("material.diffuse", 1.0f);
    shader->set_float("material.specular", 0.8f);
    shader->set_float("material.shiness", 32.0f);

    shader->set_vec3("dirlight.direction", lightDirection);
    shader->set_vec3("dirlight.ambient", glm::vec3(ambientStrength));
    shader->set_vec3("dirlight.diffuse", glm::vec3(diffuseStrength));
    shader->set_vec3("dirlight.specular", glm::vec3(specularStrength));


    shader->set_vec3("spotlight.position", glm::vec3(1.3f, 2.5f, -2.8f));
    shader->set_vec3("spotlight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
    shader->set_vec3 ("spotlight.color",glm::vec3(1.0f, 0.8f, 0.3f));

    shader->set_float("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
    shader->set_float("spotlight.outerCutOff", glm::cos(glm::radians(17.5f)));

    shader->set_float("spotlight.constant", 1.0f);
    shader->set_float("spotlight.linear", 0.09f);
    shader->set_float("spotlight.quadratic", 0.032f);

    if (spotlightEnabled) {
        shader->set_vec3("spotlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader->set_vec3("spotlight.diffuse", glm::vec3(3.0f, 3.0f, 3.0f));
        shader->set_vec3("spotlight.specular", glm::vec3(3.0f, 3.0f, 3.0f));

        shader->set_vec3 ("spotlight.color",glm::vec3(1.0f, 0.8f, 0.3f));
    }
    else {
        shader->set_vec3("spotlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader->set_vec3("spotlight.diffuse", glm::vec3(0.0f));
        shader->set_vec3("spotlight.specular", glm::vec3(0.0f));
    }

    glm::mat4 model=glm::mat4(1.0f);
    model=glm::translate(model, glm::vec3(1.3f, -1.0f, -2.8f));
    model=glm::scale(model, glm::vec3(0.823f));
    shader->set_mat4("model", model);
    lamp->draw(shader);
}

void MainController::draw_bulb() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    engine::resources::Model* bulbModel = resources->model("cube");
    engine::resources::Shader* bulbShader = resources->shader("bulb");

    bulbShader->use();
    bulbShader->set_mat4("projection", graphics->projection_matrix());
    bulbShader->set_mat4("view", graphics->camera()->view_matrix());


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.26f, 0.62f, -2.7f));
    model = glm::scale(model, glm::vec3(0.028f));
    bulbShader->set_mat4("model", model);

    if (spotlightEnabled) {
        glm::vec3 warmYellowHDR = glm::vec3(10.0f, 8.5f, 4.0f);
        bulbShader->set_vec3("lightColor", warmYellowHDR);
    }
    else {
        bulbShader->set_vec3("lightColor", glm::vec3(0.0f, 0.0f, 0.0f));
    }

    bulbModel->draw(bulbShader);
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

    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    float dt=platform->dt();

    beeAngle += dt;
    if (platform->key(engine::platform::KEY_L).state()==engine::platform::Key::State::JustPressed) {
        lightSequenceStarted=true;
        lightTimer=0.0f;
        spotlightEnabled=true;
    }

    if (lightSequenceStarted) {
        lightTimer+=dt;
        if (lightTimer>=2.0f && lightTimer<7.0f) {
            spotlightEnabled=false;
        }
        if (lightTimer>=7.0f) {
            spotlightEnabled=true;
            lightSequenceStarted=false;
        }
    }
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

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->bloom_begin();

    //clear_buffers
    draw_tree();
    draw_house();
    draw_bee();
    draw_lamp();
    draw_bulb();
    draw_skybox();


    auto gaussian_blur_shader        = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("gaussian_blur");
    auto bloom_shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("bloom");
    graphics->bloom_end(gaussian_blur_shader, bloom_shader, 1.0f);

    auto gui_controller = engine::core::Controller::get<GUIController>();
    if (gui_controller && gui_controller->is_enabled()) {
        gui_controller->draw();
    }

}

void MainController::end_draw() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();


    auto platform=engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}


}