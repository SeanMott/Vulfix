//defines a Dynamic List

#ifndef CDataStructs_List_h
#define CDataStructs_List_h

/*
* Based from:
Source: https://www.youtube.com/watch?v=QKZXQc8EBDk&t=1004s
Author: Ianertson || Dynamic list in C
*/

#ifdef __cplusplus

extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

	typedef struct
	{
		size_t itemSize; //the size of the chosen item in memory
		size_t length; //the amount of items in the List
		void** items; //the items in the List

	} CDataStructs_List;

	//creates a List
	CDataStructs_List* CDataStructs_List_Create(size_t itemSize);
	//destroys a List
	void CDataStructs_List_Destroy(CDataStructs_List* list, void(*freeMethod)(void* item));

	//appends a item to the List || stops duplicates and NULL items
	void CDataStructs_List_Append(CDataStructs_List* list, void* item);
	//appends a item to the List || allows a duplicate and stops Null items
	void CDataStructs_List_AppendDuplicate(CDataStructs_List* list, void* item);
	//appends a item to the List || stops duplicates and allows NULL items
	void CDataStructs_List_AppendNull(CDataStructs_List* list, void* item);
	//appends a item to the List || allows duplicates and NULL items
	void CDataStructs_List_AppendNullDuplicate(CDataStructs_List* list, void* item);
	//removes a item from the List
	void CDataStructs_List_RemoveItem(CDataStructs_List* list, void* item, void(*freeMethod)(void*));
	//removes a item by position from the List
	void CDataStructs_List_RemovePos(CDataStructs_List* list, uint32_t pos, void(*freeMethod)(void*));

	//finds if a item is in the List
	bool CDataStructs_List_IsItem(CDataStructs_List* list, void* item, bool checkValidPrams);
	//gets the position of the item in the List || -1 will be returned if no item is their.
	size_t CDataStructs_List_ItemPosition(CDataStructs_List* list, void* item, bool checkValidPrams);

	//prints the contents of the List
	void CDataStructs_List_Print(CDataStructs_List* list);

#ifdef __cplusplus
}
#endif

#endif