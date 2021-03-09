#include <Vulfix\Instance.h>
#include <Logger.h>

//inits the Create Info
void Vulfix_Instance_InitCreateInfo(Vulfix_Instance_CreateInfo* info)
{
	if (!info)
	{
		LogFatalError("NULL Create Info", "Instance Create Info was NULL can not init!");
	}

	//info->appName = calloc(11, sizeof(char));
	info->appName = "Vulfix App";
	//calloc(5, sizeof(char));
	info->engineName = "None";

	info->applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	info->engineVersion = VK_MAKE_VERSION(1, 0, 0);
	info->apiVersion = VK_API_VERSION_1_0;

	info->flags = 0; //flag bits

	//layers
	info->layerNames = NULL;
	info->layerCount = 0;

	//extensions
	info->extensionNames = NULL;
	info->extensionCount = 0;

	//next pointers
	info->applicationPNext = NULL;
	info->instancePNext = NULL;

	//allocators
	info->allocator = NULL;
	info->deallocator = NULL;
}

//creates a Instance
Vulfix_Instance* Vulfix_Instance_Create(Vulfix_Instance_CreateInfo* info)
{
	if (!info)
	{
		LogFatalError("NULL Create Info", "Instance Create Info was NULL can not make instance!");
	}

	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = info->appName;
	appInfo.applicationVersion = info->applicationVersion;
	appInfo.pEngineName = info->engineName;
	appInfo.engineVersion = info->engineVersion;
	appInfo.apiVersion = info->apiVersion;
	appInfo.pNext = info->applicationPNext;

	VkInstanceCreateInfo instInfo;
	instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instInfo.pApplicationInfo = &appInfo;
	instInfo.flags = info->flags; //flag bits
	instInfo.pNext = info->instancePNext;

	//get extensions
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
	VkExtensionProperties* extensions = calloc(extensionCount, sizeof(VkExtensionProperties));
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions);

	//GLFW get required
	extensionCount = 0;
	const char** glfwExtensions = NULL;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

	instInfo.ppEnabledExtensionNames = glfwExtensions;
	instInfo.enabledExtensionCount = extensionCount;
	//instInfo.ppEnabledExtensionNames = info->extensionNames;
	//instInfo.enabledExtensionCount = info->extensionCount;

	//clean up extensions
	free(extensions);

	//layers
	instInfo.ppEnabledLayerNames = info->layerNames;
	instInfo.enabledLayerCount = info->layerCount;

	//make instance
	Vulfix_Instance* instance = malloc(sizeof(Vulfix_Instance));
	if (vkCreateInstance(&instInfo, info->allocator, &instance->instance) != VK_SUCCESS)
	{
		//free(instance);
		LogFatalError("NULL Instance Creation", "Failed to create a Vulkan Instance!");
	}

	instance->deallocator = info->deallocator;
	return instance;
}

//destroys a Instance
void Vulfix_Instance_Destroy(Vulfix_Instance* instance)
{
	if (!instance)
	{
		LogWarning("NULL Instance", "Instance was NULL can not destroy!");
		return;
	}

	vkDestroyInstance(instance->instance, instance->deallocator);
}