#ifndef DEMO_LIB_H
#define DEMO_LIB_H

#include <stdint.h>

typedef enum
{
    DL_EventType_Created,
    DL_EventType_Destroyed,
    DL_EventType_Changed,
} DL_EventType;

typedef struct
{
    DL_EventType type;
} DL_Event;

typedef struct
{
    DL_Event baseEvent;
    const char* creationString;
} DL_CreatedEvent;

typedef struct
{
    DL_Event baseEvent;
    uint8_t destroyedByte;
} DL_DestroyedEvent;

typedef enum
{
    ChangedEvent_StateA,
    ChangedEvent_StateB,
} ChangedState;

typedef struct
{
    DL_Event baseEvent;
    ChangedState changedState;
} DL_ChangedEvent;

// Provide events as a list of event pointers.
// The list of pointers and the values pointed to are read-only.
typedef DL_Event const * const * DL_Event_List;

int
DL_GetEvents(
    _Out_ uint32_t* count,
    _Out_ DL_Event_List* events
    );

int
DL_ReturnEvents(
    uint32_t count,
    DL_Event_List events
    );

int
DL_HandleCreatedEvent(
    const char* creationString
    );

int
DL_HandleDestroyedEvent(
    uint8_t destroyedByte
    );

int
DL_HandleChangedEvent(
    ChangedState changedState
    );

#endif // DEMO_LIB_H
