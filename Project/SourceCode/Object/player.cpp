#include "player.hpp"
#include "../Command/command_handler.hpp"
#include "../Part/player_state_controller.hpp"

Player::Player() :
	CharaBase(ObjName.PLAYER, ObjTag.PLAYER, ModelPath.CHARA_06, MassKind::kMedium),
	m_state								(std::make_shared<PlayerStateController>()),
	m_move_speed						(0.0f),
	m_look_dir_correct_angle			(0.0f),
	m_confirm_look_dir_threshold_angle	(0.0f),
	m_bone_pos_corrector				(std::make_shared<BonePosCorrector>())
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
	const auto gun = std::make_shared<AssaultRifle>();
	AddWeapon(gun);
	//AttachWeapon(gun);

	// TODO : 仮で銃のオブジェ登録
	ObjManager::GetInstance()->AddObj(gun);
	CollisionManager::GetInstance()->AddCollideObj(gun);
	//PhysicsManager::GetInstance()->AddPhysicalObj(m_current_attach_gun);
	//PhysicsManager::GetInstance()->AddIgnoreObjGravity(ObjName.ASSAULT_RIFLE);

	// TODO : 仮で銃用のカメラを登録
	const auto camera_manager = CameraManager::GetInstance();

	const auto rot_camera = std::make_shared<RotControlVirtualCamera>(1);
	camera_manager->AddVirtualCamera(rot_camera);
	rot_camera->AttachTarget(m_transform);

	// TODO : 仮で所持残り弾数を設定
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

	m_look_dir_correct_angle			= kLookDirCorrectAngle;
	m_confirm_look_dir_threshold_angle	= kConfirmLookDirThresholdAngle * math::kDegreesToRadian;

	m_state->Update(this);
	m_animator->Update();

	CalcMoveDir(m_velocity);
	CalcLookDir();
	CalcVelocity();
	CalcCapsuleColliderLength();

	UpdateTransform(m_look_dir.at(TimeKind::kCurrent), kModelScale);

	// MEMO : Updateでは弾丸の発射判定のみを行うものとする

	m_is_landing = false;
}

void Player::LateUpdate()
{
	if (!IsActive()) { return; }

	m_state->LateUpdate(this);

	// MEMO : LateUpdateで実際の弾丸を発射させる

	// ボーン位置修正
	//m_modeler->ApplyMatrix();
	//m_bone_pos_corrector->CorrectGunPoseBone(
	//	m_modeler->GetModelHandle(),
	//	m_look_dir.at(TimeKind::kCurrent),
	//	m_camera->GetTransform()->GetMatrix(CoordinateKind::kWorld),
	//	std::dynamic_pointer_cast<GunBase>(m_current_attach_weapon)->IsAiming());

	//m_current_attach_weapon->LateUpdate();
}

void Player::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
	//m_current_attach_weapon->Draw();

	for (auto& collider : m_collider)
	{
		const auto shape = collider->GetShape();
		if (shape != nullptr)
		{
			shape->Draw(true, 0, 0xffffff);
		}
	}
}

void Player::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kLandingTrigger:
		m_is_landing = true;
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

void Player::SetLookDirCorrectValueForAim()
{
	m_look_dir_correct_angle			= kLookDirCorrectAngleForAim;
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
	m_move_dir[TimeKind::kCurrent] += v3d::GetNormalizedV(distance_v) * kMoveDirCorrectSpeed;
	const float distance = VSize(m_move_dir[TimeKind::kNext] - m_move_dir[TimeKind::kCurrent]);
	if (distance < kConfirmMoveDirThresholdDistance)
	{
		m_move_dir[TimeKind::kCurrent] = m_move_dir[TimeKind::kNext];
	}
}

void Player::CalcLookDir()
{
	// FIXME : 振動発生中。look_dirのY軸が0でないことが原因を思われる。

	// ヨー角回転を取得し、-π～πで値を管理する
	const VECTOR current_yaw = math::GetYawRotVector(m_look_dir.at(TimeKind::kCurrent));
	const VECTOR next_yaw	 = math::GetYawRotVector(m_look_dir.at(TimeKind::kNext));
	VECTOR distance = next_yaw - current_yaw;
	distance.y = math::ConnectMinusPiToPi(distance.y);

	// カメラを基準にして右側であった場合は反転
	if (distance.y > 0) { m_look_dir_correct_angle *= -1; }

	// 回転を適用
	const Quaternion rot_q = quat::CreateQuaternion(axis::GetWorldYAxis(), -m_look_dir_correct_angle);
	m_look_dir.at(TimeKind::kCurrent) = math::GetRotatedPos(m_look_dir.at(TimeKind::kCurrent), rot_q);

	const float angle = math::GetAngleBetweenTwoVector(m_look_dir.at(TimeKind::kNext), m_look_dir.at(TimeKind::kCurrent));
	//DrawFormatString(0, 60, 0xffffff, "angle : %f", angle * math::kRadianToDegrees);
	if (angle < m_confirm_look_dir_threshold_angle)
	{
		m_look_dir.at(TimeKind::kCurrent) = m_look_dir.at(TimeKind::kNext);
	}
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
	if (forward_param)	{ velocity += forward	* (forward_param	- InputChecker::kStickDeadZone); }
	if (backward_param) { velocity += forward	* (backward_param	+ InputChecker::kStickDeadZone); }
	if (left_param)		{ velocity += right		* (left_param		+ InputChecker::kStickDeadZone); }
	if (right_param)	{ velocity += right		* (right_param		- InputChecker::kStickDeadZone); }

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
