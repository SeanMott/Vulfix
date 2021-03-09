//defines a genaric File Handler

#ifndef FileHandler_h
#define FileHandler_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

	//reads a text file
	const char* Vulfix_FileHandler_ReadTextFile(const char* filepath);

	//checks if two strings mathc
	bool Vulfix_FileHandler_StringMatch(const char* string1, const char* string2);


#ifdef __cplusplus
}
#endif

#endif