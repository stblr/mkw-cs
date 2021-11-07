#include "ResourceManager.hxx"

#include "../host_system/SystemManager.hxx"

namespace System {

REPLACE void ResourceManager::process() {
    OSMessageQueue *messageQueue = SystemManager::getInstance()->getMessageQueue();
    OSReceiveMessage(messageQueue, nullptr, OS_MESSAGE_BLOCK);
}

} // namespace System
