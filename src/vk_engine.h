// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <vk_types.h>
#include <vk_descriptors.h>

constexpr unsigned int FRAME_OVERLAP = 2;

struct DeletionQueue
{
    std::deque<std::function<void()>> deletors;

    void push_function(std::function<void()>&& function) { deletors.push_back(function); }

    void flush()
    {
        // Call the deletors in reverse order - LIFOc
        for (auto it = deletors.rbegin(); it != deletors.rend(); ++it)
        {
            (*it)();
        }

        deletors.clear();
    }
};

struct FrameData
{
    VkCommandPool _commandPool;
    VkCommandBuffer _mainCommandBuffer;
    VkSemaphore _swapchainSemaphore, _renderSemaphore;
    VkFence _renderFence;
    DeletionQueue _deletionQueue;
};

struct ComputePushConstants
{
    glm::vec4 data1;
    glm::vec4 data2;
    glm::vec4 data3;
    glm::vec4 data4;
};

struct ComputeEffect
{
    const char* name;
    VkPipeline pipeline;
    VkPipelineLayout layout;
    ComputePushConstants pushConstants;

};

class VulkanEngine
{
public:
    bool _isInitialized{false};
    int _frameNumber{0};
    bool stop_rendering{false};
    VkExtent2D _windowExtent{1700, 900};

    struct SDL_Window* _window{nullptr};

    static VulkanEngine& Get();

    // initializes everything in the engine
    void init();

    // shuts down the engine
    void cleanup();

    // draw loop
    void draw();
    void draw_background(VkCommandBuffer cmd);
    void draw_imgui(VkCommandBuffer imguiCmd, VkImageView targetView);
    void draw_geometry(const VkCommandBuffer& cmd);

    // run main loop
    void run();

    VkInstance _instance;
    VkDebugUtilsMessengerEXT _debug_messenger;
    VkPhysicalDevice _chosenGPU;
    VkDevice _device;
    VkSurfaceKHR _surface;

    VkSwapchainKHR _swapchain;
    VkFormat _swapchainImageFormat;

    std::vector<VkImage> _swapchainImages;
    std::vector<VkImageView> _swapchainImageViews;
    VkExtent2D _swapchainExtent;

    FrameData _frames[FRAME_OVERLAP];
    FrameData& get_current_frame() { return _frames[_frameNumber % FRAME_OVERLAP]; };

    VkQueue _graphicsQueue;
    uint32_t _graphicsQueueFamily;

    DeletionQueue _mainDeletionQueue;

    VmaAllocator _allocator;

    // Draw resources
    AllocatedImage _drawImage = {};
    VkExtent2D _drawExtent;

    // Descriptors
    DescriptorAllocator globalDescriptorAllocator;
    VkDescriptorSet _drawImageDescriptors; // for storing the render image
    VkDescriptorSetLayout _drawImageDescriptorLayout;

    // Pipeline
    VkPipeline _gradientPipeline;
    VkPipelineLayout _gradientPipelineLayout;

    // ImGUI - immediate commands
    VkCommandBuffer _immCommandBuffer;
    VkCommandPool _immCommandPool;
    VkFence _immFence;

    void immediate_submit(std::function<void(VkCommandBuffer cmd)>&&);

    std::vector<ComputeEffect> backgroundEffects;
    int currentComputeEffect = 0;

    // Triangle pipeline
    VkPipeline _trianglePipeline;
    VkPipelineLayout _trianglePipelineLayout;

private:
    void init_vulkan();
    void init_swapchain();
    void init_commands();
    void init_sync_structures();
    void init_descriptors();
    void init_pipelines();
    void init_imgui();

    void create_swapchain(uint32_t width, uint32_t height);
    void destroy_swapchain();

    void init_triangle_pipeline();
};
