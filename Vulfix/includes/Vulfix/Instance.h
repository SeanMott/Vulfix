//defines the Vulkan instance

#ifndef Instance_h
#define Instance_h

#ifdef __cplusplus
extern "C" {
#endif

#include <vulkan\vulkan.h>
//#include <stdbool.h>
#include <stdint.h>

	//defines the create info
	typedef struct
	{
		//application info
		uint32_t applicationVersion, engineVersion, apiVersion;

		//instance info
		VkInstanceCreateFlags flags;
		uint32_t layerCount, extensionCount;
		const char* const* layerNames;
		const char* const* extensionNames;

		//names
		const char* appName;
		const char* engineName;

		//next pointers
		const void* applicationPNext;
		const void* instancePNext;

		//allocator
		const VkAllocationCallbacks* allocator;
		const VkAllocationCallbacks* deallocator;

	} Vulfix_Instance_CreateInfo;

	//inits the Create Info
	void Vulfix_Instance_InitCreateInfo(Vulfix_Instance_CreateInfo* info);

	//defines the instance
	typedef struct
	{
		VkInstance instance;
		const VkAllocationCallbacks* deallocator;

	} Vulfix_Instance;

	//creates a Instance
	Vulfix_Instance* Vulfix_Instance_Create(Vulfix_Instance_CreateInfo* info);
	//destroys a Instance
	void Vulfix_Instance_Destroy(Vulfix_Instance* instance);

#ifdef __cplusplus
}
#endif
#endif