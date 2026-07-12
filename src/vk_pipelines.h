#pragma once 
#include <vk_types.h>

namespace vkutil {

    class PipelineBuilder
    {
    public:
        PipelineBuilder();
        void clear();
        VkPipeline build_pipeline(const VkDevice&);

        void set_shaders(const VkShaderModule&, const VkShaderModule&);
        void set_input_topology(VkPrimitiveTopology);
        void set_polygon_mode(VkPolygonMode);
        void set_cull_mode(VkCullModeFlags, VkFrontFace);
        void set_multisampling_none();
        void disable_blending();
        void set_color_attachment_format(VkFormat);
        void set_depth_format(VkFormat);
        void disable_depth_test();

        VkPipelineLayout _pipelineLayout;

    private:
        std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
        VkPipelineInputAssemblyStateCreateInfo _inputAssemblyState;
        VkPipelineRasterizationStateCreateInfo _rasterizationState;
        VkPipelineMultisampleStateCreateInfo _multisampleState;
        VkPipelineDepthStencilStateCreateInfo _depthStencilState;

        VkPipelineColorBlendAttachmentState _colorBlendAttachmentState;
        VkFormat _colorAttachmentFormat;

        VkPipelineRenderingCreateInfo _renderPipeline;
    };

    bool load_shader_module(const char* filePath, VkDevice device, VkShaderModule* outShaderModule);

};