#include "player.hpp"
#include "../Command/command_handler.hpp"
#include "../Part/player_state_controller.hpp"

Player::Player() :
	CharacterBase(ObjName.PLAYER, ObjTag.PLAYER, MassKind::kMedium),
	m_state								(std::make_shared<PlayerStateController>()),
	m_bone_pos_corrector				(std::make_shared<BonePosCorrector>()),
	m_input_slope						(v3d::GetZeroV()),
	m_look_dir_offset_speed				(0.0f),
	m_prev_health						(0.0f),
	m_is_grabbed						(false),
	m_weapon_shortcut_selecter			(std::make_shared<WeaponShortcutSelecter>())
{
	// イベントの登録
	EventSystem::GetInstance()->Subscribe<OnDownedEnemySpottedEvent>(this, &Player::AddMeleeCandidate);

	m_health[HealthPartKind::kMain] = std::make_shared<Health>(2000.0f, 1500.0f);
	m_prev_health = m_health.at(HealthPartKind::kMain)->GetCurrentHealth();

	// モデル・アニメーションを設定
	m_modeler  = std::make_shared<Modeler>(m_transform, ModelPath.SWAT_02, kBasicAngle, kBasicScale);
	m_animator = std::make_shared<PlayerAnimator>(m_modeler, m_state, m_current_held_weapon, m_current_equip_weapon[WeaponSlotKind::kMain]);
	SetColliderModelHandle(m_modeler->GetModelHandle());

	m_invincible_time = kInvincibleTime;

	// 初期pos・dirを設定
	m_look_dir[TimeKind::kCurrent] = m_look_dir[TimeKind::kNext] = VGet(0.0f, 0.0f, 1.0f);
	m_transform->SetPos(CoordinateKind::kWorld, VGet(0.0f, -54.0f, 0.0f));
	m_transform->SetRot(CoordinateKind::kWorld, m_look_dir.at(TimeKind::kCurrent));

	// コライダー・トリガーを設定
	m_collider_creator->CreateCapsuleCollider	(this, m_modeler, kCapsuleRadius);
	m_collider_creator->CreateLandingTrigger	(this, kLandingTriggerRadius);
	m_collider_creator->CreateVisibleTrigger	(this, m_modeler);
	
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
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<OnDownedEnemySpottedEvent>(this, &Player::AddMeleeCandidate);

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

	NotifyHealth();
	JudgeInvincible();
	DecisionMeleeTarget();

	if (m_current_held_weapon) { m_current_held_weapon->Update(); }

	for (const auto& attach_weapon : m_attach_weapons)
	{
		if (attach_weapon.second)
		{
			attach_weapon.second->Update();
		}
	}

	m_move_dir_offset_speed	= kMoveDirOffsetSpeed;
	m_look_dir_offset_speed	= kLookDirOffsetSpeed;

	m_weapon_shortcut_selecter->Update(std::static_pointer_cast<Player>(shared_from_this()));
	m_state					  ->Update(std::static_pointer_cast<Player>(shared_from_this()));
	m_animator				  ->Update();

	CalcMoveDir();
	CalcLookDir();
	CalcMoveVelocity();

	m_collider_creator->CalcCapsuleColliderDirAndLength	(m_modeler, m_collider, m_transform);
	m_collider_creator->CalcVisibleTriggerPos			(m_modeler, m_collider);

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

	RemoveMeleeCandidate();
}

void Player::DrawToShadowMap() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	if (m_current_held_weapon) { m_current_held_weapon->DrawToShadowMap(); }

	for (const auto& attach_weapon : m_attach_weapons)
	{
		if (attach_weapon.second) { attach_weapon.second->DrawToShadowMap(); }
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

	for (const auto& collider : m_collider)
	{
		const auto shape = collider.second->GetShape();
		if (shape != nullptr)
		{
			shape->Draw(true, 0, 0xffffff);
		}
	}
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
		if (action_state_kind == player_state::ActionStateKind::kFrontKick)
		{
			OnCollideFrontMelee(target_obj);
		}
		else if (action_state_kind == player_state::ActionStateKind::kRoundhouseKick)
		{
			OnCollideVersatilityMelee(target_obj);
		}

	default:
		break;
	}
}

void Player::OnDamage(const HealthPartKind part_kind, const float damage)
{
	if (!m_health.count(part_kind)) { return; }

	m_health.at(part_kind)->OnDamage(damage);
	m_invincible_timer	= m_invincible_time;
	m_is_invincible		= true;

	// 通知処理
	if (part_kind == HealthPartKind::kMain)
	{
		// ダメージ通知
		const OnDamageEvent event{ damage, damage / m_health.at(part_kind)->GetMaxHealth() };
		EventSystem::GetInstance()->Publish(event);
	}
}

void Player::OnGrabbed(const VECTOR& brabber_pos, const VECTOR& brabber_dir)
{
	m_is_grabbed = true;

	m_look_dir.at(TimeKind::kNext) = -brabber_dir;
	m_destination_pos = brabber_pos + brabber_dir * 17.0f;
}

void Player::OnRelease()
{
	m_is_grabbed = false;
}

void Player::OnGrabbedDamage(const float damage)
{
	OnDamage(HealthPartKind::kMain, damage);
}

void Player::OnCollideFrontMelee(PhysicalObjBase* target_obj)
{
	// front kick

	const auto dir = v3d::GetNormalizedV(m_transform->GetForward(CoordinateKind::kWorld) + VGet(0.0f, 0.5f, 0.0f));
	target_obj->OnKnockback(dir, 170.0f, 5.0f);
}

void Player::OnCollideBackMelee(PhysicalObjBase* target_obj)
{

}

void Player::OnCollideVersatilityMelee(PhysicalObjBase* target_obj)
{
	// roundhouse kick

	const auto dir = v3d::GetNormalizedV(m_transform->GetForward(CoordinateKind::kWorld) + VGet(0.0f, 0.5f, 0.0f));
	target_obj->OnKnockback(dir, 170.0f, 5.0f);
}

void Player::AttackFrontMelee(const VECTOR& target_pos, const VECTOR& target_dir)
{
	// front kick

	m_look_dir.at(TimeKind::kNext) = -target_dir;
	m_destination_pos = target_pos + target_dir * 24.0f;
}

void Player::AttackBackMelee(const VECTOR& target_pos, const VECTOR& target_dir)
{
	// suplex

	m_look_dir.at(TimeKind::kNext) = target_dir;
	m_destination_pos = target_pos - target_dir * 24.0f;
}

void Player::AttackVersatilityMelee(const VECTOR& target_pos)
{
	// roundhouse kick

	const auto pos				= m_transform->GetPos(CoordinateKind::kWorld);
	const auto pos_y0			= VGet(pos.x, 0.0f, pos.z);
	const auto target_pos_y0	= VGet(target_pos.x, 0.0f, target_pos.z);
	const auto dir				= v3d::GetNormalizedV(target_pos_y0 - pos_y0);

	m_look_dir.at(TimeKind::kNext) = dir;
	m_destination_pos = target_pos - dir * 24.0f;
}


#pragma region Event
void Player::AddMeleeCandidate(const OnDownedEnemySpottedEvent& event)
{
	m_melee_candidate.emplace_back(MeleeCandidateData(event.target_obj_handle, event.camera_diff_angle));
}
#pragma endregion


//void Player::CalcMoveDirFirstFrame()
//{
//	// なす角が90°以上の場合は移動方向を保存しない
//	const float angle = math::GetAngleBetweenTwoVector(m_move_dir.at(TimeKind::kCurrent), m_move_dir.at(TimeKind::kNext));
//	if (angle >= 90.0f * math::kDegreesToRadian)
//	{
//		m_move_dir.at(TimeKind::kCurrent) = v3d::GetZeroV();
//	}
//}


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
#pragma endregion


#pragma region State
void Player::Move()
{
	m_move_dir.at(TimeKind::kNext) = v3d::GetZeroV();
	m_input_slope = v3d::GetZeroV();

	CalcInputSlopeFromPad();
	CalcInputSlopeFromCommand();

	CalcMoveSpeed();
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

void Player::UpdateGrabbed()
{
	m_move_speed = 20.0f;

	CalcCorrectMoveDir();

	ReleaseWeapon();
}

void Player::UpdateMelee()
{
	m_move_speed = 20.0f;

	CalcCorrectMoveDir();

	// 強制的に無敵状態にする
	ActivateInvincibleForcibly();
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

//void Player::CalcMoveOffsetSideSlashKnife()
//{
//	math::Increase(m_move_speed, 10 * FPS::GetDeltaTime(), 10.0f);
//}
#pragma endregion


#pragma region Getter
float Player::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kPlayer);
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


void Player::RemoveMeleeCandidate()
{
	m_melee_candidate.clear();
}

void Player::DecisionMeleeTarget()
{
	if (m_melee_candidate.empty()) { return; }

	MeleeTargetSelecter target_selecter;
	target_selecter.SelectMeleeTarget(m_transform->GetForward(CoordinateKind::kWorld), m_melee_candidate);
}

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

	//// 継続して入力されているvelocityを取得
	//if (   command->IsExecute(CommandKind::kMoveUpPlayer,	 TimeKind::kCurrent)
	//	&& command->IsExecute(CommandKind::kMoveUpPlayer,    TimeKind::kPrev))
	//{
	//	continue_input_slope += forward;
	//}
	//if (   command->IsExecute(CommandKind::kMoveDownPlayer,  TimeKind::kCurrent)
	//	&& command->IsExecute(CommandKind::kMoveDownPlayer,  TimeKind::kPrev))
	//{
	//	continue_input_slope -= forward;
	//}
	//if (   command->IsExecute(CommandKind::kMoveLeftPlayer,  TimeKind::kCurrent)
	//	&& command->IsExecute(CommandKind::kMoveLeftPlayer,  TimeKind::kPrev))
	//{
	//	continue_input_slope -= right;
	//}
	//if (   command->IsExecute(CommandKind::kMoveRightPlayer, TimeKind::kCurrent)
	//	&& command->IsExecute(CommandKind::kMoveRightPlayer, TimeKind::kPrev))
	//{
	//	continue_input_slope += right;
	//}
	//continue_input_slope = v3d::GetNormalizedV(continue_input_slope) * (InputChecker::kStickMaxSlope - InputChecker::kStickDeadZone);

	// 継続して入力されていたvelocityが、現在のvelocityと逆を向いていた場合現在のvelocityを縮める
	if (std::abs(math::GetAngleBetweenTwoVector(m_input_slope, continue_input_slope) - DX_PI_F) < math::kEpsilonLow)
	{
		m_input_slope += continue_input_slope;
	}

	m_move_dir.at(TimeKind::kNext) = v3d::GetNormalizedV(m_input_slope);
}

void Player::CalcMoveVelocity()
{
	m_move_velocity = m_move_dir.at(TimeKind::kCurrent) * m_move_speed;
	m_velocity += m_move_velocity;
}

void Player::NotifyHealth()
{
	// 瀕死状態通知
	const auto parcent = (m_health.at(HealthPartKind::kMain)->GetMaxHealth() / 270.0f) * 45;
	if (m_health.at(HealthPartKind::kMain)->GetCurrentHealth() < parcent)
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

	m_prev_health = m_health.at(HealthPartKind::kMain)->GetCurrentHealth();
}

void Player::CalcLookDir()
{
	const auto move_state			= static_cast<player_state::MoveStateKind>		  (m_state->GetMoveState		(TimeKind::kCurrent)->GetStateKind());
	const auto action_state			= static_cast<player_state::ActionStateKind>	  (m_state->GetActionState		(TimeKind::kCurrent)->GetStateKind());
	const auto weapon_action_state	= static_cast<player_state::WeaponActionStateKind>(m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind());

	// TODO : LookDir補正自体の挙動はCharaBase共通なため、条件分をのちに修正
	// CalcLookDir関数をCharaBaseに移行
	if (   move_state		   != player_state::MoveStateKind		 ::kMove
		&& action_state		   != player_state::ActionStateKind		 ::kGrabbed
		&& action_state		   != player_state::ActionStateKind		 ::kRoundhouseKick
		&& action_state		   != player_state::ActionStateKind		 ::kFrontKick
		&& action_state		   != player_state::ActionStateKind		 ::kSuplex
		&& weapon_action_state != player_state::WeaponActionStateKind::kAimKnife
		&& weapon_action_state != player_state::WeaponActionStateKind::kFirstSideSlashKnife
		&& weapon_action_state != player_state::WeaponActionStateKind::kSecondSideSlashKnife
		&& weapon_action_state != player_state::WeaponActionStateKind::kSpinningSlashKnife
		&& weapon_action_state != player_state::WeaponActionStateKind::kAimGun)
	{
		return;
	}

	// ヨー角回転を取得し、-π～πで値を管理する
	const auto current_yaw	= math::GetYawRotVector(m_look_dir.at(TimeKind::kCurrent));
	const auto next_yaw		= math::GetYawRotVector(m_look_dir.at(TimeKind::kNext));
	auto distance = next_yaw - current_yaw;
	distance.y = math::ConnectMinusValueToValue(distance.y, DX_PI_F);

	// カメラを基準にして右側であった場合は反転
	if (distance.y > 0) { m_look_dir_offset_speed *= -1; }

	// 回転を適用
	const auto look_dir_offset_speed = -m_look_dir_offset_speed * GetDeltaTime();
	const auto rot_q = quat::CreateQuaternion(axis::GetWorldYAxis(), look_dir_offset_speed);
	m_look_dir.at(TimeKind::kCurrent) = math::GetRotatedPos(m_look_dir.at(TimeKind::kCurrent), rot_q);

	// 終了判定
	const auto angle = math::GetYawBetweenTwoVector(m_look_dir.at(TimeKind::kNext), m_look_dir.at(TimeKind::kCurrent));
	const auto dynamic_threshold = std::abs(look_dir_offset_speed * math::kStopThreshold);
	if (angle < dynamic_threshold)
	{
		m_look_dir.at(TimeKind::kCurrent) = m_look_dir.at(TimeKind::kNext);
	}
}

//void Player::CalcCameraAimPos()
//{
//	m_modeler->ApplyMatrix();
//
//	// 追跡するボーンから行列を取得
//	const auto	model_handle	= m_modeler->GetModelHandle();
//	const auto	frame_num		= MV1SearchFrame(model_handle, BonePath.SPINE_2);
//	auto		frame_mat		= MV1GetFrameLocalWorldMatrix(model_handle, frame_num);
//	auto		aim_pos			= MGetTranslateElem(frame_mat);
//	
//	if (!IsTrackCameraOriginBone())
//	{
//		// ボーンと同じ高さの位置を追跡
//		const auto begin_pos	= m_transform->GetPos(CoordinateKind::kWorld);
//		const auto distance		= begin_pos - aim_pos;
//		aim_pos					= begin_pos + m_transform->GetUp(CoordinateKind::kWorld) * VSize(distance);
//	}
//
//	m_current_camera_aim_pos = math::GetApproachedVector(m_current_camera_aim_pos, aim_pos, kCameraAimOffsetBasicSpeed * VSize(aim_pos - m_current_camera_aim_pos));
//
//	m_camera_aim_transform->SetPos(CoordinateKind::kWorld, m_current_camera_aim_pos);
//}

VECTOR Player::GetMoveForward()
{
	const auto camera = CinemachineBrain::GetInstance()->GetMainCamera();
	auto forward = camera->GetTransform()->GetForward(CoordinateKind::kWorld);
	forward.y = 0.0f;

	return v3d::GetNormalizedV(forward);
}

VECTOR Player::GetMoveRight()
{
	const auto camera = CinemachineBrain::GetInstance()->GetMainCamera();
	auto right = camera->GetTransform()->GetRight(CoordinateKind::kWorld);
	right.y = 0.0f;
	
	return v3d::GetNormalizedV(right);
}
