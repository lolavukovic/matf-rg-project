//
// Created by lola on 8/2/26.
//

#include <engine/graphics/PointShadow.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace engine::graphics {

void PointShadow::init(unsigned int shadowWidth, unsigned int shadowHeight) {
    m_shadow_width = shadowWidth;
    m_shadow_height = shadowHeight;

    glGenFramebuffers(1, &m_shadow_fbo);

    glGenTextures(1, &m_depth_cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_depth_cubemap);

    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                     m_shadow_width, m_shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depth_cubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PointShadow::begin(const glm::vec3& lightPos, float nearPlane, float farPlane, engine::resources::Shader* depthShader) {

    glViewport(0, 0, m_shadow_width, m_shadow_height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);

    float aspect = (float)m_shadow_width / (float)m_shadow_height;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, nearPlane, farPlane);

    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));

    depthShader->use();
    for (unsigned int i = 0; i < 6; ++i) {
        depthShader->set_mat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    }
    depthShader->set_float("far_plane", farPlane);
    depthShader->set_vec3("lightPos", lightPos);
}

void PointShadow::end(int screenWidth, int screenHeight) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, screenWidth, screenHeight);
}

void PointShadow::bind_depth_map(unsigned int texture_unit) const {
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_depth_cubemap);
}


void PointShadow::cleanup() {
    if (m_shadow_fbo) glDeleteFramebuffers(1, &m_shadow_fbo);
    if (m_depth_cubemap) glDeleteTextures(1, &m_depth_cubemap);
}

} // namespace engine::graphics