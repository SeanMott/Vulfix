//test bed for Vulfix

//Vulfix
#include <Vulfix\Instance.h>
//#include <Logger.h>

//lib
#include <vulkan\vulkan.h>
#include <GLFW/glfw3.h>

//other
#include <stdio.h>
//#include <mat4x4.hpp>
//#include <vec4.hpp>
//#include <iostream>

//logging method
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	//add checks for differnt flags for logging user callbacks
	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		printf("Vulkan Validation Layer: %s\n", pCallbackData->pMessage);

	//if(messageType == )
	//printf("Validation Layer: %s\n", pCallbackData->pMessage);
	//std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

//defines a method for creating the logger || convert to Vulfix
static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	//gets the create debug logger func pointer
	PFN_vkCreateDebugUtilsMessengerEXT func =  (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	//if it got it make the logger
	if (func != NULL)
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);

	//otherwise throw error
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}

//defines a method to destroy the logger || convert to Vulfix
static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator)
{
	//gets the func pointer
	PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	
	//if it got it destroy the logger
	if (func != NULL)
		func(instance, debugMessenger, pAllocator);

	//if it didn't throw a warning
	else
		printf("Vulfx Warning: Failed Dustruction || Failed to get the func pointer for \"vkDestroyDebugUtilsMessengerEXT\". Vulfix can't destroy the Debug Logger, check your Vulkan SDK, Github for updates, and around the net. Good Luck.\n");
}

//entry point
int main(int args, char* argv[])
{
	//--init app

	//makes a Display || add into a class once memory bugs are fixed
	if (!glfwInit())
	{
		printf("Failed to init GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Vulfix App", NULL, NULL);
	if (!window)
	{
		printf("Failed to make window!");
		return -1;
	}

	//set up Vulkan objects
	
	//instance
	Vulfix_Instance_CreateInfo instInfo;
	Vulfix_Instance_InitCreateInfo(&instInfo);
	
	//set layers
	const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
	instInfo.layerNames = layers;
	instInfo.layerCount = 1;

	//set extensions || we don't need to add any extra ones
	//const char* extensions[] = {  };
	//instInfo.extensionNames = extensions;
	//instInfo.extensionCount = 1;

#ifdef VULFIX_DIST
	instInfo.enableDebugger = false;
#else
	instInfo.enableDebugger = true;
#endif

	Vulfix_Instance* instance = Vulfix_Instance_Create(&instInfo);

	//debugger

	VkDebugUtilsMessengerEXT debugLogger;

	//sets up the debug struct
	VkDebugUtilsMessengerCreateInfoEXT loggerCreateInfo{};
	loggerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	loggerCreateInfo.messageSeverity = /*VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | */VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	loggerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	loggerCreateInfo.pfnUserCallback = debugCallback;
	loggerCreateInfo.pUserData = nullptr; // Optional

	//creates a logger
	if (CreateDebugUtilsMessengerEXT(instance->instance, &loggerCreateInfo, NULL, &debugLogger) != VK_SUCCESS)
		printf("Vulfix Error: Failed to create Debug Logger. Check that the right layers, extensions, and \"enableDebugger\" was set to true in the Instance Create Info. Otherwise check Github for updates and info around the net. Good Luck.\n");

	//Vulfix_DebugLogger_CreateInfo loggerInfo;
	//Vulfix_DLogger_InitCreateInfo(&loggerInfo);
	//Vulfix_DebugLogger* logger = Vulfix_DLogger_Create(&loggerInfo)

	//Device (GPU)

	//shader

	//texture

	//pipeline

	//--game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); //poll inputs

	}

	//--cleans up resources

	//destroy Vulfix objects

	//destroy if validation layers are enabled
	if (instInfo.enableDebugger)
		DestroyDebugUtilsMessengerEXT(instance->instance, debugLogger, NULL);

	Vulfix_Instance_Destroy(instance);
	
	//clean up Display
	glfwDestroyWindow(window);
	glfwTerminate();

	getchar();
	return 0;
}