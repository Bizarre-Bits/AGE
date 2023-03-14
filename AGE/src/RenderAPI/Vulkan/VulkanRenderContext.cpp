//
// Created by alex on 3/7/23.
//

#include "agepch.h"

#include <map>
#include <set>

#include "VulkanRenderContext.h"
#include "QueueFamilyIndices.h"

namespace AGE {
  static const std::vector<const char*> s_ValidationLayers = {"VK_LAYER_KHRONOS_validation"};
  static const std::vector<const char*> s_RequiredExtensions = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

#if defined(DEBUG)
  constexpr bool c_EnableValidationLayers{true};
#else
  constexpr bool c_EnableValidationLayers{false};
#endif

  VulkanRenderContext::VulkanRenderContext(const VulkanRenderContextCreateInfo& createInfo) {
    Init(createInfo);
  }

  void VulkanRenderContext::Init(const VulkanRenderContextCreateInfo& createInfo) {
    CreateInstance(createInfo);
    CreateSurface(createInfo);
    SetupDebugMessenger();
    PickPhysicalDevice();
    CreateLogicalDevice();
    RetrieveQueues();
  }

  void VulkanRenderContext::SwapBuffers() {

  }

  void VulkanRenderContext::CreateInstance(const VulkanRenderContextCreateInfo& createInfo) {
    if (c_EnableValidationLayers && !CheckValidationLayerSupport()) {
      AGE_CORE_CRITICAL("Validation layers required, but not supported");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = createInfo.AppName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "AGEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;

    std::vector<const char*> extensions = GetRequiredExtensions();

    instanceCreateInfo.enabledExtensionCount = extensions.size();
    instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (c_EnableValidationLayers) {
      instanceCreateInfo.enabledLayerCount = s_ValidationLayers.size();
      instanceCreateInfo.ppEnabledLayerNames = s_ValidationLayers.data();

      PopulateDebugMessengerCreateInfo(debugCreateInfo);
      instanceCreateInfo.pNext = &debugCreateInfo;
    } else {
      instanceCreateInfo.enabledLayerCount = 0;
      instanceCreateInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&instanceCreateInfo, nullptr, &m_VkInstance) != VK_SUCCESS) {
      AGE_CORE_CRITICAL("Failed to create a Vulkan instance");
    }
    AGE_CORE_TRACE("Created VkInstance");

    uint32_t instanceExtensionCount{0};
    vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr);

    std::vector<VkExtensionProperties> instanceExtensions(instanceExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, instanceExtensions.data());

    AGE_CORE_TRACE("Available instance extensions:");
    for (const auto& extensionInfo: instanceExtensions)
      AGE_CORE_TRACE("\t{0}", extensionInfo.extensionName);
  }

  void VulkanRenderContext::SetupDebugMessenger() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    PopulateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugMessenger(m_VkInstance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS) {
      AGE_CORE_CRITICAL("Failed to create Vulkan debug messenger");
    }
  }

  VulkanRenderContext::~VulkanRenderContext() {
    Cleanup();
  }

  void VulkanRenderContext::Cleanup() {
    vkDestroyDevice(m_LogicalDevice, nullptr);
    DestroyDebugMessenger(m_VkInstance, m_DebugMessenger, nullptr);
    vkDestroySurfaceKHR(m_VkInstance, m_Surface, nullptr);
    vkDestroyInstance(m_VkInstance, nullptr);
  }

  bool VulkanRenderContext::CheckValidationLayerSupport() {
    uint32_t layerCount{0};
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* requiredLayerName: s_ValidationLayers) {
      bool layerFound{false};
      for (const auto& availableLayer: availableLayers) {
        if (strcmp(availableLayer.layerName, requiredLayerName) == 0) {
          layerFound = true;
          break;
        }
      }
      if (!layerFound)
        return false;
    }

    return true;
  }

  std::vector<const char*> VulkanRenderContext::GetRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (c_EnableValidationLayers) {
      extensions.reserve(glfwExtensionCount + s_RequiredExtensions.size());
      extensions.insert(extensions.end(), s_RequiredExtensions.begin(), s_RequiredExtensions.end());
    }

    return extensions;
  }

  VkBool32 VulkanRenderContext::debugCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT messageType,
      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
      void* pUserData
  ) {
    switch (messageSeverity) {
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        AGE_CORE_TRACE("{0}", pCallbackData->pMessage);
        break;
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        AGE_CORE_INFO("{0}", pCallbackData->pMessage);
        break;
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        AGE_CORE_WARN("{0}", pCallbackData->pMessage);
        break;
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        AGE_CORE_ERROR("{0}", pCallbackData->pMessage);
        break;
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
      AGE_CORE_CRITICAL("{0}", pCallbackData->pMessage);
        break;
      default:
        AGE_CORE_TRACE("{0}", pCallbackData->pMessage);
        break;
    }
    return VK_FALSE;
  }

  VkResult VulkanRenderContext::CreateDebugMessenger(VkInstance instance,
                                                     const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                     const VkAllocationCallbacks* pAllocator,
                                                     VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func == nullptr)
      return VK_ERROR_EXTENSION_NOT_PRESENT;

    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  }

  void VulkanRenderContext::DestroyDebugMessenger(
      VkInstance instance,
      VkDebugUtilsMessengerEXT messenger,
      const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
      func(instance, messenger, pAllocator);
  }

  void VulkanRenderContext::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;
  }

  void VulkanRenderContext::PickPhysicalDevice() {
    uint32_t deviceCount{0};
    vkEnumeratePhysicalDevices(m_VkInstance, &deviceCount, nullptr);

    if (deviceCount == 0) {
      AGE_CORE_CRITICAL("Failed to find a GPU with Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_VkInstance, &deviceCount, devices.data());

    std::map<uint32_t, const VkPhysicalDevice&> deviceRates;
    for (const auto& device: devices) {
      uint32_t rating = RatePhysicalDeviceSuitability(device);
      deviceRates.emplace(rating, device);
    }

    AGE_CORE_INFO("Available physical devices:");
    for (auto [rate, dev]: deviceRates) {
      VkPhysicalDeviceProperties props{};
      vkGetPhysicalDeviceProperties(dev, &props);
      AGE_CORE_INFO("\t{0} ({1})", props.deviceName, rate);
    }

    const auto [maxRate, maxDevice] = *deviceRates.rbegin();
    if (maxRate == 0) {
      AGE_CORE_CRITICAL("Failed to find a suitable GPU");
    }

    m_PhysicalDevice = maxDevice;
  }

  uint32_t VulkanRenderContext::RatePhysicalDeviceSuitability(VkPhysicalDevice device) {
    constexpr uint32_t GB{1024 * 1024 * 1024};
    uint32_t rate{0};

    if (!FindQueueFamilies(device, m_Surface).IsComplete())
      return 0;

    VkPhysicalDeviceFeatures features{};
    vkGetPhysicalDeviceFeatures(device, &features);

    if (!features.geometryShader)
      return 0;

    VkPhysicalDeviceProperties props{};
    vkGetPhysicalDeviceProperties(device, &props);

    if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      rate += 1000;
    }

    VkPhysicalDeviceMemoryProperties memProps{};
    vkGetPhysicalDeviceMemoryProperties(device, &memProps);

    std::vector<VkMemoryHeap> memoryHeaps(memProps.memoryHeaps, memProps.memoryHeaps + memProps.memoryHeapCount);
    for (const auto& heap: memoryHeaps) {
      if (heap.flags & VkMemoryHeapFlagBits::VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
        rate += heap.size / GB;
    }

    return rate;
  }

  void VulkanRenderContext::RetrieveQueues() {
    QueueFamilyIndices indices{FindQueueFamilies(m_PhysicalDevice, m_Surface)};
    vkGetDeviceQueue(m_LogicalDevice, indices.GraphicsFamily.value(), 0, &m_GraphicsQueue);
    vkGetDeviceQueue(m_LogicalDevice, indices.PresentFamily.value(), 0, &m_PresentQueue);
  }

  void VulkanRenderContext::CreateLogicalDevice() {
    QueueFamilyIndices indices{FindQueueFamilies(m_PhysicalDevice, m_Surface)};

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies{indices.GraphicsFamily.value(), indices.PresentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t family: uniqueQueueFamilies) {
      VkDeviceQueueCreateInfo queueCreateInfo{};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = indices.GraphicsFamily.value();
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &queuePriority;
      queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures physicalDeviceFeatures{};
    vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &physicalDeviceFeatures);

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = queueCreateInfos.size();
    createInfo.pEnabledFeatures = &physicalDeviceFeatures;

    createInfo.enabledExtensionCount = 0;

    if (c_EnableValidationLayers) {
      createInfo.enabledLayerCount = s_ValidationLayers.size();
      createInfo.ppEnabledLayerNames = s_ValidationLayers.data();
    } else {
      createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice) != VK_SUCCESS) {
      AGE_CORE_CRITICAL("Failed to create a Vulkan logical device");
    }
  }

  void VulkanRenderContext::CreateSurface(const VulkanRenderContextCreateInfo& contextCreateInfo) {
    if (glfwCreateWindowSurface(m_VkInstance, contextCreateInfo.WindowHandle, nullptr, &m_Surface) != VK_SUCCESS) {
      AGE_CORE_CRITICAL("Failed to create window surface");
    }
  }
} // AGE