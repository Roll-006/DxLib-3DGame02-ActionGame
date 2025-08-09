#include "rot_control_virtual_camera.hpp"
#include "../Command/command_handler.hpp"

RotControlVirtualCamera::RotControlVirtualCamera(const int priority) :
	VirtualCameraBase	(ObjName.ROT_CONTROL_VIRTUAL_CAMERA, VirtualCameraKind::kControl),
	m_data				(ControlVirtualCameraData())
{
	m_priority = priority;
	m_active_scene_kind.emplace_back(SceneKind::kPlay);

	m_aim->SetTrackedObjOffset	(VGet(10.0f, 10.0f, 0.0f));
	m_aim->SetHorizontalDamping	(0.05f);
	m_aim->SetVerticalDamping	(0.0f);

	m_body->SetFollowOffset		(VGet(30.0f, 20.0f, -90.0f));
	m_body->SetDamping			(VGet(0.015f, 0.015f, 0.0f));
}

RotControlVirtualCamera::~RotControlVirtualCamera()
{

}

void RotControlVirtualCamera::Init()
{

}

void RotControlVirtualCamera::Update()
{
	if (!IsActive()) { return; }

	InitMove();
	Move();
}

void RotControlVirtualCamera::LateUpdate()
{
	if (!IsActive()) { return; }

	MATRIX m = MGetIdent();
	CreateRotationXYZMatrix(&m, m_data.input_angle[TimeKind::kCurrent].x, m_data.input_angle[TimeKind::kCurrent].y, m_data.input_angle[TimeKind::kCurrent].z);
	m_target_transform->SetRot(CoordinateKind::kWorld, MGetRotElem(m));

	CalcTransform();
}

void RotControlVirtualCamera::DrawToShadowMap() const
{
	if (!IsActive()) { return; }
}

void RotControlVirtualCamera::Draw() const
{
	if (!IsActive()) { return; }
}


#pragma region コマンド
//void RotControlVirtualCamera::MoveUp()
//{
//	// 視点リセット中は操作不可
//	if (m_data.is_init_aiming) { return; }
//
//	m_data.move_dir.x = -1;
//}
//
//void RotControlVirtualCamera::MoveDown()
//{
//	// 視点リセット中は操作不可
//	if (m_data.is_init_aiming) { return; }
//
//	m_data.move_dir.x = 1;
//}
//
//void RotControlVirtualCamera::MoveLeft()
//{
//	// 視点リセット中は操作不可
//	if (m_data.is_init_aiming) { return; }
//
//	m_data.move_dir.y = -1;
//}
//
//void RotControlVirtualCamera::MoveRight()
//{
//	// 視点リセット中は操作不可
//	if (m_data.is_init_aiming) { return; }
//
//	m_data.move_dir.y = 1;
//}

void RotControlVirtualCamera::InitAim()
{
	if (m_data.is_init_aiming) { return; }

	// 追跡対象のforwardを目標とする
	const VECTOR forward					= m_target_transform->GetForward(CoordinateKind::kWorld);
	m_data.input_angle[TimeKind::kNext]		= math::GetYawRotVector(forward);
	m_data.input_angle[TimeKind::kNext].y	= math::ConnectMinusPiToPi(m_data.input_angle[TimeKind::kNext].y);

	m_data.init_angle_speed					= kInitAngleSpeed;
	m_data.init_end_threshold				= kInitAngleEndThreshold;
	m_data.is_init_aiming					= true;
}

void RotControlVirtualCamera::InitYawAim()
{	
	if (m_data.is_init_aiming) { return; }

	// ヨー角以外はカメラ自身の姿勢をそのまま保つ
	const VECTOR forward				= m_target_transform->GetForward(CoordinateKind::kWorld);
	const float  yaw					= math::GetYaw(forward);
	m_data.input_angle[TimeKind::kNext]	= m_data.input_angle[TimeKind::kCurrent];
	m_data.input_angle[TimeKind::kNext].y	= math::ConnectMinusPiToPi(yaw);

	m_data.init_angle_speed					= kInitYawSpeed;
	m_data.init_end_threshold				= kInitYawEndThreshold;
	m_data.is_init_aiming					= true;
}
#pragma endregion


void RotControlVirtualCamera::InitMove()
{
	m_data.move_dir = v3d::GetZeroV();
	m_data.velocity = v3d::GetZeroV();
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
	if (m_data.is_init_aiming)														{ return; }
	if (m_data.move_dir != v3d::GetZeroV())											{ return; }
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
		m_data.velocity.x = -math::GetUnitValue<int, float>(InputChecker::kStickDeadZone,  InputChecker::kStickMaxSlope, up_param);
	}
	if (down_param)
	{
		m_data.velocity.x =  math::GetUnitValue<int, float>(InputChecker::kStickDeadZone, -InputChecker::kStickMinSlope, -down_param);
	}
	if (left_param)
	{
		m_data.velocity.y = -math::GetUnitValue<int, float>(InputChecker::kStickDeadZone, -InputChecker::kStickMinSlope, -left_param);
	}
	if (right_param)
	{
		m_data.velocity.y =  math::GetUnitValue<int, float>(InputChecker::kStickDeadZone,  InputChecker::kStickMaxSlope, right_param);
	}
	m_data.velocity *= kMoveSpeedWithStick;

	// 入力方向も合わせて取得
	m_data.move_dir = v3d::GetNormalizedV(m_data.velocity);
}

void RotControlVirtualCamera::CalcMoveDirFromMouse()
{
	if (m_data.is_init_aiming)															{ return; }
	if (m_data.move_dir != v3d::GetZeroV())												{ return; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kKeyboard)	{ return; }

	// 移動速度を取得
	Vector2D<float> velocity_2d = InputChecker::GetInstance()->GetMouseVelocity(TimeKind::kCurrent);
	m_data.velocity = VGet(velocity_2d.y, velocity_2d.x, 0.0f) * kMoveSpeedWithMouse;

	// 入力方向も合わせて取得
	m_data.move_dir = v3d::GetNormalizedV(m_data.velocity);
}

void RotControlVirtualCamera::CalcMoveDirFromCommand()
{
	if (m_data.is_init_aiming) { return; }

	const auto input	= InputChecker	::GetInstance();
	const auto command	= CommandHandler::GetInstance();

	// パッド・マウスの入力は例外的にコマンド入力の動きを適用しない
	for (int i = 0; i < 4; ++i)
	{
		if (input->IsInput(static_cast<mouse::SlideDirKind>(i))) { return; }
	}
	for (int i = 0; i < 8; ++i)
	{
		if (input->IsInput(static_cast<pad::StickKind>(i))) { return; }
	}

	// コマンドパターンで入力された場合の速度・方向を取得
	if (command->IsExecuting(CommandKind::kMoveUpCamera))    { m_data.move_dir.x = -1; }
	if (command->IsExecuting(CommandKind::kMoveDownCamera))  { m_data.move_dir.x =  1; }
	if (command->IsExecuting(CommandKind::kMoveLeftCamera))  { m_data.move_dir.y = -1; }
	if (command->IsExecuting(CommandKind::kMoveRightCamera)) { m_data.move_dir.y =  1; }

	if (m_data.move_dir != v3d::GetZeroV())
	{
		m_data.move_dir = v3d::GetNormalizedV(m_data.move_dir);
		m_data.velocity = m_data.move_dir * kMoveSpeedWithButton;
	}
}

void RotControlVirtualCamera::CalcInputAngle()
{
	// 視点リセット
	CalcInitAim();

	const auto time_manager = GameTimeManager::GetInstance();
	m_data.velocity *= time_manager->GetDeltaTime(TimeScaleController::LayerKind::kCamera);

	// 角度を取得
	const auto command = CommandHandler::GetInstance();
	if (command->IsExecuting(CommandKind::kMoveUpCamera))	{ m_data.input_angle[TimeKind::kCurrent].x += m_data.velocity.x; }
	if (command->IsExecuting(CommandKind::kMoveDownCamera))	{ m_data.input_angle[TimeKind::kCurrent].x += m_data.velocity.x; }
	if (command->IsExecuting(CommandKind::kMoveLeftCamera))	{ m_data.input_angle[TimeKind::kCurrent].y += m_data.velocity.y; }
	if (command->IsExecuting(CommandKind::kMoveRightCamera)){ m_data.input_angle[TimeKind::kCurrent].y += m_data.velocity.y; }

	m_data.input_angle[TimeKind::kCurrent].y = math::ConnectMinusPiToPi(m_data.input_angle[TimeKind::kCurrent].y);

	// 角度制限
	if (m_data.input_angle[TimeKind::kCurrent].x < kMinVerticalInputAngle * math::kDegToRad) { m_data.input_angle[TimeKind::kCurrent].x = kMinVerticalInputAngle * math::kDegToRad; }
	if (m_data.input_angle[TimeKind::kCurrent].x > kMaxVerticalInputAngle * math::kDegToRad) { m_data.input_angle[TimeKind::kCurrent].x = kMaxVerticalInputAngle * math::kDegToRad; }
}

void RotControlVirtualCamera::CalcInitAim()
{
	if (!m_data.is_init_aiming) { return; }

	VECTOR distance_v	= m_data.input_angle[TimeKind::kNext] - m_data.input_angle[TimeKind::kCurrent];
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
	const auto time_manager = GameTimeManager::GetInstance();
	m_data.input_angle[TimeKind::kCurrent] += dir * distance * m_data.init_angle_speed * time_manager->GetDeltaTime(TimeScaleController::LayerKind::kCamera);

	// 終了判定
	distance = VSize(m_data.input_angle[TimeKind::kNext] - m_data.input_angle[TimeKind::kCurrent]);
	if (distance < m_data.init_end_threshold)
	{
		m_data.input_angle[TimeKind::kCurrent] = m_data.input_angle.at(TimeKind::kNext);
		m_data.is_init_aiming = false;
	}
}
