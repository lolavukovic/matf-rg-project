//
// Created by lola on 7/19/26.
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP

#include "glm/vec3.hpp"


#include <engine/core/Controller.hpp>
#include <engine/platform/Input.hpp>
#include <engine/platform/PlatformEventObserver.hpp>
#include <engine/graphics/PointShadow.hpp>

namespace  app {
class MainController : public engine::core::Controller {
public:
    std::string_view name() const override {
        return "app::MainController";
    }
    float ambient_strength=0.3f;
    float diffuse_strength=0.7f;
    float specular_strength=1.0f;

    glm::vec3 light_direction=glm::vec3(0.7f, -1.0f, -0.3f);
    glm::vec3 spotlight_color = glm::vec3(1.0f, 0.8f, 0.3f);

private:
    void initialize() override;
    bool loop() override;
    void draw_tree();
    void draw_house();
    void draw_bee();
    void draw_lamp();
    void draw_ground();
    void draw_bulb();
    void begin_draw() override;
    void draw() override;
    void end_draw() override;
    void update() override;
    void update_camera();
    void draw_skybox();


    bool m_light_sequence_started=false;
    bool m_spotlight_enabled=true;
    float m_light_timer=0.0f;


    glm::vec3 m_light_pos = glm::vec3(1.3f, 0.7f, -2.8f);
    float m_far_plane = 25.0f;
    float m_near_plane = 0.1f;

    void render_scene_geometry(engine::resources::Shader* shadow_shader);
    void setup_light_shader_uniforms(engine::resources::Shader* shader);

};
};



#endif//MATF_RG_PROJECT_MAINCONTROLLER_HPP
