#include "player.hpp"
#include "../Command/command_handler.hpp"
#include "../Part/player_state_controller.hpp"

Player::Player() :
	CharacterBase(ObjName.PLAYER, ObjTag.PLAYER, MassKind::kMedium),
	m_state									(std::make_shared<PlayerStateController>()),
	m_bone_pos_corrector					(std::make_shared<BonePosCorrector>()),
	m_input_slope							(v3d::GetZeroV()),
	m_prev_health							(0.0f),
	m_is_grabbed							(false),
	m_is_escape								(false),
	m_escape_start_timer					(0.0f),
	m_can_search_stealth_kill_target		(true),
	m_can_search_melee_target				(true),
	m_weapon_shortcut_selecter				(std::make_shared<WeaponShortcutSelecter>()),
	m_melee_target							(nullptr),
	m_top_priority_downed_chara				(nullptr),
	m_stealth_kill_target					(nullptr),
	m_grabber								(nullptr),
	m_escape_gauge							(std::make_shared<Gauge>(100.0f))
{
	m_health[HealthPartKind::kMain] = std::make_shared<Gauge>(2000.0f, 1500.0f);
	m_prev_health = m_health.at(HealthPartKind::kMain)->GetCurrentValue();

	// モデル・アニメーションを設定
	m_modeler  = std::make_shared<Modeler>(m_transform, ModelPath.SWAT_02, kBasicAngle, kBasicScale);
	m_animator = std::make_shared<PlayerAnimator>(m_modeler, m_state, m_current_held_weapon, m_current_equip_weapon[WeaponSlotKind::kMain]);
	SetColliderModelHandle(m_modeler->GetModelHandle());

	m_invincible_time = kInvincibleTime;

	// 初期pos・dirを設定
	m_look_dir[TimeKind::kCurrent] = m_look_dir[TimeKind::kNext] = VGet(0.0f, 0.0f, 1.0f);
	m_transform->SetPos(CoordinateKind::kWorld, VGet(362.0f, -102.0f, 109.0f));
	m_transform->SetRot(CoordinateKind::kWorld, m_look_dir.at(TimeKind::kCurrent));

	// コライダー・トリガーを設定
	m_collider_creator->CreateCapsuleCollider	(this, m_modeler, kCapsuleRadius);
	m_collider_creator->CreateLandingTrigger	(this, kLandingTriggerRadius);
	m_collider_creator->CreateVisibleTrigger	(this, m_modeler);

	const auto pos = m_transform->GetPos(CoordinateKind::kWorld);
	AddCollider(std::make_shared<Collider>(ColliderKind::kCollisionAreaTrigger, std::make_shared<Sphere>(pos + kCollisionAreaOffset, kCollisionAreaRadius), this));
	
	// TODO : 仮後に変更
	{
		// 武器設定
		const auto assault_rifle	= std::make_shared<AssaultRifle>();
		const auto rocket_launcher	= std::make_shared<RocketLauncher>();
		const auto knife			= std::make_shared<Knife>();
		assault_rifle  ->AddToObjManager();
		rocket_launcher->AddToObjManager();
		m_weapon_shortcut_selecter->AttachShortcutWeapon(WeaponShortcutPosKind::kInsideLeft, assault_rifle);
		m_weapon_shortcut_selecter->AttachShortcutWeapon(WeaponShortcutPosKind::kOutsideDown, rocket_launcher);
		AddItem(assault_rifle);
		AddItem(rocket_launcher);
		AddItem(knife);
		EquipWeapon(assault_rifle,		WeaponSlotKind::kMain);
		EquipWeapon(rocket_launcher,	WeaponSlotKind::kMain);
		EquipWeapon(knife,				WeaponSlotKind::kSub);
		AttachWeapon(assault_rifle);
		AttachWeapon(rocket_launcher);
		AttachWeapon(knife);

		m_current_remaining_bullet_num = 10000;
	}
}

Player::~Player()
{
	for (const auto& item : m_items)
	{
		for (const auto& i : item.second)
		{
			std::dynamic_pointer_cast<ObjBase>(i)->RemoveToObjManager();
		}
	}
}

void Player::Init()
{

}

void Player::Update()
{
	if (!IsActive()) { return; }

	if (CheckHitKey(KEY_INPUT_0))
	{
		m_health.at(HealthPartKind::kMain)->Decrease(500.0f);
	}

	NotifyHealth();
	JudgeInvincible();

	m_is_calc_look_dir	= false;

	if (m_current_held_weapon) { m_current_held_weapon->Update(); }

	for (const auto& attach_weapon : m_attach_weapons)
	{
		if (attach_weapon.second)
		{
			attach_weapon.second->Update();
		}
	}

	m_move_dir_offset_speed				= kMoveDirOffsetSpeed;
	m_look_dir_offset_speed				= kLookDirOffsetSpeed;
	m_can_search_stealth_kill_target	= true;
	m_can_search_melee_target			= true;

	m_weapon_shortcut_selecter->Update(std::static_pointer_cast<Player>(shared_from_this()));
	m_state					  ->Update(std::static_pointer_cast<Player>(shared_from_this()));
	m_animator				  ->Update();

	CalcMoveDir();
	CalcLookDir();
	CalcMoveVelocity();

	m_collider_creator->CalcCapsuleColliderPos	(m_modeler, m_colliders);
	m_collider_creator->CalcVisibleTriggerPos	(m_modeler, m_colliders);

	ApplyLookDirToRot(m_look_dir.at(TimeKind::kCurrent));
}

void Player::LateUpdate()
{
	if (!IsActive()) { return; }

	m_state->LateUpdate(std::static_pointer_cast<Player>(shared_from_this()));

	if (m_current_held_weapon) { m_current_held_weapon->LateUpdate(); }

	for (const auto& attach_weapon : m_attach_weapons)
	{
		if (attach_weapon.second)
		{
			attach_weapon.second->TrackOwnerHolster();
			attach_weapon.second->LateUpdate();
		}
	}
}

void Player::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	if (m_current_held_weapon) { m_current_held_weapon->Draw(); }

	for (const auto& attach_weapon : m_attach_weapons)
	{
		if (attach_weapon.second) { attach_weapon.second->Draw(); }
	}

	DrawColliders();

	const auto p = m_transform->GetPos(CoordinateKind::kWorld);
	printfDx("%f, %f, %f\n", p.x, p.y, p.z);
}

void Player::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	PhysicalObjBase*	target_obj				= hit_collider_pair.target_collider->GetOwnerObj();
	const auto			target_name				= target_obj->GetName();
	const auto			target_tag				= target_obj->GetTag();
	const auto			target_collider_kind	= hit_collider_pair.target_collider->GetColliderKind();
	const auto			action_state_kind		= static_cast<player_state::ActionStateKind>(m_state->GetActionState(TimeKind::kCurrent)->GetStateKind());

	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kLandingTrigger:
		m_is_landing = true;
		break;
		 
	case ColliderKind::kAttackTrigger:
		if (target_tag == ObjTag.ENEMY && target_collider_kind == ColliderKind::kCollider)
		{
			const auto character = dynamic_cast<CharacterBase*>(target_obj);
			if (action_state_kind == player_state::ActionStateKind::kFrontKick)
			{
				AttackFrontMelee(character);
			}
			else if (action_state_kind == player_state::ActionStateKind::kRoundhouseKick)
			{
				AttackVersatilityMelee(character);
			}
		}
		break;

	default:
		break;
	}
}

void Player::OnDamage(const HealthPartKind part_kind, const float damage)
{
	if (!m_health.count(part_kind)) { return; }

	m_health.at(part_kind)->Decrease(damage);
	m_invincible_timer	= m_invincible_time;
	m_is_invincible		= true;

	// 通知処理
	if (part_kind == HealthPartKind::kMain)
	{
		// ダメージ通知
		const OnDamageToPlayerEvent event{ damage, damage / m_health.at(part_kind)->GetMaxValue() };
		EventSystem::GetInstance()->Publish(event);
	}
}


#pragma region 掴み
void Player::UpdateGrabbed()
{
	m_move_speed = 20.0f;

	AllowCalcLookDir();
	CalcCorrectMoveDir();
	ReleaseWeapon();

	const auto delta_time = GetDeltaTime();
	m_escape_start_timer += delta_time;

	if (m_escape_start_timer >= m_grabber->GetDamageOverTimeStartTime())
	{
		const auto command		= CommandHandler::GetInstance();
		const auto input_mode	= command->GetInputModeKind(CommandKind::kEscape);
		auto	   increase		= 0.0f;

		switch (input_mode)
		{
		case InputModeKind::kSingle:
			increase = m_escape_gauge->GetMaxValue() / 16;
			break;

		case InputModeKind::kHold:
			increase = m_escape_gauge->GetMaxValue() / 1.8f * delta_time;
			break;

		default:
			break;
		}

		// 入力されたかつデルタタイムが有効な場合のみ値を増加
		if (delta_time > 0.0f && command->IsExecute(CommandKind::kEscape, TimeKind::kCurrent))
		{
			m_escape_gauge->Increase(increase);
		}

		// 脱出
		if (m_escape_gauge->IsMax())
		{
			m_grabber->OnEscape();
			m_is_escape = true;
		}
	}
}

void Player::OnGrabbed(const std::shared_ptr<IGrabber> grabber, const VECTOR& brabber_pos, const VECTOR& brabber_dir)
{
	m_is_grabbed	= true;
	m_is_escape		= false;
	m_grabber		= grabber;

	m_look_dir.at(TimeKind::kNext) = -brabber_dir;
	m_destination_pos = brabber_pos + brabber_dir * 17.0f;

	m_escape_start_timer = 0.0f;
	m_escape_gauge->DecreaseZero();
}

void Player::OnRelease()
{
	m_is_grabbed	= false;
	m_grabber		= nullptr;
}

bool Player::CanEscape() const
{
	return m_grabber ? m_escape_start_timer >= m_grabber->GetDamageOverTimeStartTime() : false;
}
#pragma endregion


#pragma region メレー
void Player::UpdateMelee()
{
	m_move_speed			= 40.0f;
	m_look_dir_offset_speed = 7.0f;

	AllowCalcLookDir();
	CalcCorrectMoveDir();

	// 強制的に無敵状態にする
	ActivateInvincibleForcibly();
}

void Player::SetupFrontMelee()
{
	// front kick
	const auto target_obj		= std::dynamic_pointer_cast<ObjBase>(m_melee_target);
	const auto target_transform = target_obj->GetTransform();
	const auto target_pos		= target_transform->GetPos(CoordinateKind::kWorld);
	const auto target_forward	= target_transform->GetForward(CoordinateKind::kWorld);

	m_look_dir.at(TimeKind::kNext) = -target_forward;
	m_destination_pos = target_pos + target_forward * 24.0f;

	RemoveMeleeTarget();
}

void Player::SetupBackMelee()
{
	// suplex
	const auto target_obj		= std::dynamic_pointer_cast<ObjBase>(m_melee_target);
	const auto target_transform = target_obj->GetTransform();
	const auto target_pos		= target_transform->GetPos(CoordinateKind::kWorld);
	const auto target_forward	= target_transform->GetForward(CoordinateKind::kWorld);

	m_look_dir.at(TimeKind::kNext) = target_forward;
	m_destination_pos = target_pos - target_forward * 24.0f;

	RemoveMeleeTarget();
}

void Player::SetupVersatilityMelee()
{
	// roundhouse kick
	const auto target_obj		= std::dynamic_pointer_cast<ObjBase>(m_melee_target);
	const auto target_transform = target_obj->GetTransform();
	const auto target_pos		= target_transform->GetPos(CoordinateKind::kWorld);

	const auto pos				= m_transform->GetPos(CoordinateKind::kWorld);
	const auto pos_y0			= VGet(pos.x, 0.0f, pos.z);
	const auto target_pos_y0	= VGet(target_pos.x, 0.0f, target_pos.z);
	const auto dir				= v3d::GetNormalizedV(target_pos_y0 - pos_y0);

	m_look_dir.at(TimeKind::kNext) = dir;
	m_destination_pos = target_pos - dir * 24.0f;

	RemoveMeleeTarget();
}

void Player::AttackFrontMelee(CharacterBase* target)
{
	// front kick
	if (target->IsInvincible()) { return; }

	const auto dir = v3d::GetNormalizedV(m_transform->GetForward(CoordinateKind::kWorld) + VGet(0.0f, 0.5f, 0.0f));
	target->OnDamage(HealthPartKind::kMain, 1000.0f);
	target->OnKnockback(dir, 170.0f, 100.0f);

	const auto time_manager = GameTimeManager::GetInstance();
	time_manager->SetTimeScale(TimeScaleLayerKind::kWorld,  0.07f, 0.4f);
	time_manager->SetTimeScale(TimeScaleLayerKind::kPlayer, 0.07f, 0.4f);
	time_manager->SetTimeScale(TimeScaleLayerKind::kEffect, 0.07f, 0.4f);
}

void Player::AttackBackMelee(CharacterBase* target)
{
	if (target->IsInvincible()) { return; }
}

void Player::AttackVersatilityMelee(CharacterBase* target)
{
	// roundhouse kick
	if (target->IsInvincible()) { return; }

	const auto dir = v3d::GetNormalizedV(m_transform->GetForward(CoordinateKind::kWorld) + VGet(0.0f, 0.5f, 0.0f));
	target->OnDamage(HealthPartKind::kMain, 1000.0f);
	target->OnKnockback(dir, 170.0f, 100.0f);

	const auto time_manager = GameTimeManager::GetInstance();
	time_manager->SetTimeScale(TimeScaleLayerKind::kWorld,  0.07f, 0.4f);
	time_manager->SetTimeScale(TimeScaleLayerKind::kPlayer, 0.07f, 0.4f);
	time_manager->SetTimeScale(TimeScaleLayerKind::kEffect, 0.07f, 0.4f);
}
#pragma endregion


#pragma region ステルスキル
void Player::UpdateStealthKill()
{
	m_move_speed			= 60.0f;
	m_look_dir_offset_speed = 8.0f;

	AllowCalcLookDir();
	CalcCorrectMoveDir();

	// 強制的に無敵状態にする
	ActivateInvincibleForcibly();

	DetachWeapon(GetCurrentEquipWeapon(WeaponSlotKind::kSub));
	HoldWeapon	(GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

void Player::SetupStealthKill()
{
	const auto target_character	= std::dynamic_pointer_cast<CharacterBase>(m_stealth_kill_target);
	const auto target_transform = target_character->GetTransform();
	const auto target_pos		= target_transform->GetPos(CoordinateKind::kWorld);
	const auto target_forward	= target_transform->GetForward(CoordinateKind::kWorld);

	m_look_dir.at(TimeKind::kNext) = target_forward;
	m_destination_pos = target_pos - target_forward * 20.0f;

	m_stealth_kill_target->OnStealthKill();
	target_character->OnDamage(HealthPartKind::kMain, 2000.0f);

	// 掴んだことを演出カメラに通知
	const StealthKillEvent event{ m_modeler };
	EventSystem::GetInstance()->Publish(event);
}
#pragma endregion


#pragma region 武器
void Player::EquipWeapon(const std::shared_ptr<WeaponBase>& weapon, const WeaponSlotKind slot_kind)
{
	m_current_equip_weapon[slot_kind] = weapon;
	m_current_equip_weapon[slot_kind]->AttachOwner(m_modeler, GetName());
}

void Player::UnequipWeapon(const WeaponSlotKind slot_kind)
{
	if (m_current_equip_weapon.count(slot_kind))
	{
		m_current_equip_weapon.at(slot_kind)->DetachOwner();
		m_current_equip_weapon.at(slot_kind) = nullptr;
	}
}

void Player::HoldWeapon(const std::shared_ptr<WeaponBase>& weapon)
{
	m_current_held_weapon = weapon;
	m_current_held_weapon->AttachOwner(m_modeler, GetName());
}

void Player::HoldWeapon(const int obj_handle)
{
	auto weapon = ObjManager::GetInstance()->GetObj<WeaponBase>(obj_handle);

	if (weapon)
	{
		m_current_held_weapon = weapon;
		m_current_held_weapon->AttachOwner(m_modeler, GetName());
	}
}

void Player::ReleaseWeapon()
{
	if (m_current_held_weapon)
	{
		m_current_held_weapon->DetachOwner();
		m_current_held_weapon = nullptr;
	}
}

void Player::AttachWeapon(const std::shared_ptr<WeaponBase>& weapon)
{
	m_attach_weapons[weapon->GetHolsterKind()] = weapon;
	m_attach_weapons[weapon->GetHolsterKind()]->AttachOwner(m_modeler, GetName());
}

void Player::AttachWeapon(const int obj_handle)
{
	auto weapon = ObjManager::GetInstance()->GetObj<WeaponBase>(obj_handle);

	if (weapon)
	{
		m_attach_weapons[weapon->GetHolsterKind()] = weapon;
		m_attach_weapons[weapon->GetHolsterKind()]->AttachOwner(m_modeler, GetName());
	}
}

void Player::DetachWeapon(const std::shared_ptr<WeaponBase>& weapon)
{
	// 自身が装着されていれば着脱する
	if (m_attach_weapons.count(weapon->GetHolsterKind()))
	{
		if (m_attach_weapons[weapon->GetHolsterKind()] == weapon)
		{
			m_attach_weapons[weapon->GetHolsterKind()]->DetachOwner();
			m_attach_weapons[weapon->GetHolsterKind()] = nullptr;
		}
	}
}

void Player::DetachWeapon(const HolsterKind holster_kind)
{
	m_attach_weapons[holster_kind]->DetachOwner();
	m_attach_weapons.erase(holster_kind);
}

std::shared_ptr<WeaponBase> Player::GetCurrentEquipWeapon(const WeaponSlotKind slot_kind) const
{
	return m_current_equip_weapon.count(slot_kind) ? m_current_equip_weapon.at(slot_kind) : nullptr;
}

std::shared_ptr<WeaponBase> Player::GetCurrentHeldWeapon()
{
	return m_current_held_weapon;
}

std::shared_ptr<WeaponBase> Player::GetCurrentAttachWeapon(const HolsterKind holster_kind) const
{
	return m_attach_weapons.count(holster_kind) ? m_attach_weapons.at(holster_kind) : nullptr;
}

WeaponKind Player::GetCurrentEquipWeaponKind(const WeaponSlotKind slot_kind)
{
	return m_current_equip_weapon.count(slot_kind) ? m_current_equip_weapon.at(slot_kind)->GetWeaponKind() : WeaponKind::kNone;
}

WeaponKind Player::GetCurrentHeldWeaponKind()
{
	return m_current_held_weapon ? m_current_held_weapon->GetWeaponKind() : WeaponKind::kNone;
}

WeaponKind Player::GetCurrentAttachWeaponKind(const HolsterKind holster_kind) const
{
	return m_attach_weapons.count(holster_kind) ? m_attach_weapons.at(holster_kind)->GetWeaponKind() : WeaponKind::kNone;
}
#pragma endregion


#pragma region State
void Player::Move()
{
	m_move_dir.at(TimeKind::kNext) = v3d::GetZeroV();
	m_input_slope = v3d::GetZeroV();

	CalcInputSlopeFromPad();
	CalcInputSlopeFromCommand();

	CalcMoveSpeed();
	AllowCalcLookDir();
}

void Player::SetLookDirOffsetValueForAim()
{
	m_look_dir_offset_speed = kLookDirOffsetSpeedForAim;
}

void Player::DirOfMovement()
{
	if (m_move_dir.at(TimeKind::kCurrent) != v3d::GetZeroV())
	{
		m_look_dir.at(TimeKind::kNext) = v3d::GetNormalizedV(m_move_dir.at(TimeKind::kCurrent));
	}
}

void Player::DirOfCameraForward()
{
	m_look_dir.at(TimeKind::kNext) = GetMoveForward();
}

void Player::CalcMoveSpeed()
{
	const auto current_action_state_kind = static_cast<player_state::ActionStateKind>(m_state->GetActionState(TimeKind::kCurrent)->GetStateKind());

	if (   current_action_state_kind != player_state::ActionStateKind::kActionNull
		&& current_action_state_kind != player_state::ActionStateKind::kCrouch)
	{
		return;
	}

	if (VSize(m_input_slope) <= kWalkStickSlopeLimit - InputChecker::kStickDeadZone)
	{
		// 速い状態から歩き状態に移行した場合、急速に減速させる
		if (m_move_speed > kWalkSpeed) { m_move_speed = kWalkSpeed; }

		math::Increase(m_move_speed, kAcceleration, kSlowWalkSpeed, false);
		math::Decrease(m_move_speed, kAcceleration, kSlowWalkSpeed);
		return;
	}

	// 遅い状態からダッシュ状態に移行した場合、急速に加速させる
	if (m_move_speed < kSlowWalkSpeed) { m_move_speed = kSlowWalkSpeed; }

	math::Increase(m_move_speed, kAcceleration, kWalkSpeed, false);
	math::Decrease(m_move_speed, kAcceleration, kWalkSpeed);
}

void Player::CalcMoveSpeedStop()
{
	const auto state = static_cast<player_state::WeaponActionStateKind>(m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind());

	if (   state == player_state::WeaponActionStateKind::kFirstSideSlashKnife
		|| state == player_state::WeaponActionStateKind::kSecondSideSlashKnife
		|| state == player_state::WeaponActionStateKind::kSpinningSlashKnife) {
		return;
	}

	// 速い状態から歩き状態に移行した場合、急速に減速させる
	if (m_move_speed > kSlowWalkSpeed) { m_move_speed = kSlowWalkSpeed; }

	math::Decrease(m_move_speed, kAcceleration, 0.0f);
}

void Player::CalcMoveSpeedRun()
{
	if (m_state->GetMoveState(TimeKind::kCurrent)->GetStateKind() == static_cast<int>(player_state::MoveStateKind::kIdle)) { return; }

	// 遅い状態からダッシュ状態に移行した場合、急速に加速させる
	if (m_move_speed < kWalkSpeed) { m_move_speed = kWalkSpeed; }

	math::Increase(m_move_speed, kAcceleration, kRunSpeed, false);

	m_look_dir_offset_speed = kLookDirOffsetSpeedForRun;
}

void Player::SideSlashKnifeOffsetMove()
{
	m_look_dir.at(TimeKind::kNext)	= GetMoveForward();
	m_move_dir.at(TimeKind::kNext)	= m_look_dir.at(TimeKind::kNext);

	m_look_dir_offset_speed = kLookDirOffsetSpeedForSideSlash;
	m_move_speed = 30.0f;
}

void Player::SpinningSlashKnifeOffsetMove()
{
	m_move_dir.at(TimeKind::kNext) = m_look_dir.at(TimeKind::kCurrent);
	m_move_speed = 50.0f;
}
#pragma endregion


#pragma region Getter
float Player::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kPlayer);
}
#pragma endregion


void Player::CalcInputSlopeFromPad()
{
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kPad) { return; }

	const auto forward	= GetMoveForward();
	const auto right	= GetMoveRight();

	// 各方向のパラメーターを取得
	const auto input			= InputChecker::GetInstance();
	const auto forward_param	= input->GetInputParameter(pad::StickKind::kLSUp);
	const auto backward_param	= input->GetInputParameter(pad::StickKind::kLSDown);
	const auto left_param		= input->GetInputParameter(pad::StickKind::kLSLeft);
	const auto right_param		= input->GetInputParameter(pad::StickKind::kLSRight);

	// 入力値を取得
	if (forward_param)	{ m_input_slope += forward * (forward_param	 - InputChecker::kStickDeadZone); }
	if (backward_param) { m_input_slope += forward * (backward_param + InputChecker::kStickDeadZone); }
	if (left_param)		{ m_input_slope += right   * (left_param	 + InputChecker::kStickDeadZone); }
	if (right_param)	{ m_input_slope += right   * (right_param	 - InputChecker::kStickDeadZone); }

	m_move_dir.at(TimeKind::kNext) = v3d::GetNormalizedV(m_input_slope);
}

void Player::CalcInputSlopeFromCommand()
{
	if (m_move_dir.at(TimeKind::kNext) != v3d::GetZeroV()) { return; }

	const auto command					= CommandHandler::GetInstance();
	const auto forward					= GetMoveForward();
	const auto right					= GetMoveRight();
	auto	   current_input_slope		= v3d::GetZeroV();
	auto	   continue_input_slope		= v3d::GetZeroV();
	
	// 現在入力されているvelocityを取得
	if (command->IsExecute(CommandKind::kMoveUpPlayer,    TimeKind::kCurrent))
	{
		current_input_slope += forward;
	}
	if (command->IsExecute(CommandKind::kMoveDownPlayer,  TimeKind::kCurrent))
	{
		current_input_slope -= forward;
	}
	if (command->IsExecute(CommandKind::kMoveLeftPlayer,  TimeKind::kCurrent))
	{
		current_input_slope -= right;
	}
	if (command->IsExecute(CommandKind::kMoveRightPlayer, TimeKind::kCurrent))
	{
		current_input_slope += right;
	}
	m_input_slope = v3d::GetNormalizedV(current_input_slope) * InputChecker::kStickMaxSlope;

	// 継続して入力されていたvelocityが、現在のvelocityと逆を向いていた場合現在のvelocityを縮める
	if (std::abs(math::GetAngleBetweenTwoVector(m_input_slope, continue_input_slope) - DX_PI_F) < math::kEpsilonLow)
	{
		m_input_slope += continue_input_slope;
	}

	m_move_dir.at(TimeKind::kNext) = v3d::GetNormalizedV(m_input_slope);
}

void Player::NotifyHealth()
{
	// 瀕死状態通知
	const auto parcent = (m_health.at(HealthPartKind::kMain)->GetMaxValue() / 270.0f) * 45;
	if (m_health.at(HealthPartKind::kMain)->GetCurrentValue() < parcent)
	{
		// 瀕死状態突入通知
		if (m_prev_health >= parcent)
		{
			const EnterNearDeathEvent event{};
			EventSystem::GetInstance()->Publish(event);
		}

		const NearDeathEvent event{};
		EventSystem::GetInstance()->Publish(event);
	}

	m_prev_health = m_health.at(HealthPartKind::kMain)->GetCurrentValue();
}

VECTOR Player::GetMoveForward()
{
	// MEMO : cinemachine brainを介するよりDxLib既存の関数を使用したほうが取得が早い
	auto forward = GetCameraFrontVector();
	forward.y = 0.0f;

	return v3d::GetNormalizedV(forward);
}

VECTOR Player::GetMoveRight()
{
	auto right = GetCameraRightVector();
	right.y = 0.0f;
	
	return v3d::GetNormalizedV(right);
}
