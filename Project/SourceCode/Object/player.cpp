#include "player.hpp"
#include "../Command/command_handler.hpp"
#include "../Part/player_state_controller.hpp"

Player::Player() :
	CharacterBase(ObjName.PLAYER, ObjTag.PLAYER, MassKind::kMedium),
	m_subject							(std::make_shared<Subject<Player>>()),
	m_state								(std::make_shared<PlayerStateController>()),
	m_bone_pos_corrector				(std::make_shared<BonePosCorrector>()),
	m_input_slope						(v3d::GetZeroV()),
	m_move_speed						(0.0f),
	m_look_dir_offset_angle				(0.0f),
	m_confirm_look_dir_threshold_angle	(0.0f),
	m_current_equip_weapon				(nullptr),
	m_current_equip_knife				(nullptr),
	m_weapon_shortcut_selecter			(std::make_shared<WeaponShortcutSelecter>())
{
	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.SWAT, kBasicAngle, kBasicScale);
	SetColliderModelHandle(m_modeler->GetModelHandle());

	// 初期pos・dirを設定
	m_look_dir[TimeKind::kCurrent] = m_look_dir[TimeKind::kNext] = VGet(0.0f, 0.0f, 1.0f);
	m_transform->SetPos(CoordinateKind::kWorld, VGet(0, 500, 0));
	m_transform->SetRot(CoordinateKind::kWorld, m_look_dir.at(TimeKind::kCurrent));

	// コライダー・トリガーを設定
	CreateCharaBasisCollider(kCapsuleRadius, kLandingTriggerRadius);

	// 各アニメーション追加
	m_animator = std::make_shared<PlayerAnimator>(m_modeler, m_state, m_current_held_weapon, m_current_equip_weapon);

	// TODO : 仮後に変更
	{
		// 武器設定
		const auto assault_rifle = std::make_shared<AssaultRifle>();
		const auto rocket_launcher = std::make_shared<RocketLauncher>();
		const auto knife = std::make_shared<Knife>();
		assault_rifle->AddToObjManager();
		rocket_launcher->AddToObjManager();
		m_weapon_shortcut_selecter->AttachShortcutWeapon(WeaponShortcutPosKind::kInsideLeft, assault_rifle);
		m_weapon_shortcut_selecter->AttachShortcutWeapon(WeaponShortcutPosKind::kOutsideDown, rocket_launcher);
		AddItem(assault_rifle);
		AddItem(rocket_launcher);
		AddItem(knife);
		EquipWeapon(assault_rifle);
		EquipWeapon(rocket_launcher);
		EquipKnife(knife);
		AttachWeapon(assault_rifle);
		AttachWeapon(rocket_launcher);
		AttachWeapon(knife);

		m_current_remaining_bullet_num = 10000;
	}
}

Player::~Player()
{

}

void Player::Init()
{

}

void Player::Update()
{
	if (!IsActive()) { return; }

	m_look_dir_offset_angle				= kLookDirOffsetAngle			* math::kDegToRad;
	m_confirm_look_dir_threshold_angle	= kConfirmLookDirThresholdAngle * math::kDegToRad;

	m_weapon_shortcut_selecter	->Update(this);
	m_state						->Update(this);
	m_animator					->Update();

	CalcMoveDir();
	CalcLookDir();
	CalcVelocity();

	CalcCapsuleColliderLength();

	ApplyLookDirToRot(m_look_dir.at(TimeKind::kCurrent));
}

void Player::LateUpdate()
{
	if (!IsActive()) { return; }

	m_state->LateUpdate(this);

	for (const auto& attach_weapon : m_attach_weapons)
	{
		if (attach_weapon.second)
		{
			attach_weapon.second->TrackOwnerHolster();
		}
	}
}

void Player::DrawToShadowMap() const
{
	if (!IsActive()) { return; }

	m_modeler->DrawToShadowMap();

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

	//DxLibHelper::DrawModelFrames(m_modeler->GetModelHandle(), "", 0.05f, 20.0f);

	if (m_current_held_weapon) { m_current_held_weapon->Draw(); }

	for (const auto& attach_weapon : m_attach_weapons)
	{
		if (attach_weapon.second) { attach_weapon.second->Draw(); }
	}

	for (const auto& collider : m_collider)
	{
		const auto shape = collider->GetShape();
		if (shape != nullptr)
		{
			shape->Draw(true, 0, 0xffffff);
		}
	}

	//DrawFormatString(500, 40, 0xffffff, "%f", m_velocity.y);
	//
	const auto look_dir_current = m_look_dir.at(TimeKind::kCurrent);
	//const auto look_dir_next	= m_look_dir.at(TimeKind::kNext);
	DrawFormatString(0,  80, 0xffffff, "look_dir_current : %f %f, %f", look_dir_current.x, look_dir_current.y, look_dir_current.z);
	//DrawFormatString(0, 100, 0xffffff, "look_dir_next    : %f %f, %f", look_dir_next.x,    look_dir_next.y,    look_dir_next.z);
	//
	//const auto p = m_transform->GetPos(CoordinateKind::kWorld) + VGet(0, 40, 0);
	//DrawLine3D(p, p + look_dir_current * 100, 0xff0000);
	//DrawLine3D(p, p + look_dir_next    * 100, 0xffffff);
	//
	//DrawFormatString(0,   20, 0xffffff, "m_current_remaining_bullet_num : %d", m_current_remaining_bullet_num);
	DrawFormatString(500, 20, 0xffffff, "move_speed       : %f", m_move_speed);
	DrawFormatString(500, 40, 0xffffff, "m_look_dir_offset_angle    : %f", m_look_dir_offset_angle * math::kRadToDeg);
	DrawFormatString(500, 60, 0xffffff, "move_dir_current : %f, %f ,%f", m_move_dir.at(TimeKind::kCurrent).x, m_move_dir.at(TimeKind::kCurrent).y, m_move_dir.at(TimeKind::kCurrent).z);
	//if(m_current_held_weapon)DrawFormatString(0, 20, 0xffffff, "%s", m_current_held_weapon->GetName().c_str());
}

void Player::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	std::shared_ptr<ShapeBase> shape = nullptr;

	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kLandingTrigger:
		m_is_landing = true;
		
		shape = hit_collider_pair.target_collider->GetShape();

		// 図形の登録がされていない場合はモデルで判定を行う
		if (shape == nullptr)
		{
			
		}
		else
		{
			
		}
		break;

	default:
		break;
	}
}

//void Player::CalcMoveDirFirstFrame()
//{
//	// なす角が90°以上の場合は移動方向を保存しない
//	const float angle = math::GetAngleBetweenTwoVector(m_move_dir[TimeKind::kCurrent], m_move_dir[TimeKind::kNext]);
//	if (angle >= 90.0f * math::kDegreesToRadian)
//	{
//		m_move_dir[TimeKind::kCurrent] = v3d::GetZeroV();
//	}
//}


#pragma region 武器
void Player::UnequipWeapon()
{
	m_current_equip_weapon = nullptr;
}

void Player::UnequipKnife()
{
	m_current_equip_knife = nullptr;
}
#pragma endregion


#pragma region State
void Player::Move()
{
	m_move_dir[TimeKind::kPrev] = m_move_dir[TimeKind::kCurrent];
	m_move_dir[TimeKind::kNext] = v3d::GetZeroV();
	m_input_slope = v3d::GetZeroV();

	CalcInputSlopeFromPad();
	CalcInputSlopeFromCommand();

	CalcMoveSpeed();
}

void Player::SetLookDirOffsetValueForAim()
{
	m_look_dir_offset_angle				= kLookDirOffsetAngleForAim			  * math::kDegToRad;
	m_confirm_look_dir_threshold_angle	= kConfirmLookDirThresholdAngleForAim * math::kDegToRad;
}

void Player::DirOfMovement()
{
	if (m_move_dir[TimeKind::kCurrent] != v3d::GetZeroV())
	{
		m_look_dir.at(TimeKind::kNext) = v3d::GetNormalizedV(m_move_dir[TimeKind::kCurrent]);
	}
}

void Player::DirOfCameraForward()
{
	m_look_dir.at(TimeKind::kNext) = GetMoveForward();
}

void Player::CalcMoveSpeed()
{
	const auto current_action_state_kind = m_state->GetActionState(TimeKind::kCurrent)->GetStateKind();

	if (   current_action_state_kind != static_cast<int>(player_state::ActionStateKind::kActionNull)
		&& current_action_state_kind != static_cast<int>(player_state::ActionStateKind::kCrouch)) { return; }

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
	if (m_state->GetMoveState(TimeKind::kCurrent)->GetStateKind() == static_cast<int>(player_state::MoveStateKind::kMoveNull)) { return; }

	// 遅い状態からダッシュ状態に移行した場合、急速に加速させる
	if (m_move_speed < kWalkSpeed) { m_move_speed = kWalkSpeed; }

	math::Increase(m_move_speed, kAcceleration, kRunSpeed, false);

	m_look_dir_offset_angle = 5.7f * math::kDegToRad;
}

void Player::SideSlashKnifeOffsetMove()
{
	m_look_dir.at(TimeKind::kNext)	= GetMoveForward();
	m_move_dir[TimeKind::kNext]		= m_look_dir.at(TimeKind::kNext);

	m_look_dir_offset_angle = 0.5f * math::kDegToRad;
	m_move_speed = 30.0f;
}

void Player::SpinningSlashKnifeOffsetMove()
{
	m_move_dir[TimeKind::kNext] = m_look_dir.at(TimeKind::kCurrent);
	m_move_speed = 50.0f;
}

//void Player::CalcMoveOffsetSideSlashKnife()
//{
//	math::Increase(m_move_speed, 10 * FPS::GetDeltaTime(), 10.0f);
//}
#pragma endregion


float Player::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleController::LayerKind::kPlayer);
}

WeaponKind Player::GetCurrentEquipWeaponKind()
{
	return m_current_equip_weapon ? m_current_equip_weapon->GetWeaponKind() : WeaponKind::kNone;
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

	m_move_dir[TimeKind::kNext] = v3d::GetNormalizedV(m_input_slope);
}

void Player::CalcInputSlopeFromCommand()
{
	if (m_move_dir[TimeKind::kNext] != v3d::GetZeroV()) { return; }

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

	m_move_dir[TimeKind::kNext] = v3d::GetNormalizedV(m_input_slope);
}

void Player::CalcVelocity()
{
	m_velocity = m_move_dir[TimeKind::kCurrent] * m_move_speed;
}

void Player::CalcMoveDir()
{
	// 現在のdirを目的とするdirに近づけていく
	m_move_dir[TimeKind::kCurrent] = math::GetApproachedVector(
		m_move_dir[TimeKind::kCurrent], 
		m_move_dir[TimeKind::kNext], 
		kMoveDirOffsetSpeed);
}

void Player::CalcLookDir()
{
	const auto move_state			= static_cast<player_state::MoveStateKind>		  (m_state->GetMoveState		(TimeKind::kCurrent)->GetStateKind());
	const auto weapon_action_state	= static_cast<player_state::WeaponActionStateKind>(m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind());

	if (   move_state		   != player_state::MoveStateKind::kMove
		&& weapon_action_state != player_state::WeaponActionStateKind::kAimKnife
		&& weapon_action_state != player_state::WeaponActionStateKind::kFirstSideSlashKnife
		&& weapon_action_state != player_state::WeaponActionStateKind::kSecondSideSlashKnife
		&& weapon_action_state != player_state::WeaponActionStateKind::kSpinningSlashKnife
		&& weapon_action_state != player_state::WeaponActionStateKind::kAimGun)
	{
		return;
	}

	// ヨー角回転を取得し、-π～πで値を管理する
	const VECTOR current_yaw = math::GetYawRotVector(m_look_dir.at(TimeKind::kCurrent));
	const VECTOR next_yaw	 = math::GetYawRotVector(m_look_dir.at(TimeKind::kNext));
	VECTOR distance = next_yaw - current_yaw;
	distance.y = math::ConnectMinusPiToPi(distance.y);

	// カメラを基準にして右側であった場合は反転
	if (distance.y > 0) { m_look_dir_offset_angle *= -1; }

	// 回転を適用
	const Quaternion rot_q = quat::CreateQuaternion(axis::GetWorldYAxis(), -m_look_dir_offset_angle);
	m_look_dir.at(TimeKind::kCurrent) = math::GetRotatedPos(m_look_dir.at(TimeKind::kCurrent), rot_q);

	const float angle = math::GetYawBetweenTwoVector(m_look_dir.at(TimeKind::kNext), m_look_dir.at(TimeKind::kCurrent));
	if (angle < m_confirm_look_dir_threshold_angle)
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
	const auto camera = CameraManager::GetInstance()->GetMainCamera();
	auto forward = camera->GetTransform()->GetForward(CoordinateKind::kWorld);
	forward.y = 0.0f;

	return v3d::GetNormalizedV(forward);
}

VECTOR Player::GetMoveRight()
{
	const auto camera = CameraManager::GetInstance()->GetMainCamera();
	auto right = camera->GetTransform()->GetRight(CoordinateKind::kWorld);
	right.y = 0.0f;
	
	return v3d::GetNormalizedV(right);
}
