#include "hooking/hooking.h"

bool isReliableMessage(rage::netQueuedMessage* msg) {
	rage::datBitBuffer buffer(msg->m_data_buffer, UINT_MAX);
	buffer.m_flagBits |= 1; //Read
	buffer.Seek(10); //Size
	return buffer.Read<bool>(1); //Flags
}
void freeMessage(rage::netQueuedMessage* msg, rage::sysMemAllocator* allocator) {
	if (msg->m_data_buffer) {
		allocator->TryFree(msg->m_data_buffer);
	}
	allocator->TryFree(msg);
}
void* hooks::allocateReliable(rage::netConnection* pCon, i32 RequiredMemory) {
	if (!pCon || !RequiredMemory) {
		return nullptr;
	}
	rage::sysMemAllocator* allocator{ pCon->m_allocator };
	void* mem{ allocator->Allocate(RequiredMemory, 0, 0) };
	if (mem) {
		return mem;
	}
	LOG(Warn, "Failed to allocate {}mb for reliable message ({}mb free)", RequiredMemory * 1024, allocator->GetMemoryAvailable());
	pointers::g_conMgrTryFree(pCon->m_net_connection_mgr);
	mem = allocator->Allocate(RequiredMemory, 0, 0);
	if (mem) {
		return mem;
	}
	LOG(Warn, "Failed to allocate {}mb for reliable message ({}mb free). Memory was attempted to be freeded but failed, cleaing all messages.", RequiredMemory * 1024, allocator->GetMemoryAvailable());
	while (pCon->m_normal_message_queue.m_count) {
		rage::netQueuedMessage* msg{ pCon->m_normal_message_queue.m_first };
		pointers::g_removeMessageFromQueue(&pCon->m_normal_message_queue, msg);
		if (isReliableMessage(msg)) {
			pointers::g_removeMessageFromUnacknowledgedReliables(&pCon->m_unacked_reliable_message_list, &msg->unk_004C);
		}
		freeMessage(msg, allocator);
	}
	while (pCon->m_reliables_resend_queue.m_count) {
		rage::netQueuedMessage* msg{ pCon->m_reliables_resend_queue.m_first };
		pointers::g_removeMessageFromQueue(&pCon->m_reliables_resend_queue, msg);
		pointers::g_removeMessageFromUnacknowledgedReliables(&pCon->m_unacked_reliable_message_list, &msg->unk_004C);
		freeMessage(msg, allocator);
	}
	mem = allocator->Allocate(RequiredMemory, 0, 0);
	if (mem) {
		return mem;
	}
	return nullptr;
}