#pragma once
#include <cstdint>
#include <vcruntime_string.h>
#include <cstddef>

class alignas(16) scrVector {
public:
	scrVector() = default;
	scrVector(float x, float y, float z) : x(x), y(y), z(z) {}
public:
	alignas(8) float x{};
	alignas(8) float y{};
	alignas(8) float z{};
};
union scrValue {
	int32_t Int;
	uint32_t Uns;
	float Float;
	const char* String;
	scrValue* Reference;
	size_t Any;
	bool operator==(const scrValue& val) {
		return Int == val.Int;
	}
};
#pragma pack(push, 8)
enum class eThreadState : uint32_t {
	running,
	blocked,
	aborted,
	halted,
	reset_instruction_count
};
class scrThreadSerialised {
public:
	uint32_t m_thread_id; //0x0000
	uint32_t m_script_hash; //0x0004
	eThreadState m_state; //0x0008
	uint32_t m_pointer_count; //0x000C
	uint32_t m_frame_pointer; //0x0010
	uint32_t m_stack_pointer; //0x0014
	float m_timer_a; //0x0018
	float m_timer_b; //0x001C
	float m_wait; //0x0020
	int32_t m_min_pc; //0x0024
	int32_t m_max_pc; //0x0028
	char m_tls[36]; //0x002C
	uint32_t m_stack_size; //0x0050
	uint32_t m_catch_pointer_count; //0x0054
	uint32_t m_catch_frame_pointer; //0x0058
	uint32_t m_catch_stack_pointer; //0x005C
	uint32_t m_priority; //0x0060
	uint8_t m_call_depth; //0x0060
	uint8_t unk_0061; //0x0061
	uint16_t unk_0062; //0x0062
	char m_callstack[16]; //0x0068
}; //Size: 0x0078
static_assert(sizeof(scrThreadSerialised) == 0x78);
#pragma pack(pop)
class scrThreadInfo {
public:
	scrThreadInfo() = default;
	scrThreadInfo(scrValue* Ret, uint32_t Count, scrValue* Arugments) : Return(Ret), ArgCount(Count), Args(Arugments) {}
	void Reset() {
		ArgCount = 0;
		BufferCount = 0;
		memset(Orig, NULL, sizeof(Orig));
		memset(Buffer, NULL, sizeof(Buffer));
	}
public:
	scrValue* Return;
	uint32_t ArgCount;
	scrValue* Args;
	int32_t BufferCount;
	scrValue* Orig[4];
	scrVector Buffer[4];
	void CopyReferencedData() {
		int bc = BufferCount;
		while (bc--) {
			Orig[bc][0].Float = Buffer[bc].x;
			Orig[bc][1].Float = Buffer[bc].y;
			Orig[bc][2].Float = Buffer[bc].z;
		}
	}
};
using Cmd = void(*)(scrThreadInfo*);
class scrNativeRegistration {
public:
	scrNativeRegistration* m_next; //0x0000
	scrNativeRegistration* m_previous; //0x0008
	Cmd m_handlers[7]; //0x0010
	uint32_t m_num_entries; //0x0048
	uint32_t m_num_entries_2; //0x004C
	uint64_t m_hashes[7 * 2]; //0x0050

	scrNativeRegistration* get_next_registration() {
		uint64_t result{};
		uint64_t nextRegistration{ uint64_t(&m_next) };
		uint64_t previousRegistration{ nextRegistration ^ (uint64_t)m_previous };
		int8_t* registrations{ (int8_t*)&result - nextRegistration };
		for (uint8_t i{ 2 }; i; --i) {
			*(uint32_t*)&registrations[nextRegistration] = static_cast<uint32_t>(previousRegistration) ^ *(uint32_t*)nextRegistration;
			nextRegistration += 4;
		}
		return reinterpret_cast<scrNativeRegistration*>(result);
	}
	uint32_t get_num_entries() {
		return static_cast<uint32_t>(((uintptr_t)&m_num_entries) ^ m_num_entries ^ m_num_entries_2);
	}
	uint64_t get_hash(uint64_t index) {
		uint64_t result{};
		auto nativeAddress{ 16 * index + uintptr_t(&m_next) + 0x54 };
		auto registrations{ (char*)&result - nativeAddress };
		auto addressIndex{ nativeAddress ^ *(uint32_t*)(nativeAddress + 0x8) };
		for (uint8_t i{ 2 }; i; --i) {
			*(uint32_t*)&registrations[nativeAddress] = static_cast<uint32_t>(addressIndex ^ *(uint32_t*)(nativeAddress));
			nativeAddress += 4;
		}
		return result;
	}
}; //Size: 0x00C0
static_assert(sizeof(scrNativeRegistration) == 0xC0);
#pragma pack(push, 1)
class scrNativeRegistrationTable {
public:
	scrNativeRegistration* m_entries[255]; //0x0000
	uint32_t m_seed; //0x07F8
	bool m_initialized; //0x07FC

	Cmd get_handler(uint64_t hash) {
		for (auto entry{ m_entries[(uint8_t)(hash & 0xFF)] }; entry; entry = entry->get_next_registration()) {
			for (uint32_t i{}, end{ entry->get_num_entries() }; i < end; ++i) {
				if (auto entry_hash = entry->get_hash(i); entry_hash == hash) {
					return entry->m_handlers[i];
				}
			}
		}
		return nullptr;
	}
}; //Size: 0x07FD
static_assert(sizeof(scrNativeRegistrationTable) == 0x7FD);
#pragma pack(pop)
class scrProgram {
public:
	virtual ~scrProgram() = default;
	void* m_pgbase_unk{}; //0x0008
	uint8_t** m_code_blocks; //0x0010
	uint32_t m_hash; //0x0018
	uint32_t m_code_size; //0x001C
	uint32_t m_arg_count; //0x0020
	uint32_t m_local_count; //0x0024
	uint32_t m_global_count; //0x0028
	uint32_t m_native_count; //0x002C
	scrValue* m_local_data; //0x0030
	scrValue** m_global_data; //0x0038
	Cmd* m_natives; //0x0040
	uint32_t m_proc_count; //0x0048
	const char** m_proc_names; //0x0050
	uint32_t m_name_hash; //0x0058
	uint32_t m_ref_count; //0x005C
	const char* m_name; //0x0060
	const char** m_strings_data; //0x0068
	uint32_t m_strings_count; //0x0070
	char m_breakpoints[12]; //0x0074

	bool is_valid() const {
		return m_code_size != 0;
	}
	uint32_t get_num_code_pages() const {
		return (m_code_size + 0x3FFF) >> 14;
	}
	uint32_t get_code_page_size(uint32_t page) const {
		auto num = get_num_code_pages();
		if (page < num) {
			if (page == num - 1)
				return m_code_size & 0x3FFF;
			return 0x4000;
		}
		return 0;
	}
	uint32_t get_full_code_size() const {
		auto numPages = get_num_code_pages();
		if (!numPages)
			return 0;
		if (numPages == 1)
			--numPages;
		return (numPages * 0x4000) + (m_code_size & 0x3FFF);
	}
	uint8_t* get_code_page(std::uint32_t page) const {
		return m_code_blocks[page];
	}
	uint64_t get_string_page(uint64_t index) const {
		return *reinterpret_cast<uint64_t*>(uintptr_t(this) + offsetof(scrProgram, m_strings_data)) + index * 8;
	}
	uint8_t* get_code_address(uint32_t index) const {
		if (index < m_code_size)
			return &m_code_blocks[index >> 14][index & 0x3FFF];
		return nullptr;
	}
	const char* get_string(uint32_t index) const {
		if (index < m_strings_count)
			return &m_strings_data[index >> 14][index & 0x3FFF];
		return nullptr;
	}
	Cmd* get_native(Cmd entrypoint) {
		for (uint32_t i{}; i != m_native_count; ++i) {
			if (m_natives[i] == entrypoint) {
				return m_natives + i;
			}
		}
		return nullptr;
	}
	uint64_t* get_native_table() {
		return *reinterpret_cast<uint64_t**>(uintptr_t(this) + offsetof(scrProgram, m_natives));
	}
}; //Size: 0x0080
static_assert(sizeof(scrProgram) == 0x80);