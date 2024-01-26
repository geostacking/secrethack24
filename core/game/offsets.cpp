#include "offsets.hpp"
#include "il2cpp.hpp"
#include "../utilities/memory.hpp"

namespace offsets
{
	uintptr_t proj_do_hit = g_il2mgr.method( xs( "Projectile" ), xs( "DoHit" ), 3 );
	uintptr_t update_velocity = g_il2mgr.method( xs( "Projectile" ), xs( "UpdateVelocity" ), 1 );
	uintptr_t get_random_velocity = g_il2mgr.method( xs( "ItemModProjectile" ), xs( "GetRandomVelocity" ), 0 );

	uintptr_t get_material = g_il2mgr.method( xs( "Renderer" ), xs( "get_material" ), 0, xs( "UnityEngine" ) );
	uintptr_t get_shader = g_il2mgr.method( xs( "Material" ), xs( "get_shader" ), 0, xs( "UnityEngine" ) );
	uintptr_t find = g_il2mgr.method( xs( "Shader" ), xs( "Find" ), 1, xs( "UnityEngine" ) );
	uintptr_t set_shader = g_il2mgr.method( xs( "Material" ), xs( "set_shader" ), 1, xs( "UnityEngine" ) );
	uintptr_t CreateWithShader = g_il2mgr.method( xs( "Material" ), xs( "CreateWithShader" ), 2, xs( "UnityEngine" ) );
	uintptr_t set_color = g_il2mgr.method( xs( "Material" ), xs( "SetColor" ), 2, xs( "UnityEngine" ) );
	uintptr_t set_pass = g_il2mgr.method( xs( "Material" ), xs( "SetPass" ), 0, xs( "UnityEngine" ) );
	uintptr_t set_int = g_il2mgr.method( xs( "Material" ), xs( "SetInt" ), 2, xs( "UnityEngine" ) );
	uintptr_t set_float = g_il2mgr.method( xs( "Material" ), xs( "SetFloat" ), 2, xs( "UnityEngine" ) );

	uintptr_t _displayName = g_il2mgr.value( xs( "BasePlayer" ), xs( "_displayName" ) );
	uintptr_t userID = g_il2mgr.value( xs( "BasePlayer" ), xs( "userID" ) );
	uintptr_t clActiveItem = g_il2mgr.value( xs( "BasePlayer" ), xs( "clActiveItem" ) );
	uintptr_t movement = g_il2mgr.value( xs( "BasePlayer" ), xs( "movement" ) );
	uintptr_t GestureViewModel = g_il2mgr.value( xs( "BasePlayer" ), xs( "GestureViewModel" ) );
	uintptr_t sway = g_il2mgr.value( xs( "BaseViewModel" ), xs( "sway" ) );
	uintptr_t lower = g_il2mgr.value( xs( "BaseViewModel" ), xs( "lower" ) );
	uintptr_t bob = g_il2mgr.value( xs( "BaseViewModel" ), xs( "bob" ) );

	uintptr_t bob_amount_run = g_il2mgr.value( xs( "ViewmodelBob" ), xs( "bobAmountRun" ) );
	uintptr_t bob_amount_walk = g_il2mgr.value( xs( "ViewmodelBob" ), xs( "bobAmountWalk" ) );
	uintptr_t bob_speed_run = g_il2mgr.value( xs( "ViewmodelBob" ), xs( "bobSpeedRun" ) );
	uintptr_t bob_speed_walk = g_il2mgr.value( xs( "ViewmodelBob" ), xs( "bobSpeedWalk" ) );
	uintptr_t should_lower = g_il2mgr.value( xs( "ViewmodelLower" ), xs( "shouldLower" ) );

	uintptr_t playereyes_get_center = g_il2mgr.method( xs( "PlayerEyes" ), xs( "get_center" ), 0 );

	uintptr_t waterlevel_test = g_il2mgr.method( xs( "WaterLevel" ), xs( "Test" ), 4 );

	uintptr_t view = g_il2mgr.method( xs( "BaseViewModel" ), xs( "get_ActiveModel" ), 0 );

	uintptr_t animator = g_il2mgr.value( xs( "BaseViewModel" ), xs( "animator" ) );
	uintptr_t Speed = g_il2mgr.value( xs( "Animator" ), xs( "Animator" ) );

	uintptr_t body_rotation = g_il2mgr.value( xs( "PlayerEyes" ), xs( "<bodyRotation>k__BackingField" ) );
	uintptr_t get_body_lean_offset = g_il2mgr.method( xs( "PlayerEyes" ), xs( "get_BodyLeanOffset" ), 0 );
	uintptr_t view_offset = g_il2mgr.value( xs( "PlayerEyes" ), xs( "viewOffset" ) );

	uintptr_t get_projectile_velocity_scale = g_il2mgr.method( xs( "BaseProjectile" ), xs( "GetProjectileVelocityScale" ), 1, xs( "getMax" ), xs( "" ), 1 );

	uintptr_t projectile_velocity_scale = g_il2mgr.value( xs( "BaseProjectile" ), xs( "projectileVelocityScale" ) );
	uintptr_t projectile_distance_scale = g_il2mgr.value( xs( "BaseProjectile" ), xs( "distanceScale" ) );

    uintptr_t push_matrix = g_il2mgr.method( xs( "GL" ), xs( "PushMatrix" ), 0, xs( "UnityEngine" ) );
    uintptr_t begin = g_il2mgr.method( xs( "GL" ), xs( "Begin" ), 1, xs( "UnityEngine" ) );
    uintptr_t vertex = g_il2mgr.method( xs( "GL" ), xs( "Vertex" ), 1, xs( "UnityEngine" ) );
    uintptr_t end = g_il2mgr.method( xs( "GL" ), xs( "End" ), 0, xs( "UnityEngine" ) );
    uintptr_t pop_matrix = g_il2mgr.method( xs( "GL" ), xs( "PopMatrix" ), 0, xs( "UnityEngine" ) );
	uintptr_t set_gl_color = g_il2mgr.method( xs( "GL" ), xs( "Color" ), 1, xs( "UnityEngine" ) );
	
	uintptr_t input = g_il2mgr.value( xs( "BasePlayer" ), xs( "input" ) );
	uintptr_t inventory = g_il2mgr.value( xs( "BasePlayer" ), xs( "inventory" ) );
	uintptr_t Frozen = g_il2mgr.value( xs( "BasePlayer" ), xs( "Frozen" ) );
	uintptr_t clothingBlocksAiming = g_il2mgr.value( xs( "BasePlayer" ), xs( "clothingBlocksAiming" ) );
	uintptr_t clothingMoveSpeedReduction = g_il2mgr.value( xs( "BasePlayer" ), xs( "clothingMoveSpeedReduction" ) );

	uintptr_t HasPlayerFlag = g_il2mgr.value( xs( "BasePlayer" ), xs( "HasPlayerFlag" ) );
	uintptr_t clientTeam = g_il2mgr.value( xs( "BasePlayer" ), xs( "clientTeam" ) );
	uintptr_t playerFlags = g_il2mgr.value( xs( "BasePlayer" ), xs( "playerFlags" ) );
	uintptr_t player_model = g_il2mgr.value( xs( "BasePlayer" ), xs( "playerModel" ) );
	uintptr_t player_eyes = g_il2mgr.value( xs( "BasePlayer" ), xs( "eyes" ) );
	uintptr_t player_lastSentTick = g_il2mgr.value( xs( "BasePlayer" ), xs( "lastSentTick" ) );
	uintptr_t player_lastSentTickTime = g_il2mgr.value( xs( "BasePlayer" ), xs( "lastSentTickTime" ) );
	uintptr_t invitetoteam = g_il2mgr.method( xs( "BasePlayer" ), xs( "InviteToTeam" ), 1 );
	uintptr_t baseplayer_get_height = g_il2mgr.method( xs( "BasePlayer" ), xs( "GetHeight" ), 1 );
	uintptr_t get_max_speed = g_il2mgr.method( xs( "BasePlayer" ), xs( "GetMaxSpeed" ), 0 );
	uintptr_t baseplayer_get_radius = g_il2mgr.method( xs( "BasePlayer" ), xs( "GetRadius" ), 0 );
	uintptr_t baseplayer_onladder = g_il2mgr.method( xs( "BasePlayer" ), xs( "OnLadder" ), 0 );

	uintptr_t baseplayer_get_jumpheight = g_il2mgr.method( xs( "BasePlayer" ), xs( "GetJumpHeight" ), 0 );
	uintptr_t model = g_il2mgr.value( xs( "BaseEntity" ), xs( "model" ) );

	uintptr_t GetParentVelocity = g_il2mgr.method( xs( "BaseEntity" ), xs( "GetParentVelocity" ), 0 );
	uintptr_t GetMountVelocity = g_il2mgr.method( xs( "BaseEntity" ), xs( "GetMountVelocity" ), 0 );
	

	uintptr_t modelstate = g_il2mgr.value( xs( "BasePlayer" ), xs( "modelState" ) );
	uintptr_t playereyes_get_position = g_il2mgr.method( xs( "PlayerEyes" ), xs( "get_position" ), 0 );
	uintptr_t chams = g_il2mgr.value( xs( "SkinnedMultiMesh" ), xs( "<Renderers>k__BackingField" ) );
	uintptr_t LoadAsset_Internal = g_il2mgr.method( xs( "AssetBundle" ), xs( "LoadAsset_Internal" ), 2, xs( "UnityEngine" ) );
	uintptr_t LoadFromFile_Internal = g_il2mgr.method( xs( "AssetBundle" ), xs( "LoadFromFile_Internal" ), 3, xs( "UnityEngine" ) );
	uintptr_t get_Renderers = g_il2mgr.method( xs( "SkinnedMultiMesh" ), xs( "get_Renderers" ), 0 );
	uintptr_t DoHit = g_il2mgr.method( xs( "Projectile" ), xs( "DoHit" ), 3 );
	uintptr_t teamID = g_il2mgr.value( xs( "PlayerTeam" ), xs( "teamID" ) );
	uintptr_t underwater_effect = g_il2mgr.value( xs( "Admin" ), xs( "underwater_effect" ) );

	uintptr_t Animator = g_il2mgr.method( xs( "PlayerModel" ), xs( "get_animator" ), 0 );
	uintptr_t Nohands = g_il2mgr.value( xs( "PlayerModel" ), xs( "get_ShouldShowHands" ) );
	uintptr_t playermodel_position = g_il2mgr.value( xs( "PlayerModel" ), xs( "position" ) );
	uintptr_t playermodel_newVelocity = g_il2mgr.value( xs( "PlayerModel" ), xs( "newVelocity" ) );
	uintptr_t playermodel_get_IsNpc = g_il2mgr.value( xs( "PlayerModel" ), xs( "<IsNpc>k__BackingField" ) );
	uintptr_t playermodel_multimesh = g_il2mgr.value( xs( "PlayerModel" ), xs( "_multiMesh" ) );

	uintptr_t boneTransforms = g_il2mgr.value( xs( "Model" ), xs( "boneTransforms" ) );
	uintptr_t get_mousePosition = g_il2mgr.method( xs( "Input" ), xs( "get_mousePosition" ), 0, xs( "UnityEngine" ) );
	uintptr_t screen_get_width = g_il2mgr.method( xs( "Screen" ), xs( "get_width" ), 0, xs( "UnityEngine" ) );
	uintptr_t screen_get_height = g_il2mgr.method( xs( "Screen" ), xs( "get_height" ), 0, xs( "UnityEngine" ) );
	uintptr_t input_get_key_down = g_il2mgr.method( xs( "Input" ), xs( "GetKeyDown" ), 1, xs( "UnityEngine" ) );
	uintptr_t input_get_key_up = g_il2mgr.method( xs( "Input" ), xs( "GetKeyUp" ), 1, xs( "UnityEngine" ) );
	uintptr_t input_get_key = g_il2mgr.method( xs( "Input" ), xs( "GetKey" ), 1, xs( "UnityEngine" ) );
	uintptr_t gamephysics_line_of_site = g_il2mgr.method( xs( "GamePhysics" ), xs( "LineOfSight" ), 4 );
	uintptr_t ignore_layer_collision = methods::resolve_icall( xs( "UnityEngine.Physics::IgnoreLayerCollision()" ) );
	uintptr_t mathf_clamp = g_il2mgr.method( xs( "Mathf" ), xs( "Clamp" ), 3, xs( "UnityEngine" ) );
	uintptr_t mathf_atan2 = g_il2mgr.method( xs( "Mathf" ), xs( "Atan2" ), 2, xs( "UnityEngine" ) );
	uintptr_t mathf_max = g_il2mgr.method( xs( "Mathf" ), xs( "Max" ), 2, xs( "UnityEngine" ) );

	uintptr_t mathf_abs = g_il2mgr.method( xs( "Mathf" ), xs( "Abs" ), 1, xs( "UnityEngine" ) );
	uintptr_t transform_get_position = g_il2mgr.method( xs( "Transform" ), xs( "get_position" ), 0, xs( "UnityEngine" ) );
	uintptr_t transform_set_position = g_il2mgr.method( xs( "Transform" ), xs( "set_position" ), 1, xs( "UnityEngine" ) );
	uintptr_t transform_inverse_transform_point = g_il2mgr.method( xs( "Transform" ), xs( "InverseTransformPoint" ), 1, xs( "UnityEngine" ) );
	uintptr_t calcsize = g_il2mgr.method( xs( "GUIStyle" ), xs( "CalcSize" ), 0, xs( "UnityEngine" ) );
	uintptr_t transform_transform_direction = g_il2mgr.method( xs( "Transform" ), xs( "TransformDirection" ), 1, xs( "UnityEngine" ) );
	uintptr_t real_time_since_startup = g_il2mgr.method( xs( "Time" ), xs( "get_realtimeSinceStartup" ), 0, xs( "UnityEngine" ) );
	uintptr_t get_fixed_time = g_il2mgr.method( xs( "Time" ), xs( "get_fixedTime" ), 0, xs( "UnityEngine" ) );
	uintptr_t get_fixed_delta_time = g_il2mgr.method( xs( "Time" ), xs( "get_fixedDeltaTime" ), 0, xs( "UnityEngine" ) );
	uintptr_t get_smooth_delta_time = g_il2mgr.method( xs( "Time" ), xs( "get_smoothDeltaTime" ), 0, xs( "UnityEngine" ) );

	uintptr_t baseentity_bounds_padding = g_il2mgr.method( xs( "BaseEntity" ), xs( "BoundsPadding" ), 0 );
	uintptr_t baseentity__name = g_il2mgr.value( xs( "BaseEntity" ), xs( "_name" ) );
	uintptr_t get_iconSprite = g_il2mgr.method( xs( "Item" ), xs( "get_iconSprite" ), 0 );
	uintptr_t get_texture = g_il2mgr.method( xs( "Sprite" ), xs( "get_texture" ), 0, xs( "UnityEngine" ) );

	uintptr_t set_aspect = g_il2mgr.method( xs( "Camera" ), xs( "set_aspect" ), 1, xs( "UnityEngine" ) );

	uintptr_t player_name_id_username = g_il2mgr.value( xs( "PlayerNameID" ), xs( "username" ) );

	uintptr_t CanAttack = g_il2mgr.method( xs( "BaseProjectile" ), xs( "CanAttack" ), 0 );
	uintptr_t noAimingWhileCycling = g_il2mgr.value( xs( "BaseProjectile" ), xs( "noAimingWhileCycling" ) );

	uintptr_t mounted = g_il2mgr.value( xs( "BasePlayer" ), xs( "mounted" ) );
	
	uintptr_t clientTickInterval = g_il2mgr.value( xs( "BasePlayer" ), xs( "clientTickInterval" ) );
	uintptr_t baseentity_closest_point = g_il2mgr.method( xs( "BaseEntity" ), xs( "ClosestPoint" ), 1 );
	uintptr_t baseentity_serverrpc_1arg = g_il2mgr.method( xs( "BaseEntity" ), xs( "ServerRPC" ), 1 );
	uintptr_t baseentity_serverrpc_4arg = g_il2mgr.method( xs( "BaseEntity" ), xs( "ServerRPC" ), 4 );

	uintptr_t baseentity_sendsignalbroadcast = g_il2mgr.method( xs( "BaseEntity" ), xs( "SendSignalBroadcast" ), 2 );
	uintptr_t component_get_transform = g_il2mgr.method( xs( "Component" ), xs( "get_transform" ), 0, xs( "UnityEngine" ) );
	uintptr_t get_transform_up = g_il2mgr.method( xs( "Transform" ), xs( "get_up" ), 0, xs( "UnityEngine" ) );
	uintptr_t treeentity_hit_direction = g_il2mgr.value( xs( "TreeEntity" ), xs( "hitDirection" ) );
	uintptr_t basecombatentity__lifestate = g_il2mgr.value( xs( "BaseCombatEntity" ), xs( "lifestate" ) );
	uintptr_t basecombatentity__health = g_il2mgr.value( xs( "BaseCombatEntity" ), xs( "_health" ) );
	uintptr_t stashcontainer_burriedOffset = g_il2mgr.value( xs( "StashContainer" ), xs( "burriedOffset" ) );
	uintptr_t basecombatentity__maxHealth = g_il2mgr.value( xs( "BaseCombatEntity" ), xs( "_maxHealth" ) );
	uintptr_t basecombatentity_lifestate = g_il2mgr.value( xs( "BaseCombatEntity" ), xs( "lifestate" ) );
	uintptr_t playerwalkmovement_flying = g_il2mgr.value( xs( "PlayerWalkMovement" ), xs( "flying" ) );
	uintptr_t playerwalkmovement_swimming = g_il2mgr.value( xs( "PlayerWalkMovement" ), xs( "swimming" ) );
	uintptr_t playerwalkmovement_grounded = g_il2mgr.value( xs( "PlayerWalkMovement" ), xs( "grounded" ) );
	uintptr_t playerwalkmovement_capsuleCenter = g_il2mgr.value( xs( "PlayerWalkMovement" ), xs( "capsuleCenter" ) );
	uintptr_t playerwalkmovement_capsuleHeight = g_il2mgr.value( xs( "PlayerWalkMovement" ), xs( "capsuleHeight" ) );
	uintptr_t playerwalkmovement_ducking = g_il2mgr.value( xs( "BaseMovement" ), xs( "<Ducking>k__BackingField" ) );
	uintptr_t playerwalkmovement_grounded_field = g_il2mgr.value( xs( "BaseMovement" ), xs( "<Grounded>k__BackingField" ) );
	uintptr_t playerwalkmovement_running = g_il2mgr.value( xs( "BaseMovement" ), xs( "<Running>k__BackingField" ) );


	uintptr_t playerwalkmovement_gravityTestRadius = g_il2mgr.value( xs( "PlayerWalkMovement" ), xs( "gravityTestRadius" ) );
	uintptr_t playerwalkmovement_groundAngle = g_il2mgr.value( xs( "PlayerWalkMovement" ), xs( "groundAngle" ) );
	uintptr_t playerwalkmovement_groundAngleNew = g_il2mgr.value( xs( "PlayerWalkMovement" ), xs( "groundAngleNew" ) );
	uintptr_t playerwalkmovement_previousVelocity = g_il2mgr.value( xs( "PlayerWalkMovement" ), xs( "previousVelocity" ) );
	uintptr_t playerwalkmovement_wasFalling = g_il2mgr.value( xs( "PlayerWalkMovement" ), xs( "wasFalling" ) );
	uintptr_t playerwalkmovement_capsule_radius = g_il2mgr.value( xs( "CapsuleCollider" ), xs( "radius" ) );
	uintptr_t playerwalkmovement_maxAngleWalking = g_il2mgr.value( xs( "PlayerWalkMovement" ), xs( "maxAngleWalking" ) );
	uintptr_t playerwalkmovement_maxAngleClimbing = g_il2mgr.value( xs( "PlayerWalkMovement" ), xs( "maxAngleClimbing" ) );
	uintptr_t playerwalkmovement_groundTime = g_il2mgr.value( xs( "PlayerWalkMovement" ), xs( "groundTime" ) );
	uintptr_t playerwalkmovement_gravityMultiplier = g_il2mgr.value( xs( "PlayerWalkMovement" ), xs( "gravityMultiplier" ) );
	uintptr_t playerwalkmovement_jumpTime = g_il2mgr.value( xs( "PlayerWalkMovement" ), xs( "jumpTime" ) );
	uintptr_t playerwalkmovement_landTime = g_il2mgr.value( xs( "PlayerWalkMovement" ), xs( "landTime" ) );
	uintptr_t playerwalkmovement_TeleportTo = g_il2mgr.method( xs( "PlayerWalkMovement" ), xs( "TeleportTo" ), 2 );

	uintptr_t item_heldEntity = g_il2mgr.value( xs( "Item" ), xs( "heldEntity" ) );
	uintptr_t baseprojectile_DoAttack = g_il2mgr.value( xs( "BaseProjectile" ), xs( "DoAttack" ) );
	uintptr_t baseprojectile_aimCone = g_il2mgr.value( xs( "BaseProjectile" ), xs( "aimCone" ) );
	uintptr_t bowweapon_wasAiming = g_il2mgr.value( xs( "BowWeapon" ), xs( "wasAiming" ) );
	uintptr_t bowweapon_attackReady = g_il2mgr.value( xs( "BowWeapon" ), xs( "attackReady" ) );
	uintptr_t basemelee_maxDistance = g_il2mgr.value( xs( "BaseMelee" ), xs( "maxDistance" ) );
	uintptr_t basemelee_attack_radius = g_il2mgr.value( xs( "BaseMelee" ), xs( "attackRadius" ) );
	uintptr_t basemelee_dothrow = g_il2mgr.method( xs( "BaseMelee" ), xs( "DoThrow" ), 0 );
	uintptr_t basemelee_can_throw_as_projectile = g_il2mgr.value( xs( "BaseMelee" ), xs( "canThrowAsProjectile" ) );

	uintptr_t item_icon_queued_for_looting = g_il2mgr.value( xs( "ItemIcon" ), xs( "queuedForLooting" ) );
	uintptr_t item_icon_run_timed_action = g_il2mgr.method( xs( "ItemIcon" ), xs( "RunTimedAction" ), 0 );

	uintptr_t baseprojectile_aimconePenalty = g_il2mgr.value( xs( "BaseProjectile" ), xs( "aimconePenalty" ) );
	uintptr_t baseprojectile_stancePenaltyScale = g_il2mgr.value( xs( "BaseProjectile" ), xs( "stancePenaltyScale" ) );

	uintptr_t baseprojectile_hipAimCone = g_il2mgr.value( xs( "BaseProjectile" ), xs( "hipAimCone" ) );
	uintptr_t baseprojectile_aimConePenaltyMax = g_il2mgr.value( xs( "BaseProjectile" ), xs( "aimConePenaltyMax" ) );
	uintptr_t baseprojectile_aimconePenaltyRecoverTime = g_il2mgr.value( xs( "BaseProjectile" ), xs( "aimconePenaltyRecoverTime" ) );
	uintptr_t baseprojectile_aimconePenaltyRecoverDelay = g_il2mgr.value( xs( "BaseProjectile" ), xs( "aimconePenaltyRecoverDelay" ) );
	uintptr_t baseprojectile_stancePenalty = g_il2mgr.value( xs( "BaseProjectile" ), xs( "stancePenalty" ) );
	uintptr_t baseprojectile_sightAimConeScale = g_il2mgr.value( xs( "BaseProjectile" ), xs( "sightAimConeScale" ) );
	uintptr_t baseprojectile_sightAimConeOffset = g_il2mgr.value( xs( "BaseProjectile" ), xs( "sightAimConeOffset" ) );
	uintptr_t baseprojectile_hipAimConeScale = g_il2mgr.value( xs( "BaseProjectile" ), xs( "hipAimConeScale" ) );
	uintptr_t baseprojectile_hipAimConeOffset = g_il2mgr.value( xs( "BaseProjectile" ), xs( "hipAimConeOffset" ) );

	uintptr_t baseprojectile_LaunchProjectile = g_il2mgr.method( xs( "BaseProjectile" ), xs( "LaunchProjectile" ), 0 );
	uintptr_t baseprojectile_ShotFired = g_il2mgr.method( xs( "BaseProjectile" ), xs( "ShotFired" ), 0 );
	uintptr_t baseprojectile_UpdateAmmoDisplay = g_il2mgr.method( xs( "BaseProjectile" ), xs( "UpdateAmmoDisplay" ), 0 );
	uintptr_t baseprojectile_DidAttackClientside = g_il2mgr.method( xs( "BaseProjectile" ), xs( "DidAttackClientside" ), 0 );
	uintptr_t baseprojectile_BeginCycle = g_il2mgr.method( xs( "BaseProjectile" ), xs( "BeginCycle" ), 0 );

	uintptr_t magazine_contents = g_il2mgr.value( xs( "Magazine" ), xs( "contents" ) );
	uintptr_t magazine_ammoType = g_il2mgr.value( xs( "Magazine" ), xs( "ammoType" ) );

	uintptr_t building_privlidge_cachedProtectedMinutes = g_il2mgr.value( xs( "BuildingPrivlidge" ), xs( "cachedProtectedMinutes" ) );	
	uintptr_t building_privlidge_authorizedPlayers = g_il2mgr.value( xs( "BuildingPrivlidge" ), xs( "authorizedPlayers" ) );
	
	uintptr_t baseprojectile_baseProjectile = g_il2mgr.value( xs( "Entity" ), xs( "baseProjectile" ) );
	uintptr_t baseprojectile_aimconePenaltyPerShot = g_il2mgr.value( xs( "BaseProjectile" ), xs( "aimconePenaltyPerShot" ) );
	uintptr_t baseprojectile_createdProjectiles = g_il2mgr.value( xs( "BaseProjectile" ), xs( "createdProjectiles" ) );
	uintptr_t baseprojectile_automatic = g_il2mgr.value( xs( "BaseProjectile" ), xs( "automatic" ) );
	uintptr_t baseprojectile_is_burst = g_il2mgr.value( xs( "BaseProjectile" ), xs( "isBurstWeapon" ) );
	
	uintptr_t baseprojectile_aimSway = g_il2mgr.value( xs( "BaseProjectile" ), xs( "aimSway" ) );
	uintptr_t baseprojectile_aimSwaySpeed = g_il2mgr.value( xs( "BaseProjectile" ), xs( "aimSwaySpeed" ) );
	uintptr_t baseprojectile_primaryMagazine = g_il2mgr.value( xs( "BaseProjectile" ), xs( "primaryMagazine" ) );
	uintptr_t baseprojectile_recoil = g_il2mgr.value( xs( "BaseProjectile" ), xs( "recoil" ) );
	uintptr_t attackentity_repeatDelay = g_il2mgr.value( xs( "AttackEntity" ), xs( "repeatDelay" ) );
	uintptr_t attackentity_deployDelay = g_il2mgr.value( xs( "AttackEntity" ), xs( "deployDelay" ) );
	uintptr_t flintstrikeweapon_successFraction = g_il2mgr.value( xs( "FlintStrikeWeapon" ), xs( "successFraction" ) );
	uintptr_t flintstrikeweapon_didSparkThisFrame = g_il2mgr.value( xs( "FlintStrikeWeapon" ), xs( "_didSparkThisFrame" ) );
	uintptr_t client_convar = g_il2mgr.init_class( xs( "Client" ), xs( "ConVar" ) );
	uintptr_t graphics_convar = g_il2mgr.init_class( xs( "Graphics" ), xs( "ConVar" ) );
	uintptr_t flashbang_overlay_instance = g_il2mgr.init_class( xs( "FlashbangOverlay" ), xs( "Instance" ) );
	uintptr_t compoundbow_currentHoldProgress = g_il2mgr.value( xs( "CompoundBowWeapon" ), xs( "currentHoldProgress" ) );
	uintptr_t compoundbow_movementPenaltyRampUpTime = g_il2mgr.value( xs( "CompoundBowWeapon" ), xs( "movementPenaltyRampUpTime" ) );
	uintptr_t compoundbow_stringBonusDamage = g_il2mgr.value( xs( "CompoundBowWeapon" ), xs( "stringBonusDamage" ) );
	uintptr_t input_convar = g_il2mgr.init_class( xs( "Input" ), xs( "ConVar" ) );

	uintptr_t recoilproperties_newRecoilOverride = g_il2mgr.value( xs( "RecoilProperties" ), xs( "newRecoilOverride" ) );
	uintptr_t recoilproperties_recoilYawMin = g_il2mgr.value( xs( "RecoilProperties" ), xs( "recoilYawMin" ) );
	uintptr_t recoilproperties_recoilYawMax = g_il2mgr.value( xs( "RecoilProperties" ), xs( "recoilYawMax" ) );
	uintptr_t recoilproperties_recoilPitchMin = g_il2mgr.value( xs( "RecoilProperties" ), xs( "recoilPitchMin" ) );
	uintptr_t recoilproperties_recoilPitchMax = g_il2mgr.value( xs( "RecoilProperties" ), xs( "recoilPitchMax" ) );
	uintptr_t admin_convar = g_il2mgr.init_class( xs( "Admin" ), xs( "ConVar" ) );
	uintptr_t adminambientmultiplier = g_il2mgr.init_class( xs( "Admin" ), xs( "adminambientmultiplier" ) );
	uintptr_t adminreflectionmultiplier = g_il2mgr.init_class( xs( "Admin" ), xs( "adminreflectionmultiplier" ) );

	uintptr_t worldItem_item = g_il2mgr.value( xs( "WorldItem" ), xs( "item" ) );
	uintptr_t heldentity_isdeployed = g_il2mgr.value( xs( "HeldEntity" ), xs( "isDeployed" ) );
	uintptr_t item_info = g_il2mgr.value( xs( "Item" ), xs( "info" ) );
	uintptr_t itemdefinition_itemid = g_il2mgr.value( xs( "ItemDefinition" ), xs( "itemid" ) );
	uintptr_t ProjectileWeaponMod_projectileVelocity = g_il2mgr.value( xs( "ProjectileWeaponMod" ), xs( "projectileVelocity" ) );
	uintptr_t itemdefinition_displayName = g_il2mgr.value( xs( "ItemDefinition" ), xs( "displayName" ) );
	uintptr_t itemdefinition_shortname = g_il2mgr.value( xs( "ItemDefinition" ), xs( "shortname" ) );
	uintptr_t item_amount = g_il2mgr.value( xs( "Item" ), xs( "amount" ) );
	uintptr_t item_uid = g_il2mgr.value( xs( "Item" ), xs( "uid" ) );
	uintptr_t playerinventory_containerBelt = g_il2mgr.value( xs( "PlayerInventory" ), xs( "containerBelt" ) );
	uintptr_t playerinventory_containerWear = g_il2mgr.value( xs( "PlayerInventory" ), xs( "containerWear" ) );

	uintptr_t itemcontainer_itemList = g_il2mgr.value( xs( "ItemContainer" ), xs( "itemList" ) );
	uintptr_t projectile_integrity = g_il2mgr.value( xs( "Projectile" ), xs( "integrity" ) );
	uintptr_t projectile_traveledDistance = g_il2mgr.value( xs( "Projectile" ), xs( "traveledDistance" ) );
	uintptr_t projectile_maxDistance = g_il2mgr.value( xs( "Projectile" ), xs( "maxDistance" ) );
	uintptr_t projectile_traveledTime = g_il2mgr.value( xs( "Projectile" ), xs( "traveledTime" ) );
	uintptr_t projectile_owner = g_il2mgr.value( xs( "Projectile" ), xs( "owner" ) );
	uintptr_t projectile_mod = g_il2mgr.value( xs( "Projectile" ), xs( "mod" ) );
	uintptr_t itemmodprojectile_projectileSpread = g_il2mgr.value( xs( "ItemModProjectile" ), xs( "projectileSpread" ) );
	uintptr_t itemmodprojectile_projectileVelocity = g_il2mgr.value( xs( "ItemModProjectile" ), xs( "projectileVelocity" ) );
	uintptr_t itemmodprojectile_projectileVelocitySpread = g_il2mgr.value( xs( "ItemModProjectile" ), xs( "projectileVelocitySpread" ) );
	uintptr_t projectile_initialVelocity = g_il2mgr.value( xs( "Projectile" ), xs( "initialVelocity" ) );
	uintptr_t projectile_previousVelocity = g_il2mgr.value( xs( "Projectile" ), xs( "previousVelocity" ) );
	uintptr_t projectile_currentVelocity = g_il2mgr.value( xs( "Projectile" ), xs( "currentVelocity" ) );
	uintptr_t projectile_drag = g_il2mgr.value( xs( "Projectile" ), xs( "drag" ) );
	uintptr_t projectile_gravityModifier = g_il2mgr.value( xs( "Projectile" ), xs( "gravityModifier" ) );
	uintptr_t projectile_thickness = g_il2mgr.value( xs( "Projectile" ), xs( "thickness" ) );
	uintptr_t projectile_hittest = g_il2mgr.value( xs( "Projectile" ), xs( "hitTest" ) );

	
	uintptr_t hittest_hitEntity = g_il2mgr.value( xs( "HitTest" ), xs( "HitEntity" ) );
	uintptr_t hittest_hitNormal = g_il2mgr.value( xs( "HitTest" ), xs( "HitNormal" ) );
	uintptr_t hittest_hitTransform = g_il2mgr.value( xs( "HitTest" ), xs( "HitTransform" ) );
	uintptr_t hittest_hitPoint = g_il2mgr.value( xs( "HitTest" ), xs( "HitPoint" ) );
	uintptr_t hittest_hitDistance = g_il2mgr.value( xs( "HitTest" ), xs( "HitDistance" ) );
	uintptr_t hittest_attackray = g_il2mgr.value( xs( "HitTest" ), xs( "AttackRay" ) );
	uintptr_t hittest_maxDistance = g_il2mgr.value( xs( "HitTest" ), xs( "MaxDistance" ) );
	uintptr_t hittest_didHit = g_il2mgr.value( xs( "HitTest" ), xs( "DidHit" ) );
	uintptr_t hittest_damageProperties = g_il2mgr.value( xs( "HitTest" ), xs( "damageProperties" ) );
	uintptr_t playerinput_bodyAngles = g_il2mgr.value( xs( "PlayerInput" ), xs( "bodyAngles" ) );
	uintptr_t playerinput_headRotation = g_il2mgr.value( xs( "PlayerInput" ), xs( "headRotation" ) );
	uintptr_t event_get_current = g_il2mgr.method( xs( "Event" ), xs( "get_current" ), 0, xs( "UnityEngine" ) );
	uintptr_t event_get_type = g_il2mgr.method( xs( "Event" ), xs( "get_type" ), 0, xs( "UnityEngine" ) );
	uintptr_t cursor_set_lockState = g_il2mgr.method( xs( "Cursor" ), xs( "set_lockState" ), 1, xs( "UnityEngine" ) );
	uintptr_t gui_get_skin = g_il2mgr.method( xs( "GUI" ), xs( "get_skin" ), 0, xs( "UnityEngine" ) );
	uintptr_t gui_set_color = g_il2mgr.method( xs( "GUI" ), xs( "set_color" ), 1, xs( "UnityEngine" ) );
	uintptr_t gui_drawtexture = g_il2mgr.method( xs( "GUI" ), xs( "DrawTexture" ), 2, xs( "UnityEngine" ) );
	uintptr_t gui_label = g_il2mgr.method( xs( "GUI" ), xs( "Label" ), 3, xs( "UnityEngine" ) );
	uintptr_t texture2d_get_whitetexture = g_il2mgr.method( xs( "Texture2D" ), xs( "get_whiteTexture" ), 0, xs( "UnityEngine" ) );
	uintptr_t guistyle_set_fontsize = g_il2mgr.method( xs( "GUIStyle" ), xs( "set_fontSize" ), 1, xs( "UnityEngine" ) );
	uintptr_t guistyle_set_alignment = g_il2mgr.method( xs( "GUIStyle" ), xs( "set_alignment" ), 1, xs( "UnityEngine" ) );
	uintptr_t guicontent_temp = g_il2mgr.method( xs( "GUIContent" ), xs( "Temp" ), 1, xs( "UnityEngine" ) );
	uintptr_t ddraw_get = g_il2mgr.method( xs( "DDraw" ), xs( "Get" ), 0, xs( "UnityEngine" ) );
	uintptr_t ddraw_line = g_il2mgr.method( xs( "DDraw" ), xs( "Line" ), 6, xs( "UnityEngine" ) );
	uintptr_t ddraw_sphere = g_il2mgr.method( xs( "DDraw" ), xs( "Sphere" ), 5, xs( "UnityEngine" ) );

	uintptr_t ddraw_arrow = g_il2mgr.method( xs( "DDraw" ), xs( "Arrow" ), 5, xs( "UnityEngine" ) );
	uintptr_t ddraw_text = g_il2mgr.method( xs( "DDraw" ), xs( "Text" ), 4, xs( "UnityEngine" ) );
	uintptr_t modelstate_set_onLadder = g_il2mgr.method( xs( "ModelState" ), xs( "set_onLadder" ), 1 );
	uintptr_t modelstate_set_sprinting = g_il2mgr.method( xs( "ModelState" ), xs( "set_sprinting" ), 1 );
	uintptr_t SetFlying = g_il2mgr.method( xs( "ModelState" ), xs( "set_flying" ), 1 ); 
	uintptr_t SetMounted = g_il2mgr.method( xs( "ModelState" ), xs( "set_mounted" ), 1 );
	uintptr_t set_pose_type = g_il2mgr.value( xs( "ModelState" ), xs( "poseType" ) );
	uintptr_t waterLevel = g_il2mgr.value( xs( "ModelState" ), xs( "waterLevel" ) );	
	uintptr_t get_ducked = g_il2mgr.method( xs( "ModelState" ), xs( "get_ducked" ), 0 );

	uintptr_t gamemanager_createprefab = g_il2mgr.method( xs( "GameManager" ), xs( "CreatePrefab" ), 2 );
	uintptr_t prefab = g_il2mgr.method( xs( "BaseNetworkable" ), xs( "ShortPrefabName" ), 0 );

	uintptr_t gamemanager_destroy = g_il2mgr.method( xs( "GameManager" ), xs( "Destroy" ), 2 );
	uintptr_t buildingblock_canaffordupgrade = g_il2mgr.method( xs( "BuildingBlock" ), xs( "CanAffordUpgrade" ), 3 );
	uintptr_t buildingblock_canchangetograde = g_il2mgr.method( xs( "BuildingBlock" ), xs( "CanChangeToGrade" ), 3 );
	uintptr_t buildingblock_upgradetograde = g_il2mgr.method( xs( "BuildingBlock" ), xs( "UpgradeToGrade" ), 3 );
	uintptr_t baseplayer_consolemessage = g_il2mgr.method( xs( "BasePlayer" ), xs( "ConsoleMessage" ), 1 );
	
	uintptr_t Settime = g_il2mgr.method( xs( "Time" ), xs( "set_timeScale" ), 1, xs( "UnityEngine" ) );
	uintptr_t set_atmosphere_rayleigh = g_il2mgr.method( xs( "Weather" ), xs( "set_atmosphere_rayleigh" ), 1, xs( "ConVar" ) );
	uintptr_t set_atmosphere_mie = g_il2mgr.method( xs( "Weather" ), xs( "set_atmosphere_mie" ), 1, xs( "ConVar" ) );
	uintptr_t set_atmosphere_brightness = g_il2mgr.method( xs( "Weather" ), xs( "set_atmosphere_brightness" ), 1, xs( "ConVar" ) );

	uintptr_t set_rain = g_il2mgr.method( xs( "Weather" ), xs( "set_rain" ), 1, xs( "ConVar" ) );
	uintptr_t set_storm_chance = g_il2mgr.method( xs( "Weather" ), xs( "set_storm_chance" ), 1, xs( "ConVar" ) );
	uintptr_t set_fog_chance = g_il2mgr.method( xs( "Weather" ), xs( "set_fog_chance" ), 1, xs( "ConVar" ) );

	uintptr_t alwayssprint = g_il2mgr.method( xs( "Input" ), xs( "alwayssprint" ), 1, xs( "ConVar" ) );

	uintptr_t time_get_time = g_il2mgr.method( xs( "Time" ), xs( "get_time" ), 0, xs( "UnityEngine" ) );
	uintptr_t time_get_deltaTime = g_il2mgr.method( xs( "Time" ), xs( "get_deltaTime" ), 0, xs( "UnityEngine" ) );
	uintptr_t debug_draw_ray = g_il2mgr.method( xs( "Debug" ), xs( "DrawLine" ), 4, xs( "UnityEngine" ) );
	uintptr_t create = g_il2mgr.method( xs( "GameObject" ), xs( "Internal_CreateGameObject" ), 2, xs( "UnityEngine" ) );
	uintptr_t dont_destroy_on_load = g_il2mgr.method( xs( "Object" ), xs( "DontDestroyOnLoad" ), 1, xs( "UnityEngine" ) );
	uintptr_t object_name = g_il2mgr.value( xs( "Object" ), xs( "name" ) );
	uintptr_t add_component = g_il2mgr.method( xs( "GameObject" ), xs( "AddComponent" ), 1, xs( "UnityEngine" ) );
	uintptr_t guiutility_rotatearoundpivot = g_il2mgr.method( xs( "GUIUtility" ), xs( "RotateAroundPivot" ), 2, xs( "UnityEngine" ) );
	uintptr_t randomrange = g_il2mgr.method( xs( "Random" ), xs( "Range" ), 2, xs( "UnityEngine" ) );
	uintptr_t baseplayer_mounted = g_il2mgr.value( xs( "BasePlayer" ), xs( "mounted" ) );
	uintptr_t basemountable_canwielditems = g_il2mgr.value( xs( "BaseMountable" ), xs( "canWieldItems" ) );
	uintptr_t basemountable_ignoreVehicleParent = g_il2mgr.value( xs( "BaseMountable" ), xs( "ignoreVehicleParent" ) );
	uintptr_t tod_sky_get_instance = g_il2mgr.method( xs( "TOD_Sky" ), xs( "get_Instance" ), 0 );
	uintptr_t attackentity_nextattacktime = g_il2mgr.value( xs( "AttackEntity" ), xs( "nextAttackTime" ) );
	uintptr_t attackentity_timesincedeploy = g_il2mgr.value( xs( "AttackEntity" ), xs( "timeSinceDeploy" ) );
	uintptr_t attackentity_deploydelay = g_il2mgr.value( xs( "AttackEntity" ), xs( "deployDelay" ) );
	uintptr_t basemelee_damageproperties = g_il2mgr.value( xs( "BaseMelee" ), xs( "damageProperties" ) );
	uintptr_t basemelee_processattack = g_il2mgr.method( xs( "BaseMelee" ), xs( "ProcessAttack" ), 1 );
	uintptr_t basemelee_blockSprintOnAttack = g_il2mgr.value( xs( "BaseMelee" ), xs( "blockSprintOnAttack" ) );
	uintptr_t basemelee_isAutomatic = g_il2mgr.method( xs( "BaseMelee" ), xs( "isAutomatic" ), 1 );

	uintptr_t attackentity_hasattackcooldown = g_il2mgr.method( xs( "AttackEntity" ), xs( "HasAttackCooldown" ), 0 );

	uintptr_t attackentity_startattackcooldown = g_il2mgr.method( xs( "AttackEntity" ), xs( "StartAttackCooldown" ), 1 );
	uintptr_t playereyes_bodyforward = g_il2mgr.method( xs( "PlayerEyes" ), xs( "BodyForward" ), 0 );
	uintptr_t playereyes_movementright = g_il2mgr.method( xs( "PlayerEyes" ), xs( "MovementRight" ), 0 );
	uintptr_t playereyes_movementforward = g_il2mgr.method( xs( "PlayerEyes" ), xs( "MovementForward" ), 0 );

	uintptr_t playereyes_bodyright = g_il2mgr.method( xs( "PlayerEyes" ), xs( "BodyRight" ), 0 );
	uintptr_t playereyes_eye_rotation = g_il2mgr.method( xs( "PlayerEyes" ), xs( "get_rotation" ), 0 );

	uintptr_t physics_sphere_cast = g_il2mgr.method( xs( "Physics" ), xs( "SphereCast" ), 4, xs( "UnityEngine" ) );
	uintptr_t physics_checkcapsule = g_il2mgr.method( xs( "Physics" ), xs( "CheckCapsule" ), 5, xs( "UnityEngine" ) );
	uintptr_t physics_get_gravity = g_il2mgr.method( xs( "Physics" ), xs( "gravity" ), 0, xs( "UnityEngine" ) );

	uintptr_t playertick_position = g_il2mgr.value( xs( "PlayerTick" ), xs( "position" ) );
	uintptr_t rigidbody_setVelocity = g_il2mgr.method( xs( "Rigidbody" ), xs( "set_velocity" ), 1, xs( "UnityEngine" ) );
	uintptr_t basemodel = g_il2mgr.value( xs( "BasePlayer" ), xs( "PlayerModel" ) );
	
	uintptr_t reloadStartDuration = g_il2mgr.value( xs( "BaseProjectile" ), xs( "reloadStartDuration" ) );
	uintptr_t reloadTime = g_il2mgr.value( xs( "BaseProjectile" ), xs( "reloadTime" ) );
	uintptr_t nextReloadTime = g_il2mgr.value( xs( "BaseProjectile" ), xs( "nextReloadTime" ) );

	uintptr_t OnLand = g_il2mgr.method( xs( "BasePlayer" ), xs( "OnLand" ), 1 );
	uintptr_t GetComponent = g_il2mgr.method( xs( "Component" ), xs( "GetComponent" ), 1, xs( "UnityEngine" ) );
	uintptr_t GetComponentsInChildren = g_il2mgr.method( xs( "Component" ), xs( "GetComponentsInChildren" ), 1, xs( "UnityEngine" ) );
	
	uintptr_t Sp = g_il2mgr.method( xs( "BasePlayer" ), xs( "set_displayName" ), 1 );
}