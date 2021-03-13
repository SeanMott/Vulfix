#include <Vulfix\DebugLogger.h>

#include <Logger.h>

#include <stdio.h>

PFN_vkCreateDebugUtilsMessengerEXT Vulfix_DLogger_GetCreateFunc(VkInstance instance)
{
	if (instance == VK_NULL_HANDLE)
	{
		Vulfix_Log_NULLError("Instance", "Vulfix_DLogger_GetCreateFunc", "Instance was NULL, can not get debug log create func pointer.");
		return NULL;
		//printf("Vulfix Error: NULL Instance", )
	}

	//gets the create debug logger func pointer
	PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	
	if (func != NULL)
		return func;
	else
	{
		Vulfix_Log_NULLError("Extension Function Pointer", "Vulfix_DLogger_GetCreateFunc", "Failed to get the method pointer \"vkCreateDebugUtilsMessengerEXT\", check your layers, extensions, and if debug is enabled when creating the instance.");
		//printf("Vulfix Error: NULL Extension Function Pointer || Failed to get the method pointer \"vkCreateDebugUtilsMessengerEXT\"!\n");
		return NULL;
	}
}

PFN_vkDestroyDebugUtilsMessengerEXT Vulfix_DLogger_GetDestroyFunc(VkInstance instance)
{
	if (instance == VK_NULL_HANDLE)
	{
		Vulfix_Log_NULLError("Instance", "Vulfix_DLogger_GetDestroyFunc", "Instance was NULL, can not get debug log destroy func pointer.");
		return NULL;
		//printf("Vulfix Error: NULL Instance", )
	}

	//gets the destroy debug logger func pointer
	PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	
	if (func != NULL)
		return func;
	else
	{
		Vulfix_Log_NULLError("Extension Function Pointer", "Vulfix_DLogger_GetDestroyFunc", "Failed to get the method pointer \"vkDestroyDebugUtilsMessengerEXT\", check your layers, extensions, and if debug is enabled when creating the instance.");
		//printf("Vulfix Error: NULL Extension Function Pointer || Failed to get the method pointer \"vkDestroyDebugUtilsMessengerEXT\"!\n");
		return NULL;
	}
}

void Vulfix_DLogger_InitVulkanCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* vulkanLoggerCreateInfo)
{
	if (vulkanLoggerCreateInfo == NULL)
	{
		Vulfix_Log_NULLError("VkDebugUtilsMessengerCreateInfoEXT", "Vulfix_DLogger_InitVulkanCreateInfo", "The Vulkan Debug Logger create info must be given to this function. If you want to use the Vulfix version call \"Vulfix_DLogger_InitCreateInfo\".");
		return;
	}

	vulkanLoggerCreateInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	vulkanLoggerCreateInfo->messageSeverity = Vulfix_MsgSev_All;
	vulkanLoggerCreateInfo->messageType = Vulfix_MsgType_All;
	vulkanLoggerCreateInfo->pfnUserCallback = NULL;
	vulkanLoggerCreateInfo->pUserData = NULL;
	vulkanLoggerCreateInfo->pNext = NULL;
}

void Vulfix_DLogger_InitCreateInfo(Vulfix_DebugLogger_CreateInfo* info)
{
	if (info == NULL)
	{
		Vulfix_Log_NULLError("Vulfix_DebugLogger_CreateInfo", "Vulfix_DLogger_InitCreateInfo", "The Create info is needed if your using Vulfix to set up the debug logger.");
		return;
	}

	//what kinds of messages should be recived
	info->messageSeverity = Vulfix_MsgSev_All;
	info->messageType = Vulfix_MsgType_All;

	//flags
	info->flags = 0;

	//the method the user writes to handle logging
	info->debugLogCallback = NULL;

	//data pointers
	info->pNext = NULL;
	info->pUserData = NULL;

	//allocators
	info->allocator = NULL;
	info->deallocator = NULL;
}

Vulfix_DebugLogger* Vulfix_DLogger_Create(VkInstance instance, Vulfix_DebugLogger_CreateInfo* info)
{
	if (instance == VK_NULL_HANDLE)
	{
		Vulfix_Log_NULLError("Instance", "Vulfix_DLogger_Create", "Instance is needed to make a Logger.");
		return NULL;
	}

	if (!info)
	{
		Vulfix_Log_NULLError("Vulfix Debug Logger Create Info", "Vulfix_DLogger_Create", "The Vulfix Logger create info is needed.");
		return NULL;
	}

	//sets up the logger data
	VkDebugUtilsMessengerCreateInfoEXT loggerInfo;
	Vulfix_DLogger_InitVulkanCreateInfo(&loggerInfo);
	loggerInfo.flags = info->flags;
	loggerInfo.messageSeverity = info->messageSeverity;
	loggerInfo.messageType = info->messageType;
	loggerInfo.pfnUserCallback = info->debugLogCallback;
	loggerInfo.pNext = info->pNext;
	loggerInfo.pUserData = info->pUserData;

	//gets a create pointer
	PFN_vkCreateDebugUtilsMessengerEXT func = Vulfix_DLogger_GetCreateFunc(instance);
	if (func == NULL)
		return NULL;

	//makes the logger
	Vulfix_DebugLogger* logger = malloc(sizeof(Vulfix_DebugLogger));
	logger->debugLogger = VK_NULL_HANDLE;
	if (func(instance, &loggerInfo, info->allocator, &logger->debugLogger) != VK_SUCCESS)
	{
		//free(logger);
		Vulfix_Log_FailedObjectCreation("Debug Logger", "Vulfix_DLogger_Create", "Check if debug is enabled and that the create info was set up properly.");
		return NULL;
	}

	logger->deallocator = info->deallocator;
	return logger;
}

void Vulfix_DLogger_Destroy(VkInstance instance, Vulfix_DebugLogger* logger)
{
	if (instance == VK_NULL_HANDLE)
	{
		Vulfix_Log_NULLError("Instance", "Vulfix_DLogger_Create", "Instance is needed to destroy a Logger.");
		return;
	}

	if (!logger || logger->debugLogger == VK_NULL_HANDLE)
	{
		Vulfix_Log_NULLWarning("Vulfix Debug Logger", "Vulfix_DLogger_Create", "The Vulfix Logger is NULL so it can't be destroyed.");
		return;
	}

	//gets the destroy func pointer
	PFN_vkDestroyDebugUtilsMessengerEXT func = Vulfix_DLogger_GetDestroyFunc(instance);
	if (func == NULL)
	{
		Vulfix_Log_FailedObjectDestruction("Debug Logger", "Vulfix_DLogger_Destroy", "Failed to destroy the debug logger since Vulfix couldn't get the destroy func pointer.");
		return;
	}

	//attempts to destroy the debug logger
	func(instance, logger->debugLogger, logger->deallocator);
}