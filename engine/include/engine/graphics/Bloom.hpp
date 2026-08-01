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
    ~Bloom()=default;

    void init(int width, int height);
    void begin();
    void render(engine::resources::Shader* blurShader, engine::resources::Shader* bloomShader, float exposure = 1.0f);
    void cleanup();

private:
    int m_width = 0;
    int m_height = 0;

    unsigned int m_hdrFBO = 0;
    unsigned int m_colorBuffers[2] = {0, 0};
    unsigned int m_pingpongFBO[2] = {0, 0};
    unsigned int m_pingpongColorbuffers[2] = {0, 0};
    unsigned int m_rboDepth = 0;

    unsigned int m_quadVAO = 0;
    unsigned int m_quadVBO = 0;

    void render_quad();
};

} // namespace engine::graphics


#endif//MATF_RG_PROJECT_BLOOM_HPP