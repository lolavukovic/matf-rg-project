//
// Created by lola on 7/19/26.
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP

#include "glm/vec3.hpp"


#include <engine/core/Controller.hpp>
#include <engine/platform/Input.hpp>
#include <engine/platform/PlatformEventObserver.hpp>

namespace  app {
class MainController : public engine::core::Controller {
public:
    std::string_view name() const override {
        return "app::MainController";
    }
    float ambientStrength=0.3f;
    float diffuseStrength=0.7f;
    float specularStrength=1.0f;

    glm::vec3 lightDirection=glm::vec3(0.7f, -1.0f, -0.3f);

private:
    void initialize() override;
    bool loop() override;
    void draw_tree();
    void draw_house();
    void draw_bee();
    void draw_lamp();
    void draw_bulb();
    void begin_draw() override;
    void draw() override;
    void end_draw() override;
    void update() override;
    void update_camera();
    void draw_skybox();
    bool lightSequenceStarted=false;
    bool spotlightEnabled=true;
    float lightTimer=0.0f;

};
};



#endif//MATF_RG_PROJECT_MAINCONTROLLER_HPP
