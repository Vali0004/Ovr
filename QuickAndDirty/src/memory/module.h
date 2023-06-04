#pragma once
#include "pch/pch.h"
#include "mem.h"

struct headers {
	HMODULE m_handle{};
	IMAGE_DOS_HEADER* m_dos{};
	IMAGE_NT_HEADERS64* m_nt{};
	IMAGE_OPTIONAL_HEADER64 m_optional{};
	IMAGE_EXPORT_DIRECTORY* m_exportDir{};
	void set(HMODULE handle) {
		m_handle = handle;
		m_dos = decltype(m_dos)(m_handle);
		m_nt = decltype(m_nt)(m_handle + m_dos->e_lfanew);
		m_optional = m_nt->OptionalHeader;
		m_exportDir = decltype(m_exportDir)(m_handle + m_optional.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	}
};
struct hmodule {
	hmodule(std::string_view name = {}) : m_name(name), m_handle(get()), m_begin(m_handle) {
		m_headers.set(m_handle);
		m_size = m_headers.m_nt->OptionalHeader.SizeOfImage;
		m_end = m_begin.add(m_size);
	}

	std::string_view m_name{};
	headers m_headers{};
	HMODULE m_handle{};
	mem m_begin{};
	mem m_end{};
	size_t m_size{};
	FARPROC getProcess(std::string search) {
		return GetProcAddress(m_handle, search.c_str());
	}
	HMODULE get() {
		if (m_name.empty())
			return GetModuleHandleA(NULL);
		return GetModuleHandleA(m_name.data());
	}
};