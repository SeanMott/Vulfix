//test bed for Vulfix

//Vulfix
#include <Vulfix\Instance.h>
//#include <Logger.h>

//lib
#include <vulkan\vulkan.h>
#include <GLFW/glfw3.h>

//other
#include <stdio.h>
#include <mat4x4.hpp>
#include <vec4.hpp>
#include <iostream>


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
	
	//set extensions
	//instInfo.extensions = {""};
	//instInfo.extensionCount = 1;
	
	//set layers
	//instInfo.layers = {""};
	//instInfo.layerCount = 1;

	Vulfix_Instance* instance = Vulfix_Instance_Create(&instInfo);

	//debugger

	//Device (GPU)

	//--game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

	}

	//--cleans up resources

	//destroy Vulfix objects
	Vulfix_Instance_Destroy(instance);
	
	//clean up Display
	glfwDestroyWindow(window);
	glfwTerminate();

	getchar();
	return 0;
}