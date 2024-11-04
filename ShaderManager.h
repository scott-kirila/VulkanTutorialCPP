//
// Created by hetan on 6/14/2024.
//

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

//#include "shaderc/shaderc.hpp"

#include "Utilities.h"

class ShaderManager {
public:
    static std::vector<char> ReadFile(const std::string& filename);

    static vk::ShaderModule CreateShaderModule(const std::vector<char>& code, vk::Device device);

private:
    const std::vector<Vertex> vertices = {
        {{ 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    const char* m_VertexShader = R"(
        #version 460

        layout(location = 0) out vec3 fragColor;

        vec2 positions[3] = vec2[](
            vec2(0.0, -0.5),
            vec2(0.5, 0.5),
            vec2(-0.5, 0.5)
        );

        vec3 colors[3] = vec3[](
            vec3(1.0, 0.0, 0.0),
            vec3(0.0, 1.0, 0.0),
            vec3(0.0, 0.0, 1.0)
        };

        void main() {
            gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
            fragColor = colors[gl_VertexIndex];
        }
    )";

    const char* m_FragmentShader = R"(
        #version 460

        layout(location = 0) out vec4 outColor;

        void main() {
            outColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
    )";
};



#endif //SHADERMANAGER_H
