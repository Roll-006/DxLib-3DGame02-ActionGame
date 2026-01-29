#pragma once
#include "../Base/character_base.hpp"
#include "../Interface/i_playable_character.hpp"
#include "../Interface/i_humanoid.hpp"
#include "../Interface/i_item_collectable.hpp"
#include "../Interface/i_weapon_equippable.hpp"
#include "../Interface/i_fireable.hpp"
#include "../Interface/i_grabbable.hpp"
#include "../Interface/i_melee_attackable.hpp"
#include "../Interface/i_stealth_killer.hpp"

#include "../GameTime/game_time_manager.hpp"
#include "../Event/event_system.hpp"

#include "../InverseKinematics/humanoid_foot_ik_solver.hpp"
#include "../State/PlayerState/player_state.hpp"

#include "assault_rifle.hpp"
#include "rocket_launcher.hpp"
#include "knife.hpp"
#include "../Part/weapon_shortcut_selecter.hpp"
#include "../Part/frame_pos_corrector.hpp"
#include "../Part/melee_target_searcher.hpp"

class Player final : 
	public CharacterBase, 
	public IPlayableCharacter, 
	public IHumanoid, 
	public IItemCollectable, 
	public IWeaponEquippable, 
	public IFireable, 
	public IGrabbable, 
	public IMeleeAttackable, 
	public IStealthKiller
{
public:
	Player();
	~Player() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;
	void OnProjectPos() override;
	void OnDamage(const HealthPartKind part_kind, const float damage) override;


	#pragma region インターフェイス群

	#pragma region 操作キャラクター
	void OnAllowControl()	 override { m_can_control = true; }
	void OnDisallowControl() override { m_can_control = false; }

	[[nodiscard]] const bool CanControl() const override { return m_can_control; }
	#pragma endregion


	#pragma region Humanoid
	[[nodiscard]] const std::shared_ptr<const HumanoidFrameGetter> GetHumanoidFrame() const override { return m_humanoid_frame; }
	#pragma endregion


	#pragma region アイテム
	void StopAddPickupableItem() override { m_can_add_acquirable_item = false; }

	void PickUpItem() override;

	void AddItem	(const std::shared_ptr<IItem>& item) override;
	void RemoveItem	(const std::shared_ptr<IItem>& item) override;

	void AddPickUpCandidateItem(const SpottedItemEvent& event) override;
	void RemovePickUpCandidateItem(const int obj_handle) override;
	void RemovePickUpCandidateItems() override { m_pick_up_candidate_items.clear(); }

	void AddPickupableItem(const std::shared_ptr<IItem>& item) override { m_pickupable_item = item; }
	void RemovePickupableItem() override { m_pickupable_item = nullptr; }

	[[nodiscard]] std::shared_ptr<IItem>&				GetPickupableItem()		override { return m_pickupable_item; }
	[[nodiscard]] const std::vector<SpottedObjData>&	GetCandidateItems()		override { return m_pick_up_candidate_items; }
	[[nodiscard]] const bool							CanAddPickupableItem()	override { return m_can_add_acquirable_item; }
	#pragma endregion


	#pragma region 武器
	void EquipWeapon	(const std::shared_ptr<WeaponBase>& weapon, const WeaponSlotKind slot_kind)	override;
	void UnequipWeapon	(const WeaponSlotKind slot_kind)			override;

	void HoldWeapon		(const std::shared_ptr<WeaponBase>& weapon)	override;
	void HoldWeapon		(const int obj_handle)						override;
	void ReleaseWeapon	()											override;

	void AttachWeapon	(const std::shared_ptr<WeaponBase>& weapon)	override;
	void AttachWeapon	(const int obj_handle)						override;
	void DetachWeapon	(const std::shared_ptr<WeaponBase>& weapon)	override;
	void DetachWeapon	(const HolsterKind holster_kind)			override;

	[[nodiscard]] const std::shared_ptr<WeaponBase>	GetCurrentEquipWeapon		(const WeaponSlotKind slot_kind) const override;
	[[nodiscard]] const std::shared_ptr<WeaponBase>	GetCurrentHeldWeapon		()	override;
	[[nodiscard]] const std::shared_ptr<WeaponBase>	GetCurrentAttachWeapon		(const HolsterKind holster_kind) const override;
	[[nodiscard]] const WeaponKind					GetCurrentEquipWeaponKind	(const WeaponSlotKind slot_kind) override;
	[[nodiscard]] const WeaponKind					GetCurrentHeldWeaponKind	()	override;
	[[nodiscard]] const WeaponKind					GetCurrentAttachWeaponKind	(const HolsterKind holster_kind) const override;
	#pragma endregion


	#pragma region 銃を撃つ者
	[[nodiscard]] const std::shared_ptr<AmmoHolder> GetAmmoHolder() const override { return m_ammo_holder; }
	#pragma endregion


	#pragma region 掴み
	void UpdateGrabbed() override;
	void OnGrabbed(const std::shared_ptr<IGrabber> grabber, const VECTOR& brabber_pos, const VECTOR& brabber_dir) override;
	void OnRelease() override;

	[[nodiscard]] const std::shared_ptr<const IGrabber>	GetGrabber()		const override	{ return m_grabber; }
	[[nodiscard]] const std::shared_ptr<const Gauge>	GetEscapeGauge()	const override	{ return m_escape_gauge; }
	[[nodiscard]] const bool							CanEscape()			const override;
	[[nodiscard]] const bool							IsGrabbed()			const override	{ return m_is_grabbed; }
	[[nodiscard]] const bool							IsEscape()			const override	{ return m_is_escape; }
	#pragma endregion


	#pragma region メレー
	void UpdateMelee()				override;
	void StopSearchMeleeTarget()	override { m_can_search_melee_target = false; }

	void SetupFrontMelee()			override;
	void SetupBackMelee	()			override;
	void SetupVersatilityMelee()	override;

	void AttackFrontMelee		(CharacterBase* target) override;
	void AttackBackMelee		(CharacterBase* target) override;
	void AttackVersatilityMelee	(CharacterBase* target) override;
	
	void AddTopPriorityDownedChara	(const std::shared_ptr<IMeleeHittable>& downed_character)	override { m_top_priority_downed_chara = downed_character; }
	void AddMeleeTarget				(const std::shared_ptr<IMeleeHittable>& melee_target)		override { m_melee_target = melee_target; }
	void RemoveTopPriorityDownedChara()	override { m_top_priority_downed_chara = nullptr; }
	void RemoveMeleeTarget()			override { m_melee_target = nullptr; }

	[[nodiscard]] std::shared_ptr<IMeleeHittable>&	GetMeleeTarget()					override	{ return m_melee_target; }
	[[nodiscard]] std::shared_ptr<IMeleeHittable>&	GetTopPriorityDownedChara()			override	{ return m_top_priority_downed_chara; }
	[[nodiscard]] const bool						CanSearchMeleeTarget()		const	override	{ return m_can_search_melee_target; }
	#pragma endregion


	#pragma region ステルスキル
	void UpdateStealthKill() override;
	void StopSearchStealthKillTarget() override { m_can_search_stealth_kill_target = false; }
	void SetupStealthKill() override;

	void AddStealthKillTarget(const std::shared_ptr<IStealthKillable>& stealth_kill_target) override { m_stealth_kill_target = stealth_kill_target; }
	void RemoveStealthKillTarget() override { m_stealth_kill_target = nullptr; }

	[[nodiscard]] std::shared_ptr<IStealthKillable>& GetStealthKillTarget() override { return m_stealth_kill_target; }
	[[nodiscard]] const bool CanSearchStealthKillTarget() const	override { return m_can_search_stealth_kill_target; }
	#pragma endregion

	#pragma endregion


	#pragma region State
	void Move();

	void OnFootIK();
	void OnCrouchIK();

	/// @brief エイミング時の見る方向を修正するための値を設定する
	void SetLookDirOffsetValueForAim();

	/// @brief 進行方向を向く
	void DirOfMovement();
	/// @brief カメラのforward(Y軸は0)方向を向く
	void DirOfCameraForward();

	void CalcMoveSpeed();
	void CalcMoveSpeedStop();
	void CalcMoveSpeedRun();

	void SideSlashKnifeOffsetMove();
	void SpinningSlashKnifeOffsetMove();
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] const float																	GetDeltaTime()									const override;
	[[nodiscard]] const std::shared_ptr<const player_state::State>								GetState()										const	{ return m_state; }
	[[nodiscard]] const std::shared_ptr<const FramePosCorrector>								GetFramePosCorrector()							const	{ return m_frame_pos_corrector; }
	[[nodiscard]] const std::vector<std::shared_ptr<IItem>>										GetCurrentHaveItem(const ItemKind item_kind)	const	{ return m_items.at(item_kind); }
	[[nodiscard]] const std::unordered_map<WeaponSlotKind, std::shared_ptr<WeaponBase>>&		GetCurrentEquipWeapons()								{ return m_current_equip_weapon; }
	[[nodiscard]] const std::shared_ptr<const WeaponShortcutSelecter>							GetWeaponShortcutSelecter()						const	{ return m_weapon_shortcut_selecter; }
	[[nodiscard]] const float																	GetMoveSpeed()									const	{ return m_move_speed; }
	[[nodiscard]] const bool																	IsVictoryPose()									const	{ return m_is_victory_pose; }
	[[nodiscard]] const bool																	IsNearDeath()									const	{ return m_is_near_death; }
	#pragma endregion

private:
	#pragma region Event
	void DeadAllEnemy(const DeadAllEnemyEvent& event);
	#pragma endregion


	void JudgeVictoryPose();

	void CalcInputSlopeFromPad();
	void CalcInputSlopeFromCommand();

	void NotifyHealth();

	/// @brief カメラのforwardをもとにした進行方向ベクトルを取得する
	/// @return カメラのforwardのY軸を消し飛ばしたベクトル
	[[nodiscard]] VECTOR GetMoveForward();
	/// @brief カメラのrightをもとにした進行方向ベクトルを取得する
	/// @return カメラのrightのY軸を消し飛ばしたベクトル
	[[nodiscard]] VECTOR GetMoveRight();

private:
	static constexpr VECTOR kBasicAngle							= { 0.0f, DX_PI_F, 0.0f };
	static constexpr float  kBasicScale							= 0.3f;

	static constexpr float  kInvincibleTime						= 0.5f;

	static constexpr float  kSlowWalkSpeed						= 30.0f;
	static constexpr float  kWalkSpeed							= 70.0f;
	static constexpr float  kRunSpeed							= 125.0f;
	static constexpr float  kAcceleration						= 100.0f;					// 加速度(減速度も共通)

	static constexpr float  kMoveDirOffsetSpeed					= 5.0f;					// 移動方向の補正速度
	static constexpr float  kLookDirOffsetSpeed					= 4.0f;					// 見る方向の補正角度
	static constexpr float  kLookDirOffsetSpeedForRun			= 6.0f;					// ダッシュ時の見る方向の補正角度
	static constexpr float  kLookDirOffsetSpeedForAim			= 10.0f;				// エイミング時の見る方向を回転させる角度
	static constexpr float  kLookDirOffsetSpeedForSideSlash		= 0.5f;					// 切り裂き攻撃時の見る方向を回転させる角度
	static constexpr float  kConfirmMoveDirThresholdDistance	= 0.08f;				// 目的の移動方向に到達したと判定する閾値

	static constexpr int    kWalkStickSlopeLimit				= 15000;				// 歩き状態とするスティック傾きの上限

	static constexpr float  kCapsuleRadius						= 8.0f;
	static constexpr float  kCollisionAreaRadius				= 37.0f;
	static constexpr VECTOR kCollisionAreaOffset				= { 0.0f, 26.0f, 0.0f };
	static constexpr float  kLandingTriggerRadius				= 6.0f;

	static constexpr float  kCameraAimOffsetBasicSpeed			= 40.0f;

private:
	std::shared_ptr<player_state::State>		m_state;
	std::shared_ptr<FramePosCorrector>			m_frame_pos_corrector;

	VECTOR										m_input_slope;
	bool										m_can_control;

	float										m_prev_health;
	bool										m_is_grabbed;							// 捕まれたかを判定
	bool										m_is_escape;
	bool										m_is_near_death;
	float										m_escape_start_timer;
	bool										m_can_add_acquirable_item;
	bool										m_can_search_stealth_kill_target;
	bool										m_can_search_melee_target;

	bool										m_is_victory_pose;
	bool										m_is_contains_victory_pose;
	float										m_victory_pose_wait_time;

	std::shared_ptr<AmmoHolder>											m_ammo_holder;

	std::unordered_map<ItemKind, std::vector<std::shared_ptr<IItem>>>	m_items;							// 所持しているアイテム
	std::unordered_map<WeaponSlotKind, std::shared_ptr<WeaponBase>>		m_current_equip_weapon;				// 現在装備している武器
	std::shared_ptr<WeaponBase>											m_current_held_weapon;				// 現在手に持っている武器
	std::unordered_map<HolsterKind, std::shared_ptr<WeaponBase>>		m_attach_weapons;					// 装着している武器
	std::shared_ptr<WeaponShortcutSelecter>								m_weapon_shortcut_selecter;			// ショートカットに登録されている武器
	
	std::shared_ptr<IItem>												m_pickupable_item;					// 取得可能アイテム
	std::vector<SpottedObjData>											m_pick_up_candidate_items;			// 取得候補アイテム

	std::shared_ptr<IMeleeHittable>										m_melee_target;
	std::shared_ptr<IMeleeHittable>										m_top_priority_downed_chara;
	std::shared_ptr<IStealthKillable>									m_stealth_kill_target;

	std::shared_ptr<IGrabber>											m_grabber;
	std::shared_ptr<Gauge>												m_escape_gauge;

	//HumanoidLegRayData													m_leg_ray_data;
	//std::shared_ptr<HumanoidFootIKSolver>								m_humanoid_foot_ik;
	std::shared_ptr<HumanoidFrameGetter>								m_humanoid_frame;
};
