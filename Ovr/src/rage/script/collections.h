#pragma once
#include "pch/pch.h"
#include "rage/classdefs.h"

namespace rage::ysc {
	class stringPageCollection { //This was fucking CANCER
	public:
		stringPageCollection() {
			createNewPage();
		}
		~stringPageCollection() {
			for (char* heap : stringHeaps) {
				delete[] heap;
			}
		}

		void createNewPage() {
			stringHeaps.push_back(new char[scrStringSize]);
		}
		u32 addString(ccp str) {
			if (auto it{ stringIndexLookup.find(str) }; it != stringIndexLookup.end()) {
				return it->second;
			}
			u32 stringLength{ static_cast<u32>(strlen(str)) };
			//Check if current offset is not at the beginning of a new page
			if (u32 remainingSpaceInPage{ scrStringSize - (stringHeapSize & scrStringMask) }; stringHeapSize & scrStringMask && stringLength + 1 > remainingSpaceInPage) {
				createNewPage();
			}
			//Check if the current page is full
			else if (static_cast<u64>(stringHeapSize) + stringLength + 1 > (stringHeaps.size() << scrStringShift)) {
				createNewPage();
			}
			strcpy(&stringHeaps[stringHeapSize >> scrStringShift][stringHeapSize & scrStringMask], str);
			stringHeapSize += stringLength + 1;
			stringIndexLookup[str] = stringHeapSize - stringLength - 1;
			return stringHeapSize - stringLength - 1;
		}
		ccp getString(u32 index) {
			if (index < stringHeapSize) {
				return &stringHeaps[index >> scrStringShift][index & scrStringMask];
			}

			return nullptr;
		}
		u32 getStringHeapSize() {
			return stringHeapSize;
		}
		u32 getStringHeapCount() {
			return (stringHeapSize + scrStringMask) >> scrStringShift;
		}
		u32 getStringChunkSize(u32 i) {
			return i == getStringHeapCount() - 1 ? stringHeapSize - (i << scrStringShift) : scrStringSize;
		}
	public:
		std::vector<char*> stringHeaps{};
	private:
		std::unordered_map<ccp, u32> stringIndexLookup{};
		u32 stringHeapSize{};
	};
	class codePageCollection { //This was fucking CANCER
	public:
		codePageCollection() {
			createNewPage();
		}
		~codePageCollection() {
			for (u8* block : codePages) {
				delete[] block;
			}
		}

		void createNewPage() {
			codePages.push_back(new u8[scrPageSize]);
		}
		u8* getPage(u32 number_of_opcodes) {
			//Check if current offset is not at the beginning of a new page
			if (u32 remainingSpaceInPage{ scrPageSize - (codePageSize & scrPageMask) }; codePageSize & scrPageMask && number_of_opcodes > remainingSpaceInPage) {
				createNewPage();
			}
			//Check if the current page is full
			else if (static_cast<u64>(codePageSize) + number_of_opcodes > (codePages.size() << scrPageShift)) {
				createNewPage();
			}
			return codePages.back();
		}
		u32& getCodePageSize() {
			return codePageSize;
		}
		u32 getCodePageCount() {
			return (codePageSize + scrPageMask) >> scrPageShift;
		}
		u32 getCodePageSize(u32 i) {
			return i == getCodePageSize() - 1 ? codePageSize - (i << scrPageShift) : scrPageSize;
		}
	public:
		std::vector<u8*> codePages{};
	private:
		u32 codePageSize{};
	};
}