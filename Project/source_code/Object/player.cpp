#include "player.hpp"

Player::Player(std::shared_ptr<Camera> camera) :
	PhysicalObjBase	(ObjName.PLAYER, ObjTag.PLAYER, MassKind::kMedium),
	m_modeler		(nullptr),
	m_animator		(nullptr),
	m_camera		(camera),
	m_dir			(v3d::GetZeroVector()),
	m_velocity		(v3d::GetZeroVector()),
	m_move_speed	(0.0f),
	m_is_move		(false),
	m_is_run		(false)
{
	m_modeler	= std::make_shared<Modeler> (GetTransform(), ModelPath.CHARA_01);
	m_animator	= std::make_shared<Animator>(m_modeler, 3.0f);

	// 初期pos・dirを設定
	m_dir = VGet(0.0f, 0.0f, -1.0f);
	m_transform->SetRotation(CoordinateKind::kWorld, m_dir);
	m_transform->SetPos		(CoordinateKind::kWorld, VGet(0, 0, 0));

	// 衝突用の図形を設定
	MakeCollider(std::make_shared<Capsule>());
	AddTrigger(TriggerKind::kLanding, std::make_shared<Sphere>());

	// 各アニメーション追加
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdle), AnimPath.IDLE_01, AnimTag.NONE, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalk), AnimPath.WALK_01, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kJog),  AnimPath.JOG_01,  AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kRun),  AnimPath.RUN_01,  AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kJump), AnimPath.JUMP_01, AnimTag.NONE, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kFall), AnimPath.FALL_01, AnimTag.NONE, 20.0f, true);
	m_animator->AttachAnim	 (static_cast<int>(PlayerAnimKind::kIdle));
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
}

void Player::Draw() const
{
	m_modeler->Draw();

	DrawFormatString(0,  0, 0xffffff, "speed    : %f", m_move_speed);
	DrawFormatString(0, 20, 0xffffff, "is_move  : %d", m_is_move);
	DrawFormatString(0, 40, 0xffffff, "dir      : %f, %f, %f", m_dir.x, m_dir.y, m_dir.z);
	DrawFormatString(0, 60, 0xffffff, "velocity : %f, %f, %f", m_velocity.x, m_velocity.y, m_velocity.z);
	matrix::Draw(m_transform->GetMatrix(CoordinateKind::kWorld), VGet(0, 100, 0));

	DrawLine3D(v3d::GetZeroVector(), m_transform->GetRight	(CoordinateKind::kWorld) * 100, 0xff0000);
	DrawLine3D(v3d::GetZeroVector(), m_transform->GetUp		(CoordinateKind::kWorld) * 100, 0x00ff22);
	DrawLine3D(v3d::GetZeroVector(), m_transform->GetForward(CoordinateKind::kWorld) * 100, 0x0077ff);
}

void Player::OnCollide(const PhysicalObjBase& check_hit_obj)
{

}

void Player::OnGravity()
{

}

void Player::Move()
{
	m_is_move = false;

	// 各方向の移動
	CalcHorizontalVelocity();
	CalcVerticalVelocity();

	if (m_is_move)
	{
		m_velocity = m_dir * m_move_speed;
		//m_transform->SetRotation(CoordinateKind::kWorld, m_dir);
		m_transform->SetPos		(CoordinateKind::kWorld, m_transform->GetPos(CoordinateKind::kWorld) + m_velocity);
	}

	Sphere sp(m_transform->GetPos(CoordinateKind::kWorld), 40);
	sp.Draw(true, 0, 0xffffff);
}

void Player::CalcHorizontalVelocity()
{
	// カメラの向きから移動方向を抽出
	const VECTOR right = m_camera->GetTransform()->GetRight(CoordinateKind::kWorld);
	VECTOR forwrd = m_camera->GetTransform()->GetForward(CoordinateKind::kWorld);
	forwrd.y = 0.0f;
	forwrd = v3d::GetNormalizedVector(forwrd);

	// 各方向のパラメーターを取得
	const int forward_param		= InputChecker::GetInstance()->GetInputParameter(pad::StickKind::kLSUp);
	const int backward_param	= InputChecker::GetInstance()->GetInputParameter(pad::StickKind::kLSDown);
	const int left_param		= InputChecker::GetInstance()->GetInputParameter(pad::StickKind::kLSLeft);
	const int right_param		= InputChecker::GetInstance()->GetInputParameter(pad::StickKind::kLSRight);

	// 移動方向を取得
	VECTOR move_velocity = v3d::GetZeroVector();
	if (forward_param)
	{
		move_velocity += forwrd * (forward_param  - InputChecker::kStickDeadZone);
		m_is_move = true;
	}
	if (backward_param)
	{
		move_velocity += forwrd * (backward_param + InputChecker::kStickDeadZone);
		m_is_move = true;
	}
	if (left_param)
	{
		move_velocity += right  * (left_param     + InputChecker::kStickDeadZone);
		m_is_move = true;
	}
	if (right_param)
	{
		move_velocity += right  * (right_param    - InputChecker::kStickDeadZone);
		m_is_move = true;
	}

	DrawFormatString(0, 300, 0xffffff, "forward_param  : %d", forward_param);
	DrawFormatString(0, 320, 0xffffff, "backward_param : %d", backward_param);
	DrawFormatString(0, 340, 0xffffff, "left_param     : %d", left_param);
	DrawFormatString(0, 360, 0xffffff, "right_param    : %d", right_param);

	// 移動速度を計算
	CalcMoveSpeed(VSize(move_velocity));

	if (m_is_move){ m_dir = v3d::GetNormalizedVector(move_velocity); }
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

	if (input_slope <= kWalkStickSlopeLimit - InputChecker::kStickDeadZone)
	{
		// 速い状態から歩き状態に移行した場合、急速に減速させる
		if (m_move_speed > kJogSpeed) { m_move_speed = kJogSpeed; }

		Acceleration(kWalkSpeed);
		Deceleration(kWalkSpeed);
		return;
	}

	if (input_slope <= kJogStickSlopeLimit  - InputChecker::kStickDeadZone)
	{
		Acceleration(kJogSpeed);
		Deceleration(kJogSpeed);
		return;
	}

	// 遅い状態からダッシュ状態に移行した場合、急速に加速させる
	if (m_move_speed < kJogSpeed) { m_move_speed = kJogSpeed; }
	Acceleration(kRunSpeed);
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
