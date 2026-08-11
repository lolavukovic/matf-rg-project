//
// Created by lola on 8/1/26.
//

#ifndef MATF_RG_PROJECT_BLOOM_HPP
#define MATF_RG_PROJECT_BLOOM_HPP

#include <engine/graphics/OpenGL.hpp>
#include <engine/resources/Shader.hpp>

namespace engine::graphics {

class Bloom {
public:
    Bloom() = default;
    ~Bloom()= default;

    void init(int width, int height);
    void begin();
    void render(engine::resources::Shader* blur_shader, engine::resources::Shader* bloom_shader, float exposure = 1.0f);
    void cleanup();
    void render_quad();

private:
    int m_width = 0;
    int m_height = 0;

    unsigned int m_hdr_fbo = 0;
    unsigned int m_color_buffers[2] = {0, 0};
    unsigned int m_pingpong_fbo[2] = {0, 0};
    unsigned int m_pingpong_colorbuffers[2] = {0, 0};
    unsigned int m_rbo_depth = 0;

    unsigned int m_quad_vao = 0;
    unsigned int m_quad_vbo = 0;
};

} // namespace engine::graphics


#endif//MATF_RG_PROJECT_BLOOM_HPP