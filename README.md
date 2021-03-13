# Vulfix
Vulfix is a Vulkan lib that speeds up the boiler plate code. It doesn't take away the low level control but wraps it in functions and struct objects.

# Example

```c

    //sets up a Vulkan Instance
	Vulfix_Instance_CreateInfo instInfo;
	Vulfix_Instance_InitCreateInfo(&instInfo);
	
	//set layers
	const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
	instInfo.layerNames = layers;
	instInfo.layerCount = 1;

	//set extra layers if you want them. GLFW is used to get required ones
	//const char* extensions[] = {  };
	//instInfo.extensionNames = extensions;
	//instInfo.extensionCount = 1;

#ifdef VULFIX_DIST
	instInfo.enableDebugger = false;
#else
	instInfo.enableDebugger = true;
#endif

	Vulfix_Instance* instance = Vulfix_Instance_Create(&instInfo);

    //sets up a debugger
	Vulfix_DebugLogger_CreateInfo loggerInfo;
	Vulfix_DLogger_InitCreateInfo(&loggerInfo);
	loggerInfo.debugLogCallback = debugCallback;
	loggerInfo.messageSeverity = Vulfix_MsgSev_Error;
	//all is the default for the inited create info
	//loggerInfo.messageType = Vulfix_MsgType_All;
	Vulfix_DebugLogger* logger = Vulfix_DLogger_Create(instance->instance, &loggerInfo);

```

# Builindg
Vulfix can be built with the premake file.
