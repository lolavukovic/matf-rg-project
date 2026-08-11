//
// Created by lola on 8/2/26.
//

#ifndef MATF_RG_PROJECT_POINTSHADOW_HPP
#define MATF_RG_PROJECT_POINTSHADOW_HPP

#include <engine/graphics/OpenGL.hpp>
#include <engine/resources/Shader.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace engine::graphics {
class PointShadow {
public:
    PointShadow() = default;
    ~PointShadow()=default;

    void init(unsigned int shadow_width = 1024, unsigned int shadow_height = 1024);

    void begin(const glm::vec3& light_pos, float near_plane, float far_plane, engine::resources::Shader* depth_shader);

    void end(int screen_width, int screen_height);

    unsigned int depth_cubemap() const { return m_depth_cubemap; }

    void bind_depth_map(unsigned int texture_unit = 0) const;

    float far_plane() const { return m_far_plane; }

    void cleanup();

private:
    unsigned int m_shadow_fbo = 0;
    unsigned int m_depth_cubemap = 0;
    unsigned int m_shadow_width = 1024;
    unsigned int m_shadow_height = 1024;
    float m_far_plane = 25.0f;
};
}

#endif//MATF_RG_PROJECT_POINTSHADOW_HPP
