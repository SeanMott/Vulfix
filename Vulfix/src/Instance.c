#include <Vulfix\Instance.h>
#include <Logger.h>

#include <GLFW\glfw3.h>

//#include <stdlib.h>
#include <string.h>
//#include <cstring>

#include <List.h>

#ifndef VULFIX_DIST

//is used by the logger if debug is enabled
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		printf("Vulkan Validation Layer: %s\n", pCallbackData->pMessage);

	return VK_FALSE;
}

#endif

//cleans up the Dynamic List if it's used
static void CleanList(void* string)
{
	free(string);
	string = NULL;
}

//checks for validation layer support || returns false if a layer was not on the system
static bool Vulfix_Instance_CheckLayerSupport(const char** layers, uint32_t count)
{
	if (layers == NULL)
	{
		Vulfix_Log_NULLError("layers", "Vulfix_Instance_CheckLayerSupport", "No layers were found, can not check their validity.");
		return false;
	}

	//gets the system layers
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, NULL);
	VkLayerProperties* availablleLayers = (VkLayerProperties*)calloc(layerCount, sizeof(VkLayerProperties));
	vkEnumerateInstanceLayerProperties(&layerCount, availablleLayers);

	//checks if the layers are availablle
	bool isFound = false;
	for (int32_t l = 0; l < count; ++l)
	{
		isFound = false;

		//the layers found on the system
		for (int32_t al = 0; al < layerCount; ++al)
		{
			if (!strcmp(layers[l], availablleLayers[al].layerName))
			{
				isFound = true;
				break;
			}
		}

		//if it wasn't found break
		if (!isFound)
		{
			printf("Vulfix Warning: NULL Validation Layer || %s was not found on this system!\n", layers[l]);
			free(availablleLayers);
			availablleLayers = NULL;
			return false;
		}
	}

	//clean up
	free(availablleLayers);
	availablleLayers = NULL;
	return true;
}

//inits the Create Info
void Vulfix_Instance_InitCreateInfo(Vulfix_Instance_CreateInfo* info)
{
	if (!info)
	{
		LogFatalError("NULL Create Info", "Instance Create Info was NULL can not init!");
	}

	info->enableDebugger = true; //if debugging layers should be used

	info->appName = "Vulfix App";
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

	//check layers if they are enabled
	if (info->enableDebugger)
		Vulfix_Instance_CheckLayerSupport(info->layerNames, info->layerCount);

	//set the application and instance creation structs
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
	//uint32_t extensionCount = 0;
	//vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
	//VkExtensionProperties* extensions = calloc(extensionCount, sizeof(VkExtensionProperties));
	//vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions);

	//gets required extentions
	uint32_t extensionCount = 0;
	const char** GLFWExtensions = NULL;

	GLFWExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);
	if (extensionCount == 0 || GLFWExtensions == NULL)
	{
		LogFatalError("Required Extension Failed", "GLFW failed to get the needed extensions!");
	}

	//allocates the list if it's needed
	CDataStructs_List* list = NULL;
	if (info->extensionCount > 0 && info->extensionNames != NULL || info->enableDebugger)
	{
		list = CDataStructs_List_Create(sizeof(char*));

		//adds the required extentions
		for (int32_t i = 0; i < extensionCount; ++i)
			CDataStructs_List_Append(list, (void*)GLFWExtensions[i]);
	}

	//add the user's extensions
	if (info->extensionCount > 0 && info->extensionNames != NULL)
	{
		for (int32_t i = 0; i < info->extensionCount; ++i)
			CDataStructs_List_Append(list, (void*)info->extensionNames[i]);
	}

	//adds the debug logger extension
	if (info->enableDebugger)
		CDataStructs_List_Append(list, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	//log all the extensions
	/*if (list != NULL)
	{
		for (int32_t i = 0; i < list->length; ++i)
			printf("%s\n", list->items[i]);
	}*/

	//sets the extensions to use
	if (list == NULL)
	{
		instInfo.ppEnabledExtensionNames = GLFWExtensions;
		instInfo.enabledExtensionCount = extensionCount;
	}
	else
	{
		instInfo.ppEnabledExtensionNames = (const char**)list->items;
		instInfo.enabledExtensionCount = list->length;
	}

	//if debug is enabled created a logger || allows object destruction and construction be tracked.
#ifndef VULFIX_DIST
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (info->enableDebugger)
	{
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;// | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;// | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugCreateInfo.pfnUserCallback = DebugCallback;
		debugCreateInfo.pUserData = NULL; // Optional

		//makes a debug logger
		instInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
#endif

	//sets the layers to use
	if (info->enableDebugger)
	{
		instInfo.ppEnabledLayerNames = info->layerNames;
		instInfo.enabledLayerCount = info->layerCount;
	}
	else
	{
		instInfo.ppEnabledLayerNames = NULL;
		instInfo.enabledLayerCount = 0;
	}

	//make instance
	Vulfix_Instance* instance = (Vulfix_Instance*)malloc(sizeof(Vulfix_Instance));
	instance->instance = VK_NULL_HANDLE;
	if (vkCreateInstance(&instInfo, info->allocator, &instance->instance) != VK_SUCCESS)
	{
		//free(instance);
		LogFatalError("NULL Instance Creation", "Failed to create a Vulkan Instance!");
	}

	instance->deallocator = info->deallocator; //sets a deallocator if one to use
	//tells the instance if the debugger is enabled
	instance->debuggerIsEnabled = info->enableDebugger;

	CDataStructs_List_Destroy(list, CleanList); //cleans up the list

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