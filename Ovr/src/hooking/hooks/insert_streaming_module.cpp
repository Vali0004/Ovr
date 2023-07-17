#include "hooking/hooking.h"
#include "memory/patch.h"

static thread_local std::string g_currentStreamingName;
static thread_local u32 g_currentStreamingIndex;
std::string getCurrentStreamingName() {
	return g_currentStreamingName;
}
u32 getCurrentStreamingIndex() {
	return g_currentStreamingIndex;
}
static thread_local void* g_currentStreamingModuleCallback;
static void setCurrentStreamingModuleCallback(void* func) {
	g_currentStreamingModuleCallback = func;
}
static void wrapStreamingLoad(rage::strStreamingModule* strModule, u32 index, void* data, void* a4) {
	u32 moduleBase = strModule->m_base_index;
	g_currentStreamingName = rage::strStreamingModuleMgr::Get().GetStreamingNameForIndex(moduleBase + index);
	g_currentStreamingIndex = moduleBase + index;
	((decltype(&wrapStreamingLoad))g_currentStreamingModuleCallback)(strModule, index, data, a4);
	g_currentStreamingName = "";
	g_currentStreamingIndex = 0;
}
i32 hooks::insertStreamingModule(rage::strStreamingModuleMgr* pMgr, rage::strStreamingModule* pModule) {
	void** vt = *(void***)pModule;
	class streamingOnLoad : public jitasm::Frontend {
	public:
		streamingOnLoad(void* origFunc) : m_origFunc(origFunc) {}
	private:
		virtual void InternalMain() override {
			push(rbx);
			push(rsi);
			push(rbp);
			push(rdi);

			sub(rsp, 0x28);

			mov(rbx, rcx); //Streaming module
			mov(rsi, rdx); //Index in module
			mov(rbp, r8);  //Data pointer
			mov(rdi, r9);  //Unknown

			//Save the current callback
			mov(rcx, (u64)m_origFunc);

			mov(rax, (u64)setCurrentStreamingModuleCallback);
			call(rax);

			//Return original arguments
			mov(rcx, rbx);
			mov(rdx, rsi);
			mov(r8, rbp);
			mov(r9, rdi);

			add(rsp, 0x28);

			pop(rdi);
			pop(rbp);
			pop(rsi);
			pop(rbx);

			mov(rax, (u64)wrapStreamingLoad);
			jmp(rax);
		}
	private:
		void* m_origFunc;
	};
	auto stub{ std::make_unique<streamingOnLoad>(vt[12]) };
	put((void*)&vt[12], stub->GetCode());
	RET_CALL(insertStreamingModule, pMgr, pModule);
}