#pragma once
#include "pch/pch.h"
#include "util/util.h"
#include "rage/classes.h"
#include "rage/enums.h"

namespace util {
	class packet : public rage::datBitBuffer {
	private:
		u8 m_bufferData[916]{};
	public:
		eNetMessage m_msg{};
	public:
		packet(eNetMessage msg) : rage::datBitBuffer(m_bufferData, sizeof(m_bufferData), true), m_msg(msg) {
			writeCursor();
		}
		packet() : m_msg(eNetMessage::MsgInvalid) {}
	public:
		void writeCursor() {
			//Write the cursor
			if (m_msg != eNetMessage::MsgInvalid) {
				Write<int>(0x3246, 14);
				if (int(m_msg) > 0xFF) {
					Write<bool>(true, 1);
					Write<eNetMessage>(m_msg, 16);
				}
				else {
					Write<bool>(false, 1);
					Write<eNetMessage>(m_msg, 8);
				}
			}
		}
	public:
		bool send(uint32_t msgId) {
			return pointers::g_queuePacketViaMsgId(util::network::session::get()->m_net_connection_mgr, msgId, m_bufferData, GetMaxPossibleBit(), 1, nullptr);
		}
	};
}