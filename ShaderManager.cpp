//
// Created by hetan on 6/14/2024.
//

#include "ShaderManager.h"

#include <fstream>

std::vector<char> ShaderManager::ReadFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    const std::streampos fileSize = file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

vk::ShaderModule ShaderManager::CreateShaderModule(const std::vector<char> &code, vk::Device device)
{
    auto createInfo = vk::ShaderModuleCreateInfo(
        {},
        code.size(),
        reinterpret_cast<const uint32_t*>(code.data())
    );

    return device.createShaderModule(createInfo, nullptr);
}
