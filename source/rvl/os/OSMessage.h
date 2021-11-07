#pragma once

#include <Common.h>

typedef struct OSMessageQueue OSMessageQueue;
typedef void *OSMessage;

struct OSMessageQueue {
    u8 _00[0x20];
};

#define OS_MESSAGE_BLOCK 1

BOOL OSReceiveMessage(OSMessageQueue *mq, OSMessage *msg, s32 flags);
