//
// Created by lola on 8/1/26.
//


#include "engine/graphics/GraphicsController.hpp"


#include <engine/graphics/Bloom.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/resources/Shader.hpp>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace engine::graphics {

void Bloom::init(int width, int height) {
    m_width = width;
    m_height = height;

    glGenFramebuffers(1, &m_hdr_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_hdr_fbo);

    glGenTextures(2, m_color_buffers);
    for (unsigned int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, m_color_buffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_color_buffers[i], 0);
    }

    glGenRenderbuffers(1, &m_rbo_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo_depth);

    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("HDR framebuffer is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(2, m_pingpong_fbo);
    glGenTextures(2, m_pingpong_colorbuffers);
    for (unsigned int i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_pingpong_fbo[i]);
        glBindTexture(GL_TEXTURE_2D, m_pingpong_colorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingpong_colorbuffers[i], 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            spdlog::error("Ping-pong framebuffer {} is not complete!", i);
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Bloom::begin() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_hdr_fbo);
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Bloom::render(engine::resources::Shader* blur_shader, engine::resources::Shader* bloom_shader, float exposure) {

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    bool horizontal = true, first_iteration = true;
    unsigned int amount = 10;

    if (blur_shader) {
        blur_shader->use();
        for (unsigned int i = 0; i < amount; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, m_pingpong_fbo[horizontal]);
            glViewport(0, 0, m_width, m_height);
            blur_shader->set_bool("horizontal", horizontal);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, first_iteration ? m_color_buffers[1] : m_pingpong_colorbuffers[!horizontal]);
            blur_shader->set_int("image", 0);

            render_quad();
            horizontal = !horizontal;
            if (first_iteration) first_iteration = false;
        }
    }


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    int screenWidth = static_cast<int>(graphics->perspective_params().Width);
    int screenHeight = static_cast<int>(graphics->perspective_params().Height);
    glViewport(0, 0, screenWidth, screenHeight);

    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (bloom_shader) {
        bloom_shader->use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_color_buffers[0]);
        bloom_shader->set_int("scene", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_pingpong_colorbuffers[!horizontal]);
        bloom_shader->set_int("bloomBlur", 1);
        bloom_shader->set_bool("bloom", true);

        bloom_shader->set_float("exposure", exposure);


        render_quad();
    }

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Bloom::cleanup() {
    if (m_hdr_fbo) glDeleteFramebuffers(1, &m_hdr_fbo);
    if (m_rbo_depth) glDeleteRenderbuffers(1, &m_rbo_depth);
    if (m_color_buffers[0]) glDeleteTextures(2, m_color_buffers);
    if (m_pingpong_fbo[0]) glDeleteFramebuffers(2, m_pingpong_fbo);
    if (m_pingpong_colorbuffers[0]) glDeleteTextures(2, m_pingpong_colorbuffers);
    if (m_quad_vao) {
        glDeleteVertexArrays(1, &m_quad_vao);
        glDeleteBuffers(1, &m_quad_vbo);
    }
}

void Bloom::render_quad() {
    if (m_quad_vao == 0) {
        float quadVertices[] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
        };
        glGenVertexArrays(1, &m_quad_vao);
        glGenBuffers(1, &m_quad_vbo);
        glBindVertexArray(m_quad_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }
    glBindVertexArray(m_quad_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

} // namespace engine::graphics
