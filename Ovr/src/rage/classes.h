#pragma once
#include "memory/pointers.h"
#include "classdefs.h"

class CMoveObjectPooledObject;
class Vector2;
class Vector3;
class Vector4;
namespace rage {
	template <typename T>
	class atRTTI {
		DEFINE_AT_RTTI(T)
	};
	struct vector2 {
		float x, y;
		Vector2 serialize();
	};
	struct vector3 {
		float x, y, z;
		Vector3 serialize();
	};
	struct vector4 {
		float x, y, z, w;
		Vector4 serialize();
	};
	union matrix34 {
		float data[3][4];
		struct { struct { float x, y, z, w; } rows[3]; };
	};
	union matrix44 {
		float data[4][4];
		struct { struct { float x, y, z, w; } rows[4]; };
	};
}
class CNavigation {
public:
	char pad_0000[32]; //0x0000
	float m_heading; //0x0020
	float m_heading2; //0x0024
	char pad_0028[8]; //0x0028
	rage::vector3 m_rotation; //0x0030
	char pad_003C[20]; //0x003C
	rage::vector3 m_position; //0x0054
}; //Size: 0x0060
static_assert(sizeof(CNavigation) == 0x5C);
namespace rage {
	#pragma pack(push, 8)
	class scrVector {
	public:
		scrVector() : x(0.f), y(0.f), z(0.f) {}
		scrVector(float x, float y, float z) : x(x), y(y), z(z) {}
	public:
		float x, y, z;
	};
	#pragma pack(pop)
	class netLoggingInterface {
	public:
	};
	class nonPhysicalPlayerDataBase {
	public:
		virtual ~nonPhysicalPlayerDataBase() = default;    // 0 (0x00)
		virtual void unk_0x08() = 0;                       // 1 (0x08)
		virtual void unk_0x10() = 0;                       // 2 (0x10)
		virtual void unk_0x18() = 0;                       // 3 (0x18)
		virtual void log(netLoggingInterface* logger) = 0; // 4 (0x20)
	};
	union netAddress {
		uint32_t m_packed; //0x0000
		struct {
			uint8_t m_field4; //0x0000
			uint8_t m_field3; //0x0001
			uint8_t m_field2; //0x0002
			uint8_t m_field1; //0x0003
		};
	};
	static_assert(sizeof(netAddress) == 0x04);
	class netSocketAddress {
	public:
		union netAddress m_ip; //0x0000
		uint16_t m_port; //0x0004
	};
	static_assert(sizeof(netSocketAddress) == 0x08);
	class rlGamerHandle {
	public:
		rlGamerHandle() : m_rockstar_id(NULL), m_platform(3) {}
		rlGamerHandle(uint64_t rockstar_id) : m_rockstar_id(rockstar_id), m_platform(3) {}
	public:
		uint64_t m_rockstar_id; //0x0000
		uint64_t m_platform; //0x0008
	}; //Size: 0x0010
	static_assert(sizeof(rlGamerHandle) == 0x10);
#pragma pack(push, 8)
	class rlPeerInfo {
	public:
		char m_certificate[96]; //0x0000
		uint64_t m_peer_id; //0x0060
		class rage::rlGamerHandle m_gamer_handle; //0x0068
		char m_aes_key[32]; //0x0078
		uint8_t unk_0098; //0x0098
		class rage::netSocketAddress m_unk_address; //0x0099
		class rage::netSocketAddress m_public_address; //0x00A9
		class rage::netSocketAddress m_relay_address; //0x00A1
		class rage::netSocketAddress m_local_address;  //0x00B1
		uint32_t unk_00B9; //0x00B9
	}; //Size: 0x00C0
	static_assert(sizeof(rage::rlPeerInfo) == 0xC0);
#pragma pack(pop)
	class rlGamerInfo : public rlPeerInfo {
	public:
		uint64_t m_peer_address; //0x00C0
		uint64_t m_platform_data; //0x00C8
		rlGamerHandle m_unk_gamer_handle; //0x00D0
		uint32_t m_ros_privilege; //0x00E0
		char m_name[20]; //0x00E4
	}; //Size: 0x00F8
	static_assert(sizeof(rlGamerInfo) == 0xF8);
	#pragma pack(push, 8)
	class netPlayer : public atRTTI<netPlayer> {
	public:
		virtual ~netPlayer();
		virtual void Reset();
		virtual bool IsConnected();
		virtual const char* GetName();
		virtual void ActiveUpdate();
		virtual bool IsNetworkHost();
		virtual rlGamerInfo* GetGamerInfo();
		virtual void DestroyGamerDataSyncNode();

		char pad_0008[8]; //0x0008
		class CNonPhysicalPlayerData* m_non_physical_player; //0x0010
		uint32_t m_msg_id; //0x0018
		char pad_001C[4]; //0x001C
		uint8_t m_active_id; //0x0020
		uint8_t m_player_id; //0x0021
		char pad_0022[3]; //0x0022
		uint16_t m_complaints; //0x0026
		char pad_0027[17]; //0x0028
		class CNetGamePlayer* m_unk_net_player_list[10]; //0x0040
		uint32_t unk_0090; //0x0090
		uint64_t unk_0098; //0x0098
	}; //Size: 0x00A0
	static_assert(sizeof(netPlayer) == 0xA0);
	#pragma pack(pop)
#pragma pack(push, 8)
	class netPlayerMgrBase {
	public:
		virtual ~netPlayerMgrBase();
		virtual void Initialize();
		virtual void Shutdown();
		virtual void unk_0x18();
		virtual class CNetGamePlayer* RawAddPlayer(void* a1, void* a2, void* a3, class rlGamerInfo* gamer_info, class CNonPhysicalPlayerData* non_physical_player_data);
		virtual void RemovePlayer(class CNetGamePlayer* net_game_player);
		virtual void UpdatePlayerListsForPlayer(class CNetGamePlayer* net_game_player);
		virtual class CNetGamePlayer* AddPlayer(void* a1, void* a2, void* a3, class rlGamerInfo* gamer_info, class CNonPhysicalPlayerData* non_physical_player_data);

		char pad_0008[8]; //0x0008
		uint64_t* m_network_bandwidth_manager; //0x0010
		char pad_0018[208]; //0x0018
		class CNetGamePlayer* m_local_net_player; //0x00E8
		char pad_00F0[144]; //0x00F0
		class CNetGamePlayer* m_player_list[32]; //0x0180
		uint16_t m_player_limit; //0x0280
		char pad_0282[10]; //0x0282
		uint16_t m_player_count; //0x028C
		char pad_0290[1618]; //0x0290
	}; //Size: 0x08E0
	static_assert(sizeof(netPlayerMgrBase) == 0x8E0);
#pragma pack(pop)
#pragma pack(push, 4)
	class fwDrawData {
	public:
		uint64_t unk_000; //0x0000
		uint64_t unk_008; //0x0008
		char pad_0010[8]; //0x0010
		uint32_t dword18; //0x0018
		uint32_t unk_001C; //0x0028
		uint64_t unk_0020; //0x0028
		uint32_t unk_0028; //0x0028
	}; //Size: 0x002C
	static_assert(sizeof(fwDrawData) == 0x2C);
#pragma pack(pop)
	class pgBase {
	public:
		virtual ~pgBase() = default;
	private:
		void* m_pgunk;
	};
	class datBase {
	public:
		virtual ~datBase() = default;
	};
	class pgBase;
	template <typename T, typename Base>
	class atDNode : public Base {
	public:
		T m_data;
		void* m_unk;
		atDNode<T, Base>* m_next;
	};
	template <typename Node>
	class atDList {
	public:
		Node* m_head;
		Node* m_tail;
	};
	template <typename T>
	class fwRefAwareBaseImpl : public T {
	private:
		void* m_ref; //0x0008
	};
	class fwRefAwareBase : public fwRefAwareBaseImpl<datBase> {
	public:
	};
	static_assert(sizeof(fwRefAwareBase) == 0x10);
	class fwExtension {
	public:
		virtual ~fwExtension() = default;
		virtual void unk_0x08() = 0;
		virtual void unk_0x10() = 0;
		virtual uint32_t get_id() = 0;
	}; //Size: 0x0008
	static_assert(sizeof(fwExtension) == 0x8);
	class fwExtensionContainer {
	public:
		fwExtension* m_entry; //0x0000
		fwExtensionContainer* m_next; //0x0008
	}; //Size: 0x0010
	static_assert(sizeof(fwExtensionContainer) == 0x10);
	class fwExtensibleBase : public fwRefAwareBase {
	public:
		virtual bool is_of_type(uint32_t hash) = 0;
		virtual uint32_t const& get_type() = 0;
		fwExtensionContainer* m_extension_container; //0x0010
		void* m_extensible_unk; //0x0018
		template <typename T>
		bool is_of_type() {
			static auto name = (typeid(T).name()) + 6; // Skip "class "
			static auto name_hash = joaat(name);
			return is_of_type(name_hash);
		}
	}; //Size: 0x0020
	static_assert(sizeof(fwExtensibleBase) == 0x20);
#pragma pack(push, 1)
	class CBaseModelInfo {
	public:
		char pad_0000[24]; //0x0000
		uint32_t m_model; //0x0018
		char pad_001C[20]; //0x001C
		struct vector3 m_min_dimensions; //0x0030
		char pad_003C[4]; //0x003C
		struct vector3 m_max_dimensions; //0x0040
		char pad_004C[81]; //0x004C
		uint8_t m_model_type; //0x009D
		char pad_009E[6]; //0x009E
		uint8_t get_model_type() {
			return m_model_type & 0xFF;
		}
	}; //Size: 0x00A4
	static_assert(sizeof(CBaseModelInfo) == 0xA4);
#pragma pack(pop)
#pragma pack(push, 1)
	class fwEntity : public fwExtensibleBase {
	public:
		DEFINE_AT_RTTI(fwEntity)
		virtual void* _0x38(void*, void*) = 0;
		virtual void AddExtension(void* extension) = 0; // 0x40
		virtual void _0x48() = 0; // not implemented
		virtual void _0x50() = 0; // only implemented by CEntityBatch
		virtual void _0x58() = 0;
		virtual void SetModelInfo(std::uint16_t* model_index) = 0; // 0x60
		virtual void _0x68(int, vector4*) = 0;
		virtual void* _0x70(int) = 0;
		virtual CNavigation* GetNavigation() = 0; // 0x78
		virtual CMoveObjectPooledObject* CreateMoveObject() = 0; // 0x80
		virtual std::uint32_t* GetType() = 0; // 0x88
		virtual void _0x90() = 0;
		virtual float _0x98() = 0;
		virtual bool TryRequestInverseKinematics(rage::crmtRequestPose* pose, rage::crmtRequestIk* ik) = 0; // 0xA0 implemented only by CPed
		virtual bool TryRequestFacialAnims(void*) = 0; // 0xA8 implemented only by CPed
		virtual void* _0xB0() = 0;
		virtual std::uint8_t _0xB8() = 0; // implemented only by CPed
		virtual rage::crFrameFilter* GetFrameFilter() = 0; // 0xC0
		virtual rage::fwAudEntity* GetEntityAudio() = 0; // 0xC8
		virtual void _0xD0() = 0;
		virtual void SetTransform(matrix44* matrix, bool update_pos) = 0; // 0xD8
		virtual void SetTransform2(matrix44* matrix, bool update_pos) = 0; // 0xE0
		virtual void SetPosition(vector4* pos, bool update_pos) = 0; // 0xE8
		virtual void SetHeading(float heading, bool update_pos) = 0; // 0xF0
		virtual void SetEntityTypeFlags() = 0; // 0xF8
		virtual void _0x100() = 0; // not implemented
		virtual void UpdatePhysics(CNavigation* navigation) = 0; // 0x108
		virtual void UpdatePhysics2(CNavigation* navigation) = 0; // 0x110
		virtual void UpdatePosition() = 0; // 0x118

		class CBaseModelInfo* m_model_info; //0x0020
		uint8_t m_entity_type; //0x0028
		char gap29; //0x0029
		uint16_t gap2A; //0x002A
		uint32_t m_entity_flags; //0x002D
		class CNavigation* m_navigation; //0x0030
		uint16_t gap38; //0x0038
		uint16_t gap3A; //0x003A
		uint32_t gap3C; //0x003C
		class rage::fwDynamicEntityComponent* m_dynamic_entity_component; //0x0040 (stores attachments and stuff)
		class rage::fwDrawData* m_draw_data; //0x0048
		class rage::fwDynamicEntityComponent* gap50; //0x0050
		uint64_t gap58; //0x0058
		matrix44 m_transformation_matrix; //0x0060
		rage::fwEntity* m_render_focus_entity; //0x00A0
		uint32_t m_render_focus_distance; //0x00A8
		uint32_t m_flags_2; //0x00AC
		uint32_t m_shadow_flags; //0x00B0
		char gapB4[4]; //0x00B4
		uint8_t byteB8; //0x00B8

		void set_entity_flag(eEntityFlags flag, bool value) {
			value ? m_entity_flags |= (uint32_t)flag : m_entity_flags &= ~(uint32_t)flag;
		}
		float& get_heading() {
			return m_navigation->m_heading;
		}
		void set_heading(float v) {
			m_navigation->m_heading = v;
		}
		rage::vector3& get_position() {
			return reinterpret_cast<rage::vector3&>(m_transformation_matrix.rows[3]);
		}
		rage::vector4& get_position_full() {
			return reinterpret_cast<rage::vector4&>(m_transformation_matrix.rows[3]);
		}
		void model_to_world(const vector3& model_coords, vector3& world_coords) {
			world_coords.x = model_coords.x * m_transformation_matrix.data[0][0] + model_coords.y * m_transformation_matrix.data[1][0] + model_coords.z * m_transformation_matrix.data[2][0] + m_transformation_matrix.data[3][0];
			world_coords.y = model_coords.x * m_transformation_matrix.data[0][1] + model_coords.y * m_transformation_matrix.data[1][1] + model_coords.z * m_transformation_matrix.data[2][1] + m_transformation_matrix.data[3][1];
			world_coords.z = model_coords.x * m_transformation_matrix.data[0][2] + model_coords.y * m_transformation_matrix.data[1][2] + model_coords.z * m_transformation_matrix.data[2][2] + m_transformation_matrix.data[3][2];
		}
	}; //Size: 0x00B9
	static_assert(sizeof(fwEntity) == 0xB9);
#pragma pack(pop)
	class datBitBuffer {
	public:
		datBitBuffer(uint8_t* data, uint32_t size, bool flagBitsToWrite = false) {
			m_data = data;
			m_bitOffset = 0;
			m_maxBit = size * 8;
			m_bitsRead = 0;
			m_curBit = 0;
			m_highestBitsRead = 0;
			m_flagBits = 0;
			if (flagBitsToWrite) {
				m_flagBits &= 0xFCu;
				m_flagBits &= 0xFDu;
				m_flagBits |= 0xFCu;
			}
			else {
				m_flagBits &= ~2u;
				m_flagBits |= 1u;
			}
		}
		datBitBuffer(void* data, uint32_t size, bool flagBitsToWrite = false) : datBitBuffer((uint8_t*)data, size, flagBitsToWrite) {}
		datBitBuffer(bool flagBitsToWrite = false) : datBitBuffer((uint8_t*)nullptr, NULL, flagBitsToWrite) {}

		bool IsFlagSet(uint8_t flag) {
			return (m_flagBits & flag) != NULL;
		}
		uint32_t GetPosition() {
			return m_bitsRead;
		}
		uint32_t GetMaxPossibleBit() {
			return (m_highestBitsRead + 7) >> 3;
		}
		bool Seek(uint32_t bits) {
			if (bits >= 0) {
				uint32_t length = IsFlagSet(1) ? m_maxBit : m_curBit;
				if (bits <= length) {
					m_bitsRead = bits;
					return true;
				}
			}
			return false;
		}
		int64_t SeekCur(uint32_t bits) {
			m_bitsRead += static_cast<uint32_t>(bits);
			if (m_bitsRead > m_curBit)
				m_curBit = m_bitsRead;
			return m_bitsRead;
		}
		int64_t AddNumberOfBits(uint32_t bits) {
			m_bitsRead += static_cast<uint32_t>(bits);
			if (m_bitsRead > m_highestBitsRead)
				m_highestBitsRead = m_bitsRead;
			return m_bitsRead;
		}
		uint32_t GetDataLength() {
			uint32_t leftoverBit{ (m_curBit % 8u) ? 1u : 0u };
			return (m_curBit / 8u) + leftoverBit;
		}
		uint32_t GetMaxDataLength() {
			return (m_flagBits & 1) != NULL ? m_maxBit : m_curBit;
		}
		bool EnsureBitData(uint32_t bits) {
			if (IsFlagSet(2) || m_bitsRead + bits > GetMaxDataLength())
				return false;
			return true;
		}
		uint64_t WriteBitsSingle(uint32_t value, int32_t bits) {
			return pointers::g_writeBitsSingle(m_data, value, bits, m_bitsRead + m_bitOffset);
		}
		uint32_t ReadBitsSingle(uint32_t numBits) {
			auto const totalBits = (m_flagBits & 1) ? m_maxBit : m_curBit;
			if ((m_flagBits & 2) || m_bitsRead + numBits > totalBits)
				return 0;
			auto const bufPos = m_bitsRead + m_bitOffset;
			auto const initialBitOffset = bufPos & 0b111;
			auto const start = &((uint8_t*)m_data)[bufPos / 8];
			auto const next = &start[1];
			auto result = (start[0] << initialBitOffset) & 0xff;
			for (auto i = 0; i < ((numBits - 1) / 8); i++) {
				result <<= 8;
				result |= next[i] << initialBitOffset;
			}
			if (initialBitOffset)
				result |= next[0] >> (8 - initialBitOffset);
			m_bitsRead += static_cast<uint32_t>(numBits);
			if (m_bitsRead > m_highestBitsRead)
				m_highestBitsRead = m_bitsRead;
			return result >> ((8 - numBits) % 8);
		}
		template <typename t>
		bool Write(t value, uint32_t bits = 0) {
			uint32_t bitsToWrite{ bits ? bits : sizeof(t) };
			bool res{ WriteBitsSingle((uint32_t)value, bitsToWrite) ? true : false };
			SeekCur(bitsToWrite);
			return res;
		}
		template <typename t>
		t Read(uint32_t bits) {
			if (bits <= 32) {
				return t(ReadBitsSingle(bits));
			}
			else {
				uint32_t Low{ ReadBitsSingle(bits - 32u) };
				uint32_t High{ ReadBitsSingle(32u) };
				return t(High | (Low << 32));
			}
		}
		template <typename t>
		t ReadSigned(uint32_t bits) {
			int32_t sign{ Read<int>(1) };
			int32_t data{ Read<int>(bits - 1) };
			return t(sign + (data ^ -sign));
		}
		bool ReadPeerId(uint64_t* value) {
			if (!EnsureBitData(0x20))
				return false;
			*value = Read<uint64_t>(0x20);
			return true;
		}
		bool WriteString(char* string, int length) {
			auto extended = Write<bool>(length > m_maxBit ? true : false, 1);
			auto len = Write<int32_t>(extended ? 15 : 7);
			if (len > length)
				return false;
			WriteArray(string, len * 8);
			return true;
		}
		bool ReadString(char* string, int length) {
			auto extended = Read<bool>(1);
			auto len = Read<int32_t>(extended ? 15 : 7);
			if (len > length)
				return false;
			ReadArray(string, len * 8);
			if (string[len - 1] != '\0')
				return false;
			return true;
		}
		bool ReadBool(bool* boolean) {
			if (!EnsureBitData(1))
				return false;
			*boolean = Read<uint8_t>(1);
			return true;
		}
		bool WriteBool(bool value) {
			return Write<bool>(value);
		}
		bool ReadByte(uint8_t* integer, int bits) {
			if (!EnsureBitData(bits))
				return false;
			*integer = Read<uint8_t>(bits);
			return true;
		}
		bool WriteByte(uint8_t value, int bits) {
			return Write<uint8_t>(value, bits);
		}
		bool ReadWord(uint16_t* integer, int bits) {
			if (!EnsureBitData(bits))
				return false;
			*integer = Read<uint16_t>(bits);
			return true;
		}
		bool WriteWord(uint16_t value, int bits) {
			return Write<uint16_t>(value, bits);
		}
		bool ReadDword(uint32_t* integer, int bits) {
			if (!EnsureBitData(bits))
				return false;
			*integer = Read<uint32_t>(bits);
			return true;
		}
		bool WriteDword(uint32_t value, int bits) {
			return Write<uint32_t>(value, bits);
		}
		bool ReadInt32(int32_t* integer, int bits) {
			if (!EnsureBitData(bits))
				return false;
			*integer = ReadSigned<int32_t>(bits);
			return true;
		}
		bool ReadQword(uint64_t* integer, int bits) {
			if (!EnsureBitData(bits))
				return false;
			*integer = Read<uint64_t>(bits);
			return true;
		}
		bool WriteQword(uint64_t value, int bits) {
			if (bits <= 32) {
				if (IsFlagSet(1))
					return false;
				if ((m_bitsRead + bits) <= m_maxBit) {
					if (!IsFlagSet(2)) {
						Write<uint32_t>(value, bits);
					}
					return true;
				}
				return false;
			}
			else {
				if (IsFlagSet(1))
					return false;
				if ((m_bitsRead + 32) > m_maxBit)
					return false;
				if (!IsFlagSet(2)) {
					Write<uint32_t>(value, 32);
				}
				if (IsFlagSet(1) || (bits - 32) + m_bitsRead > m_maxBit) {
					return false;
				}
				else {
					if (!IsFlagSet(2)) {
						Write<uint32_t>(SHIDWORD(value), bits - 32);
					}
				}
			}
			return true;
		}
		bool ReadInt64(int64_t* integer, int bits) {
			if (!EnsureBitData(bits))
				return false;
			*integer = ReadSigned<int64_t>(bits);
			return true;
		}
		bool WriteArray(void* array, int size) {
			return false;
		}
		bool ReadArray(void* array, int size) {
			return pointers::g_readBitbufArray(this, array, size, 0);
		}
	public:
		uint8_t* m_data; //0x0000
		uint32_t m_bitOffset; //0x0008
		uint32_t m_maxBit; //0x000C
		uint32_t m_bitsRead; //0x0010
		uint32_t m_curBit; //0x0014
		uint32_t m_highestBitsRead; //0x0018
		uint8_t m_flagBits; //0x001C
	}; //Size: 0x0020
	static_assert(sizeof(datBitBuffer) == 0x20);
	class netGameEvent {
	public:
		virtual ~netGameEvent() = default;
		virtual const char* get_name() { return 0; };
		virtual bool is_in_scope(netPlayer* player) { return 0; };
		virtual bool time_to_resend(std::uint32_t time) { return 0; };
		virtual bool can_change_scope() { return 0; };
		virtual void prepare_data(datBitBuffer* buffer, netPlayer* source_player, netPlayer* target_player) {};
		virtual void handle_data(datBitBuffer* buffer, netPlayer* source_player, netPlayer* target_player) {};
		virtual bool decide(netPlayer* source_player, netPlayer* target_player) { return 0; };
		virtual void prepare_reply(datBitBuffer* buffer, netPlayer* reply_player) {};
		virtual void handle_reply(datBitBuffer* buffer, netPlayer* souce_player) {};
		virtual void prepare_extra_data(datBitBuffer* buffer, bool is_reply, netPlayer* player, netPlayer* player2) {};
		virtual void handle_extra_data(datBitBuffer* buffer, bool is_reply, netPlayer* player, netPlayer* player2) {};
	private:
		virtual void unk_0x60() {};
		virtual void unk_0x68() {};
		virtual void unk_0x70() {};
		virtual void unk_0x78() {};
	public:
		virtual bool operator==(netGameEvent const& other) { return 0; };
		virtual bool operator!=(netGameEvent const& other) { return 0; };
		virtual bool must_persist() { return 0; };
		virtual bool must_persist_when_out_of_scope() { return 0; };
		virtual bool has_timed_out() { return 0; };
		std::uint16_t m_id;          // 0x08
		bool m_requires_reply;       // 0x0A
	private:
		char m_padding1[0x05];       // 0x0B
	public:
		netPlayer* m_source_player;  // 0x10
		netPlayer* m_target_player;  // 0x18
		std::uint32_t m_resend_time; // 0x20
	private:
		std::uint16_t m_0x24;        // 0x24
		std::uint8_t m_0x26;         // 0x26
		std::uint8_t m_0x27;         // 0x27
		std::uint32_t m_0x28;        // 0x28
		char m_padding2[0x04];
	};
	template <typename T>
	class atArray {
	public:
		T* begin() { return m_data; }
		T* end() { return m_data + m_size; }
		const T* begin() const { return m_data; }
		const T* end() const { return m_data + m_size; }
		T* data() { return m_data; }
		const T* data() const { return m_data; }
		std::uint16_t size() const { return m_size; }
		std::uint16_t capacity() const { return m_capacity; }
		T& operator[](std::uint16_t index) { return m_data[index]; }
		const T& operator[](std::uint16_t index) const { return m_data[index]; }
	private:
		T* m_data;
		std::uint16_t m_size;
		std::uint16_t m_capacity;
	};
	class scrProgram : public pgBase {
	public:
		std::uint8_t** m_code_blocks;  // 0x10
		std::uint32_t m_hash;          // 0x18
		std::uint32_t m_code_size;     // 0x1C
		std::uint32_t m_arg_count;     // 0x20
		std::uint32_t m_local_count;   // 0x24
		std::uint32_t m_global_count;  // 0x28
		std::uint32_t m_native_count;  // 0x2C
		void* m_local_data;            // 0x30
		std::int64_t** m_global_data;  // 0x38
		void** m_native_entrypoints;   // 0x40
		char m_padding6[0x10];         // 0x48
		std::uint32_t m_name_hash;     // 0x58
		char m_padding7[0x04];         // 0x5C
		const char* m_name;            // 0x60
		const char** m_strings_data;   // 0x68
		std::uint32_t m_strings_count; // 0x70
		char m_padding8[0x0C];         // 0x74
		bool is_valid() const {
			return m_code_size != 0;
		}
		std::uint32_t get_num_code_pages() const {
			return (m_code_size + 0x3FFF) >> 14;
		}
		std::uint32_t get_code_page_size(std::uint32_t page) const {
			auto num = get_num_code_pages();
			if (page < num) {
				if (page == num - 1)
					return m_code_size & 0x3FFF;
				return 0x4000;
			}
			return 0;
		}
		std::uint32_t get_full_code_size() const {
			auto numPages = get_num_code_pages();
			if (!numPages)
				return 0;
			if (numPages == 1)
				--numPages;
			return (numPages * 0x4000) + (m_code_size & 0x3FFF);
		}
		std::uint8_t* get_code_page(std::uint32_t page) const {
			return m_code_blocks[page];
		}
		std::uint8_t* get_code_address(std::uint32_t index) const {
			if (index < m_code_size)
				return &m_code_blocks[index >> 14][index & 0x3FFF];
			return nullptr;
		}
		const char* get_string(std::uint32_t index) const {
			if (index < m_strings_count)
				return &m_strings_data[index >> 14][index & 0x3FFF];
			return nullptr;
		}
		void** get_address_of_native_entrypoint(void* entrypoint) {
			for (std::uint32_t i = 0; i < m_native_count; ++i)
				if (m_native_entrypoints[i] == entrypoint)
					return m_native_entrypoints + i;
			return nullptr;
		}
	};
	class scrProgramTableEntry {
	public:
		scrProgram* m_program;     // 0x00
		char m_Pad1[0x04];         // 0x08
		uint32_t m_hash;           // 0x0C
	};
	class scrProgramTable {
	public:
		scrProgramTableEntry* m_data;    // 0x00
		char m_padding[0x10];            // 0x08
		std::uint32_t m_size;            // 0x18
		std::list<uint32_t> all_script_hashes() {
			std::list<uint32_t> hash;
			for (uint32_t i = 0; i < m_size; ++i)
				if (m_data[i].m_program != nullptr && m_data[i].m_program->m_name != nullptr)
					hash.push_back(m_data[i].m_hash);
			return hash;
		}
		std::list<std::string> all_script_names() {
			std::list<std::string> hash;
			for (uint32_t i = 0; i < m_size; ++i)
				if (m_data[i].m_program != nullptr && m_data[i].m_program->m_name != nullptr)
					hash.push_back(m_data[i].m_program->m_name);
			return hash;
		}
		scrProgram* find_script(uint32_t hash) {
			for (std::uint32_t i = 0; i < m_size; ++i)
				if (m_data[i].m_hash == hash)
					return m_data[i].m_program;
			return nullptr;
		}
		scrProgramTableEntry* begin() {
			return m_data;
		}
		scrProgramTableEntry* end() {
			return m_data + m_size;
		}
	};
	//Thread
	//Thread State
	enum class eThreadState : uint32_t {
		running,
		sleeping,
		killed,
		paused,
		breakpoint
	};
	//Thread
#pragma pack(push, 8)
	class scrThreadContext {
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
	static_assert(sizeof(scrThreadContext) == 0x78);
#pragma pack(pop)
	class tlsContext {
	public:
		char pad_0000[180]; //0x0000
		uint32_t m_unknown_byte; //0x00B4
		class sysMemAllocator* m_allocator; //0x00B8
		class sysMemAllocator* m_tls_entry; //0x00C0
		class sysMemAllocator* m_unk_allocator; //0x00C8
		uint32_t m_console_handle; //0x00D0
		char pad_00D4[188]; //0x00D4
		uint64_t unk_0190; //0x0190
		char pad_0194[1712]; //0x0194
		class scrThread* m_script_thread; //0x0848
		bool m_is_script_thread_active; //0x00850

		static tlsContext* get() { return *(tlsContext**)(__readgsqword(0x58)); }
		static tlsContext** getPointer() { return (tlsContext**)(__readgsqword(0x58)); }
	}; //Size: 0x0850
	static_assert(sizeof(tlsContext) == 0x858);
	union scrValue {
		int32_t Int;
		uint32_t Uns;
		float Float;
		LPCSTR String;
		scrValue* Reference;
		size_t Any;
		bool operator==(const scrValue& val) {
			return Int == val.Int;
		}
	};
	class scrThread {
	public:
		virtual ~scrThread() = default;                   //0 (0x00)
		virtual eThreadState reset(uint32_t script_hash, void* args, uint32_t arg_count) { return m_context.m_state; }     //1 (0x08)
		virtual eThreadState run() { return m_context.m_state; }                //2 (0x10)
		virtual eThreadState tick(uint32_t ops_to_execute) { return m_context.m_state; }          //3 (0x18)
		virtual void kill() {}                //4 (0x20)
		static scrThread** getPointer() {
			auto tls = uint64_t(rage::tlsContext::get());
			return reinterpret_cast<scrThread**>(tls + offsetof(rage::tlsContext, m_script_thread));
		}
		static scrThread* get() {
			return rage::tlsContext::get()->m_script_thread;
		}
	public:
		scrThreadContext* getContext() {
			return &m_context;
		}
		void setScriptName(const char* name) {
			strncpy_s(m_name, name, sizeof(m_name) - 1);
			m_name[sizeof(m_name) - 1] = '\0';
		}
	public:
		class scrThreadContext m_context; //0x0000
		char unk_0078[48]; //0x0078
		scrValue* m_stack; //0x00B0
		uint32_t unk_00B8; //0x00B8
		uint32_t m_arg_size; //0x00BC
		uint32_t m_arg_loc; //0x00C0
		uint32_t unk_00C4; //0x00C4 - Some sort of arg
		const char* m_exit_message; //0x00C8
		uint32_t unk_00D0; //0x00D0
		char m_name[64]; //0x00D4
		class scriptHandler* m_handler; //0x0110
		class scriptHandlerNetComponent* m_net_component; //0x0118
	}; //Size: 0x0128
	static_assert(sizeof(scrThread) == 0x128);
	class sysMemAllocator : public atRTTI<sysMemAllocator> {
	public:
		virtual ~sysMemAllocator() = 0;
		virtual void SetQuitOnFail(bool) = 0;
		virtual void* Allocate(size_t size, size_t align, int subAllocator) = 0;
		virtual void* TryAllocate(size_t size, size_t align, int subAllocator) = 0;
		virtual void Free(void* pointer) = 0;
		virtual void TryFree(void* pointer) = 0;
		virtual void Resize(void* pointer, size_t size) = 0;
		virtual sysMemAllocator* GetAllocator(int allocator) const = 0;
		virtual sysMemAllocator* GetAllocator(int allocator) = 0;
		virtual sysMemAllocator* GetPointerOwner(void* pointer) = 0;
		virtual size_t GetSize(void* pointer) const = 0;
		virtual size_t GetMemoryUsed(int memoryBucket) = 0;
		virtual size_t GetMemoryAvailable() = 0;
	public:
		static sysMemAllocator* UpdateAllocatorValue() {
			auto tls = rage::tlsContext::get();
			auto gtaTlsEntry = *reinterpret_cast<sysMemAllocator**>(uintptr_t(tls) + offsetof(rage::tlsContext, m_allocator));
			if (!gtaTlsEntry)
				tls->m_allocator = gtaTlsEntry;
			tls->m_tls_entry = gtaTlsEntry;
			return gtaTlsEntry;
		}
		static sysMemAllocator* get() {
			auto allocValue = rage::tlsContext::get()->m_allocator;
			if (!allocValue)
				allocValue = UpdateAllocatorValue();
			return allocValue;
		}
		static sysMemAllocator** getPointer() {
			auto tls = uintptr_t(*(uintptr_t*)__readgsqword(0x58));
			return reinterpret_cast<sysMemAllocator**>(tls + offsetof(rage::tlsContext, m_allocator));
		}
		static sysMemAllocator* getEntry() {
			auto tls = uintptr_t(*(uintptr_t*)__readgsqword(0x58));
			return *reinterpret_cast<sysMemAllocator**>(tls + offsetof(rage::tlsContext, m_tls_entry));
		}
		static sysMemAllocator** getEntryPointer() {
			auto tls = uintptr_t(*(uintptr_t*)__readgsqword(0x58));
			return reinterpret_cast<sysMemAllocator**>(tls + offsetof(rage::tlsContext, m_tls_entry));
		}
	};
	class scriptIdBase {
	public:
		virtual ~scriptIdBase() = default;                            // 0 (0x00)
		//Assumes the script thread's identity.
		virtual void assume_thread_identity(scrThread*) {};           // 1 (0x08)
		//Returns whether the hash of the script id is valid.
		virtual bool is_valid() {};                                   // 2 (0x10)
		//Gets the hash of the script id.
		virtual uint32_t* get_hash(uint32_t* out) {};                 // 3 (0x18)
		//Gets an unknown value from the script id.
		virtual std::uint32_t* get_hash2(std::uint32_t* out) {};      // 4 (0x20)
		//Gets the name of the script id.
		virtual const char* get_name() {};                            // 5 (0x28)
		//Serializes the script id from the buffer.
		virtual void deserialize(datBitBuffer* buffer) {};            // 6 (0x30)
		//Serializes the script id to the buffer.
		virtual void serialize(datBitBuffer* buffer) {};              // 7 (0x38)
		//Calculates some information with the position hash & instance id.
		virtual std::uint32_t _0x40() {};                             // 8 (0x40)
		//Calls _0x40 and returns it's value added to another value.
		virtual std::uint32_t _0x48() {};                             // 9 (0x48)
		//Logs some information about the script id.
		virtual void log_information(void* logger) {}; // 10 (0x50)
		//Copies the information of other to this object.
		virtual void copy_data(scriptIdBase* other) {}                // 11 (0x58)
		//Returns whether the other script id is equal.
		virtual bool operator==(scriptIdBase*) {};                    // 12 (0x60)
		virtual bool _0x68(void*) {};                                 // 13 (0x68)
	};
	class scriptId : public scriptIdBase {
	public:
		uint32_t m_hash;          // 0x08
		char m_name[0x20];        // 0x0C
	};
	class scriptResource {
	public:
		virtual ~scriptResource() = default;
	};
	class scriptHandler {
	public:
		class atDScriptObjectNode : public atDNode<scriptHandlerObject*> {};
	public:
		virtual ~scriptHandler() = default;                                                                   //  0 (0x00)
		virtual bool _0x08() = 0;                                                                             //  1 (0x08)
		virtual void _0x10() = 0;                                                                             //  2 (0x10)
		virtual void cleanup_objects() = 0;                                                                   //  3 (0x18)
		virtual scriptId* _0x20() = 0;                                                                        //  4 (0x20)
		virtual scriptId* get_id() = 0;                                                                       //  5 (0x28)
		//Returns whether the script handler belongs to a networked script.
		virtual bool is_networked() = 0;                                                                      //  6 (0x30)
		//Initializes the network component for the script handler.
		virtual void init_net_component() = 0;                                                                //  7 (0x38)
		//Deletes the script handler's network component, if it exists.
		virtual void reset_net_component() = 0;                                                               //  8 (0x40)
		//Destroys the script handler.
		virtual bool destroy() = 0;                                                                           //  9 (0x48)
		//Adds the object to the script handler's list of objects.
		virtual void add_object(scriptHandlerObject*, bool is_network, bool is_network_and_scriptcheck) = 0; // 10 (0x50)
		//Something related to reservations.
		virtual void _0x58(void*) = 0;                                                                        // 11 (0x58)
		virtual void register_resource(scriptResource*, void*) = 0;                                           // 12 (0x60)
		virtual void _0x68() = 0;                                                                             // 13 (0x68)
		virtual void _0x70() = 0;                                                                             // 14 (0x70)
		virtual void _0x78() = 0;                                                                             // 15 (0x78)
		virtual void _0x80() = 0;                                                                             // 16 (0x80)
		virtual void _0x88() = 0;                                                                             // 17 (0x88)
		virtual void _0x90() = 0;                                                                             // 18 (0x90)
		virtual void _0x98() = 0;                                                                             // 19 (0x98)
	public:
		void* m_0x08;                                // 0x08
		void* m_0x10;                                // 0x10
		scrThread* m_script_thread;                  // 0x18
		atDList<atDScriptObjectNode> m_objects;      // 0x20
		scriptResource* m_resource_list_head;        // 0x30
		scriptResource* m_resource_list_tail;        // 0x38
		void* m_0x40;                                // 0x40
		scriptHandlerNetComponent* m_net_component;  // 0x48
		std::uint32_t m_0x50;                        // 0x50
		std::uint32_t m_0x54;                        // 0x54
		std::uint32_t m_0x58;                        // 0x58
		std::uint32_t m_0x60;                        // 0x5C
	};
	class scriptHandlerNetComponent {
	public:
		virtual ~scriptHandlerNetComponent() = default;
	public:
		scriptHandler* m_script_handler; // 0x08
	};
	class rlSessionInfo {
	public:
		uint64_t m_rockstar_id; //0x0000
		uint64_t m_session_token; //0x0008
		class rage::rlPeerInfo m_peer_info; //0x0010
	};
	static_assert(sizeof(rlSessionInfo) == 0xD0);
	class rlSessionByGamerTaskResult {
	public:
		rlSessionByGamerTaskResult() : m_gamer_handle(NULL) {}
		rlGamerHandle m_gamer_handle;
		rlSessionInfo m_session_info;
	};
	class rlPresenceEventInviteAccepted {
	public:
		rlPresenceEventInviteAccepted(rlGamerInfo& gamerInfo, rlSessionInfo& sessionInfo, rlGamerHandle& gamerHandle) : m_gamer_info(gamerInfo), m_session_info(sessionInfo), m_gamer_handle(gamerHandle) {
			m_self = this;
		}
		virtual ~rlPresenceEventInviteAccepted() = default;
		virtual void unk_0000() = 0;

		rlPresenceEventInviteAccepted* m_self; //0x0008
		rlGamerInfo m_gamer_info; //0x0010
		rlSessionInfo m_session_info; //0x00A8
		rlGamerHandle m_gamer_handle; //0x0118
	};
	class netSyncTree {
	public:
		char pad_0000[48]; //0x0000
		class netSyncTreeNode* m_sync_tree_node; //0x0030
	}; //Size: 0x0038
	class netSyncTreeNode {
	public:
		char pad_0000[192]; //0x0000
		uint32_t m_player_model; //0x00C0
		uint32_t m_ped_model; //0x00C4
		uint32_t m_vehicle_model; //0x00C8
		char pad_00CC[84]; //0x00CC
		uint32_t m_pickup_model; //0x0120
		char pad_0124[44]; //0x0124
		uint32_t m_object_model; //0x0150
		char pad_0154[692]; //0x0154
	}; //Size: 0x0408
	class netObject {
	public:
		int16_t m_object_type; //0x0008
		int16_t m_object_id; //0x000A
		char pad_000C[61]; //0x000C
		int8_t m_owner_id; //0x0049
		int8_t m_control_id; //0x004A
		int8_t m_next_owner_id; //0x004B
		bool m_is_remote; //0x004C
		bool m_wants_to_delete; //0x004D
		char pad_004E[1]; //0x004E
		bool m_should_not_be_deleted; //0x004F
		char pad_0050[32]; //0x0050
		uint32_t m_players_acked; //0x0070
		char pad_0074[116]; //0x0074

		virtual ~netObject() = 0;
		virtual void mov1() = 0;
		virtual void mov2() = 0;
		virtual void m_8() = 0;
		virtual void m_10() = 0;
		virtual void m_18() = 0;
		virtual void* m_20() = 0;
		virtual void m_28() = 0;
		virtual netSyncTree* GetSyncTree() = 0;
		virtual void m_38() = 0;
		virtual void m_40() = 0;
		virtual void m_48() = 0;
		virtual void m_50() = 0;
		virtual void m_58() = 0;
		virtual void m_60() = 0;
		virtual void m_68() = 0;
		virtual void m_70() = 0;
		virtual void m_78() = 0;
		virtual CObject* GetGameObject() = 0;
		virtual void m_88() = 0;
		virtual void m_90() = 0;
		virtual void m_98() = 0;
		virtual int GetObjectFlags() = 0;
		virtual void m_A8() = 0;
		virtual void m_B0() = 0;
		virtual void m_B8() = 0;
		virtual void m_C0() = 0;
		virtual void m_C8() = 0;
		virtual int GetSyncFrequency() = 0;
		virtual void m_D8() = 0;
		virtual void m_E0() = 0;
		virtual void m_E8() = 0;
		virtual void m_F0() = 0;
		virtual void m_F8() = 0;
		virtual void Update() = 0;
		virtual bool m_108_1604() = 0; // added in 1604
		virtual void m_108() = 0;
		virtual void m_110() = 0;
		virtual void m_118() = 0;
		virtual void m_120() = 0;
		virtual void m_128() = 0;
		virtual void m_130() = 0;
		virtual void m_138() = 0;
		virtual void m_140() = 0;
		virtual void m_148() = 0;
		virtual void m_150() = 0;
		virtual bool m_158(void* player, int type, int* outReason) = 0;
		virtual void m_160() = 0;
		virtual bool m_168(int* outReason) = 0;
		virtual void m_170() = 0;
		virtual void m_178() = 0;
		virtual void m_180() = 0;
		virtual void m_188() = 0;
		virtual void m_190() = 0;
		virtual void m_198() = 0;
		virtual void m_1A0() = 0;
		virtual void m_1A8() = 0;
		virtual void m_1B0() = 0;
		virtual void m_1B8() = 0;
		virtual void m_1C0() = 0;
		virtual void m_1C8() = 0;
		virtual void m_1D0() = 0;
		virtual void m_1D8() = 0;
		virtual void m_1E0() = 0;
		virtual void m_1E8() = 0;
		virtual void m_1F0() = 0;
		virtual void m_1F8() = 0;
		virtual void m_200() = 0;
		virtual void m_208() = 0;
		virtual void m_210() = 0;
		virtual void m_218() = 0;
		virtual void m_220() = 0;
		virtual void m_228() = 0;
		virtual void m_230() = 0;
		virtual void m_238() = 0;
		virtual void m_240() = 0;
		virtual void m_248() = 0;
		virtual void m_250() = 0;
		virtual void m_258() = 0;
		virtual void m_260() = 0;
		virtual void m_268() = 0;
		virtual void m_270() = 0;
		virtual void m_278() = 0;
		virtual void m_280() = 0;
		virtual void m_288() = 0;
		virtual void m_290() = 0;
		virtual void m_298() = 0;
		virtual void m_2A0() = 0;
		virtual void m_2A8() = 0;
		virtual void m_2B0() = 0;
		virtual void m_2B8() = 0;
		virtual void m_2C0() = 0;
		virtual void m_2C8() = 0;
		virtual void m_2D0() = 0;
		virtual void m_2D8() = 0;
		virtual void m_2E0() = 0;
		virtual void m_2E8() = 0;
		virtual void m_2F0() = 0;
		virtual void m_2F8() = 0;
		virtual void m_300() = 0;
		virtual void m_308() = 0;
		virtual void m_310() = 0;
		virtual void m_318() = 0;
		virtual void m_320() = 0;
		virtual void UpdatePendingVisibilityChanges() = 0;
	};
	static_assert(sizeof(netObject) == 0xE8);
	class netObjectMgrBase {
	public:
		class atDNetObjectNode : public atDNode<netObject*> {};
	public:
		virtual ~netObjectMgrBase() = default;
		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;
		virtual void Update(bool) = 0;
		virtual void AddEntity(void*, void*) = 0;
		virtual void UnregisterNetworkObject_(void* unk, bool force) = 0;
		virtual void UnregisterNetworkObject(netObject* object, int reason, bool force1, bool force2) = 0;
		virtual void ChangeOwner(netObject* object, CNetGamePlayer* player, int migrationType) = 0;
		virtual void HandleJoiningPlayer(CNetGamePlayer* player) = 0;
		virtual void HandleLeavingPlayer(CNetGamePlayer* player) = 0;
		virtual void _0x50(CNetGamePlayer* player) = 0;
		virtual void _0x58() = 0;
		virtual void RegisterNetworkObject(netObject* object) = 0;
		virtual void PackCloneCreate(netObject* object, CNetGamePlayer* player, datBitBuffer* buffer) = 0;
		virtual bool PackCloneRemove(netObject* object, CNetGamePlayer* player, bool) = 0;
		virtual void _0x78(netObject* object, void*) = 0;
		virtual void _0x80() = 0;
		virtual void _0x88() = 0;
		virtual const char* _0x90(int) = 0;
		virtual bool HandleCloneCreate(CNetGamePlayer* source, CNetGamePlayer* target, std::uint16_t object_type, std::uint16_t object_id, uint16_t object_flags, void*, std::uint32_t timestamp) = 0;
		virtual void HandleCloneCreateAck(CNetGamePlayer* source, CNetGamePlayer* target, std::uint16_t object_flags, uint32_t ack_code) = 0;
		virtual int HandleCloneSync(CNetGamePlayer* source, CNetGamePlayer* target, std::uint16_t object_type, std::uint16_t object_id, void*, std::uint16_t, std::uint32_t timestamp) = 0;
		virtual void HandleCloneSyncAck(CNetGamePlayer* source, CNetGamePlayer* target, void*, std::uint16_t objectId, uint32_t ack_code) = 0;
		virtual void HandleCloneRemove(CNetGamePlayer* source, CNetGamePlayer* target, std::uint16_t object_id, int) = 0;
		virtual void HandleCloneRemoveAck(CNetGamePlayer* source, CNetGamePlayer* target, std::uint16_t object_id, uint32_t ack_code) = 0;
		virtual void _0xC8() = 0;
	public:
		atDList<atDNetObjectNode> m_objects_owned_by_player[32];
	};
	class CEntityDrawHandler : public rage::fwDrawData {
	public:
	};
	static_assert(sizeof(CEntityDrawHandler) == 0x2C);
	class CEntity : public fwEntity {
	public:
		virtual void* _0x120() = 0; // implemented only by CPed
		virtual void UpdatePositionImpl() = 0; // 0x128
		virtual void _0x130() = 0;
		virtual void _0x138(void*) = 0;
		virtual void _0x140() = 0;
		virtual void _0x148(int) = 0;
		virtual bool _0x150() = 0;
		virtual CEntityDrawHandler* CreateDrawHandler() = 0; // 0x158
		virtual int GetTypeFlags() = 0; // 0x160
		virtual int GetTypeFlags2() = 0; // 0x168
		virtual bool _0x170() = 0; // implemented only by CPickup
		virtual bool _0x178() = 0;
		virtual void _0x180(bool) = 0;
		virtual bool _0x188() = 0;
		virtual bool _0x190() = 0;
		virtual void ClearDecals() = 0; // 0x198
		virtual void GetModelBounds(rage::vector3* bounds) = 0; // 0x1A0
		virtual void GetModelBounds2(rage::vector3* bounds) = 0; // 0x1A8
		virtual float GetBoundingBoxSize() = 0; // 0x1B0
		virtual float _0x1B8(void*) = 0;
		virtual float _0x1C0(void*) = 0;
		virtual rage::vector3* _0x1C8() = 0;
		virtual rage::vector3* GetCameraOffset() = 0; // 0x1D0
		virtual void GetCameraBasePosition(rage::vector3* pos) = 0; // 0x1D8
		virtual bool _0x1E0() = 0;
		virtual bool Update() = 0; // 0x1E8 always returns true
		virtual bool _0x1F0() = 0;
		virtual void Warp(rage::vector3* pos, float heading, bool update_pos) = 0; // 0x1F8

		uint8_t unk_00B9; //0x00B9
		char pad_00BA[6]; //0x00BA
		uint32_t m_flags_3; //0x00C0
		uint32_t m_flags_4; //0x00C4
		uint32_t unk_00C8; //0x00C8
		uint32_t unk_00CC; //0x00CC
	}; //Size: 0x00D0
	static_assert(sizeof(CEntity) == 0xD0);
	class CDynamicEntity : public CEntity {
	public:
		class netObject* m_net_object; //0x00D0
		char pad_00D8[16]; //0x00D8
		uint64_t unk_00E8; //0x00E8
	}; //Size: 0x00F0
	static_assert(sizeof(CDynamicEntity) == 0xF0);
#pragma pack(push, 1)
	class CPhysical : public CDynamicEntity {
	public:
		char pad_00F0[144]; //0x00F0
		uint64_t unk_0180; //0x0180
		uint32_t m_damage_bits; //0x0188
		uint8_t m_hostility; //0x018C
		char pad_018D[3]; //0x018D
		uint8_t unk_0190; //0x0190
		char pad_0191[3]; //0x0191
		uint32_t unk_0194; //0x0194
		char pad_0198[232]; //0x0198
		float m_health; //0x0280
		float m_maxhealth; //0x0284
		uint64_t unk_0280; //0x0280
		char pad_02B0[72]; //0x0290
		uint64_t unk_02D8; //0x02D8
		uint64_t unk_02E0; //0x02E0
		uint32_t unk_02E8; //0x02E8
	}; //Size: 0x02EC
	static_assert(sizeof(CPhysical) == 0x2EC);
#pragma pack(pop)
	class scriptHandlerMgr {
	public:
		virtual ~scriptHandlerMgr();
		virtual bool initialize(); //1
		virtual void _0x10(); //2
		virtual void shutdown(); //3
		virtual void _0x20(); //4
		virtual void _0x28(); //5
		virtual void _0x30(); //6
		virtual void _0x38(scrThread*); //7
		virtual scriptHandler* create_script_handler(); //8
		virtual scriptHandler* get_script_handler(scriptId* id); //9
		virtual void attach_thread(scrThread* thread); //10
		virtual void detach_thread(scrThread* thread); //11
		virtual void on_player_join(netPlayer* player); //12
		virtual void on_player_left(netPlayer* player); //13
		virtual int32_t _0x70();
		virtual void* _0x78();
	public:
		char pad_0008[40]; //0x0008
		bool m_initialized; //0x0030
		bool m_initialized_2; //0x0031
		char pad_0032[14]; //0x0032
		netLoggingInterface* m_logger; //0x0040
	}; //Size: 0x0048
	class scrVectorSpace {
	public:
		void Reset() {
			BufferCount = 0;
		}
		void CopyReferencedParametersOut() {
			while (BufferCount--) {
				Orig[BufferCount][0].Float = Buffer[BufferCount].x;
				Orig[BufferCount][1].Float = Buffer[BufferCount].y;
				Orig[BufferCount][2].Float = Buffer[BufferCount].z;
			}
		}
		uint32_t BufferCount; //0x0000
		scrValue* Orig[4]; //0x0004
		scrVector Buffer[4]; //0x0020
	}; //Size: 0x0060
	class scrNativeCallContext {
	public:
		void reset() {
			ArgCount = 0;
			VectorSpace.Reset();
		}
		template <typename t>
		void pushArg(t&& value) {
			static_assert(sizeof(t) <= sizeof(std::uint64_t));
			*reinterpret_cast<std::remove_cv_t<std::remove_reference_t<t>>*>(reinterpret_cast<std::uint64_t*>(Args) + (ArgCount++)) = std::forward<t>(value);
		}
		template <typename t>
		t& getArg(std::size_t index) {
			static_assert(sizeof(t) <= sizeof(std::uint64_t));
			return *reinterpret_cast<t*>(reinterpret_cast<std::uint64_t*>(Args) + index);
		}
		template <typename t>
		void setArg(std::size_t index, t&& value) {
			static_assert(sizeof(t) <= sizeof(std::uint64_t));
			*reinterpret_cast<std::remove_cv_t<std::remove_reference_t<t>>*>(reinterpret_cast<std::uint64_t*>(Args) + index) = std::forward<t>(value);
		}
		template <typename t>
		t* getRetValue() {
			return reinterpret_cast<t*>(Return);
		}
		template <typename t>
		void setRetValue(t value) {
			*reinterpret_cast<std::remove_cv_t<std::remove_reference_t<t>>*>(Return) = value;
		}
	public:
		scrValue* Return; //0x0000
		uint32_t ArgCount; //0x0008
		scrValue* Args; //0x0010
		scrVectorSpace VectorSpace;
	}; //Size: 0x00E0
	class scrNativeRegistration {
	public:
		scrNativeRegistration* m_next; //0x0000
		scrNativeRegistration* m_previous; //0x0008
		scrCmd m_handlers[7]; //0x0010
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
		void set_next_registration(scrNativeRegistration* registration) {
			m_next = (scrNativeRegistration*)(((uint64_t)((uint64_t*)&m_next) << 32) ^ ((uint32_t)((uint64_t*)&m_next) << 0) ^ (uint64_t)registration);
			m_previous = NULL;
		}
		uint32_t get_num_entries() {
			return static_cast<uint32_t>(((uintptr_t)&m_num_entries) ^ m_num_entries ^ m_num_entries_2);
		}
		void set_num_entries(uint32_t entries) {
			m_num_entries = (uint32_t)&m_num_entries ^ entries;
			m_num_entries_2 = entries;
		}
		uint64_t get_hash(uint32_t index) {
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
		void set_hash(uint32_t index, uint64_t newHash) {
			auto hash = &m_hashes[index * 2];
			hash[0] = ((uint64_t)hash << 32) ^ ((uint32_t)hash << 0) ^ (uint64_t)newHash;
			hash[1] = 0;
		}
	}; //Size: 0x00C0
	static_assert(sizeof(scrNativeRegistration) == 0xC0);
#pragma pack(push, 1)
	class scrNativeRegistrationTable {
	public:
		scrNativeRegistration* m_entries[255]; //0x0000
		uint32_t m_seed; //0x07F8
		bool m_initialized; //0x07FC

		rage::scrCmd get_handler(uint64_t hash) {
			for (auto entry = m_entries[(uint8_t)(hash & 0xFF)]; entry; entry = entry->get_next_registration()) {
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
#pragma pack(push, 1)
	class JSONSerialiser {
	public:
		JSONSerialiser(char* buffer, uint32_t length) : m_buffer(buffer), m_max_length(length) {
			unk_0000 = 0;
			unk_0004 = 0;
			m_current_length = 0;
			unk_0018 = 1;
			m_flags = 0;
		}
	public:
		uint32_t unk_0000; //0x0000
		uint32_t unk_0004; //0x0004
		char* m_buffer; //0x0008
		uint32_t m_current_length; //0x0010
		uint32_t m_max_length; //0x0014
		uint32_t unk_0018; //0x0018
		uint8_t m_flags; //0x001C

		std::string get_buffer() {
			return m_buffer;
		}
	}; //Size: 0x0020
	static_assert(sizeof(JSONSerialiser) == 0x1D);
#pragma pack(pop)
	class rlMetric {
	public:
		virtual ~rlMetric() = default; //Deconstructor
		virtual int _0x08() { return 0; }; //Returns a constant integerlike 4, 5, 6
		virtual int _0x10() { return 0; }; //Returns a constant integer like 0
		virtual char const* get_name() { return ""; }; //Short name of the metric
		virtual bool to_json(JSONSerialiser* jsonStream) { return false; }; //Prints the metric out to a JSON stream
		virtual int get_size() { return 0; }; //Size in bytes of derived object (for copying)
		virtual uint32_t get_name_hash() { return 0; };     //Joaat of short name
	};
	template <typename t>
	class ObfVar {
	public:
		t getData() {
			auto v105 = m_unk4;
			auto v28 = m_unk1 & v105;
			auto v94 = m_unk2 & ~v105;
			return v28 | v94;
		}
		operator t() {
			return getData();
		}
		#if _WIN32
		void setData(t val) {
			auto seed = time(nullptr);
			m_unk3 = seed;
			seed = time(nullptr);
			m_unk4 = seed;

			auto v48 = val & ~seed;
			m_unk1 = seed & val;
			m_unk2 = v48;
		}
		void operator =(t val) {
			setData(val);
		}
		#endif
	private:
		t m_unk1;
		t m_unk2;
		t m_unk3;
		t m_unk4;
	};
	class rlSessionDetail {
	public:
		class rage::rlPeerInfo m_peer_info;
		char pad_0060[8]; //0x0060
		class rage::rlSessionInfo m_session_info; //0x0068
		char pad_00D8[14]; //0x00D8
		uint16_t m_session_type; //0x00E6
		char pad_00E8[324]; //0x00E8
		uint32_t m_player_count; //0x022C
		uint32_t m_unk_player_count; //0x0230
		char pad_0234[2]; //0x0234
		int16_t m_unk_pos_x; //0x0236
		int16_t m_unk_pos_y; //0x0238
		int16_t m_unk_pos_z; //0x023A
		uint8_t m_matchmaking_property_ids[32]; //0x023C
		char pad_025C[2]; //0x025C
		uint16_t m_rank; //0x025E
		char pad_0260[1]; //0x0260
		uint8_t m_mental_state; //0x0261
		char pad_0262[21]; //0x0262
		uint8_t m_population_density; //0x0277
		char pad_0278[320]; //0x0278
	}; //Size: 0x03CA
	static_assert(sizeof(rlSessionDetail) == 0x478);
	class rlMatchmakingFindResult {
	public:
		class rage::rlSessionDetail m_result_session_details[15]; //0x0000
		char pad_4308[168]; //0x4308
	}; //Size: 0x43B0
	static_assert(sizeof(rage::rlMatchmakingFindResult) == 0x43B0);
	class netGamePlayerData {
	public:
		class rlGamerHandle m_handle; //0x0000
		bool m_is_activity_spectator; //0x0010
		char pad_0011[7]; //0x0011
		uint64_t m_crew_id; //0x0018
		uint16_t m_rank; //0x0020
		uint16_t m_debug_unk; //0x0022
		char pad_0024[4]; //0x0024
		uint32_t m_nat_type; //0x0028
		bool m_is_rockstar_dev; //0x002C
		char pad_002D[3]; //0x002D
	}; //Size: 0x0030
	static_assert(sizeof(rage::netGamePlayerData) == 0x30);
#pragma pack(push, 8)
	class netConnectionPeer {
	public:
		netSocketAddress m_internal_address; //0x0000
		netSocketAddress m_external_address; //0x0008
		uint64_t m_peer_id; //0x0010
		uint32_t unk_0018; //0x0018
		uint16_t unk_001C; //0x001C
		uint8_t m_platform; //0x001E
	}; //Size: 0x0020
	static_assert(sizeof(netConnectionPeer) == 0x20);
#pragma pack(pop)
	namespace netConnection {
#pragma pack(push, 1)
		class InFrame {
		public:
			virtual ~InFrame() = default;
			virtual void Destroy() = 0;
			virtual uint32_t GetId() = 0;
			virtual uint32_t GetPayloadSize() = 0;
			virtual bool IsChanneled() = 0;

			uint32_t m_timestamp; //0x0008
			char pad_0008[52]; //0x000C
			uint32_t m_msg_id; //0x0040
			uint32_t m_connection_identifier; //0x0044
			InFrame* m_this; //0x0048
			uint32_t m_platform_data; //0x0050
			char pad_0050[44]; //0x0058
			uint32_t m_length; //0x0080
			char pad_007C[4]; //0x0084
			void* m_data; //0x0088
		}; //Size: 0x0090
		static_assert(sizeof(rage::netConnection::InFrame) == 0x90);
#pragma pack(pop)
	}
	class netConMgr {
	public:
		char pad_0000[200];
	};
	class snPlayer {
	public:
		uint64_t m_msg_id; //0x0000
		class rage::rlGamerInfo m_gamer_info; //0x0008
	}; //Size: 0x00100
	static_assert(sizeof(rage::snPlayer) == 0x100);
	class snPeer {
	public:
		class rage::rlGamerInfo m_gamer_data; //0x0000
		char pad_00F8[40]; //0x00F8
	}; //Size: 0x0120
	static_assert(sizeof(rage::snPeer) == 0x120);
	class rlRemoteGamer {
	public:
		rage::rlGamerHandle m_handle;
		char pad_0010[4]; //0x0010
		uint32_t m_timeout_time; //0x0014
		uint32_t m_time_unk; //0x0018
		char pad_001C[4]; //0x001C
	}; //Size: 0x0020
	static_assert(sizeof(rage::rlRemoteGamer) == 0x20);
	class rlSession {
	public:
		virtual ~rlSession() = default;

		char pad_0008[264]; //0x0008
		class rage::rlSessionInfo m_session_info; //0x0110
		char pad_01E0[288]; //0x01E0
		uint64_t m_session_id; //0x0300
		char pad_0308[1136]; //0x0308
	}; //Size: 0x778
	static_assert(sizeof(rage::rlSession) == 0x778);
#pragma pack(push, 1)
	class snSession {
	public:
		rage::sysMemAllocator* m_memory_allocator; //0x0000
		char pad_0008[64]; //0x0008
		rage::netConnectionManager* m_net_connection_mgr; //0x0048
		char pad_0050[48]; //0x0050
		class rage::rlSession m_rl_session; //0x0080
		class rage::snPlayer m_local_player; //0x0938
		uint64_t m_peer_address; //0x09D8
		char pad_09E0[144]; //0x09E0
		class rage::snPeer m_peer_storage[32]; //0x0A70
		char pad_2270[24]; //0x2270
		class rage::snPeer* m_peers[32]; //0x2288
		uint32_t m_peer_count; //0x2388
		char pad_238C[4]; //0x238C
		class rage::snPlayer m_player_storage[32]; //0x2390
		char pad_3790[24]; //0x3790
		class rage::snPlayer* m_players[32]; //0x37A8
		uint32_t m_player_count; //0x38A8
		char pad_38AC[4]; //0x38AC
		class rage::rlRemoteGamer m_remote_gamers[32]; //0x38B0
		uint32_t m_num_remote_gamers; //0x3CB0
		bool m_player_joining; //0x3CB4
		char pad_3CB5[107]; //0x3CB5
		uint32_t m_connection_identifier; //0x3D20
		char pad_3D24[8]; //0x3D24
		char m_token_key[64]; //0x3D2C
		char m_id_key[64]; //0x3D6C
		char m_info_key[64]; //0x3DAC
		char m_host_key[64]; //0x3DEC
		char m_join_key[64]; //0x3E2C
		char pad_3E6C[4]; //0x3E6C

		rage::snPlayer* getPlayerByPeerAddress(uint64_t peerAddress) {
			for (uint32_t i{}; i != m_player_count; ++i) {
				if (auto snPlayer = m_players[i]; snPlayer) {
					if (snPlayer->m_gamer_info.m_peer_address == peerAddress)
						return snPlayer;
				}
			}
			return nullptr;
		}
		rage::snPlayer* getPlayerByPlatformData(uint64_t platformData) {
			for (uint32_t i{}; i != m_player_count; ++i) {
				if (auto snPlayer = m_players[i]; snPlayer) {
					if (snPlayer->m_gamer_info.m_platform_data == platformData)
						return snPlayer;
				}
			}
			return nullptr;
		}
		rage::snPlayer* getPlayerByMsgId(uint32_t msgId) {
			for (uint32_t i{}; i != m_player_count; ++i) {
				if (auto snPlayer = m_players[i]; snPlayer) {
					if (snPlayer->m_msg_id == msgId)
						return snPlayer;
				}
			}
			return nullptr;
		}
		bool is_host() {
			return m_local_player.m_gamer_info.m_peer_address == m_peer_address;
		}
		auto session_info() {
			return m_rl_session.m_session_info;
		}
		auto session_id() {
			return m_rl_session.m_session_id;
		}
	}; //Size: 0x5730
	static_assert(sizeof(rage::snSession) == 0x5590);
#pragma pack(pop)
	class snMsgRemoveGamersFromSessionCmd {
	public:
		uint64_t m_session_id; //0x0000
		uint64_t m_peer_ids[32]; //0x0008
		int32_t m_unk = -1; //0x0108
		uint32_t m_num_peers; //0x010C
	}; //Size: 0x0110
	static_assert(sizeof(rage::snMsgRemoveGamersFromSessionCmd) == 0x110);
	class snMsgAddGamerToSessionCmd {
	public:
		uint64_t m_session_id; //0x0000
		class rlGamerInfo m_gamer_info; //0x0008
		class netConnectionPeer m_net_connection_peer; //0x00A0
		uint32_t unk_00C0; //0x00C0
		uint32_t m_unk_struct_size; //0x00C4
		char m_unk_struct[512]; //0x00C8
		uint32_t m_num_handles; //0x02C8
		rlGamerHandle m_handles[32]; //0x02C8
	}; //Size: 0x00A8
	enum InitFunctionType : int {
		INIT_UNKNOWN = 0,
		INIT_CORE = 1,
		INIT_BEFORE_MAP_LOADED = 2,
		INIT_AFTER_MAP_LOADED = 4,
		INIT_SESSION = 8
	};
	class InitFunctionData {
	public:
		virtual ~InitFunctionData() = default;
		virtual void initFunction(int unk) = 0;
		virtual void shutdownFunction(int unk) = 0;

		int m_init_order;
		int m_shutdown_order;
		InitFunctionType m_init_mask;
		InitFunctionType m_shutdown_mask;
		u32 m_func_hash;
	};
	class InitFunctionEntry {
	public:
		int m_order;
		rage::atArray<int> m_functions;
		InitFunctionEntry* m_next;
	};
	class InitFunctionList {
	public:
		InitFunctionType m_type;
		InitFunctionEntry* m_entries;
		InitFunctionList* m_next;
	};
	class gameSkeletonUpdateBase {
	public:
		virtual ~gameSkeletonUpdateBase() = 0;
		virtual u64 Run() = 0;

		bool m_flag;
		float m_unk_float;
		u32 m_hash;
		//Might be different for updateElement
		gameSkeletonUpdateBase* m_next_ptr;
		gameSkeletonUpdateBase* m_child_ptr;
	};
	class UpdateFunctionList {
	public:
		s32 m_type;
		gameSkeletonUpdateBase* m_entry;
		UpdateFunctionList* m_next;
	};
	class gameSkeleton {
	public:
		virtual ~gameSkeleton() = 0;

		int m_function_order;
		InitFunctionType m_function_type;
		int32_t pad;
		int m_updateType;
		rage::atArray<InitFunctionData> m_init_functions;
		void* pad2;
		char pad3[256];
		InitFunctionList* m_init_function_list;
		void* pad4;
		UpdateFunctionList* m_update_function_list;
	};
}
class CObject : public rage::fwEntity {};
#pragma pack(push, 8)
class Vector2 final {
public:
	Vector2() : x(0.f), y(0.f) {}
	Vector2(float x, float y) : x(x), y(y) {}
public:
	float x, y;
	rage::vector2 deserialize() {
		return { x, y };
	}
	bool empty() {
		return x == 0.f && y == 0.f;
	}
public:
	Vector2 operator*(const float amount) {
		return { x * amount, y * amount };
	}
	Vector2 operator*(const Vector2 vec) {
		return { x * vec.x, y * vec.y };
	}
	Vector2 operator+(const float amount) {
		return { x + amount, y + amount };
	}
	Vector2 operator+(const Vector2 vec) {
		return { x + vec.x, y + vec.y };
	}
	Vector2 operator-(const float amount) {
		return { x - amount, y - amount };
	}
	Vector2 operator-(const Vector2 vec) {
		return { x - vec.x, y - vec.y };
	}
};
#pragma pack(pop)
class Vector3 : public rage::scrVector {
public:
	Vector3() : rage::scrVector() {}
	Vector3(float x, float y, float z) : rage::scrVector(x, y, z) {}
public:
	rage::vector3 deserialize() {
		return { x, y, z };
	}
	bool empty() {
		return x == 0.f && y == 0.f && z == 0.f;
	}
public:
	Vector3 operator*(const float amount) {
		return { x * amount, y * amount, z * amount };
	}
	Vector3 operator*(const Vector3 vec) {
		return { x * vec.x, y * vec.y, z * vec.z };
	}
	Vector3 operator+(const float amount) {
		return { x + amount, y + amount, z + amount };
	}
	Vector3 operator+(const Vector3 vec) {
		return { x + vec.x, y + vec.y, z + vec.z };
	}
	Vector3 operator-(const float amount) {
		return { x - amount, y - amount, z - amount };
	}
	Vector3 operator-(const Vector3 vec) {
		return { x - vec.x, y - vec.y, z - vec.z };
	}
};
#pragma pack(push, 8)
class Vector4 final {
public:
	Vector4() = default;
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
public:
	float x, y, z, w;
	rage::vector4 deserialize() {
		return { x, y, z, w };
	}
	bool empty() {
		return x == 0.f && y == 0.f && z == 0.f && w == 0.f;
	}
public:
	Vector4 operator*(const float amount) {
		return { x * amount, y * amount, z * amount, w * amount };
	}
	Vector4 operator*(const Vector4 vec) {
		return { x * vec.x, y * vec.y, z * vec.z, w * vec.w };
	}
	Vector4 operator+(const float amount) {
		return { x + amount, y + amount, z + amount, w + amount };
	}
	Vector4 operator+(const Vector4 vec) {
		return { x + vec.x, y + vec.y, z + vec.z, w + vec.z };
	}
	Vector4 operator-(const float amount) {
		return { x - amount, y - amount, z - amount, w - amount };
	}
	Vector4 operator-(const Vector4 vec) {
		return { x - vec.x, y - vec.y, z - vec.z, w - vec.z };
	}
};
#pragma pack(pop)
inline Vector2 rage::vector2::serialize() {
	return { x, y };
}
inline Vector3 rage::vector3::serialize() {
	return { x, y, z };
}
inline Vector4 rage::vector4::serialize() {
	return { x, y, z, w };
}
#pragma pack(push, 1)
class FriendInfo {
public:
	char m_name[20]; //0x0000
	char pad_0014[36]; //0x0014
	uint64_t m_rockstar_id; //0x0038
	uint8_t unk_0x40; //0x0040
	char pad_0041[3]; //0x0041
	uint32_t m_friend_state; //0x0044
	char pad_0048[304]; //0x0048
	uint32_t m_is_joinable; //0x0178
	char pad_017C[4]; //0x017C
}; //Size: 0x0180
static_assert(sizeof(FriendInfo) == 0x180);
class FriendList {
public:
	FriendInfo m_friends[250]; //0x0000
}; //Size: 0x17700
static_assert(sizeof(FriendList) == 0x17700);
class FriendRegistry {
public:
	uint32_t m_friend_count; //0x0000
	char pad_0004[8]; //0x0004
	FriendList* m_friend_list; //0x000C

	FriendInfo* get(uint32_t idx) {
		return &m_friend_list->m_friends[idx];
	}
}; //Size: 0x0014
static_assert(sizeof(FriendRegistry) == 0x14);
#pragma pack(pop)
class CNonPhysicalPlayerData : public rage::nonPhysicalPlayerDataBase {
public:
	int32_t m_bubble_id; //0x0008
	int32_t m_player_id; //0x000C
	rage::vector3 m_position; //0x0010
};
#pragma pack(push, 4)
class CHandlingData {
public:
	char pad_0000[8]; //0x0000
	uint32_t m_model_hash; //0x0008
	float m_mass; //0x000C
	float m_initial_drag_coeff; //0x0010
	float m_downforce_multiplier; //0x0014
	float m_popup_light_rotation; //0x0018
	char pad_001C[4]; //0x001C
	rage::vector3 m_centre_of_mass; //0x0020
	char pad_002C[4]; //0x002C
	rage::vector3 m_inertia_mult; //0x0030
	char pad_003C[4]; //0x003C
	float m_buoyancy; //0x0040
	float m_drive_bias_rear; //0x0044
	float m_drive_bias_front; //0x0048
	float m_acceleration; //0x004C
	uint8_t m_initial_drive_gears; //0x0050
	char pad_0051[3]; //0x0051
	float m_drive_inertia; //0x0054
	float m_upshift; //0x0058
	float m_downshift; //0x005C
	float m_initial_drive_force; //0x0060
	float m_drive_max_flat_velocity; //0x0064
	float m_initial_drive_max_flat_vel; //0x0068
	float m_brake_force; //0x006C
	char pad_0070[4]; //0x0070
	float m_brake_bias_front; //0x0074
	float m_brake_bias_rear; //0x0078
	float m_handbrake_force; //0x007C
	float m_steering_lock; //0x0080
	float m_steering_lock_ratio; //0x0084
	float m_traction_curve_max; //0x0088
	float m_traction_curve_lateral; //0x008C
	float m_traction_curve_min; //0x0090
	float m_traction_curve_ratio; //0x0094
	float m_curve_lateral; //0x0098
	float m_curve_lateral_ratio; //0x009C
	float m_traction_spring_delta_max; //0x00A0
	float m_traction_spring_delta_max_ratio; //0x00A4
	float m_low_speed_traction_loss_mult; //0x00A8
	float m_camber_stiffness; //0x00AC
	float m_traction_bias_front; //0x00B0
	float m_traction_bias_rear; //0x00B4
	float m_traction_loss_mult; //0x00B8
	float m_suspension_force; //0x00BC
	float m_suspension_comp_damp; //0x00C0
	float m_suspension_rebound_damp; //0x00C4
	float m_suspension_upper_limit; //0x00C8
	float m_suspension_lower_limit; //0x00CC
	float m_suspension_raise; //0x00D0
	float m_suspension_bias_front; //0x00D4
	float m_suspension_bias_rear; //0x00D8
	float m_anti_rollbar_force; //0x00DC
	float m_anti_rollbar_bias_front; //0x00E0
	float m_anti_rollbar_bias_rear; //0x00E4
	float m_roll_centre_height_front; //0x00E8
	float m_roll_centre_height_rear; //0x00EC
	float m_collision_damage_mult; //0x00F0
	float m_weapon_damamge_mult; //0x00F4
	float m_deformation_mult; //0x00F8
	float m_engine_damage_mult; //0x00FC
	float m_petrol_tank_volume; //0x0100
	float m_oil_volume; //0x0104
	char pad_0108[4]; //0x0108
	rage::vector3 m_seat_offset_dist; //0x010C
	uint32_t m_monetary_value; //0x0118
	char pad_011C[8]; //0x011C
	uint32_t m_model_flags; //0x0124
	uint32_t m_handling_flags; //0x0128
	uint32_t m_damage_flags; //0x012C
	char pad_0130[12]; //0x0130
	uint32_t m_ai_handling_hash; //0x013C
}; //Size: 0x0140
static_assert(sizeof(CHandlingData) == 0x140);
#pragma pack(pop)
#pragma pack(push, 1)
class CVehicleModelInfo : public rage::CBaseModelInfo {
public:
	char pad_00A4[84]; //0x00A4
	uint8_t m_primary_color_combinations[25]; //0x00F8
	uint8_t m_secondary_color_combinations[25]; //0x0111
	uint8_t m_unk_color_combos1[25]; //0x012A
	uint8_t m_unk_color_combos2[25]; //0x0143
	uint8_t m_interior_color_combinations[25]; //0x015C
	uint8_t m_dashboard_color_combinations[25]; //0x0175
	char pad_018E[266]; //0x018E
	char m_name[12]; //0x0298
	char m_manufacturer[12]; //0x02A4
	char pad_02B0[8]; //0x02B0
	uint16_t m_modkits_count; //0x02B8
	char pad_02BA[30]; //0x02BA
	uint8_t m_passenger_capacity; //0x02D8
	char pad_02D9[103]; //0x02D9
	eVehicleType m_vehicle_type; //0x0340
	uint32_t m_unk_vehicle_type; //0x0344
	uint32_t m_diffuse_tint; //0x0348
	char pad_034C[20]; //0x034C
	struct rage::vector3 m_first_person_driveby_ik_offset; //0x0360
	char pad_036C[4]; //0x036C
	struct rage::vector3 m_first_person_driveby_unarmed_ik_offset; //0x0370
	char pad_037C[20]; //0x037C
	struct rage::vector3 m_first_person_driveby_right_passenger_ik_offset; //0x0390
	char pad_039C[36]; //0x039C
	struct rage::vector3 m_first_person_driveby_right_passenger_unarmed_ik_offset; //0x03C0
	char pad_03CC[4]; //0x03CC
	struct rage::vector3 m_first_person_projectile_driveby_ik_offset; //0x03D0
	char pad_03DC[4]; //0x03DC
	struct rage::vector3 m_first_person_projectile_driveby_passenger_ik_offset; //0x03E0
	char pad_03EC[52]; //0x03EC
	struct rage::vector3 m_first_person_mobile_phone_offset; //0x0420
	char pad_042C[4]; //0x042C
	struct rage::vector3 m_first_person_passenger_mobile_phone_offset; //0x0430
	char pad_043C[20]; //0x043C
	struct rage::vector3 m_pov_camera_offset; //0x0450
	char pad_045C[36]; //0x045C
	float m_pov_camera_vertical_adjustement_for_rollcage; //0x0480
	char pad_0484[8]; //0x0484
	float m_wheel_scale; //0x048C
	float m_wheel_scale_rear; //0x0490
	float m_default_health; //0x0494
	char pad_0498[4]; //0x0498
	float m_steer_wheel_multiplier; //0x049C
	char pad_04A0[168]; //0x04A0
	uint32_t m_model_class; //0x0548
	char pad_0550[8]; //0x550
	float m_min_seat_height; //0x0554
	char pad_0558[40]; //0x0558
	uint32_t m_is_jetski; //0x0580
	char pad_0584[7]; //0x0584
	uint32_t m_ability_flags; //0x058B
	char pad_058F[17]; //0x0584

	bool set_ability_flags(eAbilityFlags flag, bool value) {
		value ? m_ability_flags |= (int)flag : m_ability_flags &= ~(int)flag;
	}
}; //Size: 0x05A0
static_assert(sizeof(CVehicleModelInfo) == 0x5A0);
#pragma pack(pop)
#pragma pack(push, 1)
class CVehicle : public rage::CPhysical {
public:
	char gap30C[4];
	uint32_t dword310;
	uint32_t dword314;
	uint8_t m_boost_state; // 0x318
	char gap319[2];
	uint8_t m_boost_allow_recharge; // 0x31B
	uint16_t word31C;
	char gap31E[2];
	float m_boost; // 0x320
	float m_rocket_recharge_speed; // 0x324
	uint32_t dword328;
	uint8_t byte32C;
	char gap32D[3];
	uint8_t byte330;
	char gap331[3];
	uint64_t qword334;
	uint32_t dword33C;
	uint8_t byte340;
	char gap341[3];
	uint32_t dword344;
	uint8_t byte348;
	char gap349[3];
	uint64_t qword34C;
	uint64_t qword354;
	uint64_t qword35C;
	uint64_t qword364;
	uint32_t dword36C;
	uint32_t dword370;
	uint8_t byte374;
	uint16_t word375;
	uint8_t byte377;
	uint64_t qword378;
	uint64_t qword380;
	uint64_t qword388;
	uint32_t dword390;
	uint32_t dword394;
	uint8_t byte398;
	char gap399[3];
	uint64_t qword39C;
	uint64_t qword3A4;
	char gap3AC[4];
	float m_jump_boost_charge; // 0x3B0
	uint8_t byte3B4;
	char gap3B5[3];
	uint64_t qword3B8;
	uint32_t dword3C0;
	uint32_t dword3C4;
	char gap3C8[8];
	uint16_t oword3D0;
	char gap3D2[30];
	uint16_t oword3F0;
	char gap3F2[14];
	uint32_t dword400;
	uint64_t qword404;
	char gap40C[4];
	uint32_t dword410;
	uint64_t qword414;
	char gap41C[4];
	uint64_t qword420;
	uint32_t dword428;
	uint32_t dword42C;
	char gap430[1032];
	uint64_t qword838;
	float m_body_health; // 0x840
	float m_petrol_tank_health; // 0x844
	char gap848[12];
	float float854;
	char gap858[176];
	float m_engine_health; // 0x908
	char gap90C[44];
	CHandlingData* m_handling_data; // 0x938
	char gap940[2];
	uint8_t m_is_drivable; //0x0942
	uint8_t m_tyres_can_burst; //0x0943
	uint8_t m_deform_god; //0x0944
	char gap945[6];
	uint8_t byte94B;
	char gap94C;
	uint8_t byte94D;
	char gap94E;
	uint8_t byte94F;
	char gap950[3];
	uint8_t byte953;
	uint8_t byte954;
	char gap955;
	uint8_t byte956;
	uint8_t byte957;
	char gap958[2];
	uint8_t byte95A;
	char gap95B[9];
	uint64_t qword964;
	char gap96C[4];
	uint64_t qword970;
	uint64_t qword978;
	char gap980[8];
	uint64_t qword988;
	uint16_t word990;
	uint32_t dword992;
	uint16_t word996;
	uint64_t qword998;
	uint64_t qword9A0;
	uint32_t dword9A8;
	uint64_t qword9AC;
	uint64_t qword9B4;
	uint64_t qword9BC;
	uint16_t word9C4;
	uint8_t byte9C6;
	char gap9C7;
	uint32_t dword9C8;
	uint8_t byte9CC;
	char gap9CD;
	uint16_t word9CE;
	uint16_t word9D0;
	char gap9D2[2];
	uint32_t dword9D4;
	char gap9D8[8];
	uint32_t dword9E0;
	uint64_t qword9E4;
	uint32_t dword9EC;
	uint32_t dword9F0;
	char gap9F4[4];
	float m_dirt_level; // 0x9F8
	uint32_t dword9FC;
	uint64_t qwordA00;
	uint64_t qwordA08;
	uint32_t dwordA10;
	uint32_t dwordA14;
	uint8_t byteA18;
	uint16_t wordA19;
	uint8_t byteA1B;
	uint32_t dwordA1C;
	uint64_t qwordA20;
	uint32_t dwordA28;
	uint32_t dwordA2C;
	uint64_t qwordA30;
	uint64_t qwordA38;
	uint64_t qwordA40;
	uint64_t qwordA48;
	uint8_t byteA50;
	char gapA51[7];
	uint64_t qwordA58;
	uint64_t qwordA60;
	uint32_t dwordA68;
	uint32_t dwordA6C;
	uint32_t dwordA70;
	uint32_t dwordA74;
	uint32_t dwordA78;
	uint32_t dwordA7C;
	uint64_t qwordA80;
	uint8_t byteA88;
	char gapA89;
	uint16_t wordA8A;
	uint64_t qwordA8C;
	uint64_t qwordA94;
	uint64_t qwordA9C;
	uint64_t qwordAA4;
	uint32_t dwordAAC;
	uint16_t wordAB0;
	uint16_t wordAB2;
	uint8_t byteAB4;
	uint16_t wordAB5;
	uint16_t wordAB7;
	char gapAB9[2];
	char gapABB[3]; // start of some kind of flag, default value 0x1000000i64
	uint8_t m_is_targetable; // 0xABE
	char gapABF[4]; // end of some kind of flag
	uint32_t dwordAC3;
	uint32_t dwordAC7;
	uint16_t wordACB;
	uint8_t byteACD;
	uint8_t byteACE;
	char gapACF;
	uint64_t qwordAD0;
	uint64_t qwordAD8;
	uint8_t byteAE0;
	char gapAE1[3];
	uint32_t dwordAE4;
	uint32_t dwordAE8;
	uint32_t dwordAEC;
	uint32_t dwordAF0;
	char gapAF4[4];
	uint32_t dwordAF8;
	char gapAFC[20];
	uint32_t dwordB10;
	uint32_t dwordB14;
	char gapB18[24];
	uint32_t dwordB30;
	uint64_t qwordB34;
	char gapB3C[4];
	uint32_t dwordB40;
	uint64_t qwordB44;
	char gapB4C[4];
	uint16_t owordB50;
	char gapB52[14];
	uint16_t owordB60;
	char gapB62[14];
	uint32_t dwordB70;
	uint32_t dwordB74;
	uint64_t qwordB78;
	uint32_t dwordB80;
	uint32_t dwordB84;
	uint32_t dwordB88;
	uint32_t dwordB8C;
	uint32_t dwordB90;
	uint32_t dwordB94;
	uint32_t dwordB98;
	uint32_t dwordB9C;
	uint32_t dwordBA0;
	uint64_t qwordBA4;
	char gapBAC[4];
	uint32_t dwordBB0;
	uint64_t qwordBB4;
	char gapBBC[20];
	uint64_t qwordBD0;
	char gapBD8[16];
	uint32_t dwordBE8;
	char gapBEC[4];
	uint64_t qwordBF0;
	uint32_t dwordBF8;
	uint64_t qwordBFC;
	uint16_t wordC04;
	uint8_t byteC06;
	char gapC07;
	uint64_t qwordC08;
	uint32_t dwordC10;
	uint32_t dwordC14;
	uint16_t wordC18;
	char gapC1A[2];
	uint64_t qwordC1C;
	uint64_t qwordC24;
	uint64_t qwordC2C;
	uint64_t qwordC34;
	char gapC3C[4];
	uint64_t qwordC40;
	uint64_t qwordC48;
	uint32_t dwordC50;
	char gapC54[4];
	uint32_t dwordC58;
	float m_gravity; // 0xC5C - might be start of some sub-class
	char gapC60[8];
	class CPed* m_driver; //0x0C68
	class CPed* m_passengers[15]; //0x0C70
	class CPed* m_last_driver; //0x0CE8
	char gapCF0[1424]; // end of sub-class
	uint32_t dword1280;
	char gap1284[4];
	uint32_t dword1288;
	char gap128C[12];
	uint16_t word1298;
	uint8_t byte129A;
	char gap129B;
	uint64_t qword129C;
	uint64_t qword12A4;
	uint64_t qword12AC;
	uint64_t qword12B4;
	char gap12BC[4];
	uint64_t qword12C0;
	uint64_t qword12C8;
	uint16_t word12D0;
	char gap12D2[14];
	uint32_t dword12E0;
	uint32_t dword12E4;
	uint64_t qword12E8;
	uint32_t dword12F0;
	char gap12F4[4];
	uint64_t qword12F8;
	char gap1300[56];
	uint64_t qword1338;
	uint64_t qword1340;
	uint32_t dword1348;
	uint32_t dword134C;
	uint32_t dword1350;
	uint64_t qword1354;
	uint32_t dword135C;
	uint32_t dword1360;
	uint32_t dword1364;
	uint32_t dword1368;
	char gap136C[4];
	float float1370;
	float float1374;
	char gap1378[8];
	uint64_t qword1380;
	uint64_t qword1388;
	uint32_t dword1390;
	uint64_t qword1394;
	uint16_t word139C;
	uint16_t word139E;
	uint8_t byte13A0;
	uint16_t word13A1;
	char gap13A3[2];
	uint8_t byte13A5;
	uint16_t word13A6;
	uint8_t byte13A8;
	char gap13A9[3];
	uint32_t dword13AC;
	uint8_t byte13B0;
	char gap13B1[3];
	uint64_t qword13B4;
	uint32_t dword13BC;
	uint32_t dword13C0;
	uint32_t dword13C4;
	uint32_t dword13C8;
	uint8_t byte13CC;
	char gap13CD[3];
	uint64_t qword13D0;
	uint8_t byte13D8;
	char gap13D9[3];
	uint32_t dword13DC;
	char gap13E0[8];
	uint32_t dword13E8;
	char gap13EC[116];
	uint32_t dword1460;
	uint16_t word1464;
	uint8_t byte1466;
	char gap1467;
	uint64_t qword1468;

	CVehicleModelInfo* get_model_info() {
		return reinterpret_cast<CVehicleModelInfo*>(m_model_info);
	}
}; //Size: 0x1450
static_assert(sizeof(CVehicle) == 0x1450);
#pragma pack(pop)
class CItemInfo {
public:
	char pad_0000[16]; //0x0000
	uint32_t m_name; //0x0010
	uint32_t m_model; //0x0014
	uint32_t m_audio; //0x0018
	uint32_t m_slot; //0x001C
}; //Size: 0x0020
static_assert(sizeof(CItemInfo) == 0x20);
class CAmmoInfo : public CItemInfo {
public:
	int32_t m_ammo_max; //0x0020
	int32_t m_ammo_max_50; //0x0024
	int32_t m_ammo_max_100; //0x0028
	int32_t m_ammo_max_mp; //0x002C
	int32_t m_ammo_max_50_mp; //0x0030
	int32_t m_ammo_max_100_mp; //0x0034
	uint32_t m_ammo_flags; //0x0038
	uint32_t m_ammo_special_type; //0x003C
	char pad_0040[24]; //0x0040
	float m_missile_speed; //0x0058
	char pad_005C[284]; //0x005C
	uint32_t m_lock_on_time; //0x0178
}; //Size: 0x017C
static_assert(sizeof(CAmmoInfo) == 0x17C);
class CAimingInfo {
public:
	uint32_t m_name; //0x0000
	float m_heading_limit; //0x0004
	float m_sweep_pitch_min; //0x0008
	float m_sweep_pitch_max; //0x000C
}; //Size: 0x0010
static_assert(sizeof(CAimingInfo) == 0x10);
class CForceInfo {
public:
	int32_t m_bone_tag; //0x0000
	float m_front; //0x0004
	float m_back; //0x0008
}; //Size: 0x000C
static_assert(sizeof(CForceInfo) == 0xC);
class CWeaponInfo : public CItemInfo {
public:
	enum eDamageType m_damage_type; //0x0020
	char pad_0024[48]; //0x0024
	enum eFireType m_fire_type; //0x0054
	enum eWheelSlot m_wheel_slot; //0x0058
	uint32_t m_group; //0x005C
	class CAmmoInfo* m_ammo_info; //0x0060
	class CAimingInfo* m_aiming_info; //0x0068
	uint32_t m_clip_size; //0x0070
	float m_accuracy_spread; //0x0074
	float m_accurate_mode_accuracy_modifier; //0x0078
	float m_run_and_gun_accuracy; //0x007C
	float m_run_and_gun_min_accuracy; //0x0080
	float m_recoil_accuracy_max; //0x0084
	float m_recoil_error_time; //0x0088
	float m_recoil_recovery_rate; //0x008C
	float m_recoil_accuracy_to_allow_headshot_ai; //0x0090
	float m_min_headshot_distance_ai; //0x0094
	float m_max_headshot_distance_ai; //0x0098
	float m_headshot_damage_modifier_ai; //0x009C
	float m_recoil_accuracy_to_allow_headshot_player; //0x00A0
	float m_min_headshot_distance_player; //0x00A4
	float m_max_headshot_distance_player; //0x00A8
	float m_headshot_damage_modifier_player; //0x00AC
	float m_damage; //0x00B0
	float m_damage_time; //0x00B4
	float m_damage_time_in_vehicle; //0x00B8
	float m_damage_time_in_vehicle_headshot; //0x00BC
	float N000008F9; //0x00C0
	uint32_t N00000898; //0x00C4
	float m_hit_limbs_damage_modifier; //0x00C8
	float m_network_hit_limbs_damage_modifier; //0x00CC
	float m_lightly_armoured_damage_modifier; //0x00D0
	float m_vehicle_damage_modifier; //0x00D4
	float m_force; //0x00D8
	float m_force_on_ped; //0x00DC
	float m_force_on_vehicle; //0x00E0
	float m_force_on_heli; //0x00E4
	char pad_00E8[16]; //0x00E8
	float m_force_max_strength_mult; //0x00F8
	float m_force_falloff_range_start; //0x00FC
	float m_force_falloff_range_end; //0x0100
	float m_force_falloff_range_min; //0x0104
	float m_project_force; //0x0108
	float m_frag_impulse; //0x010C
	float m_penetration; //0x0110
	float m_vertical_launch_adjustment; //0x0114
	float m_drop_forward_velocity; //0x0118
	float m_speed; //0x011C
	uint32_t m_bullets_in_batch; //0x0120
	float m_batch_spread; //0x0124
	float m_reload_time_mp; //0x0128
	float m_reload_time_sp; //0x012C
	float m_vehicle_reload_time; //0x0130
	float m_anim_reload_time; //0x0134
	int32_t m_bullets_per_anime_loop; //0x0138
	float m_time_between_shots; //0x013C
	float m_time_left_between_shots_where_should_fire_is_cached; //0x0140
	float m_spinup_time; //0x0144
	float m_spin_time; //0x0148
	float m_spindown_time; //0x014C
	float m_alternate_wait_time; //0x0150
	char pad_0154[296]; //0x0154
	float m_network_player_damage_modifier; //0x027C
	float m_network_ped_damage_modifier; //0x0280
	float m_network_headshot_modifier; //0x0284
	float m_lock_on_range; //0x0288
	float m_weapon_range; //0x028C
	char pad_0290[8]; //0x0290
	float m_damage_fall_off_range_min; //0x0298
	float m_damage_fall_off_range_max; //0x029C
	float m_damage_fall_off_modifier; //0x02A0
	char pad_02A4[64]; //0x02A4
	uint32_t m_recoil_shake_hash; //0x02E4
	uint32_t m_recoil_shake_hash_first_person; //0x02E8
	float m_min_time_between_recoil_shakes; //0x02EC
	float m_recoil_shake_amplitude; //0x02F0
	float m_explosion_shake_amplitude; //0x02F4
}; //Size: 0x02F8
static_assert(sizeof(CWeaponInfo) == 0x2F8);
class CPedWeaponManager {
public:
	char pad_0000[16]; //0x0000
	class CPed* m_owner; //0x0010
	uint32_t m_selected_weapon_hash; //0x0018
	char pad_001C[4]; //0x001C
	class CWeaponInfo* m_weapon_info; //0x0020
	char pad_0028[72]; //0x0028
	class CWeaponInfo* m_vehicle_weapon_info; //0x0070
	class CObject* m_weapon_object; //0x0078
}; //Size: 0x0080
static_assert(sizeof(CPedWeaponManager) == 0x80);
class CPedFactory {
public:
	enum class PedCreateFlags {
		IS_NETWORKED = (1 << 0),
		IS_PLAYER = (1 << 1)
	};
	virtual ~CPedFactory() = default;
	virtual CPed* CreatePed(std::uint8_t* flags, std::uint16_t* model_index, rage::matrix44* matrix, bool default_component_variation, bool register_network_object, bool give_default_loadout, bool, bool) = 0; // 0x08
	virtual CPed* CreateClone(std::uint8_t* flags, std::uint16_t* model_index, rage::matrix44* matrix, bool default_component_variation, bool, bool register_network_object, bool) = 0;                          // 0x10
	virtual CPed* ClonePed(CPed* ped, bool register_network_object, bool link_blends, bool clone_compressed_damage) = 0;                                                                                          // 0x18
	virtual CPed* ClonePedToTarget(CPed* source, CPed* target, bool clone_compressed_damage) = 0;                                                                                                                 // 0x20
	virtual CPed* CreatePlayer(std::uint8_t* flags, std::uint16_t model_index, rage::matrix44* matrix, CPlayerInfo* player_info) = 0;                                                                            // 0x28
	virtual void DestroyPed(CPed* ped) = 0;                                                                                                                                                                       // 0x30

	class CPed* m_local_ped; //0x0008
}; //Size: 0x0010
static_assert(sizeof(CPedFactory) == 0x10);
class CPedModelInfo {
public:
	char pad_0000[24]; //0x0000
	uint32_t m_model_hash; //0x0018
}; //Size: 0x001C
static_assert(sizeof(CPedModelInfo) == 0x1C);
class CPedBoneInfo {
public:
	rage::vector3 m_model_coords; //0x0000
	char pad_000C[4]; //0x000C
}; //Size: 0x0010
static_assert(sizeof(CPedBoneInfo) == 0x10);
class CPedIntelligence {
public:
	char pad_0000[632]; //0x0000
	float m_oxygen_time; //0x0278
}; //Size: 0x027C
static_assert(sizeof(CPedIntelligence) == 0x27C);

#pragma pack(push, 8)
class CPedInventory : rage::atRTTI<CPedInventory> {
public:
	uint64_t unk_0008; //0x0008
	class CPed* m_ped; //0x0010
	uint64_t unk_0018; //0x0018
	uint32_t unk_0020; //0x0020
	uint64_t unk_0028; //0x0028
	uint64_t unk_0030; //0x0030
	uint32_t unk_0038; //0x0038
	char pad_003C[4]; //0x003C
	uint8_t unk_0040; //0x0040
	char pad_0041[7]; //0x0041
	uint64_t unk_0048; //0x0048
	uint32_t unk_0050; //0x0050
	uint64_t unk_0058; //0x0058
	uint64_t unk_0060; //0x0060
	uint32_t unk_0068; //0x0068
	char pad_006C[4]; //0x006C
	uint8_t unk_0070; //0x0070
	char pad_0071[7]; //0x0071
	bool m_infinite_ammo : 1; //0x0078
	bool m_infinite_clip : 1; //0x0078
	char pad_0079[7]; //0x0079
	uint64_t unk_0080; //0x0080
}; //Size: 0x0088
static_assert(sizeof(CPedInventory) == 0x88);
#pragma pack(pop)
#pragma pack(push, 1)
class CPed : public rage::CPhysical {
public:
	char gap2EC[20]; //0x0000
	rage::vector3 m_velocity; //0x0300
	char pad_030C[260]; //0x030C
	class CPedBoneInfo m_bone_info[9]; //0x0410
	char pad_04A0[2160]; //0x04A0
	class CVehicle* m_vehicle; //0x0D10
	char pad_0D18[896]; //0x0D18
	uint32_t m_ped_type; //0x1098
	char pad_109C[4]; //0x109C
	class CPedIntelligence* m_ped_intelligence; //0x10A0
	class CPlayerInfo* m_player_info; //0x10A8
	class CPedInventory* m_inventory; //0x10B0
	class CPedWeaponManager* m_weapon_manager; //0x10B8
	char pad_10C0[892]; //0x10C0
	uint8_t m_seatbelt; //0x143C
	char pad_143D[13]; //0x143D
	uint8_t m_can_switch_weapon; //0x144A
	uint8_t m_ped_task_flag; //0x144B
	char pad_144C[4]; //0x144C
	uint8_t m_forced_aim; //0x1450
	char pad_1451[187]; //0x1451
	float m_armor; //0x150C
	float unk_health_threshold; //0x1510
	float m_fatigued_health_threshold; //0x1514
	float m_injured_health_threshold; //0x1518
	float m_dying_health_threshold; //0x151C
	float m_hurt_health_threshold; //0x1520
	char pad_1524[240]; //0x1524
	uint16_t m_cash; //0x1614
	char pad_1616[842]; //0x1616
	uint8_t m_fired_sticky_bombs; //0x1960
	uint8_t m_fired_unk; //0x1961
	uint8_t m_fired_flares; //0x1962
	uint8_t m_fired_unk_2; //0x1963

	float get_speed() {
		return sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y + m_velocity.z * m_velocity.z);
	}
	rage::vector3 get_bone_coords(ePedBoneType type) {
		rage::vector3 world_coords;
		model_to_world(m_bone_info[(uint32_t)type].m_model_coords, world_coords);
		return world_coords;
	}
	bool is_current_task(ePedTask flag) {
		return m_ped_task_flag & (int)flag;
	}
	bool on_foot() {
		return is_current_task(ePedTask::Foot) && !is_current_task(ePedTask::Driving);
	}
	bool can_be_ragdolled() {
		return m_ped_type & (int)ePedType::Ragdogable;
	}
	uint32_t get_ped_type() {
		return m_ped_type << 11 >> 25;
	}
	bool has_seatbelt() {
		return m_seatbelt & 0x3;
	}
	void forced_aim(bool toggle) {
		m_forced_aim ^= (m_forced_aim ^ -(char)toggle) & 0x20;
	}
	void set_damage_bit(eEntityProofs flag, bool set) {
		set ? m_damage_bits |= (uint32_t)flag : m_damage_bits &= ~(uint32_t)flag;
	}
}; //Size: 0x1964
static_assert(sizeof(CPed) == 0x1964);
#pragma pack(pop)
#pragma pack(push, 4)
class CPlayerInfo {
public:
	char pad_0000[32]; //0x0000
	class rage::rlGamerInfo m_gamer_info; //0x0020
	char pad_0118[184]; //0x0118
	float m_swim_speed; //0x01D0
	char pad_01D4[20]; //0x1D4
	uint32_t m_water_proof; //0x01E8
	char pad_01EC[92]; //0x01EC
	class CPed* m_ped; //0x0248
	char pad_0250[40]; //0x0250
	uint32_t m_frame_flags; //0x0278
	char pad_027C[52]; //0x027C
	uint32_t m_player_controls; //0x02B0
	char pad_02B4[1256]; //0x02B4
	float m_wanted_can_change; //0x079C
	char pad_0740[304]; //0x07A0
	uint32_t m_npc_ignore; //0x08D0
	char pad_0874[12]; //0x08D4
	bool m_is_wanted; //0x08E0
	char pad_08E1[7]; //0x08E1
	uint32_t m_wanted_level; //0x08E8
	uint32_t m_wanted_level_display; //0x08EC
	char pad_08F0[1120]; //0x08F0
	float m_run_speed; //0x0D50
	float m_stamina; //0x0D54
	float m_stamina_regen; //0x0D58
	char pad_0D5C[16]; //0x0D5C
	float m_weapon_damage_mult; //0x0D6C
	float m_weapon_defence_mult; //0x0D70
	char pad_0D74[4]; //0x0D74
	float m_melee_weapon_damage_mult; //0x0D78
	float m_melee_damage_mult; //0x0D7C
	float m_melee_defence_mult; //0x0D80
	char pad_0D84[8]; //0x0D84
	float m_melee_weapon_defence_mult; //0x0D8C
}; //Size: 0x0D90
static_assert(sizeof(CPlayerInfo) == 0xD90);
#pragma pack(pop)
#pragma pack(push, 1)
class ClanData {
public:
	int64_t m_clan_member_id; //0x0000
	int64_t m_clan_id; //0x0008
	int32_t m_clan_color; //0x0010
	int32_t m_clan_member_count; //0x0014
	int32_t m_clan_created_time; //0x0018
	bool m_is_system_clan; //0x001C
	bool m_is_clan_open; //0x001D
	char m_clan_name[25]; //0x001E
	char m_clan_tag[5]; //0x0037
	char m_clan_motto[65]; //0x003C
	char pad_007D[3]; //0x007D
	int64_t m_clan_id_2; //0x0080
	char m_clan_rank_name[25]; //0x0088
	char pad_00A1[3]; //0x00A1
	int32_t m_clan_rank_order; //0x00A4
	int64_t m_clan_rank_flags; //0x00A8
	char unk_00B0[8]; //0x00B0
}; //Size: 0x00B8
static_assert(sizeof(ClanData) == 0xB8);
#pragma pack(pop)
#pragma pack(push, 8)
class CNetGamePlayer : public rage::netPlayer {
public:
	class CPlayerInfo* m_player_info; //0x00A0
	uint32_t m_matchmaking_group; //0x0008
	bool m_is_spectating; //0x000C
	char pad_00AD[3]; //0x000AD
	uint64_t unk_00B0; //0x00B0
	char unk_00B8; //0x00B8
	char pad_00B9[3]; //0x00B9
	uint32_t unk_00BC; //0x00BC
	uint32_t unk_00C0; //0x00C0
	char pad_00C4[4]; //0x00C4
	class ClanData m_clan_data; //0x00C8
	char m_crew_rank_title[25]; //0x0180
	bool m_is_rockstar_dev; //0x0199
	bool m_is_rockstar_qa; //0x019A
	bool m_is_cheater; //0x019B
	uint32_t unk_019C; //0x019C
	uint16_t unk_01A0; //0x01A0
	char unk_01A2; //0x01A2
	char pad_01A3; //0x01A3
	uint32_t m_phone_explosion_vehicle_net_id; //0x01A4
	uint16_t unk_01A8; //0x01A8
	bool m_has_started_transition; //0x01AA
	char pad_01AB[5]; //0x01AB
	class rage::rlSessionInfo m_transition_session_info; //0x01A3
	char pad_0273[16]; //0x0273
	uint64_t unk_0283; //0x0283
	uint64_t unk_028B; //0x028B
	uint32_t m_mute_count; //0x0293
	uint32_t m_mute_talkers_count; //0x0297
	char pad_029B[5]; //0x029B
	bool m_have_communication_privileges; //0x02A0
	uint16_t unk_02A1; //0x02A1
	uint16_t unk_02A3; //0x02A3
	char pad_02A5[2]; //0x02A5
	uint32_t m_cheat_report_ids[20]; //0x02A7
	uint32_t m_num_cheat_reports; //0x02F7
	uint8_t unk_02FB; //0x02FB
	char pad_02FC[3]; //0x02FC
	uint32_t unk_02FF; //0x02FF
	char unk_0303; //0x0303
	char pad_0304[3]; //0x0304
	uint32_t unk_0307; //0x0307
	uint32_t m_account_id; //0x030B
	uint32_t unk_030F; //0x030F
}; //Size: 0x0320
static_assert(sizeof(CNetGamePlayer) == 0x320);
#pragma pack(pop)
#pragma pack(push, 2)
class CNetworkPlayerMgr : public rage::netPlayerMgrBase {
public:
	class CNetGamePlayer m_net_players[32]; //0x08E0
	uint64_t unk_60E0; //0x60E0
	uint64_t unk_60E8; //0x60E8
	uint64_t unk_60F0; //0x60F0
	uint64_t unk_60F8; //0x60F8
	class CNetGamePlayer m_net_players_2[32]; //0x6100
	uint64_t unk_B900; //0xB900
	uint64_t unk_B908; //0xB908
	uint64_t unk_B910; //0xB910
	uint64_t unk_B918; //0xB918
	uint64_t unk_B920; //0xB920
	uint64_t unk_B928; //0xB928
	uint64_t unk_B930; //0xB930
	uint32_t unk_B938; //0xB938
	char pad_B93C[3]; //0xB93C
	bool unk_B93F; //0xB93F
	uint32_t unk_B940; //0xB940
	uint32_t unk_B944; //0xB944
	uint16_t unk_B948; //0xB948
}; //Size: 0xB94A
static_assert(sizeof(CNetworkPlayerMgr) == 0xD14A);
#pragma pack(pop)
class GtaThread : public rage::scrThread {
public:
	uint32_t m_script_hash; //0x0128
	uint32_t m_pointer_count_cleanup; //0x012C
	uint32_t m_unk_130; //0x0130
	uint32_t m_unk_134; //0x0134
	uint32_t m_unk_138; //0x0138
	int32_t m_unk_13C; //0x013C
	int32_t m_instance_id; //0x0140
	uint32_t m_base_instance_id; //0x0144
	uint8_t m_flag; //0x0148
	bool m_safe_for_network_game; //0x0149
	bool m_allow_host_broadcast; //0x014A
	bool m_unk_14B; //0x014A
	bool m_is_minigame_script; //0x014C
	char pad_014D[2]; //0x014D
	bool m_can_be_paused; //0x014F
	bool m_can_remove_blips_from_other_scripts; //0x0150
	bool unk_0151; //0x0151
	bool m_allow_attachment; //0x0152
	bool m_detached; //0x0153
	uint8_t m_flag_2; //0x0154
	char pad_0155[11]; //0x0155
};
static_assert(sizeof(GtaThread) == 0x160);
class CNetworkObjectMgr : public rage::netObjectMgrBase {};
#pragma pack(push, 1)
class HashNode {
public:
	int32_t m_hash; //0x0000
	uint16_t m_idx; //0x0004
	char pad_0006[2]; //0x0006
	HashNode* m_next; //0x0008
}; //Size: 0x0010
static_assert(sizeof(HashNode) == 0x10);

template <typename T>
class HashTable {
public:
	T* m_data; //0x0000
	uint16_t m_size; //0x0008
	char pad_000A[14]; //0x000A
	uint64_t m_item_size; //0x0018
	char pad_0020[64]; //0x0020
	HashNode** m_lookup_table; //0x0060
	uint16_t m_lookup_key; //0x0068
}; //Size: 0x006A
static_assert(sizeof(HashTable<void*>) == 0x6A);
#pragma pack(pop)

class CPickup {
public:
	char pad_0000[48]; //0x0000
	class CNavigation* m_navigation; //0x0030
	char pad_0038[88]; //0x0083
	rage::vector3 m_position; //0x0090
	char pad_009C[1012]; //0x009C
	int32_t m_money; //0x0490
	char pad_0484[260]; //0x0484
};//Size: 0x0588
class CPickupHandle {
public:
	CPickup* m_pointer; //0x0000 
	int32_t m_handle; //0x0008 
	char pad_0x000C[4]; //0x000C
}; //Size: 0x0010
class CPickupList {
public:
	CPickupHandle m_array[73]; //0x0000 
	uintptr_t addr(size_t index) {
		uintptr_t addr = (uintptr_t)static_cast<void*>(this) + (index * 0x10);
		return *(uintptr_t*)addr;
	}
}; //Size=0x0490
class CPickupInterface {
public:
	char pad_0x0000[0x100]; //0x0000
	CPickupList* m_list; //0x0100 
	int32_t m_size; //0x0108 
	int32_t m_count; //0x0110 
	CPickup* get(const size_t& index) {
		if (index < m_size)
			return m_list->m_array[index].m_pointer;
		return nullptr;
	}
}; //Size: 0x0114
class CObjectHandle {
public:
	CObject* m_pointer; //0x0000
	int32_t m_handle; //0x0008
	char pad_000C[4]; //0x000C
}; //Size: 0x0010
static_assert(sizeof(CObjectHandle) == 0x10);
class CObjectList {
public:
	CObjectHandle m_array[2300]; //0x0000
	uintptr_t addr(size_t index) {
		uintptr_t addr = (uintptr_t)static_cast<void*>(this) + (index * 0x10);
		return *(uintptr_t*)addr;
	}
}; //Size: 0x8FC0
class CObjectInterface {
public:
	char pad_0000[344]; //0x0000
	CObjectList* m_list; //0x0158
	int32_t m_size; //0x0160
	char pad_0164[4]; //0x0164
	int32_t m_count; //0x0168
	CObject* get(const size_t& index) {
		if (index < m_size)
			return m_list->m_array[index].m_pointer;
		return nullptr;
	}
}; //Size: 0x016C
class CPedHandle {
public:
	CPed* m_pointer; //0x0000
	int32_t m_handle; //0x0008
	char pad_000C[4]; //0x000C
}; //Size: 0x0010
static_assert(sizeof(CPedHandle) == 0x10);
class CPedList {
public:
	CPedHandle m_array[256]; //0x0000
	uintptr_t addr(size_t index) {
		uintptr_t addr = (uintptr_t)static_cast<void*>(this) + (index * 0x10);
		return *(uintptr_t*)addr;
	}
}; //Size: 0x1000
class CPedInterface {
public:
	char pad_0000[256]; //0x0000
	CPedList* m_list; //0x0100
	int32_t m_size; //0x0108
	char pad_010C[4]; //0x010C
	int32_t m_count; //0x0110
	CPed* get(const size_t& index) {
		if (index < m_size)
			return m_list->m_array[index].m_pointer;
		return nullptr;
	}
}; //Size: 0x0114
class CVehicleHandle {
public:
	class CVehicle* m_pointer; //0x0000
	int32_t m_handle; //0x0008
	char pad_000C[4]; //0x000C
}; //Size: 0x0010
static_assert(sizeof(CVehicleHandle) == 0x10);
class CVehicleList {
public:
	CVehicleHandle m_array[300]; //0x0000
	uintptr_t addr(size_t index) {
		uintptr_t addr = (uintptr_t)static_cast<void*>(this) + (index * 0x10);
		return *(uintptr_t*)addr;
	}
}; //Size: 0x12C0
class CVehicleInterface {
public:
	char pad_0000[384]; //0x0000
	CVehicleList* m_list; //0x0180
	int32_t m_size; //0x0188
	char pad_018C[4]; //0x018C
	int32_t m_count; //0x0190
	CVehicle* get(const size_t& index) {
		if (index < m_size)
			return m_list->m_array[index].m_pointer;
		return nullptr;
	}
}; //Size: 0x0194
class CCameraInterface {
public:
	char pad_0x0000[1032]; //0x0000
}; //Size: 0x0408
class CReplayInterface {
public:
	char pad_0000[8]; //0x0000
	CCameraInterface* m_camera_interface; //0x0008
	CVehicleInterface* m_vehicle_interface; //0x0010
	CPedInterface* m_ped_interface; //0x0018
	CPickupInterface* m_pickup_interface; //0x0020 
	CObjectInterface* m_object_interface; //0x0028
	char pad_0030[16]; //0x0030
}; //Size: 0x0040

class CGameScriptHandlerMgr : public rage::scriptHandlerMgr {};

class CScriptedGameEvent : public rage::netGameEvent {
public:
	char pad_00030[64]; //0x0030
	int64_t m_args[54]; //0x0070
	uint32_t m_bitset; //0x0220
	uint32_t m_args_size; //0x0224
}; //Size: 0x0228
class CNetworkIncrementStatEvent : public rage::netGameEvent {
public:
	uint32_t m_stat; //0x0030
	uint32_t m_amount; //0x0034
}; //Size: 0x0038
#pragma pack(push, 1)
class ScInfo {
public:
	char m_ticket[208]; //0x0000
	char pad_00D0[304]; //0x00D0
	char m_session_ticket[88]; //0x0200
	char pad_0258[40]; //0x0258
	uint32_t m_nonce; //0x0280
	char pad_0284[4]; //0x0284
	uint32_t m_account_id; //0x0288
	char pad_028C[16]; //0x028C
	char m_profile_pic[128]; //0x029C
	char pad_031C[32]; //0x031C
	char m_country_code[4]; //0x033C
	char pad_0340[31]; //0x0340
	char m_email_address[96]; //0x035F
	char pad_03BF[6]; //0x03BF
	char m_language_subtag[8]; //0x03C5
	char pad_03CD[2]; //0x03CD
	char m_sc_name[20]; //0x03CF
	char pad_03E3[533]; //0x03E3
	char m_session_key[16]; //0x05F8
	char pad_0608[2296]; //0x0608
}; //Size: 0x0F00
static_assert(sizeof(ScInfo) == 0xF00);
#pragma pack(pop)
class ScGame {
public:
	char pad_0000[64]; //0x0000
	char* m_string; //0x0040
	char pad_0048[256]; //0x0048
};
static_assert(sizeof(ScGame) == 0x148);
class ScGameInfo {
public:
	virtual ~ScGameInfo() = default;
	virtual ScGame* GetGame() = 0;
	virtual void Unk0010() = 0;
	virtual void Unk0018() = 0;
	virtual int GetStringIndex(const char* StringId, u64 Unk, u32 GameId) = 0;
	char pad_0008[12]; //0x0008
	uint32_t m_id; //0x0014
	char pad_0018[8]; //0x0018
	ScGame m_games[10]; //0x0020
	u64 getGamesAddress() {
		return (u64)this + offsetof(ScGameInfo, m_games);
	}
}; //Size: 0x0028
static_assert(sizeof(ScGameInfo) == 0xCF0);
#pragma pack(push, 1)
class MetricSessionMigrated : public rage::rlMetric {
public:
	char pad_0008[828]; //0x0008
	uint32_t m_num_players; //0x0344
}; //Size: 0x0348
static_assert(sizeof(MetricSessionMigrated) == 0x348);
class NetworkGameConfig {
public:
	char pad_0000[48]; //0x0000
	uint32_t m_public_slots; //0x0030
	uint32_t m_private_slots; //0x0034
	char pad_0038[272]; //0x0038
}; //Size: 0x0148
static_assert(sizeof(NetworkGameConfig) == 0x148);
class NetworkGameFilterMatchmakingComponent {
public:
	uint32_t m_filter_type; //0x0000
	char m_filter_name[24]; //0x0004
	uint32_t m_num_parameters; //0x001C
	uint16_t m_game_mode; //0x0020
	uint16_t m_session_type; //0x0022
	uint32_t m_param_unk[8]; //0x0024
	char m_param_names[8][24]; //0x0044
	char pad_0104[4]; //0x0104
	uint32_t m_param_mappings[8]; //0x0108
	char pad_0128[352]; //0x0128
	uint32_t m_param_values[8]; //0x0288
	char pad_02A8[96]; //0x02A8
	uint32_t m_enabled_params_bitset; //0x0308
	char pad_030C[8]; //0x030C

	//Do not use for actual network filters, this will break things
	void SetParameter(const char* name, uint32_t index, uint32_t value) {
		strcpy_s(m_param_names[index], name);
		m_param_mappings[index] = index;
		m_param_values[index] = value;
		m_enabled_params_bitset |= (1 << index);
		if (m_num_parameters <= index)
			m_num_parameters++;
	}
}; //Size: 0x0314
static_assert(sizeof(NetworkGameFilterMatchmakingComponent) == 0x314);
class MatchmakingAttributes {
public:
	uint32_t m_game_mode; //0x0000
	uint32_t m_num_params; //0x0004
	uint32_t m_param_unk[8]; //0x0008
	char m_param_names[8][24]; //0x0028
	uint32_t m_param_values[8]; //0x00E8
	uint32_t m_params_bitset; //0x0108
}; //Size: 0x010C
static_assert(sizeof(MatchmakingAttributes) == 0x10C);
class NetworkGameFilter {
public:
	virtual ~NetworkGameFilter() = default;
	virtual void Reset() {};
public:
	uint32_t m_build_type; //0x0008
	uint32_t m_discriminator; //0x000C
	uint32_t m_discriminator_mapping; //0x0010
	uint32_t m_region_mapping; //0x0014
	uint32_t m_language_mapping; //0x0018
	uint32_t m_mm_group_1_mapping; //0x001C
	uint32_t m_mm_group_2_mapping; //0x0020
	uint32_t m_activity_type_mapping; //0x0024
	uint32_t m_activity_id_mapping; //0x0028
	uint32_t m_activity_players_mapping; //0x002C
	class NetworkGameFilterMatchmakingComponent m_matchmaking_component; //0x0030
}; //Size: 0x0344
static_assert(sizeof(NetworkGameFilter) == 0x344);
class SessionInfoBackup {
public:
	class rage::rlSessionInfo m_session_info; //0x0000
	uint32_t m_unk; //0x00D0
	char pad_0074[4]; //0x00D4
	uint32_t m_flags; //0x00D8
}; //Size: 0x00DC
static_assert(sizeof(SessionInfoBackup) == 0xDC);
class MatchmakingSessionResult {
public:
	class rage::rlSessionDetail m_detail; //0x0000
	char pad_0478[24]; //0x0478
}; //Size: 0x0490
static_assert(sizeof(MatchmakingSessionResult) == 0x490);
class CNetRemoteComplaint {
public:
	uint64_t m_complainer_token; //0x0000
	uint64_t m_complainee_token; //0x0008
	uint32_t m_flags; //0x0010
	uint32_t m_time; //0x0014
}; //Size: 0x0018
static_assert(sizeof(CNetRemoteComplaint) == 0x18);
class CNetComplaintMgr {
public:
	uint64_t m_peer_address; //0x0000
	uint32_t m_host_peer_id; //0x0008
	char pad_000C[4]; //0x000C
	class rage::netConnectionManager* m_net_connection_mgr; //0x0010
	char pad_0018[64]; //0x0018
	uint64_t m_peer_address_in_scope[64]; //0x0058
	uint32_t m_num_tokens_in_scope; //0x0258
	char pad_025C[4]; //0x025C
	class CNetRemoteComplaint m_remote_complaints[64]; //0x0260
	uint32_t m_num_remote_complaints; //0x0860
	char pad_0864[4]; //0x0864
	uint64_t m_peer_addresses_complained[64]; //0x0868
	uint32_t m_num_tokens_complained; //0x0A68
	char pad_0A6C[520]; //0x0A6C
	uint32_t m_connection_identifier; //0x0C74
	uint32_t m_last_resend_time; //0x0C78
	char pad_0C7C[4]; //0x0C7C
	uint32_t m_time_to_resend; //0x0C80
	uint32_t m_flags; //0x0C84
	char pad_0C88[16]; //0x0C88

	//I fucking hate snake case, but I would like to keep up with the theme of these classes kekw
	inline bool has_local_complaint(uint64_t peer_address) {
		for (uint32_t i{}; i != m_num_tokens_complained; ++i)
			if (m_peer_addresses_complained[i] == peer_address)
				return true;
		return false;
	}
	inline void raise_complaint(uint64_t peer_address, bool spoof_token) {
		if (has_local_complaint(peer_address))
			return;
		auto old_token = m_peer_address;
		if (m_peer_address == old_token && spoof_token)
			m_peer_address = peer_address;
		m_peer_addresses_complained[m_num_tokens_complained++] = peer_address;
		if (m_peer_address != old_token && spoof_token)
			m_peer_address = old_token;
	}
	inline uint32_t num_of_tokens_complainted() {
		return m_num_tokens_complained;
	}
	inline void remove_complaint(uint64_t host_token) {
		if (!has_local_complaint(host_token))
			return;
		for (uint32_t i{}; i != m_num_tokens_complained; ++i)
			if (m_peer_addresses_complained[i] == host_token)
				m_peer_addresses_complained[i] = m_peer_addresses_complained[m_num_tokens_complained - 1];
		m_num_tokens_complained--;
	}
	inline void remove_all_complaints() {
		for (uint32_t i{}; i != m_num_tokens_complained; ++i)
			m_peer_addresses_complained[i] = m_peer_addresses_complained[m_num_tokens_complained - 1];
		m_num_tokens_complained = 0;
	}
}; //Size: 0x0C98
static_assert(sizeof(CNetComplaintMgr) == 0xC98);
class PlayerNameMapNode {
public:
	char m_name[24]; //0x0000
	class rage::rlGamerHandle m_handle; //0x0018
	class PlayerNameMapNode* m_next; //0x0028
	class PlayerNameMapNode* m_prev; //0x0030
}; //Size: 0x0038
static_assert(sizeof(PlayerNameMapNode) == 0x38);
class JoiningPlayerNameMap {
public:
	class PlayerNameMapNode m_names[100]; //0x0000
	char pad_15E0[40]; //0x15E0
	uint32_t m_num_name_nodes; //0x1608
	char pad_160C[796]; //0x160C
}; //Size: 0x1928
static_assert(sizeof(JoiningPlayerNameMap) == 0x1928);
class CNetBlacklistNode : public rage::rlGamerHandle {
public:
	bool m_block_rejoin; //0x0010
	char pad_0011[3]; //0x0011
	uint32_t m_added_time; //0x0014
	class CNetBlacklistNode* m_next; //0x0018
	class CNetBlacklistNode* m_prev; //0x0020
}; //Size: 0x0028
static_assert(sizeof(CNetBlacklistNode) == 0x28);
class CNetBlacklist {
public:
	class CNetBlacklistNode m_nodes[16]; //0x0000
	class CNetBlacklistNode* m_head; //0x0280
	class CNetBlacklistNode* m_tail; //0x0288
	uint32_t m_free_nodes; //0x0290
	char pad_0294[4]; //0x0294
	class CNetBlacklistNode* m_start; //0x0298
	char pad_02A0[24]; //0x02A0
}; //Size: 0x02B8
static_assert(sizeof(CNetBlacklist) == 0x2B8);
class RemotePlayerData {
public:
	class rage::netGamePlayerData m_data[32]; //0x0000
	uint32_t m_count; //0x0600
	char pad_0604[4]; //0x0604
}; //Size: 0x0608
static_assert(sizeof(RemotePlayerData) == 0x608);
class InvitedGamer {
public:
	class rage::rlGamerHandle m_handle;
	char pad_0010[12]; //0x0010
	uint32_t m_flags; //0x001C
}; //Size: 0x0020
static_assert(sizeof(InvitedGamer) == 0x20);
class InvitedGamers {
public:
	class InvitedGamer m_invited_gamers[100]; //0x0000
	uint32_t m_num_invited_gamers; //0x0C80
	char pad_0C84[4]; //0x0C84
}; //Size: 0x0C88
static_assert(sizeof(InvitedGamers) == 0xC88);
class Network {
public:
	rage::rlSessionInfo m_steam_unk_session; //0x0000
	rage::Obf32 m_num_dinput8_instances; //0x0070
	rage::Obf32 m_last_time_dinput8_checked; //0x0080
	class rage::snSession* m_game_session_ptr; //0x0090
	class rage::snSession* m_transition_session_ptr; //0x0098
	char pad_00A0[24]; //0x00A0
	class rage::snSession m_game_session; //0x00B8
	class rage::snSession m_transition_session; //0x3F28
	char pad_7D98[16]; //0x7D98
	class NetworkGameConfig m_network_game_config; //0xAC48
	class NetworkGameConfig m_network_transition_config; //0xAD90
	bool m_session_attributes_dirty; //0xAED8
	char pad_AED9[19]; //0xAED9
	uint32_t m_session_visibility_flags; //0xAEEC
	uint32_t m_transition_visibility_flags; //0xAEF0
	char pad_AEF4[60]; //0xAEF4
	class MetricSessionMigrated m_metric_session_migrated; //0xAF30
	bool m_migrated_metric_enabled; //0xB278
	char pad_B279[3]; //0xB279
	uint32_t m_game_session_state; //0xB27C
	class NetworkGameFilter m_network_game_filter; //0xB280
	char pad_B5C4[33]; //0xB5C4
	bool m_was_invited; //0xB5E5
	char pad_B5E6[10]; //0xB5E6
	class rage::rlSessionInfo m_unk_session_info; //0xB5F0
	char pad_B6C0[635]; //0xB6C0
	bool m_need_host_change; //0xB93B
	char pad_B93C[2628]; //0xB93C
	class rage::rlSessionDetail m_joining_session_detail; //0xC380
	class SessionInfoBackup m_last_joined_session; //0xC7F8
	char pad_C8D4[40]; //0xC8D4
	uint32_t m_current_matchmaking_group; //0xC8FC
	uint32_t m_matchmaking_group_max_players[5]; //0xC900
	uint32_t m_num_active_matchmaking_groups; //0xC914
	char pad_C918[8]; //0xC918
	uint8_t m_matchmaking_property_id; //0xC920
	uint8_t m_matchmaking_mental_state; //0xC921
	char pad_C922[366]; //0xC922
	class rage::rlMatchmakingFindResult m_game_session_matchmaking[3]; //0xCA90
	char pad_195A0[40]; //0x195A0
	class MatchmakingSessionResult m_game_matchmaking_session_results[10]; //0x195C8
	char pad_1C368[308]; //0x1C368
	uint32_t m_num_bosses; //0x1C49C
	bool m_num_bosses_set; //0x1C4A0
	char pad_1C4A1[7]; //0x1C4A1
	class rage::rlGamerHandle m_transition_creator_handle; //0x1C4A8
	char pad_1C4B8[12]; //0x1C4B8
	bool m_is_waiting_async; //0x1C4C4
	bool m_is_preferred_activity; //0x1C4C5
	char pad_1C4C6[2]; //0x1C4C6
	uint32_t m_in_progress_finish_time; //0x1C4C8
	char pad_1C4CC[4]; //0x1C4CC
	bool m_local_player_info_dirty; //0x1C4D0
	char pad_1C4D1[495]; //0x1C4D1
	class rage::rlGamerHandle m_inviter_handle; //0x1C6C0
	class CNetComplaintMgr m_game_complaint_mgr; //0x1C6D0
	class CNetComplaintMgr m_transition_complaint_mgr; //0x1D368
	char pad_1E000[32]; //0x1E000
	class JoiningPlayerNameMap m_unused_joining_player_name_map; //0x1E020
	char pad_1F948[8]; //0x1F948
	class CNetBlacklist m_blacklist; //0x1F950
	char pad_1FC08[8]; //0x1FC08
	class InvitedGamers m_game_invited_gamers; //0x1FC10
	char pad_20898[4864]; //0x20898
	class SessionInfoBackup m_last_joined_transition; //0x21B98
	uint32_t m_activity_max_players; //0x21C74
	uint32_t m_activity_max_spectators; //0x21C78
	char pad_21C7C[48]; //0x21C7C
	bool m_do_not_launch_from_join_as_migrated_host; //0x21CAC
	char pad_21CAD[7]; //0x21CAD
	bool m_is_activity_session; //0x21CB4
	char pad_21CB5[35]; //0x21CB5
	class RemotePlayerData m_remote_player_data; //0x21CD8
	char pad_222E0[8]; //0x222E0
	class rage::netGamePlayerData m_local_net_game_player_data; //0x222E8
	char pad_22318[608]; //0x22318
	class rage::rlMatchmakingFindResult m_transition_matchmaking[4]; //0x22578
	class NetworkGameFilter m_transition_filters[4]; //0x33438
	char pad_34148[20]; //0x34148
	uint32_t m_transition_quickmatch_group_handle_count; //0x3415C
	class rage::rlGamerHandle m_transition_quickmatch_group_handles[32]; //0x34160
	bool m_retain_activity_group; //0x34360
	char pad_34361[7]; //0x34361
	class rage::rlSessionInfo m_transition_to_activity_session_info; //0x34368
	char pad_34438[48]; //0x34438
	class MatchmakingSessionResult m_transition_matchmaking_session_results[10]; //0x34468
	char pad_37208[8]; //0x37208
	class InvitedGamers m_transition_invited_gamers; //0x37210
	char pad_37E98[16]; //0x37E98
	class rage::rlGamerHandle m_transition_to_game_handle; //0x37EA8
	class rage::rlSessionInfo m_transition_to_game_session_info; //0x37EB8
	char pad_37F88[4]; //0x37F88
	uint32_t m_transition_to_game_session_participant_count; //0x37F8C
	class rage::rlGamerHandle m_transition_to_game_session_participants[32]; //0x37F90
	char pad_38190[72]; //0x38190
	class rage::rlGamerHandle m_follower_handles[32]; //0x381D8
	uint32_t m_follower_count; //0x383D8
	char pad_383DC[628]; //0x383DC
}; //Size: 0x38650
static_assert(sizeof(Network) == 0x38650);
#pragma pack(pop)