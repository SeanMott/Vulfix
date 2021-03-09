//Windows implementation of the file handler

#include <Util\FileHandler.h>
#include <string.h>
#include <stdio.h>

const char* Vulfix_FileHandler_ReadTextFile(const char* filepath)
{
	FILE* file;
	char* text = 0;
	long length;

	file = fopen(filepath, "rb");
	if (!file)
	{
		LogData("Error: File Handler || Failed to open text file at: %s\n", filepath);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);
	text = calloc(length, length);
	if (text)
		fread(text, 1, length, file);

	fclose(file);

	return text;
}

bool Vulfix_FileHandler_StringMatch(const char* string1, const char* string2)
{
	return (strcmp(string1, string2) == 0 ? true : false);
}