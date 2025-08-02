#include "player.hpp"
#include "../Command/command_handler.hpp"
#include "../Part/player_state_controller.hpp"

Player::Player() :
	CharacterBase(ObjName.PLAYER, ObjTag.PLAYER, ModelPath.SPECIAL_FORCES, MassKind::kMedium),
	m_state								(std::make_shared<PlayerStateController>()),
	m_camera_aim_transform				(std::make_shared<Transform>()),
	m_move_speed						(0.0f),
	m_look_dir_offset_angle				(0.0f),
	m_confirm_look_dir_threshold_angle	(0.0f),
	m_current_equip_weapon				(nullptr),
	m_current_equip_knife				(nullptr),
	m_weapon_shortcut_selecter			(std::make_shared<WeaponShortcutSelecter>())
{
	// 初期pos・dirを設定
	m_look_dir[TimeKind::kCurrent] = m_look_dir[TimeKind::kNext] = VGet(0.0f, 0.0f, 1.0f);
	m_transform->SetRot		(CoordinateKind::kWorld, m_look_dir.at(TimeKind::kCurrent));
	m_transform->SetScale	(CoordinateKind::kWorld, kModelScale);

	// コライダー・トリガーを設定
	CreateCharaBasisCollider(kCapsuleRadius, kLandingTriggerRadius);

	// 各アニメーション追加
	m_animator = std::make_shared<PlayerAnimator>(m_modeler, m_state);

	// 武器設定
	const auto gun   = std::make_shared<AssaultRifle>();
	const auto knife = std::make_shared<Knife>();
	AddItem(gun);
	AddItem(knife);
	EquipWeapon(gun);
	EquipKnife (knife);
	m_weapon_shortcut_selecter->AttachShortcutWeapon(WeaponShortcutPosKind::kInsideLeft, gun);

	// TODO : 仮で銃のオブジェ登録
	ObjManager::GetInstance()->AddObj(gun);
	CollisionManager::GetInstance()->AddCollideObj(gun);

	// カメラ登録
	const auto camera_manager = CameraManager::GetInstance();
	const auto rot_camera = std::make_shared<RotControlVirtualCamera>(1);
	camera_manager->AddVirtualCamera(rot_camera);
	rot_camera->AttachTarget(m_camera_aim_transform);

	m_current_remaining_bullet_num = 10000;
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

	m_look_dir_offset_angle				= kLookDirOffsetAngle;
	m_confirm_look_dir_threshold_angle	= kConfirmLookDirThresholdAngle * math::kDegreesToRadian;

	m_weapon_shortcut_selecter	->Update(this);
	m_state						->Update(this);
	m_animator					->Update();

	CalcMoveDir(m_velocity);
	CalcLookDir();
	CalcVelocity();
	CalcCapsuleColliderLength();

	UpdateTransform(m_look_dir.at(TimeKind::kCurrent), kModelScale);
}

void Player::LateUpdate()
{
	if (!IsActive()) { return; }

	m_state->LateUpdate(this);

	CalcCameraAimPos();
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
		const auto shape = collider->GetShape();
		if (shape != nullptr)
		{
			shape->Draw(true, 0, 0xffffff);
		}
	}


	//const auto look_dir_current = m_look_dir.at(TimeKind::kCurrent);
	//const auto look_dir_next	= m_look_dir.at(TimeKind::kNext);
	//DrawFormatString(0, 20, 0xffffff, "look_dir_current : %f %f, %f", look_dir_current.x, look_dir_current.y, look_dir_current.z);
	//DrawFormatString(0, 40, 0xffffff, "look_dir_next    : %f %f, %f", look_dir_next.x,    look_dir_next.y,    look_dir_next.z);

	//const auto p = m_transform->GetPos(CoordinateKind::kWorld) + VGet(0, 40, 0);
	//DrawLine3D(p, p + look_dir_current * 100, 0xff0000);
	//DrawLine3D(p, p + look_dir_next    * 100, 0xffffff);
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

	// 移動方向の決定
	const auto command = CommandHandler::GetInstance();
	if (command->IsExecuting(CommandKind::kMoveUpPlayer))
	{
		m_move_dir[TimeKind::kNext] += GetMoveForward();
	}
	if (command->IsExecuting(CommandKind::kMoveDownPlayer))
	{
		m_move_dir[TimeKind::kNext] -= GetMoveForward();
	}
	if (command->IsExecuting(CommandKind::kMoveLeftPlayer))
	{
		m_move_dir[TimeKind::kNext] -= GetMoveRight();
	}
	if (command->IsExecuting(CommandKind::kMoveRightPlayer))
	{
		m_move_dir[TimeKind::kNext] += GetMoveRight();
	}

	// 移動速度を取得
	switch (InputChecker::GetInstance()->GetCurrentInputDevice())
	{
	case DeviceKind::kKeyboard:
		m_velocity = v3d::GetNormalizedV(m_move_dir[TimeKind::kNext]) * InputChecker::kStickMaxSlope;
		break;

	case DeviceKind::kPad:
		m_velocity = GetVelocityFromPad();
		break;
	}

	CalcMoveSpeed(VSize(m_velocity));
}

void Player::SetLookDirOffsetValueForAim()
{
	m_look_dir_offset_angle			= kLookDirOffsetAngleForAim;
	m_confirm_look_dir_threshold_angle	= kConfirmLookDirThresholdAngleForAim * math::kDegreesToRadian;
}

void Player::DirOfMovement()
{
	if (m_move_dir[TimeKind::kCurrent] != v3d::GetZeroV())
	{
		m_look_dir.at(TimeKind::kNext) = v3d::GetNormalizedV(m_move_dir[TimeKind::kCurrent]);
	}

	//auto pos = m_transform->GetPos(CoordinateKind::kWorld);
	//VECTOR o1 = { 0, 30, 0 };
	//VECTOR o2 = { 0, 40, 0 };
	//VECTOR o3 = { 0, 50, 0 };
	//DrawLine3D(pos + o1, pos + o1 + m_look_dir.at(TimeKind::kCurrent)	* 100, GetColor(255, 0, 0));
	//DrawLine3D(pos + o2, pos + o2 + m_look_dir.at(TimeKind::kNext)		* 100, GetColor(0, 255, 0));
	//DrawLine3D(pos + o3, pos + o3 + m_move_dir[TimeKind::kCurrent]		* 100, GetColor(0, 0, 255));
	//DrawFormatString(0, 40, 0xffffff, "m_look_dir.at(TimeKind::kNext).y : %f, %f, %f", m_look_dir.at(TimeKind::kNext).x, m_look_dir.at(TimeKind::kNext).y, m_look_dir.at(TimeKind::kNext).z);
}

void Player::DirOfCameraForward()
{
	m_look_dir.at(TimeKind::kNext) = GetMoveForward();
}

void Player::CalcStopSpeed()
{
	// 速い状態から歩き状態に移行した場合、急速に減速させる
	if (m_move_speed > kSlowWalkSpeed) { m_move_speed = kSlowWalkSpeed; }

	math::Decrease(m_move_speed, kAcceleration * FPS::GetDeltaTime(), 0.0f);
}

void Player::CalcMoveSpeed(const float input_slope)
{
	if (m_state->GetActionState(TimeKind::kCurrent)->GetStateKind() != static_cast<int>(player_state::ActionStateKind::kActionNull)) { return; }

	if (input_slope <= kWalkStickSlopeLimit - InputChecker::kStickDeadZone)
	{
		// 速い状態から歩き状態に移行した場合、急速に減速させる
		if (m_move_speed > kWalkSpeed) { m_move_speed = kWalkSpeed; }

		math::Increase(m_move_speed, kAcceleration * FPS::GetDeltaTime(), kSlowWalkSpeed);
		math::Decrease(m_move_speed, kAcceleration * FPS::GetDeltaTime(), kSlowWalkSpeed);
		return;
	}

	// 遅い状態からダッシュ状態に移行した場合、急速に加速させる
	if (m_move_speed < kSlowWalkSpeed) { m_move_speed = kSlowWalkSpeed; }

	math::Increase(m_move_speed, kAcceleration * FPS::GetDeltaTime(), kWalkSpeed);
	math::Decrease(m_move_speed, kAcceleration * FPS::GetDeltaTime(), kWalkSpeed);
}

void Player::CalcCrouchSpeed()
{
	if (m_state->GetMoveState(TimeKind::kCurrent)->GetStateKind() == static_cast<int>(player_state::MoveStateKind::kMoveNull)) { return; }

	// 速い状態から歩き状態に移行した場合、急速に減速させる
	if (m_move_speed > kSlowWalkSpeed) { m_move_speed = kSlowWalkSpeed; }

	math::Decrease(m_move_speed, kAcceleration * FPS::GetDeltaTime(), kCrouchWalkSpeed);
}

void Player::CalcRunSpeed()
{
	if (m_state->GetMoveState(TimeKind::kCurrent)->GetStateKind() == static_cast<int>(player_state::MoveStateKind::kMoveNull)) { return; }

	// 遅い状態からダッシュ状態に移行した場合、急速に加速させる
	if (m_move_speed < kWalkSpeed) { m_move_speed = kWalkSpeed; }

	math::Increase(m_move_speed, kAcceleration * FPS::GetDeltaTime(), kRunSpeed);
}
#pragma endregion


WeaponKind Player::GetCurrentEquipWeaponKind()
{
	return m_current_equip_weapon ? m_current_equip_weapon->GetWeaponKind() : WeaponKind::kNone;
}

void Player::CalcVelocity()
{
	m_velocity = m_move_dir[TimeKind::kCurrent] * m_move_speed;
}

void Player::CalcMoveDir(const VECTOR& velocity)
{
	// 目的とする向きと距離を取得
	m_move_dir[TimeKind::kNext] = v3d::GetNormalizedV(velocity);
	const VECTOR distance_v = m_move_dir[TimeKind::kNext] - m_move_dir[TimeKind::kCurrent];

	// 現在のdirを目的とするdirに近づけていく
	m_move_dir[TimeKind::kCurrent] += v3d::GetNormalizedV(distance_v) * kMoveDirOffsetSpeed;
	const float distance = VSize(m_move_dir[TimeKind::kNext] - m_move_dir[TimeKind::kCurrent]);
	if (distance < kConfirmMoveDirThresholdDistance)
	{
		m_move_dir[TimeKind::kCurrent] = m_move_dir[TimeKind::kNext];
	}
}

void Player::CalcLookDir()
{
	// FIXME : look_dirのY軸が0でないことがある

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
	//DrawFormatString(0, 60, 0xffffff, "angle           : %f", angle * math::kRadianToDegrees);
	//DrawFormatString(0, 80, 0xffffff, "threshold_angle : %f", m_confirm_look_dir_threshold_angle * math::kRadianToDegrees);
	if (angle < m_confirm_look_dir_threshold_angle)
	{
		m_look_dir.at(TimeKind::kCurrent) = m_look_dir.at(TimeKind::kNext);
	}
}

void Player::CalcCameraAimPos()
{
	m_modeler->ApplyMatrix();

	// 追跡するボーンから行列を取得
	const int model_handle	= m_modeler->GetModelHandle();
	const int frame_num		= MV1SearchFrame(model_handle, BonePath.SPINE_2);
	MATRIX frame_mat		= MV1GetFrameLocalWorldMatrix(model_handle, frame_num);
	
	// ボーン自体を追跡すると画面の揺れが強すぎるため同じ高さの位置を追跡
	const VECTOR begin_pos	= m_transform->GetPos(CoordinateKind::kWorld);
	const VECTOR distance	= begin_pos - MGetTranslateElem(frame_mat);
	const VECTOR aim_pos	= begin_pos + m_transform->GetUp(CoordinateKind::kWorld) * VSize(distance);
	m_camera_aim_transform->SetPos(CoordinateKind::kWorld, aim_pos);
}

VECTOR Player::GetVelocityFromPad()
{
	// 移動方向を取得
	const auto camera	= CameraManager::GetInstance()->GetMainCamera();
	const auto right	= camera->GetTransform()->GetRight(CoordinateKind::kWorld);
	auto forward		= camera->GetTransform()->GetForward(CoordinateKind::kWorld);
	forward.y = 0.0f;
	forward = v3d::GetNormalizedV(forward);

	// 各方向のパラメーターを取得
	const auto input = InputChecker::GetInstance();
	const int forward_param		= input->GetInputParameter(pad::StickKind::kLSUp);
	const int backward_param	= input->GetInputParameter(pad::StickKind::kLSDown);
	const int left_param		= input->GetInputParameter(pad::StickKind::kLSLeft);
	const int right_param		= input->GetInputParameter(pad::StickKind::kLSRight);

	// 速度ベクトルを取得
	VECTOR velocity = v3d::GetZeroV();
	if (forward_param)	{ velocity += forward * (forward_param  - InputChecker::kStickDeadZone); }
	if (backward_param) { velocity += forward * (backward_param + InputChecker::kStickDeadZone); }
	if (left_param)		{ velocity += right	  * (left_param	    + InputChecker::kStickDeadZone); }
	if (right_param)	{ velocity += right	  * (right_param    - InputChecker::kStickDeadZone); }

	return velocity;
}

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
