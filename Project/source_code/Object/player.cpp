#include "player.hpp"

Player::Player(std::shared_ptr<Camera> camera) :
	CharaBase(ObjName.PLAYER, ObjTag.PLAYER, ModelPath.CHARA_01, MassKind::kMedium),
	m_camera(camera),
	m_dir(v3d::GetZeroVector()),
	m_destination_dir(v3d::GetZeroVector()),
	m_velocity(v3d::GetZeroVector()),
	m_move_speed(0.0f),
	m_is_move(false),
	m_is_run(false)
{
	// 初期pos・dirを設定
	m_dir = m_destination_dir = VGet(0.0f, 0.0f, 1.0f);
	m_transform->SetRotation(CoordinateKind::kWorld, m_dir);
	m_transform->SetPos(CoordinateKind::kWorld, VGet(0, 0, 0));

	// 衝突用の図形を設定
	MakeCollider(std::make_shared<Capsule>());
	AddTrigger(TriggerKind::kLanding, std::make_shared<Sphere>());

	// 各アニメーション追加
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdle01), AnimPath.IDLE_01, 0, AnimTag.NONE, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdle02), AnimPath.IDLE_02, 0, AnimTag.NONE, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalk01), AnimPath.WALK_01, 1, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kJog01),  AnimPath.JOG_01,  1, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kRun01),  AnimPath.RUN_01,  1, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kJump01), AnimPath.JUMP_01, 1, AnimTag.NONE, 20.0f, false);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kFall01), AnimPath.FALL_01, 1, AnimTag.NONE, 20.0f, false);
	m_animator->AttachAnim	 (static_cast<int>(PlayerAnimKind::kIdle01));

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


	// TODO : のちに関数化
	// 仮で武器をアタッチ
	int    attach_frame_num = MV1SearchFrame(m_modeler->GetModelHandle(), BonePath.RIGHT_HAND);
	MATRIX attach_frame_mat = MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), attach_frame_num);
	MATRIX result_frame_mat;
	CreateRotationXYZMatrix(&result_frame_mat, -90.0f * math::kDegreesToRadian, 270.0f * math::kDegreesToRadian, 0.0f);
	m_current_attach_gun->GetTransform()->SetMatrix(CoordinateKind::kWorld, result_frame_mat * attach_frame_mat);
	m_current_attach_gun->GetTransform()->SetPos(CoordinateKind::kLocal, m_current_attach_gun->GetTransform()->GetPos(CoordinateKind::kLocal) + VGet(38.0f, -5.0f, -15.0f));
}

void Player::Draw() const
{
	m_modeler->Draw();
	m_current_attach_gun->Draw();

	Sphere s(m_transform->GetPos(CoordinateKind::kWorld), 40);
	s.Draw(true, 0, 0xffffff);

	//DrawFormatString(0,  0, 0xffffff, "speed    : %f", m_move_speed);
	//DrawFormatString(0, 20, 0xffffff, "is_move  : %d", m_is_move);
	//DrawFormatString(0, 40, 0xffffff, "dir      : %f, %f, %f", m_dir.x, m_dir.y, m_dir.z);
	//DrawFormatString(0, 60, 0xffffff, "velocity : %f, %f, %f", m_velocity.x, m_velocity.y, m_velocity.z);
	//matrix::Draw(m_transform->GetMatrix(CoordinateKind::kWorld), VGet(0, 100, 0));

	//DrawLine3D(v3d::GetZeroVector(), m_transform->GetRight	(CoordinateKind::kWorld) * 100, 0xff0000);
	//DrawLine3D(v3d::GetZeroVector(), m_transform->GetUp		(CoordinateKind::kWorld) * 100, 0x00ff22);
	//DrawLine3D(v3d::GetZeroVector(), m_transform->GetForward(CoordinateKind::kWorld) * 100, 0x0077ff);

}

void Player::OnCollide(const PhysicalObjBase& check_hit_obj)
{

}

void Player::OnGravity()
{

}

void Player::ChangeAnimState()
{

}

void Player::Move()
{
	// ダッシュするかを判定
	JudgeRun();

	// 各方向の移動
	CalcHorizontalVelocity();
	CalcVerticalVelocity();

	// 移動した場合は位置・回転を更新
	if (m_is_move)
	{
		m_velocity = m_dir * m_move_speed;
		//m_transform->SetRotation(CoordinateKind::kWorld, m_dir);
		m_transform->SetPos(CoordinateKind::kWorld, m_transform->GetPos(CoordinateKind::kWorld) + m_velocity);
	}
}

void Player::JudgeRun()
{
	if (InputChecker::GetInstance()->GetCurrentInputDevice() == DeviceKind::kPad)
	{
		if (InputChecker::GetInstance()->IsInput(pad::ButtonKind::kRSPush))
		{

		}
	}
	if (InputChecker::GetInstance()->GetCurrentInputDevice() == DeviceKind::kKeyboard)
	{
		
	}
}

void Player::CalcHorizontalVelocity()
{
	// カメラの向きから移動方向を抽出
	const VECTOR right = m_camera->GetTransform()->GetRight(CoordinateKind::kWorld);
	VECTOR forwrd = m_camera->GetTransform()->GetForward(CoordinateKind::kWorld);
	forwrd.y = 0.0f;
	forwrd = v3d::GetNormalizedVector(forwrd);

	// 採用するvelocityを判定
	VECTOR velocity = v3d::GetZeroVector();
	if (InputChecker::GetInstance()->GetCurrentInputDevice() == DeviceKind::kPad)
	{
		velocity = GetVelocityFromPad  (forwrd, right);
	}
	if (InputChecker::GetInstance()->GetCurrentInputDevice() == DeviceKind::kKeyboard)
	{
		velocity = GetVelocityFromMouse(forwrd, right);
	}

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

	m_destination_dir = v3d::GetNormalizedVector(velocity);

	// 現在のdirと目的のdirが一定距離離れている場合は即座に方向を補正する
	const VECTOR distance = m_destination_dir - m_dir;
	if (VSize(distance) > kDistanceDirToDir)
	{
		m_dir = m_destination_dir;
		return;
	}

	// 現在のdirを目的とするdirに近づけていく
	m_dir += v3d::GetNormalizedVector(distance) * kDirCorrectionSpeed;
	if (VSize(m_destination_dir - m_dir) < 0.1f)
	{
		m_dir = m_destination_dir;
	}
}

VECTOR Player::GetVelocityFromPad(const VECTOR& forwrd, const VECTOR& right)
{
	// 各方向のパラメーターを取得
	const auto input = InputChecker::GetInstance();
	const int forward_param	 = input->GetInputParameter(pad::StickKind::kLSUp);
	const int backward_param = input->GetInputParameter(pad::StickKind::kLSDown);
	const int left_param	 = input->GetInputParameter(pad::StickKind::kLSLeft);
	const int right_param	 = input->GetInputParameter(pad::StickKind::kLSRight);

	// 速度ベクトルを取得
	VECTOR velocity = v3d::GetZeroVector();
	if (forward_param)	{ velocity += forwrd * (forward_param  - InputChecker::kStickDeadZone); }
	if (backward_param) { velocity += forwrd * (backward_param + InputChecker::kStickDeadZone); }
	if (left_param)		{ velocity += right  * (left_param     + InputChecker::kStickDeadZone); }
	if (right_param)	{ velocity += right  * (right_param    - InputChecker::kStickDeadZone); }

	return velocity;
}

VECTOR Player::GetVelocityFromMouse(const VECTOR& forwrd, const VECTOR& right)
{
	const auto input = InputChecker::GetInstance();

	// 移動方向を取得
	VECTOR dir = v3d::GetZeroVector();
	if (input->IsInput(KEY_INPUT_W)) { dir += forwrd; }
	if (input->IsInput(KEY_INPUT_S)) { dir -= forwrd; }
	if (input->IsInput(KEY_INPUT_A)) { dir -= right; }
	if (input->IsInput(KEY_INPUT_D)) { dir += right; }

	// 速度ベクトルを取得
	return v3d::GetNormalizedVector(dir) * InputChecker::kStickMaxSlope;
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

void Player::ConvertMouseVelocityToPadVelocity()
{

}
