#include <vk_pipelines.h>
#include <fstream>
#include <vk_initializers.h>

namespace vkutil
{
    bool load_shader_module(const char* filePath, VkDevice device, VkShaderModule* outShaderModule)
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

    PipelineBuilder::PipelineBuilder()
    {
        clear();
    };

    void PipelineBuilder::clear()
    {
        _shaderStages.clear();

        _inputAssemblyState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO
        };

        _rasterizationState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO
        };

        _multisampleState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO
        };

        _depthStencilState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO
        };

        _colorBlendAttachmentState = {};
        _pipelineLayout = {};

        _renderPipeline = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO
        };
    }

    VkPipeline PipelineBuilder::build_pipeline(const VkDevice& _device)
    {
        VkPipelineViewportStateCreateInfo viewportInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .pNext = nullptr,
            .viewportCount = 1,
            .scissorCount = 1
        };

        VkPipelineColorBlendStateCreateInfo colorBlendInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .pNext = nullptr,
            .logicOpEnable = false,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments = &_colorBlendAttachmentState
        };

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO
        };

        VkDynamicState states[] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamicInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .pNext = nullptr,
            .dynamicStateCount = 2,
            .pDynamicStates = &states[0]
        };

        VkGraphicsPipelineCreateInfo pipelineInfo = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO
        };

        pipelineInfo.pNext = &_renderPipeline;
        pipelineInfo.stageCount = static_cast<uint32_t>(_shaderStages.size());
        pipelineInfo.pStages = _shaderStages.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &_inputAssemblyState;
        pipelineInfo.pViewportState = &viewportInfo;
        pipelineInfo.pRasterizationState = &_rasterizationState;
        pipelineInfo.pMultisampleState = &_multisampleState;
        pipelineInfo.pDepthStencilState = &_depthStencilState;
        pipelineInfo.pColorBlendState = &colorBlendInfo;
        pipelineInfo.pDynamicState = &dynamicInfo;
        pipelineInfo.layout = _pipelineLayout;

        VkPipeline pipeline;
        VkResult result = vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline);
        if (result != VK_SUCCESS)
        {
            fmt::println("Failed to create pipeline");
            return VK_NULL_HANDLE;
        }
        return pipeline;
    };

    // void PipelineBuilder::set_shaders(const VkShaderModule&, const VkShaderModule&);
    // void PipelineBuilder::set_input_topology(VkPrimitiveTopology);
    // void PipelineBuilder::set_polygon_mode(VkPolygonMode);
    // void PipelineBuilder::set_cull_mode(VkCullModeFlags, VkFrontFace);
    // void PipelineBuilder::set_multisampling_none();
    // void PipelineBuilder::disable_blending();
    // void PipelineBuilder::set_color_attachment_format(VkFormat);
    // void PipelineBuilder::set_depth_format();
    // void PipelineBuilder::disable_depth_test();
}