#pragma once
#include "../Base/character_base.hpp"
#include "../Interface/i_weapon_equippable.hpp"
#include "../Interface/i_fireable.hpp"
#include "../Interface/i_grabbable.hpp"
#include "../Interface/i_melee_attackable.hpp"
#include "../Interface/i_stealth_killer.hpp"

#include "../GameTime/game_time_manager.hpp"
#include "../Event/event_system.hpp"
#include "../Animator/player_animator.hpp"

#include "assault_rifle.hpp"
#include "rocket_launcher.hpp"
#include "knife.hpp"
#include "../Part/weapon_shortcut_selecter.hpp"
#include "../Part/bone_pos_corrector.hpp"
#include "../Part/melee_target_searcher.hpp"

class PlayerStateController;

class Player final : public CharacterBase, public IWeaponEquippable, public IFireable, public IGrabbable, public IMeleeAttackable, public IStealthKiller
{
public:
	Player();
	~Player() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;
	void OnDamage(const HealthPartKind part_kind, const float damage) override;

	void SetRemainingBulletNum(const int remaining_bullet_num) override { m_current_remaining_bullet_num = remaining_bullet_num; }


	#pragma region 掴み
	void UpdateGrabbed() override;
	void OnGrabbed(const std::shared_ptr<IGrabber> grabber, const VECTOR& brabber_pos, const VECTOR& brabber_dir) override;
	void OnRelease() override;

	[[nodiscard]] std::shared_ptr<IGrabber> GetGrabber()		const override	{ return m_grabber; }
	[[nodiscard]] std::shared_ptr<Gauge>	GetEscapeGauge()	const override	{ return m_escape_gauge; }
	[[nodiscard]] bool						CanEscape()			const override;
	[[nodiscard]] bool						IsGrabbed()			const override	{ return m_is_grabbed; }
	[[nodiscard]] bool						IsEscape()			const override	{ return m_is_escape; }
	#pragma endregion


	#pragma region メレー
	void UpdateMelee()	 override;
	void StopSearchMeleeTarget() override { m_can_search_melee_target = false; }

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
	[[nodiscard]] bool								CanSearchMeleeTarget()		const	override	{ return m_can_search_melee_target; }
	#pragma endregion


	#pragma region ステルスキル
	void UpdateStealthKill() override;
	void StopSearchStealthKillTarget() override { m_can_search_stealth_kill_target = false; }
	void SetupStealthKill() override;

	void AddStealthKillTarget(const std::shared_ptr<IStealthKillable>& stealth_kill_target) override { m_stealth_kill_target = stealth_kill_target; }
	void RemoveStealthKillTarget() override { m_stealth_kill_target = nullptr; }

	[[nodiscard]] std::shared_ptr<IStealthKillable>&	GetStealthKillTarget()					override	{ return m_stealth_kill_target; }
	[[nodiscard]] bool									CanSearchStealthKillTarget()	const	override	{ return m_can_search_stealth_kill_target; }
	#pragma endregion


	#pragma region アイテム
	/// @brief アイテムの所持登録 
	template<obj_concepts::ItemT ItemT>
	void AddItem(const std::shared_ptr<ItemT>& item)
	{
		const auto item_kind = item->GetItemKind();

		if (std::find(m_items[item_kind].begin(), m_items[item_kind].end(), item) == m_items[item_kind].end())
		{
			m_items[item_kind].emplace_back(item);
		}
	}
	/// @brief アイテムの所持登録を解除
	template<obj_concepts::ItemT ItemT>
	void RemoveItem(const std::shared_ptr<ItemT>& item)
	{
		const auto item_kind = item->GetItemKind();

		m_items[item_kind].erase(std::remove(m_items[item_kind].begin(), m_items[item_kind].end(), item), m_items[item_kind].end());
	}
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

	[[nodiscard]] std::shared_ptr<WeaponBase>	GetCurrentEquipWeapon		(const WeaponSlotKind slot_kind) const override;
	[[nodiscard]] std::shared_ptr<WeaponBase>	GetCurrentHeldWeapon		()	override;
	[[nodiscard]] std::shared_ptr<WeaponBase>	GetCurrentAttachWeapon		(const HolsterKind holster_kind) const override;
	[[nodiscard]] WeaponKind					GetCurrentEquipWeaponKind	(const WeaponSlotKind slot_kind) override;
	[[nodiscard]] WeaponKind					GetCurrentHeldWeaponKind	()	override;
	[[nodiscard]] WeaponKind					GetCurrentAttachWeaponKind	(const HolsterKind holster_kind) const override;
	#pragma endregion


	#pragma region State
	void Move();

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
	[[nodiscard]] float																GetDeltaTime				()	const override;
	[[nodiscard]] std::shared_ptr<PlayerStateController>							GetStateController			()	const			{ return m_state; }
	[[nodiscard]] std::shared_ptr<BonePosCorrector>									GetBonePosCorrector			()	const			{ return m_bone_pos_corrector; }
	[[nodiscard]] std::vector<std::shared_ptr<IItem>>								GetCurrentHaveItem			(const ItemKind item_kind) const { return m_items.at(item_kind); }
	[[nodiscard]] std::unordered_map<WeaponSlotKind, std::shared_ptr<WeaponBase>>&	GetCurrentEquipWeapons		()					{ return m_current_equip_weapon; }
	[[nodiscard]] std::shared_ptr<WeaponShortcutSelecter>							GetWeaponShortcutSelecter	()	const			{ return m_weapon_shortcut_selecter; }
	[[nodiscard]] float																GetMoveSpeed				()	const			{ return m_move_speed; }
	[[nodiscard]] int																GetCurrentRemainingBulletNum()	const override	{ return m_current_remaining_bullet_num; }
	#pragma endregion

private:
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

	//static constexpr float kCrouchWalkSpeed						= 10.0f;
	static constexpr float kSlowWalkSpeed						= 30.0f;
	static constexpr float kWalkSpeed							= 70.0f;
	static constexpr float kRunSpeed							= 125.0f;
	static constexpr float kAcceleration						= 1.0f;					// 加速度(減速度も共通)

	static constexpr float kMoveDirOffsetSpeed					= 5.0f;					// 移動方向の補正速度
	static constexpr float kLookDirOffsetSpeed					= 4.0f;					// 見る方向の補正角度
	static constexpr float kLookDirOffsetSpeedForRun			= 6.0f;					// ダッシュ時の見る方向の補正角度
	static constexpr float kLookDirOffsetSpeedForAim			= 10.0f;					// エイミング時の見る方向を回転させる角度
	static constexpr float kLookDirOffsetSpeedForSideSlash		= 0.5f;					// 切り裂き攻撃時の見る方向を回転させる角度
	static constexpr float kConfirmMoveDirThresholdDistance		= 0.08f;				// 目的の移動方向に到達したと判定する閾値

	static constexpr int   kWalkStickSlopeLimit					= 15000;				// 歩き状態とするスティック傾きの上限
	//static constexpr float kTurnAroundStickAngle				= 30.0f;				// 振り向きを行うスティックの入力角度

	static constexpr float  kCapsuleRadius						= 8.0f;
	static constexpr float  kCollisionAreaRadius				= 37.0f;
	static constexpr VECTOR kCollisionAreaOffset				= { 0.0f, 26.0f, 0.0f };
	static constexpr float  kLandingTriggerRadius				= 6.0f;

	static constexpr float kCameraAimOffsetBasicSpeed			= 40.0f;
	
	//static constexpr float kADSSpeed							= 70.0f;				// スコープをのぞき込む速度

private:
	std::shared_ptr<PlayerStateController>		m_state;
	std::shared_ptr<BonePosCorrector>			m_bone_pos_corrector;

	VECTOR										m_input_slope;

	float										m_prev_health;
	bool										m_is_grabbed;							// 捕まれたかを判定
	bool										m_is_escape;
	float										m_escape_start_timer;
	bool										m_can_search_stealth_kill_target;
	bool										m_can_search_melee_target;

	std::unordered_map<ItemKind, std::vector<std::shared_ptr<IItem>>>	m_items;							// 所持しているアイテム
	std::unordered_map<WeaponSlotKind, std::shared_ptr<WeaponBase>>		m_current_equip_weapon;				// 現在装備している武器
	std::shared_ptr<WeaponBase>											m_current_held_weapon;				// 現在手に持っている武器
	std::unordered_map<HolsterKind, std::shared_ptr<WeaponBase>>		m_attach_weapons;					// 装着している武器
	int																	m_current_remaining_bullet_num;		// 残弾数
	std::shared_ptr<WeaponShortcutSelecter>								m_weapon_shortcut_selecter;			// ショートカットに登録されている武器
	std::shared_ptr<IMeleeHittable>										m_melee_target;
	std::shared_ptr<IMeleeHittable>										m_top_priority_downed_chara;
	std::shared_ptr<IStealthKillable>									m_stealth_kill_target;
	std::shared_ptr<IGrabber>											m_grabber;
	std::shared_ptr<Gauge>												m_escape_gauge;

	int test;
};
