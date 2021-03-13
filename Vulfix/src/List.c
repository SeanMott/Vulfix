#include <List.h>

//#include <CDataStructs\List.h>

//#include <CDataStructs\Logger.h>

//shifts items left in Lists
static inline void CDataStructs_List_ShiftItemsLeft(CDataStructs_List* list, int32_t index)
{
    if (!list || index >= list->length || index <= -1)
        return;

    for (index; index < list->length - 1; index++)
        list->items[index] = list->items[index + 1];
}

CDataStructs_List* CDataStructs_List_Create(size_t itemSize)
{
    if (itemSize <= 0)
    {
        //CDataStructs_LogError("List Creation", "Item Size must be greater then 0!");
        return NULL;
    }

    CDataStructs_List* list = malloc(sizeof(CDataStructs_List));
    list->itemSize = itemSize;
    list->length = 0;
    list->items = NULL;

    return list;
}

void CDataStructs_List_Destroy(CDataStructs_List* list, void(*freeMethod)(void* item))
{
    if (!list)
    {
       // CDataStructs_LogError("List Destruction", "List is NULL can not be destroyed!");
        return;
    }

    if (freeMethod)
    {
        for (uint32_t i = list->length - 1; i > -1; i--)
        {
            if (!list->items[i])
                continue;
                
            freeMethod(list->items[i]);
        }
    }

    free(list);
    list = NULL;
}

void CDataStructs_List_Append(CDataStructs_List* list, void* item)
{
    if (!list)
    {
       // CDataStructs_LogError("List Append", "List is NULL can not append data!");
        return;
    }

    if (!item)
    {
       // CDataStructs_LogError("List Append", "Item is NULL can not append to List!");
        return;
    }

    //empty
    if (!list->items)
    {
        list->length++;
        list->items = calloc(1, list->itemSize);
        list->items[0] = item;
    }
    //if has data
    else
    {
        if (!CDataStructs_List_IsItem(list, item, false))
        {
            list->length++;
            list->items = realloc(list->items, list->length * list->itemSize);
            list->items[list->length - 1] = item;
        }
       // else
         //   CDataStructs_LogWarning("List Append Duplicate", "The List already contains that item, if you wish to add the same item uses \"AppendDuplicate\".");
    }
}

void CDataStructs_List_AppendDuplicate(CDataStructs_List* list, void* item)
{
    if (!list)
    {
       // CDataStructs_LogError("List Append", "List is NULL can not append data!");
        return;
    }

    if (!item)
    {
      //  CDataStructs_LogError("List Append", "Item is NULL can not append to List!");
        return;
    }

    //empty
    if (!list->items)
    {
        list->length++;
        list->items = calloc(1, list->itemSize);
        list->items[0] = item;
    }
    //if has data
    else
    {
        list->length++;
        list->items = realloc(list->items, list->length * list->itemSize);
        list->items[list->length - 1] = item;
    }
}

void CDataStructs_List_AppendNull(CDataStructs_List* list, void* item)
{
    if (!list)
    {
       // CDataStructs_LogError("List Append", "List is NULL can not append data!");
        return;
    }

    //empty
    if (!list->items)
    {
        list->length++;
        list->items = calloc(1, list->itemSize);
        list->items[0] = item;
    }
    //if has data
    else
    {
        if (!CDataStructs_List_IsItem(list, item, false))
        {
            list->length++;
            list->items = realloc(list->items, list->length * list->itemSize);
            list->items[list->length - 1] = item;
        }
      //  else
         //   CDataStructs_LogWarning("List Append Duplicate", "The List already contains that item, if you wish to add the same item uses \"AppendDuplicate\".");
    }
}

void CDataStructs_List_AppendNullDuplicate(CDataStructs_List* list, void* item)
{
    if (!list)
    {
        //CDataStructs_LogError("List Append", "List is NULL can not append data!");
        return;
    }

    //empty
    if (!list->items)
    {
        list->length++;
        list->items = calloc(1, list->itemSize);
        list->items[0] = item;
    }
    //if has data
    else
    {
        list->length++;
        list->items = realloc(list->items, list->length * list->itemSize);
        list->items[list->length - 1] = item;
    }
}

void CDataStructs_List_RemoveItem(CDataStructs_List* list, void* item, void(*freeMethod)(void*))
{
    if (!list)
    {
       // CDataStructs_LogError("List Remove", "List is NULL can not remove items!");
        return;
    }

    if (!item)
    {
       // CDataStructs_LogError("List Remove", "Item is NULL can not remove items!");
        return;
    }

    if (list->length <= 0 || !list->items)
    {
      //  CDataStructs_LogWarning("List Remove", "The List has no items, add some with \"Append\" or \"AppendDuplicate\" first.");
        return;
    }

    int32_t pos = CDataStructs_List_ItemPosition(list, item, false);
    if (pos == -1)
    {
       // CDataStructs_LogWarning("List Remove", "The List does not contain the give item, check the pointer or add it with \"Append\" or \"AppendDuplicate\" first.");
        return;
    }
    CDataStructs_List_ShiftItemsLeft(list, pos);
    if (freeMethod)
        freeMethod(item);

    list->length--;

    list->items = realloc(list->items, list->itemSize * list->length);
}

void CDataStructs_List_RemovePos(CDataStructs_List* list, uint32_t pos, void(*freeMethod)(void*))
{
    if (!list)
    {
      //  CDataStructs_LogError("List Remove", "List is NULL can not remove items!");
        return;
    }

    if (list->length <= 0 || !list->items)
    {
      //  CDataStructs_LogWarning("List Remove", "The List has no items, add some with \"Append\" or \"AppendDuplicate\" first.");
        return;
    }

    if (pos == -1 || pos >= list->length)
    {
       // CDataStructs_LogData("CDataStructs Error: List Remove || The List does not have a item at %i, append one with \"Append\" or \"AppendDuplicate\" first!\n", pos);
        return;
    }
    if (freeMethod)
        freeMethod(list->items[pos]);

    CDataStructs_List_ShiftItemsLeft(list, pos);
    list->length--;

    list->items = realloc(list->items, list->itemSize * list->length);

   // CDataStructs_LogMessage("CDataStructs_List_RemovePos is currently not supported. Check Github for updates.");
}

bool CDataStructs_List_IsItem(CDataStructs_List* list, void* item, bool checkValidPrams)
{
    if (checkValidPrams)
    {
        if (!list)
        {
          //  CDataStructs_LogError("List", "List is NULL can not check for the item, returning false!");
            return false;
        }

        if (!item)
        {
         //   CDataStructs_LogError("List", "Item is NULL can not check for the item, returning false!");
            return false;
        }
    }

    for (uint32_t i = 0; i < list->length; i++)
    {
        if (list->items[i] == item)
            return true;
    }

    return false;
}

size_t CDataStructs_List_ItemPosition(CDataStructs_List* list, void* item, bool checkValidPrams)
{
    if (checkValidPrams)
    {
        if (!list)
        {
          //  CDataStructs_LogError("List", "List is NULL can not check the position of a item, returning -1!");
            return -1;
        }

        if (!item)
        {
          //  CDataStructs_LogError("List", "Item is NULL can not check the position of a item, returning -1!");
            return -1;
        }
    }

    for (uint32_t i = 0; i < list->length; i++)
    {
        if (list->items[i] == item)
            return i;
    }

    return -1;
}

void CDataStructs_List_Print(CDataStructs_List* list)
{
    if (!list)
    {
        //CDataStructs_LogError("List", "List is Null can not print it's contents!");
        return;
    }

  //  CDataStructs_LogData("CDataStructs_List: Item Count: %i, Item Memory Size: %i, List Total Size: %i\n", list->length,
    //    list->itemSize, list->itemSize * list->length);
}
