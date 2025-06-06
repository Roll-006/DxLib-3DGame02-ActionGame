#include "player.hpp"
#include "../Manager/command_handler.hpp"

Player::Player(std::shared_ptr<Camera> camera) :
	CharaBase		(ObjName.PLAYER, ObjTag.PLAYER, ModelPath.CHARA_01, MassKind::kMedium),
	m_camera		(camera),
	m_move_dir		(v3d::GetZeroVector()),
	m_velocity		(v3d::GetZeroVector()),
	m_move_speed	(0.0f),
	m_is_move		(false),
	m_is_run		(false)
{
	// 初期pos・dirを設定
	m_dir[TimeKind::kCurrent] = m_dir[TimeKind::kNext] = VGet(0.0f, 0.0f, 1.0f);
	m_transform->SetRot(CoordinateKind::kWorld, m_dir.at(TimeKind::kCurrent));
	m_transform->SetPos(CoordinateKind::kWorld, VGet(0, 0, 0));

	// コライダーを設定
	const auto begin_pos = m_transform->GetPos(CoordinateKind::kWorld) + VGet(0.0f, kColliderCapsuleRadius, 0.0f);
	const auto segment_length = kColliderCapsuleLength - kColliderCapsuleRadius * 2.0f;
	MakeCollider(std::make_shared<Capsule>(begin_pos, m_transform->GetUp(CoordinateKind::kWorld), segment_length, kColliderCapsuleRadius));

	// トリガーを設定
	const auto pos = begin_pos - VGet(0.0f, 18.0f, 0.0f);
	AddTrigger(TriggerKind::kLanding, std::make_shared<Sphere>(pos, kLandingTriggerRadius));

	// 各アニメーション追加
	LoadAnim();
	m_animator->AttachAnim(static_cast<int>(PlayerAnimKind::kIdle01));

	// 武器設定
	AddGun(std::make_shared<AssaultRifle>());
	AttachGun(GunKind::kAssaultRifle);
}

Player::~Player()
{

}

void Player::Init()
{

}

void Player::Update()
{
	Move();
	m_animator->Update();

	// 武器はモデルの行列情報をもとに位置を決定するため一度モデルに行列を適用
	m_modeler->ApplyMatrix();
	m_current_attach_gun->TrackOwner();
}

void Player::Draw() const
{
	m_modeler->Draw();
	m_current_attach_gun->Draw();

	dynamic_cast<Capsule*>(m_collider.get())->Draw(true, 0, 0xffffff);
	dynamic_cast<Sphere*>(m_trigger.at(TriggerKind::kLanding).get())->Draw(true, 0, 0xffffff);

	auto pos  = m_transform->GetPos (CoordinateKind::kWorld);
	auto axes = m_transform->GetAxes(CoordinateKind::kWorld);
	DrawLine3D(pos, pos + axes.x_axis * 100, 0xff0000);
	DrawLine3D(pos, pos + axes.y_axis * 100, 0x00ff22);
	DrawLine3D(pos, pos + axes.z_axis * 100, 0x0077ff);
}

void Player::OnCollide(const PhysicalObjBase& check_hit_obj)
{

}

void Player::OnGravity()
{

}


#pragma region コマンド
void Player::Run()
{
	const auto command = CommandHandler::GetInstance();
	const auto input   = InputChecker  ::GetInstance();
	const auto code    = *command->GetCurrentFrameExecuteInputCode(CommandKind::kRun);

	switch (command->GetInputModeKind(CommandHandler::MoveKind::kRun))
	{
	case InputModeKind::kTrigger:
		if (input->GetInputState(code) == InputState::kSingle)
		{
			command->CountUpTrigger(CommandHandler::MoveKind::kRun);

			m_is_run = command->GetTriggerCount(CommandHandler::MoveKind::kRun) % 2 ? true : false;
		}
		break;

	case InputModeKind::kHold:
		if (input->GetInputState(code) == InputState::kHold)
		{
			m_is_run = true;
		}
		break;
	}
}

void Player::MoveForward()
{
	VECTOR forward = m_camera->GetTransform()->GetForward(CoordinateKind::kWorld);
	forward.y = 0.0f;

	m_move_dir += v3d::GetNormalizedVector(forward);
}

void Player::MoveBackward()
{
	VECTOR forward = m_camera->GetTransform()->GetForward(CoordinateKind::kWorld);
	forward.y = 0.0f;

	m_move_dir -= v3d::GetNormalizedVector(forward);
}

void Player::MoveLeft()
{
	m_move_dir -= m_camera->GetTransform()->GetRight(CoordinateKind::kWorld);
}

void Player::MoveRight()
{
	m_move_dir += m_camera->GetTransform()->GetRight(CoordinateKind::kWorld);
}
#pragma endregion


void Player::LoadAnim()
{
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdle01),					AnimPath.IDLE_01,						0, AnimTag.NONE, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleSquat01),				AnimPath.IDLE_SQUAT_01,					0, AnimTag.NONE, 20.0f, true);

	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkSquatForward01),		AnimPath.WALK_SQUAT_FORWARD_01,			0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkSquatBackward01),		AnimPath.WALK_SQUAT_BACKWARD_01,		0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkSquatLeft01),			AnimPath.WALK_SQUAT_LEFT_01,			0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkSquatRight01),			AnimPath.WALK_SQUAT_RIGHT_01,			0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkSquatForwardLeft01),	AnimPath.WALK_SQUAT_FORWARD_LEFT_01,	0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkSquatForwardRight01),	AnimPath.WALK_SQUAT_FORWARD_RIGHT_01,	0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkSquatBackwardLeft01),	AnimPath.WALK_SQUAT_BACKWARD_LEFT_01,	0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkSquatBackwardRight01),	AnimPath.WALK_SQUAT_BACKWARD_RIGHT_01,	0, AnimTag.WALK, 20.0f, true);

	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootForward01),		AnimPath.WALK_SHOOT_FORWARD_01,			0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootBackward01),		AnimPath.WALK_SHOOT_BACKWARD_01,		0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootLeft01),			AnimPath.WALK_SHOOT_LEFT_01,			0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootRight01),			AnimPath.WALK_SHOOT_RIGHT_01,			0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootForwardLeft01),	AnimPath.WALK_SHOOT_FORWARD_LEFT_01,	0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootForwardRight01),	AnimPath.WALK_SHOOT_FORWARD_RIGHT_01,	0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootBackwardLeft01),	AnimPath.WALK_SHOOT_BACKWARD_LEFT_01,	0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootBackwardRight01),	AnimPath.WALK_SHOOT_BACKWARD_RIGHT_01,	0, AnimTag.WALK, 20.0f, true);

	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kRunForward01),				AnimPath.RUN_FORWARD_01,				0, AnimTag.WALK, 20.0f, true);
}

void Player::ChangeAnimState()
{

}

void Player::Move()
{
	// 初期化
	const auto command = CommandHandler::GetInstance();
	m_move_dir = v3d::GetZeroVector();

	// ダッシュ判定をリセット
	if (command->GetInputModeKind(CommandHandler::MoveKind::kRun) == InputModeKind::kHold) { m_is_run = false; }
	if (!m_is_move)
	{
		m_is_run = false;
		command->InitTriggerCount(CommandHandler::MoveKind::kRun);
	}

	command->Execute(CommandKind::kRun,				*this);
	command->Execute(CommandKind::kMoveUpPlayer,	*this);
	command->Execute(CommandKind::kMoveDownPlayer,	*this);
	command->Execute(CommandKind::kMoveLeftPlayer,	*this);
	command->Execute(CommandKind::kMoveRightPlayer,	*this);

	// 各方向の移動
	CalcHorizontalVelocity();
	CalcVerticalVelocity();

	// 移動した場合は位置・回転を更新
	if (m_is_move)
	{
		m_velocity = m_dir.at(TimeKind::kCurrent) * m_move_speed;
		m_transform->SetRot(CoordinateKind::kWorld, m_dir.at(TimeKind::kCurrent));
		m_transform->SetPos(CoordinateKind::kWorld, m_transform->GetPos(CoordinateKind::kWorld) + m_velocity);

		m_collider->Move(m_velocity);
		for (const auto& trigger : m_trigger)
		{
			trigger.second->Move(m_velocity);
		}
	}
}

void Player::CalcHorizontalVelocity()
{
	VECTOR velocity = v3d::GetNormalizedVector(m_move_dir) * InputChecker::kStickMaxSlope;
	velocity = GetVelocityFromPad(velocity);

	// 移動判定
	m_is_move = velocity != v3d::GetZeroVector() ? true : false;

	// 移動速度・方向を計算
	CalcMoveSpeed(VSize(velocity));
	CalcDir(velocity);
}

void Player::CalcVerticalVelocity()
{

}

void Player::CalcMoveSpeed(const float input_slope)
{
	// 移動していない場合は速度を0に設定
	if (!m_is_move)
	{
		m_move_speed = 0.0f;
		return;
	}

	// 歩き処理
	if (input_slope <= kWalkStickSlopeLimit - InputChecker::kStickDeadZone)
	{
		// 速い状態から歩き状態に移行した場合、急速に減速させる
		if (m_move_speed > kWalkSpeed) { m_move_speed = kWalkSpeed; }

		Acceleration(kSlowWalkSpeed);
		Deceleration(kSlowWalkSpeed);
		return;
	}

	// ジョギング処理
	if (!m_is_run)
	{
		Acceleration(kWalkSpeed);
		Deceleration(kWalkSpeed);
		return;
	}

	// ダッシュ処理
	if (m_is_run)
	{
		// 遅い状態からダッシュ状態に移行した場合、急速に加速させる
		if (m_move_speed < kWalkSpeed) { m_move_speed = kWalkSpeed; }
		Acceleration(kRunSpeed);
	}
}

void Player::CalcDir(const VECTOR& velocity)
{
	if (!m_is_move) { return; }

	// 目的とする向きと距離を取得
	m_dir.at(TimeKind::kNext) = v3d::GetNormalizedVector(velocity);
	const VECTOR distance = m_dir.at(TimeKind::kNext) - m_dir.at(TimeKind::kCurrent);

	// 現在のdirを目的とするdirに近づけていく
	m_dir.at(TimeKind::kCurrent) += v3d::GetNormalizedVector(distance) * kDirCorrectionSpeed;
	if (VSize(m_dir.at(TimeKind::kNext) - m_dir.at(TimeKind::kCurrent)) < kConfirmDirThreshold)
	{
		m_dir.at(TimeKind::kCurrent) = m_dir.at(TimeKind::kNext);
	}
}

VECTOR Player::GetVelocityFromPad(VECTOR& velocity)
{
	if (velocity != v3d::GetZeroVector()) { return velocity; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kPad) { return velocity; }

	// 移動方向を取得
	const VECTOR right = m_camera->GetTransform()->GetRight(CoordinateKind::kWorld);
	VECTOR forward = m_camera->GetTransform()->GetForward(CoordinateKind::kWorld);
	forward.y = 0.0f;
	forward = v3d::GetNormalizedVector(forward);

	// 各方向のパラメーターを取得
	const auto input = InputChecker::GetInstance();
	const int forward_param	 = input->GetInputParameter(pad::StickKind::kLSUp   );
	const int backward_param = input->GetInputParameter(pad::StickKind::kLSDown );
	const int left_param	 = input->GetInputParameter(pad::StickKind::kLSLeft );
	const int right_param	 = input->GetInputParameter(pad::StickKind::kLSRight);

	// 速度ベクトルを取得
	if (forward_param)	{ velocity += forward * (forward_param  - InputChecker::kStickDeadZone); }
	if (backward_param) { velocity += forward * (backward_param + InputChecker::kStickDeadZone); }
	if (left_param)		{ velocity += right   * (left_param     + InputChecker::kStickDeadZone); }
	if (right_param)	{ velocity += right   * (right_param    - InputChecker::kStickDeadZone); }

	return velocity;
}

void Player::Acceleration(const float destination_speed)
{
	if (m_move_speed >= destination_speed) { return; }

	m_move_speed += kAcceleration * FPS::GetDeltaTime();
	if (m_move_speed > destination_speed)
	{
		m_move_speed = destination_speed;
	}
}

void Player::Deceleration(const float destination_speed)
{
	if (m_move_speed <= destination_speed) { return; }
	
	m_move_speed -= kAcceleration * FPS::GetDeltaTime();
	if (m_move_speed < destination_speed)
	{
		m_move_speed = destination_speed;
	}
}
