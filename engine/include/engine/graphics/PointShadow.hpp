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

    void init(unsigned int shadowWidth = 1024, unsigned int shadowHeight = 1024);

    void begin(const glm::vec3& lightPos, float nearPlane, float farPlane, engine::resources::Shader* depthShader);

    void end(int screenWidth, int screenHeight);

    unsigned int depthCubemap() const { return m_depthCubemap; }

    void bind_depth_map(unsigned int texture_unit = 0) const;

    float far_plane() const { return m_farPlane; }

    void cleanup();

private:
    unsigned int m_shadowFBO = 0;
    unsigned int m_depthCubemap = 0;
    unsigned int m_shadowWidth = 1024;
    unsigned int m_shadowHeight = 1024;
    float m_farPlane = 25.0f;
};
}

#endif//MATF_RG_PROJECT_POINTSHADOW_HPP
