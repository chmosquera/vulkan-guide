#include <vk_pipelines.h>
#include <fstream>
#include <vk_initializers.h>

bool vkutil::load_shader_module(const char* filePath, VkDevice device, VkShaderModule* outShaderModule)
{
    // ate - opens file with cursor AT the End
    std::ifstream file(filePath, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        return false;
    }

    // Since cursor is at end, we can get the file size
    size_t fileSize = (size_t)file.tellg();

    // create a buffer to store the compute shader
    // SPIR-V expects buffer to be uint32, so use that type
    std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

    // Finally, read the file and store it in the buffer
    file.seekg(0);
    file.read((char*)buffer.data(),fileSize);
    file.close();

    // Create a shader module and store the buffer that contains the shader
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = nullptr;

    createInfo.codeSize = buffer.size() * sizeof(uint32_t);
    createInfo.pCode = buffer.data();

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        return false;
    }
    *outShaderModule = shaderModule;
    return true;
}