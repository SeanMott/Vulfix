//defines a Debug Logger for Vulfix

#ifndef DebugLogger_h
#define DebugLogger_h

#ifdef __cplusplus
extern "C" {
#endif

#include <vulkan\vulkan.h>

    //defines flags for logging message type
    typedef enum
    {
        Vulfix_MsgType_General = 0x00000001, //general info should be logged
        Vulfix_MsgType_Error = 0x00000002, //a error message should be logged
        Vulfix_MsgType_Performance = 0x00000004, //logs when Vulkan isn't used optimally
        Vulfix_MsgType_All = Vulfix_MsgType_General | Vulfix_MsgType_Error | Vulfix_MsgType_Performance
    } Vulfix_DLogger_MsgType;

    //defines flags for logging message severity
    typedef enum
    {
        Vulfix_MsgSev_DriverLoaderLevel = 0x00000001, //logs messages from the driver, layer, and Vulkan loader level
        Vulfix_MsgSev_Info = 0x00000010, //logs data from resources
        Vulfix_MsgSev_Warning = 0x00000100, //any warnings Vulkan may spit out such as using unmapped memory for textures or errors in shader compile
        Vulfix_MsgSev_Error = 0x00001000, //logs Vulkan errors, misused GPU calls or something related to the speciifcation
        Vulfix_MsgSev_All = Vulfix_MsgSev_DriverLoaderLevel | Vulfix_MsgSev_Info | Vulfix_MsgSev_Warning | Vulfix_MsgSev_Error
    } Vulfix_DLogger_MsgSeverity;

    //gets create function pointer
    PFN_vkCreateDebugUtilsMessengerEXT Vulfix_DLogger_GetCreateFunc(VkInstance instance);
    //gets the destroy function pointer
    PFN_vkDestroyDebugUtilsMessengerEXT Vulfix_DLogger_GetDestroyFunc(VkInstance instance);

    //defines the create info
    typedef struct
    {
        //what kinds of messages should be recived
        VkDebugUtilsMessageSeverityFlagsEXT messageSeverity;
        VkDebugUtilsMessageTypeFlagsEXT messageType;

        //flags
        VkDebugUtilsMessengerCreateFlagsEXT flags;

        //the method the user writes to handle logging
        PFN_vkDebugUtilsMessengerCallbackEXT debugLogCallback;

        //data pointers
        const void* pNext;
        void* pUserData;

        //allocators
        const VkAllocationCallbacks* allocator;
        const VkAllocationCallbacks* deallocator;

    } Vulfix_DebugLogger_CreateInfo;

    //inits the Vulkan debug logger create info
    void Vulfix_DLogger_InitVulkanCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* vulkanLoggerCreateInfo);
    //inits the Vulfix logger util create info
    void Vulfix_DLogger_InitCreateInfo(Vulfix_DebugLogger_CreateInfo* info);

    //defines the logger
    typedef struct
    {
        VkDebugUtilsMessengerEXT debugLogger;
        const VkAllocationCallbacks* deallocator;
    } Vulfix_DebugLogger;

    //creates the logger
    Vulfix_DebugLogger* Vulfix_DLogger_Create(VkInstance instance, Vulfix_DebugLogger_CreateInfo* info);
    //destroys the logger
    void Vulfix_DLogger_Destroy(VkInstance instance, Vulfix_DebugLogger* info);

#ifdef __cplusplus
}
#endif

#endif