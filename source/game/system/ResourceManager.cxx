#include "ResourceManager.hxx"

#include "../host_system/SystemManager.hxx"

namespace System {

void ResourceManager::my_process() {
    OSMessageQueue *messageQueue = SystemManager::getInstance()->getMessageQueue();
    OSReceiveMessage(messageQueue, nullptr, OS_MESSAGE_BLOCK);
}

} // namespace System

REPLACE(process__Q26System15ResourceManagerFv);
