#pragma once
#include "framework.h"
#include "joaat.h"
#include <cassert>
#define LODWORD(x)  (*((DWORD*)&(x)))
#define HIDWORD(x)  (*((DWORD*)&(x)+1))
#define SLODWORD(x) (*((int32_t*)&(x)))
#define SHIDWORD(x) (*((int32_t*)&(x)+1))
#define DEFINE_AT_RTTI(T) private: \
	virtual T* GetIdentifier() { return nullptr; }; \
	virtual T* GetIdentifier_2() { return nullptr; }; \
	virtual uint32_t GetTypeHash() { return NULL; }; \
	virtual T* GetIfIsOfType(T* vft) { return vft; }; \
	virtual void IsOfType(T* vft) {}; \
	virtual void IsOfTypeObject(T* object) {}; \
	public:

enum class eVehicleType : uint32_t {
	Car,
	Plane,
	Unk2,
	Quadbike,
	Unk4,
	Unk5,
	AmphibiousCar,
	AmphibiousQuadbike,
	Heli,
	Unk9,
	Unk10,
	Bike,
	Bicycle,
	Boat,
	Train
};
enum class ePedBoneType {
	Head,
	LeftFoot,
	RightFoot,
	LeftAnkle,
	RightAnkle,
	LeftHand,
	RightHand,
	Neck,
	Abdomen
};
enum class ePedTask {
	None,
	Foot = 1 << 4,
	Unk = 1 << 5,
	Driving = 1 << 6
};
enum class eEntityFlags : uint32_t {
	Visible = 1 << 0
};
enum class eAbilityFlags : uint32_t {
	Parachute = (1 << 0),
};
enum class ePedType : uint32_t {
	Ragdogable = (1 << 5)
};
enum class eTaskFlags : uint32_t {
	Unk10 = (1 << 10),
	Unk11 = (1 << 11),
	Unk12 = (1 << 12),
	SuperJump = (1 << 15),
	LandOnJump = (1 << 16),
	BeastJump = (1 << 17),
	BeastJumpWithSuper = SuperJump | BeastJump,
	GracefulLanding = Unk10 | Unk11 | Unk12 | LandOnJump
};
enum eFrameFlags : uint32_t {
	eFrameFlagExplosiveAmmo = 1 << 11,
	eFrameFlagFireAmmo = 1 << 12,
	eFrameFlagExplosiveMelee = 1 << 13,
	eFrameFlagSuperJump = 1 << 14,
};
enum class eEntityProofs : uint32_t {
	Bullet = 1 << 4,
	Fire = 1 << 5,
	Collision = 1 << 6,
	Melee = 1 << 7,
	God = 1 << 8,
	Explosion = 1 << 11,
	Steam = 1 << 15,
	Drown = 1 << 16,
	Water = 1 << 24,
};
enum class eAmmoSpecialType : int32_t {
	Invalid = -1,
	None,
	ArmorPiercing,
	Explosive,
	FMJ,
	HollowPoint,
	Incendiary,
	Tracer
};
enum class eDamageType : int32_t {
	Unknown,
	None,
	Melee,
	Bullet,
	_0xC5403EC0,
	Explosive,
	Fire,
	_0xA869C908,
	Fall,
	_0xCAE075C0,
	Electric,
	BarbedWire,
	FireExtinguisher,
	Smoke,
	WaterCannon,
	_0x1C8E59AE
};
enum class eFireType : int32_t {
	None,
	Melee,
	InstantHit,
	DelayedHit,
	ProjectTile,
	VolumetricParticle
};
enum class eWheelSlot : int32_t {
	Pistol,
	SMG,
	Rifle,
	Sniper,
	UnarmedMelee,
	ShotGun,
	Heavy,
	Throwable
};
enum class eGameState : int32_t {
	Invalid = -1,
	Playing,
	Died,
	Arrested,
	FailedMission,
	LeftGame,
	Respawn,
	InMPCutscene
};
class CNavigation;
class CObject;
class Vector2;
class Vector3;
class Vector4;
class FriendInfo;
class FriendList;
class FriendRegistry;
class CNonPhysicalPlayerData;
class CHandlingData;
class CVehicleModelInfo;
class CVehicle;
class CItemInfo;
class CAmmoInfo;
class CAimingInfo;
class CForceInfo;
class CWeaponInfo;
class CPedWeaponManager;
class CPedFactory;
class CPedModelInfo;
class CPedBoneInfo;
class CPedIntelligence;
class CPedInventory;
class CPed;
class CPlayerInfo;
class ClanData;
class CNetGamePlayer;
class CNetworkPlayerMgr;
class GtaThread;
class CNetworkObjectMgr;
class HashNode;
template <typename T>
class HashTable;
class CPickup;
class CPickupHandle;
class CPickupList;
class CPickupInterface;
class CObject;
class CObjectHandle;
class CObjectList;
class CObjectInterface;
class CPedHandle;
class CPedList;
class CPedInterface;
class CVehicleHandle;
class CVehicleList;
class CVehicleInterface;
class CReplayInterface;
class CGameScriptHandlerMgr;
class CScriptedGameEvent;
class CNetworkIncrementStatEvent;
class ScInfo;
class ScGame;
class ScGameInfo;
class MetricSessionMigrated;
class NetworkGameConfig;
class NetworkGameFilterMatchmakingComponent;
class MatchmakingAttributes;
class NetworkGameFilter;
class SessionInfoBackup;
class MatchmakingSessionResult;
class CNetRemoteComplaint;
class CNetComplaintMgr;
class PlayerNameMapNode;
class JoiningPlayerNameMap;
class CNetBlacklistNode;
class CNetBlacklist;
class RemotePlayerData;
class InvitedGamer;
class InvitedGamers;
class Network;
namespace rage {
	template <typename T>
	class atRTTI;
	struct vector2;
	struct vector3;
	struct vector4;
	union matrix34;
	union matrix44;
	class scrVector;
	class netLoggingInterface;
	class nonPhysicalPlayerDataBase;
	union netAddress;
	class netSocketAddress;
	class rlGamerHandle;
	class rlPeerInfo;
	class rlGamerInfo;
	class netPlayer;
	class netPlayerMgrBase;
	class fwDrawData;
	class pgBase;
	class datBase;
	template <typename T, typename Base = datBase>
	class atDNode;
	template <typename Node>
	class atDList;
	template <typename T>
	class fwRefAwareBaseImpl;
	class fwRefAwareBase;
	class fwExtension;
	class fwExtensionContainer;
	class fwExtensibleBase;
	class CBaseModelInfo;
	class fwDynamicEntityComponent;
	class crmtRequestPose;
	class crmtRequestIk;
	class crFrameFilter;
	class fwAudEntity;
	class fwEntity;
	class datBitBuffer;
	class netGameEvent;
	template <typename T>
	class atArray;
	class scrProgram;
	class scrProgramTableEntry;
	class scrProgramTable;
	enum class eThreadState : uint32_t;
	class scriptHandlerObject;
	class scrThreadContext;
	class tlsContext;
	union scrValue;
	class scrThread;
	class sysMemAllocator;
	class scriptIdBase;
	class scriptId;
	class scriptResource;
	class scriptHandler;
	class scriptHandlerNetComponent;
	class rlSessionInfo;
	class rlSessionByGamerTaskResult;
	class rlPresenceEventInviteAccepted;
	class netSyncTree;
	class netSyncTreeNode;
	class netObject;
	class netObjectMgrBase;
	class CEntityDrawHandler;
	class CEntity;
	class CDynamicEntity;
	class CPhysical;
	class scriptHandlerMgr;
	class scrVectorSpace;
	class scrNativeCallContext;
	using scrCmd = fnptr<void(class scrNativeCallContext*)>;
	using scrNativeHash = uint64_t;
	class scrNativeRegistration;
	class scrNativeRegistrationTable;
	class JSONSerialiser;
	class rlMetric;
	template <typename t>
	class ObfVar;
	using Obf16 = ObfVar<unsigned short>;
	using Obf32 = ObfVar<unsigned int>;
	class rlSessionDetail;
	class rlMatchmakingFindResult;
	class netGamePlayerData;
	class netConnectionManager;
	class netConnectionPeer;
	namespace netConnection {
		class InFrame;
	}
	class netConMgr;
	class snPlayer;
	class snPeer;
	class rlRemoteGamer;
	class rlRemoteGamer;
	class snSession;
	class snMsgRemoveGamersFromSessionCmd;
	class snMsgAddGamerToSessionCmd;
	enum InitFunctionType : int;
	class InitFunctionData;
	class InitFunctionEntry;
	class InitFunctionList;
	class gameSkeletonUpdateBase;
	class UpdateFunctionList;
	class gameSkeleton;
	inline constexpr u32 g_initFunctionKnownHashes[] = {
		"AmbientLights"_joaat,
		"AnimBlackboard"_joaat,
		"Audio"_joaat,
		"BackgroundScripts"_joaat,
		"CActionManager"_joaat,
		"CAgitatedManager"_joaat,
		"CAmbientAnimationManager"_joaat,
		"CAmbientAudioManager"_joaat,
		"CAmbientModelSetManager"_joaat,
		"CAnimBlackboard"_joaat,
		"CAppDataMgr"_joaat,
		"CAssistedMovementRouteStore"_joaat,
		"CBoatChaseDirector"_joaat,
		"CBuses"_joaat,
		"CBusySpinner"_joaat,
		"CCheat"_joaat,
		"CCheckCRCs"_joaat,
		"CClipDictionaryStoreInterface"_joaat,
		"CClock"_joaat,
		"CCombatDirector"_joaat,
		"CCombatInfoMgr"_joaat,
		"CCompEntity"_joaat,
		"CConditionalAnimManager"_joaat,
		"CContentExport"_joaat,
		"CContentSearch"_joaat,
		"CControl"_joaat,
		"CControlMgr"_joaat,
		"CControllerLabelMgr"_joaat,
		"CCover"_joaat,
		"CCoverFinder"_joaat,
		"CCredits"_joaat,
		"CCrimeInformationManager"_joaat,
		"CCullZones"_joaat,
		"CDLCScript"_joaat,
		"CDecoratorInterface"_joaat,
		"CDispatchData"_joaat,
		"CEventDataManager"_joaat,
		"CExpensiveProcessDistributer"_joaat,
		"CExplosionManager"_joaat,
		"CExtraContent"_joaat,
		"CExtraContentWrapper"_joaat,
		"CExtraContentWrapper::Shutdown"_joaat,
		"CExtraContentWrapper::ShutdownStart"_joaat,
		"CExtraMetadataMgr"_joaat,
		"CExtraMetadataMgr::ClassInit"_joaat,
		"CExtraMetadataMgr::ClassShutdown"_joaat,
		"CExtraMetadataMgr::ShutdownDLCMetaFiles"_joaat,
		"CFlyingVehicleAvoidanceManager"_joaat,
		"CFocusEntityMgr"_joaat,
		"CFrontendStatsMgr"_joaat,
		"CGameLogic"_joaat,
		"CGameSituation"_joaat,
		"CGameStreamMgr"_joaat,
		"CGameWorld"_joaat,
		"CGameWorldHeightMap"_joaat,
		"CGameWorldWaterHeight"_joaat,
		"CGarages"_joaat,
		"CGenericGameStorage"_joaat,
		"CGestureManager"_joaat,
		"CGps"_joaat,
		"CGtaAnimManager"_joaat,
		"CHandlingDataMgr"_joaat,
		"CInstanceListAssetLoader::Init"_joaat,
		"CInstanceListAssetLoader::Shutdown"_joaat,
		"CIplCullBox"_joaat,
		"CJunctions"_joaat,
		"CLODLightManager"_joaat,
		"CLODLights"_joaat,
		"CLadderMetadataManager"_joaat,
		"CLoadingScreens"_joaat,
		"CMapAreas"_joaat,
		"CMapZoneManager"_joaat,
		"CMessages"_joaat,
		"CMiniMap"_joaat,
		"CModelInfo"_joaat,
		"CModelInfo::Init"_joaat,
		"CMovieMeshManager"_joaat,
		"CMultiplayerGamerTagHud"_joaat,
		"CNetRespawnMgr"_joaat,
		"CNetwork"_joaat,
		"CNetworkTelemetry"_joaat,
		"CNewHud"_joaat,
		"CObjectPopulationNY"_joaat,
		"COcclusion"_joaat,
		"CParaboloidShadow"_joaat,
		"CPathFind"_joaat,
		"CPathServer::InitBeforeMapLoaded"_joaat,
		"CPathServer::InitSession"_joaat,
		"CPathServer::ShutdownSession"_joaat,
		"CPathZoneManager"_joaat,
		"CPatrolRoutes"_joaat,
		"CPauseMenu"_joaat,
		"CPed"_joaat,
		"CPedAILodManager"_joaat,
		"CPedGeometryAnalyser"_joaat,
		"CPedModelInfo"_joaat,
		"CPedPopulation"_joaat,
		"CPedPopulation::ResetPerFrameScriptedMu"_joaat,
		"CPedPopulation::ResetPerFrameScriptedMultipiers"_joaat,
		"CPedPropsMgr"_joaat,
		"CPedVariationPack"_joaat,
		"CPedVariationStream"_joaat,
		"CPerformance"_joaat,
		"CPhoneMgr"_joaat,
		"CPhotoManager"_joaat,
		"CPhysics"_joaat,
		"CPickupDataManager"_joaat,
		"CPickupManager"_joaat,
		"CPlantMgr"_joaat,
		"CPlayStats"_joaat,
		"CPlayerSwitch"_joaat,
		"CPopCycle"_joaat,
		"CPopZones"_joaat,
		"CPopulationStreaming"_joaat,
		"CPopulationStreamingWrapper"_joaat,
		"CPortal"_joaat,
		"CPortalTracker"_joaat,
		"CPostScan"_joaat,
		"CPrecincts"_joaat,
		"CPrioritizedClipSetRequestManager"_joaat,
		"CPrioritizedClipSetStreamer"_joaat,
		"CProcObjectMan"_joaat,
		"CProceduralInfo"_joaat,
		"CProfileSettings"_joaat,
		"CRandomEventManager"_joaat,
		"CRecentlyPilotedAircraft"_joaat,
		"CRenderPhaseCascadeShadowsInterface"_joaat,
		"CRenderTargetMgr"_joaat,
		"CRenderThreadInterface"_joaat,
		"CRenderer"_joaat,
		"CReportMenu"_joaat,
		"CRestart"_joaat,
		"CRiots"_joaat,
		"CRoadBlock"_joaat,
		"CScaleformMgr"_joaat,
		"CScenarioActionManager"_joaat,
		"CScenarioManager"_joaat,
		"CScenarioManager::ResetExclusiveScenari"_joaat,
		"CScenarioManager::ResetExclusiveScenarioGroup"_joaat,
		"CScenarioPointManager"_joaat,
		"CScenarioPointManagerInitSession"_joaat,
		"CScene"_joaat,
		"CSceneStreamerMgr::PreScanUpdate"_joaat,
		"CScriptAreas"_joaat,
		"CScriptCars"_joaat,
		"CScriptDebug"_joaat,
		"CScriptEntities"_joaat,
		"CScriptHud"_joaat,
		"CScriptPedAIBlips"_joaat,
		"CScriptPeds"_joaat,
		"CScriptedGunTaskMetadataMgr"_joaat,
		"CShaderHairSort"_joaat,
		"CShaderLib"_joaat,
		"CSituationalClipSetStreamer"_joaat,
		"CSky"_joaat,
		"CSlownessZonesManager"_joaat,
		"CSprite2d"_joaat,
		"CStaticBoundsStore"_joaat,
		"CStatsMgr"_joaat,
		"CStreaming"_joaat,
		"CStreamingRequestList"_joaat,
		"CStuntJumpManager"_joaat,
		"CTVPlaylistManager"_joaat,
		"CTacticalAnalysis"_joaat,
		"CTask"_joaat,
		"CTaskClassInfoManager"_joaat,
		"CTaskRecover"_joaat,
		"CTexLod"_joaat,
		"CText"_joaat,
		"CThePopMultiplierAreas"_joaat,
		"CTheScripts"_joaat,
		"CTimeCycle"_joaat,
		"CTrafficLights"_joaat,
		"CTrain"_joaat,
		"CTuningManager"_joaat,
		"CUserDisplay"_joaat,
		"CVehicleAILodManager"_joaat,
		"CVehicleChaseDirector"_joaat,
		"CVehicleCombatAvoidanceArea"_joaat,
		"CVehicleDeformation"_joaat,
		"CVehicleMetadataMgr"_joaat,
		"CVehicleModelInfo"_joaat,
		"CVehiclePopulation"_joaat,
		"CVehiclePopulation::ResetPerFrameScript"_joaat,
		"CVehiclePopulation::ResetPerFrameScriptedMultipiers"_joaat,
		"CVehicleRecordingMgr"_joaat,
		"CVehicleVariationInstance"_joaat,
		"CVisualEffects"_joaat,
		"CWarpManager"_joaat,
		"CWaypointRecording"_joaat,
		"CWeaponManager"_joaat,
		"CWitnessInformationManager"_joaat,
		"CWorldPoints"_joaat,
		"CZonedAssetManager"_joaat,
		"Common"_joaat,
		"CreateFinalScreenRenderPhaseList"_joaat,
		"Credits"_joaat,
		"CutSceneManager"_joaat,
		"CutSceneManagerWrapper"_joaat,
		"FacialClipSetGroupManager"_joaat,
		"FireManager"_joaat,
		"FirstPersonProp"_joaat,
		"FirstPersonPropCam"_joaat,
		"Game"_joaat,
		"GenericGameStoragePhotoGallery"_joaat,
		"INSTANCESTORE"_joaat,
		"ImposedTxdCleanup"_joaat,
		"InitSystem"_joaat,
		"Kick"_joaat,
		"LightEntityMgr"_joaat,
		"Lights"_joaat,
		"MeshBlendManager"_joaat,
		"Misc"_joaat,
		"NewHud"_joaat,
		"Occlusion"_joaat,
		"PauseMenu"_joaat,
		"Ped"_joaat,
		"PedHeadShotManager"_joaat,
		"PedModelInfo"_joaat,
		"PedPopulation"_joaat,
		"PlantsMgr::UpdateBegin"_joaat,
		"PlantsMgr::UpdateEnd"_joaat,
		"Population"_joaat,
		"PostFX"_joaat,
		"PostFx"_joaat,
		"Pre-vis"_joaat,
		"Prioritized"_joaat,
		"Proc"_joaat,
		"ProcessAfterCameraUpdate"_joaat,
		"ProcessAfterMovement"_joaat,
		"ProcessPedsEarlyAfterCameraUpdate"_joaat,
		"Render"_joaat,
		"ResetSceneLights"_joaat,
		"Run"_joaat,
		"Script"_joaat,
		"ScriptHud"_joaat,
		"ShaderLib::Update"_joaat,
		"Situational"_joaat,
		"SocialClubMenu"_joaat,
		"Streaming"_joaat,
		"UI3DDrawManager"_joaat,
		"UIWorldIconManager"_joaat,
		"Update"_joaat,
		"VehPopulation"_joaat,
		"VideoPlayback"_joaat,
		"VideoPlaybackThumbnailManager"_joaat,
		"VideoPlaybackThumbnails"_joaat,
		"Viewport"_joaat,
		"ViewportSystemInit"_joaat,
		"ViewportSystemInitLevel"_joaat,
		"ViewportSystemShutdown"_joaat,
		"ViewportSystemShutdownLevel"_joaat,
		"Visibility"_joaat,
		"Visual"_joaat,
		"WarningScreen"_joaat,
		"Water"_joaat,
		"WaterHeightSim"_joaat,
		"World"_joaat,
		"audNorthAudioEngine"_joaat,
		"audNorthAudioEngineDLC"_joaat,
		"cStoreScreenMgr"_joaat,
		"camManager"_joaat,
		"decorators"_joaat,
		"fwAnimDirector"_joaat,
		"fwClipSetManager"_joaat,
		"fwClothStore"_joaat,
		"fwDrawawableStoreWrapper"_joaat,
		"fwDwdStore"_joaat,
		"fwDwdStoreWrapper"_joaat,
		"fwExpressionSetManager"_joaat,
		"fwFacialClipSetGroupManager"_joaat,
		"fwFragmentStoreWrapper"_joaat,
		"fwMapTypesStore"_joaat,
		"fwMetaDataStore"_joaat,
		"fwTimer"_joaat,
		"fwTxdStore"_joaat,
		"perfClearingHouse"_joaat,
		"strStreamingEngine::SubmitDeferredAsyncPlacementRequests"_joaat,
		"CAnimSceneManager"_joaat,
		"CTextInputBox"_joaat,
		"CMultiplayerChat"_joaat,
		"CCreditsText"_joaat,
		"CReplayMgr"_joaat,
		"CReplayCoordinator"_joaat,
		"CMousePointer"_joaat,
		"CVideoEditorUI"_joaat,
		"CVideoEditorInterface"_joaat,
		"VideoRecording"_joaat,
		"WatermarkRenderer"_joaat,
		"rageSecEngine"_joaat,
	};
}