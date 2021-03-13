//test bed for Vulfix

//Vulfix
#include <Vulfix\Instance.h>
#include <Vulfix\DebugLogger.h>
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
	if (messageSeverity & Vulfix_MsgSev_Error)
		printf("Vulkan Validation Layer: %s\n", pCallbackData->pMessage);

	//if(messageType == )
	//printf("Validation Layer: %s\n", pCallbackData->pMessage);
	//std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
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
	Vulfix_DebugLogger_CreateInfo loggerInfo;
	Vulfix_DLogger_InitCreateInfo(&loggerInfo);
	loggerInfo.debugLogCallback = debugCallback;
	loggerInfo.messageSeverity = Vulfix_MsgSev_Error;
	//all is the default for the inited create info
	//loggerInfo.messageType = Vulfix_MsgType_All;
	Vulfix_DebugLogger* logger = Vulfix_DLogger_Create(instance->instance, &loggerInfo);

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
		Vulfix_DLogger_Destroy(instance->instance, logger);

	Vulfix_Instance_Destroy(instance);
	
	//clean up Display
	glfwDestroyWindow(window);
	glfwTerminate();

	getchar();
	return 0;
}