#include "camera.hpp"
#include "../Manager/command_handler.hpp"

Camera::Camera() : 
	PhysicalObjBase			(ObjName.CAMERA, ObjTag.CAMERA, MassKind::kVeryLight),
	m_target_transform		(nullptr),
	m_distance_to_target	(kNormalDistance),
	m_is_invert_horizontal	(false),
	m_is_invert_vertical	(false),
	m_dir					(v3d::GetZeroVector()),
	m_angle					(v3d::GetZeroVector())
{
	SetCameraNearFar(kNear, kFar);
	SetupCamera_Perspective(kFOV * math::kDegreesToRadian);
}

Camera::~Camera()
{
	// 処理なし
}

void Camera::Init()
{
	// カメラ位置を初期位置に戻す
	m_transform->SetPos(CoordinateKind::kWorld, m_target_transform->GetPos(CoordinateKind::kWorld) - VGet(0.0f, 0.0f, -kNormalDistance));
}

void Camera::Update()
{
	for (auto& is_input : m_is_input)
	{
		is_input = false;
	}

	Move();
	SetLookDir();
}

void Camera::Draw() const
{
	// TEST : 仮で各軸を描画
	DrawLine3D(v3d::GetZeroVector(), axis::GetWorldXAxis() * 10000, 0xff0000);
	DrawLine3D(v3d::GetZeroVector(), axis::GetWorldYAxis() * 10000, 0x00ff22);
	DrawLine3D(v3d::GetZeroVector(), axis::GetWorldZAxis() * 10000, 0x0077ff);
}

void Camera::OnCollide(const PhysicalObjBase& check_hit_obj)
{

}

void Camera::OnGravity()
{

}


#pragma region コマンド
void Camera::MoveUp()
{
	m_dir.x = -1;
	m_is_input.at(static_cast<int>(InputDir::kUp))    = true;
}

void Camera::MoveDown()
{
	m_dir.x =  1;
	m_is_input.at(static_cast<int>(InputDir::kDown))  = true;
}

void Camera::MoveLeft()
{
	m_dir.y = -1;
	m_is_input.at(static_cast<int>(InputDir::kLeft))  = true;
}

void Camera::MoveRight()
{
	m_dir.y =  1;
	m_is_input.at(static_cast<int>(InputDir::kRight)) = true;
}

void Camera::Approach()
{
	// 仮で接近
	m_distance_to_target -= kApproachSpeed * FPS::GetDeltaTime();
	if (m_distance_to_target < 100.0f) { m_distance_to_target = 100.0f; }
}

void Camera::Depart()
{
	// 仮で離れる
	m_distance_to_target += kApproachSpeed * FPS::GetDeltaTime();
	if (m_distance_to_target > 1000.0f) { m_distance_to_target = 1000.0f; }
}
#pragma endregion


void Camera::AttachTarget(const std::shared_ptr<ObjBase> obj)
{
	// ターゲットを親オブジェクトとする
	m_target_transform = obj->GetTransform();
}

void Camera::AttachTarget(const std::string& obj_name)
{
	// ターゲットを親オブジェクトとする
	auto target_obj = ObjManager::GetInstance()->GetObj(obj_name);
	m_target_transform = target_obj->GetTransform();
}

void Camera::DetachTarget()
{
	m_target_transform = nullptr;
}

void Camera::InitAngle()
{

}

void Camera::SetLookDir()
{
	const VECTOR pos = m_transform->GetPos(CoordinateKind::kWorld);
	const VECTOR look_pos = pos + m_transform->GetForward(CoordinateKind::kWorld);
	SetCameraPositionAndTarget_UpVecY(pos, look_pos);
}

void Camera::Move()
{
	const auto command = CommandHandler::GetInstance();
	m_dir	   = v3d::GetZeroVector();
	m_velocity = v3d::GetZeroVector();

	command->Execute(CommandKind::kMoveUpCamera,	*this);
	command->Execute(CommandKind::kMoveDownCamera,	*this);
	command->Execute(CommandKind::kMoveLeftCamera,	*this);
	command->Execute(CommandKind::kMoveRightCamera, *this);
	command->Execute(CommandKind::kInitAngle,		*this);
	command->Execute(CommandKind::kApproachCamera,	*this);
	command->Execute(CommandKind::kDepartCamera,	*this);

	CalcDirFromPad();
	CalcDirFromMouse();

	// 操作反転処理
	ApplyInvert();

	CalcAngle();

	// 角度制限
	if (m_angle.x < kMinVerticalAngle * math::kDegreesToRadian) { m_angle.x = kMinVerticalAngle * math::kDegreesToRadian; }
	if (m_angle.x > kMaxVerticalAngle * math::kDegreesToRadian) { m_angle.x = kMaxVerticalAngle * math::kDegreesToRadian; }

	// 回転行列を生成
	MATRIX m = MGetIdent();
	CreateRotationZXYMatrix(&m, m_angle.x, m_angle.y, m_angle.z);

	// 結果を反映
	m_transform->SetRot(CoordinateKind::kWorld, MGetRotElem(m));
	const VECTOR target_pos = m_target_transform ? m_target_transform->GetPos(CoordinateKind::kWorld) : v3d::GetZeroVector();
	const VECTOR forward	= m_transform->GetForward(CoordinateKind::kWorld);
	const VECTOR pos		= target_pos - forward * m_distance_to_target;
	m_transform->SetPos(CoordinateKind::kWorld, pos);
}

void Camera::CalcAngle()
{
	const auto command = CommandHandler::GetInstance();

	// コマンドパターンで入力された場合の速度・方向を取得
	if (   command->GetCurrentFrameExecuteInputCode(CommandKind::kMoveUpCamera)
		|| command->GetCurrentFrameExecuteInputCode(CommandKind::kMoveDownCamera)
		|| command->GetCurrentFrameExecuteInputCode(CommandKind::kMoveLeftCamera)
		|| command->GetCurrentFrameExecuteInputCode(CommandKind::kMoveRightCamera))
	{
		m_dir		= v3d::GetNormalizedVector(m_dir);
		m_velocity	= m_dir * kMoveSpeedWithButton;
	}

	m_velocity *= FPS::GetDeltaTime();

	// 角度を取得
	if (m_is_input.at(static_cast<int>(InputDir::kUp)))		{ m_angle.x += m_velocity.x; }
	if (m_is_input.at(static_cast<int>(InputDir::kDown)))	{ m_angle.x += m_velocity.x; }
	if (m_is_input.at(static_cast<int>(InputDir::kLeft)))	{ m_angle.y += m_velocity.y; }
	if (m_is_input.at(static_cast<int>(InputDir::kRight)))	{ m_angle.y += m_velocity.y; }
}

void Camera::ApplyInvert()
{
	if (m_is_invert_horizontal) { m_dir.y *= -1; }
	if (m_is_invert_vertical)	{ m_dir.x *= -1; }
}

void Camera::CalcDirFromPad()
{
	if (m_dir != v3d::GetZeroVector()) { return; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kPad) { return; }

	// 各方向のパラメーターを取得
	const auto input = InputChecker::GetInstance();
	const int up_param		= input->GetInputParameter(pad::StickKind::kRSUp);
	const int down_param	= input->GetInputParameter(pad::StickKind::kRSDown);
	const int left_param	= input->GetInputParameter(pad::StickKind::kRSLeft);
	const int right_param	= input->GetInputParameter(pad::StickKind::kRSRight);

	// 速度ベクトル・入力判定を取得
	if (up_param)
	{
		m_velocity.x = -math::GetUnitValue<int, float>(InputChecker::kStickDeadZone,  InputChecker::kStickMaxSlope,  up_param);
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
		m_velocity.y =  math::GetUnitValue<int, float>(InputChecker::kStickDeadZone,  InputChecker::kStickMaxSlope,  right_param);
		m_is_input.at(static_cast<int>(InputDir::kRight)) = true;
	}
	m_velocity *= kMoveSpeedWithStick;

	// 入力方向も合わせて取得
	m_dir = v3d::GetNormalizedVector(m_velocity);
}

void Camera::CalcDirFromMouse()
{
	if (m_dir != v3d::GetZeroVector()) { return; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kKeyboard) { return; }

	const auto input = InputChecker::GetInstance();

	// 移動速度を取得
	Vector2D<float> velocity_2d = input->GetMouseVelocity(TimeKind::kCurrent);
	m_velocity = VGet(velocity_2d.y, velocity_2d.x, 0.0f) * kMoveSpeedWithMouse;

	// 入力判定を取得
	if (m_velocity.x > 0) { m_is_input.at(static_cast<int>(InputDir::kUp))	  = true; }
	if (m_velocity.x < 0) { m_is_input.at(static_cast<int>(InputDir::kDown))  = true; }
	if (m_velocity.y > 0) { m_is_input.at(static_cast<int>(InputDir::kLeft))  = true; }
	if (m_velocity.y < 0) { m_is_input.at(static_cast<int>(InputDir::kRight)) = true; }

	// 入力方向も合わせて取得
	m_dir = v3d::GetNormalizedVector(m_velocity);
}
