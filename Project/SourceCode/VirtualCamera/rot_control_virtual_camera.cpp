#include "rot_control_virtual_camera.hpp"
#include "../Manager/command_handler.hpp"

RotControlVirtualCamera::RotControlVirtualCamera() : 
	ControlVirtualCameraBase(ObjName.ROT_CONTROL_CAMERA, VirtualCameraKind::kRotControl)
{
	m_priority = kPriority;
	m_active_scene_kind.emplace_back(SceneKind::kPlay);

	m_aim ->SetAimCorrect		(VGet(100.0f, 100.0f, 100.0f));
	m_body->SetCameraCorrectPos	(VGet(50.0f, 10.0f, -200.0f));
}

RotControlVirtualCamera::~RotControlVirtualCamera()
{

}

void RotControlVirtualCamera::Init()
{

}

void RotControlVirtualCamera::Update()
{
	InitMove();

	const auto command = CommandHandler::GetInstance();
	command->Execute(CommandKind::kInitAim,			this);
	command->Execute(CommandKind::kMoveUpCamera,	this);
	command->Execute(CommandKind::kMoveDownCamera,	this);
	command->Execute(CommandKind::kMoveLeftCamera,	this);
	command->Execute(CommandKind::kMoveRightCamera, this);

	Move();

	m_aim ->SetRot				(math::ConvertEulerAnglesToRotMatrix(m_input_angle.at(TimeKind::kCurrent)));
	//m_body->SetCameraCorrectDir	(VGet(0, 1, 1));
	//m_body->SetCameraCorrectDir	(-m_transform->GetForward(CoordinateKind::kWorld) + VGet(0, 10, -10));

	m_body->Update();

	DrawSphere3D(m_aim->GetAimPos(), 10, 8, 0xffffff, 0xffffff, FALSE);
}

void RotControlVirtualCamera::LateUpdate()
{

}

void RotControlVirtualCamera::Draw() const
{

}


#pragma region コマンド
void RotControlVirtualCamera::MoveUp()
{
	// 視点リセット中は操作不可
	if (m_is_init_aiming) { return; }

	m_move_dir.x = -1;
	m_is_input.at(static_cast<int>(InputDir::kUp)) = true;
}

void RotControlVirtualCamera::MoveDown()
{
	// 視点リセット中は操作不可
	if (m_is_init_aiming) { return; }

	m_move_dir.x = 1;
	m_is_input.at(static_cast<int>(InputDir::kDown)) = true;
}

void RotControlVirtualCamera::MoveLeft()
{
	// 視点リセット中は操作不可
	if (m_is_init_aiming) { return; }

	m_move_dir.y = -1;
	m_is_input.at(static_cast<int>(InputDir::kLeft)) = true;
}

void RotControlVirtualCamera::MoveRight()
{
	// 視点リセット中は操作不可
	if (m_is_init_aiming) { return; }

	m_move_dir.y = 1;
	m_is_input.at(static_cast<int>(InputDir::kRight)) = true;
}

void RotControlVirtualCamera::InitAim()
{
	if (m_is_init_aiming) { return; }

	// 追跡対象のforwardを目標とする
	const VECTOR forward				= m_target_transform->GetForward(CoordinateKind::kWorld);
	m_input_angle.at(TimeKind::kNext)	= math::GetYawRotVector(forward);
	m_input_angle.at(TimeKind::kNext).y	= math::ConnectMinusPiToPi(m_input_angle.at(TimeKind::kNext).y);

	m_init_angle_speed					= kInitAngleSpeed;
	m_init_end_threshold				= kInitAngleEndThreshold;
	m_is_init_aiming					= true;
}

void RotControlVirtualCamera::InitYawAim()
{	
	if (m_is_init_aiming) { return; }

	// ヨー角以外はカメラ自身の姿勢をそのまま保つ
	const VECTOR forward				= m_target_transform->GetForward(CoordinateKind::kWorld);
	const float  yaw					= math::GetYaw(forward);
	m_input_angle.at(TimeKind::kNext)	= m_input_angle.at(TimeKind::kCurrent);
	m_input_angle.at(TimeKind::kNext).y	= math::ConnectMinusPiToPi(yaw);

	m_init_angle_speed					= kInitYawSpeed;
	m_init_end_threshold				= kInitYawEndThreshold;
	m_is_init_aiming					= true;
}
#pragma endregion


void RotControlVirtualCamera::InitMove()
{
	for (auto& is_input : m_is_input) { is_input = false; }
	m_move_dir = v3d::GetZeroV();
	m_velocity = v3d::GetZeroV();
}

void RotControlVirtualCamera::Move()
{
	if (!m_target_transform) { return; }

	CalcMoveDirFromPad();
	CalcMoveDirFromMouse();
	CalcMoveDirFromCommand();

	CalcInputAngle();
}

void RotControlVirtualCamera::CalcMoveDirFromPad()
{
	if (m_is_init_aiming)															{ return; }
	if (m_move_dir != v3d::GetZeroV())												{ return; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kPad)	{ return; }

	// 各方向のパラメーターを取得
	const auto input		= InputChecker::GetInstance();
	const auto up_param		= input->GetInputParameter(pad::StickKind::kRSUp);
	const auto down_param	= input->GetInputParameter(pad::StickKind::kRSDown);
	const auto left_param	= input->GetInputParameter(pad::StickKind::kRSLeft);
	const auto right_param	= input->GetInputParameter(pad::StickKind::kRSRight);

	// 速度ベクトル・入力判定を取得
	if (up_param)
	{
		m_velocity.x = -math::GetUnitValue<int, float>(InputChecker::kStickDeadZone,  InputChecker::kStickMaxSlope, up_param);
		m_is_input.at(static_cast<int>(InputDir::kUp))    = true;
	}
	if (down_param)
	{
		m_velocity.x =  math::GetUnitValue<int, float>(InputChecker::kStickDeadZone, -InputChecker::kStickMinSlope, -down_param);
		m_is_input.at(static_cast<int>(InputDir::kDown))  = true;
	}
	if (left_param)
	{
		m_velocity.y = -math::GetUnitValue<int, float>(InputChecker::kStickDeadZone, -InputChecker::kStickMinSlope, -left_param);
		m_is_input.at(static_cast<int>(InputDir::kLeft))  = true;
	}
	if (right_param)
	{
		m_velocity.y =  math::GetUnitValue<int, float>(InputChecker::kStickDeadZone,  InputChecker::kStickMaxSlope, right_param);
		m_is_input.at(static_cast<int>(InputDir::kRight)) = true;
	}
	m_velocity *= kMoveSpeedWithStick;

	// 入力方向も合わせて取得
	m_move_dir = v3d::GetNormalizedV(m_velocity);
}

void RotControlVirtualCamera::CalcMoveDirFromMouse()
{
	if (m_is_init_aiming)																{ return; }
	if (m_move_dir != v3d::GetZeroV())													{ return; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kKeyboard)	{ return; }

	// 移動速度を取得
	Vector2D<float> velocity_2d = InputChecker::GetInstance()->GetMouseVelocity(TimeKind::kCurrent);
	m_velocity = VGet(velocity_2d.y, velocity_2d.x, 0.0f) * kMoveSpeedWithMouse;

	// 入力判定を取得
	if (m_velocity.x > 0) { m_is_input.at(static_cast<int>(InputDir::kUp))		= true; }
	if (m_velocity.x < 0) { m_is_input.at(static_cast<int>(InputDir::kDown))	= true; }
	if (m_velocity.y > 0) { m_is_input.at(static_cast<int>(InputDir::kLeft))	= true; }
	if (m_velocity.y < 0) { m_is_input.at(static_cast<int>(InputDir::kRight))	= true; }

	// 入力方向も合わせて取得
	m_move_dir = v3d::GetNormalizedV(m_velocity);
}

void RotControlVirtualCamera::CalcMoveDirFromCommand()
{
	const auto command = CommandHandler::GetInstance();

	// コマンドパターンで入力された場合の速度・方向を取得
	if (	command->GetCurrentFrameExecuteInputCode(CommandKind::kMoveUpCamera)
		||	command->GetCurrentFrameExecuteInputCode(CommandKind::kMoveDownCamera)
		||	command->GetCurrentFrameExecuteInputCode(CommandKind::kMoveLeftCamera)
		||	command->GetCurrentFrameExecuteInputCode(CommandKind::kMoveRightCamera))
	{
		m_move_dir = v3d::GetNormalizedV(m_move_dir);
		m_velocity = m_move_dir * kMoveSpeedWithButton;
	}
}

void RotControlVirtualCamera::CalcInputAngle()
{
	// 視点リセット
	CalcInitAim();

	m_velocity *= FPS::GetDeltaTime();

	// 角度を取得
	if (m_is_input.at(static_cast<int>(InputDir::kUp)))		{ m_input_angle.at(TimeKind::kCurrent).x += m_velocity.x; }
	if (m_is_input.at(static_cast<int>(InputDir::kDown)))	{ m_input_angle.at(TimeKind::kCurrent).x += m_velocity.x; }
	if (m_is_input.at(static_cast<int>(InputDir::kLeft)))	{ m_input_angle.at(TimeKind::kCurrent).y += m_velocity.y; }
	if (m_is_input.at(static_cast<int>(InputDir::kRight)))	{ m_input_angle.at(TimeKind::kCurrent).y += m_velocity.y; }

	m_input_angle.at(TimeKind::kCurrent).y = math::ConnectMinusPiToPi(m_input_angle.at(TimeKind::kCurrent).y);

	// 角度制限
	if (m_input_angle.at(TimeKind::kCurrent).x < kMinVerticalInputAngle * math::kDegreesToRadian) { m_input_angle.at(TimeKind::kCurrent).x = kMinVerticalInputAngle * math::kDegreesToRadian; }
	if (m_input_angle.at(TimeKind::kCurrent).x > kMaxVerticalInputAngle * math::kDegreesToRadian) { m_input_angle.at(TimeKind::kCurrent).x = kMaxVerticalInputAngle * math::kDegreesToRadian; }
}

void RotControlVirtualCamera::CalcInitAim()
{
	if (!m_is_init_aiming) { return; }

	VECTOR distance_v	= m_input_angle.at(TimeKind::kNext) - m_input_angle.at(TimeKind::kCurrent);
	distance_v.y		= math::ConnectMinusPiToPi(distance_v.y);
	VECTOR dir			= v3d::GetNormalizedV(distance_v);

	// 右・左回りから最短経路を取得し、回転方向に反映
	float distance = VSize(distance_v);
	const float shortest = min(distance, DX_TWO_PI_F - distance);
	if (distance != shortest)
	{
		dir.y *= -1;
	}

	// 目的地に遠いほど速く移動させる
	m_input_angle.at(TimeKind::kCurrent) += dir * distance * m_init_angle_speed * FPS::GetDeltaTime();

	// 終了判定
	distance = VSize(m_input_angle.at(TimeKind::kNext) - m_input_angle.at(TimeKind::kCurrent));
	if (distance < m_init_end_threshold)
	{
		m_input_angle.at(TimeKind::kCurrent) = m_input_angle.at(TimeKind::kNext);
		m_is_init_aiming = false;
	}
}

//void RotControlVirtualCamera::CalcPos()
//{
//	const VECTOR look_pos	= m_aim->GetAimPos();
//	const VECTOR forward	= m_transform->GetForward(CoordinateKind::kWorld);
//	const VECTOR pos		= look_pos - forward * m_distance_to_target;
//	m_transform->SetPos(CoordinateKind::kWorld, pos);
//}

//void Camera::CalcDistance()
//{
//	// 対象を上から見ると離れ、下から見ると近づく
//	const float min  = kMinVerticalAngle * math::kDegreesToRadian;
//	const float max  = kMaxVerticalAngle * math::kDegreesToRadian;
//	const float rate = math::GetUnitValue<float, float>(min, max, m_input_angle.at(TimeKind::kCurrent).x);
//
//	m_distance_to_target = (kMaxDistanceToTarget - kMinDistanceToTarget) * rate + kMinDistanceToTarget;
//}

//void RotControlVirtualCamera::JudgeLookSameDirTarget()
//{
//	const VECTOR forward = m_target_transform->GetForward(CoordinateKind::kWorld);
//	const float  yaw = math::GetYaw(forward);
//	m_input_angle.at(TimeKind::kCurrent).y;
//
//	m_is_look_same_dir_target = m_input_angle.at(TimeKind::kCurrent).y == yaw ? true : false;
//}

//void RotControlVirtualCamera::ApplyInvert()
//{
//	if (m_is_invert_horizontal) { m_move_dir.y *= -1; }
//	if (m_is_invert_vertical) { m_move_dir.x *= -1; }
//}
