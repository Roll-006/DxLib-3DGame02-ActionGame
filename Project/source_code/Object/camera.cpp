#include "camera.hpp"
#include "../Manager/command_handler.hpp"

Camera::Camera() : 
	PhysicalObjBase			(ObjName.CAMERA, ObjTag.CAMERA, MassKind::kVeryLight),
	m_target_transform		(nullptr),
	m_target_modeler		(nullptr),
	m_target_bone			(""),
	m_distance_to_target	(kNormalDistance),
	m_is_invert_horizontal	(false),
	m_is_invert_vertical	(false),
	m_dir					(),
	m_velocity				(v3d::GetZeroVector()),
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
	const VECTOR look_pos	= GetLookPos();
	const VECTOR forward	= m_transform->GetForward(CoordinateKind::kWorld);
	const VECTOR pos		= look_pos - forward * m_distance_to_target;
	m_transform->SetPos(CoordinateKind::kWorld, pos);
}

void Camera::Update()
{
	for (auto& is_input : m_is_input) { is_input = false; }

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

void Camera::Approach()
{
	m_distance_to_target -= 50.0f * FPS::GetDeltaTime();
	if (std::abs(m_distance_to_target) < 100.0f)
	{
		m_distance_to_target = 100.0f;
	}
}

void Camera::Depart()
{
	m_distance_to_target += 50.0f * FPS::GetDeltaTime();
	if (std::abs(m_distance_to_target) > kNormalDistance)
	{
		m_distance_to_target = kNormalDistance;
	}
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
#pragma endregion


void Camera::AttachTarget(const std::shared_ptr<ObjBase> obj)
{
	m_target_transform = obj->GetTransform();
}

void Camera::AttachTarget(const std::string& obj_name)
{
	auto target_obj = ObjManager::GetInstance()->GetObj(obj_name);
	AttachTarget(target_obj);
}

void Camera::AttachTarget(const std::shared_ptr<ObjBase> obj, const std::shared_ptr<Modeler> modeler, const std::string& bone_path)
{
	AttachTarget(obj);

	m_target_modeler	= modeler;
	m_target_bone		= bone_path;
}

void Camera::AttachTarget(const std::string& obj_name, const std::shared_ptr<Modeler> modeler, const std::string& bone_path)
{
	AttachTarget(obj_name);

	m_target_modeler	= modeler;
	m_target_bone		= bone_path;
}

void Camera::DetachTarget()
{
	m_target_transform = nullptr;
}

void Camera::InitAngle()
{

}

void Camera::Move()
{
	const auto command = CommandHandler::GetInstance();
	m_dir	   = v3d::GetZeroVector();
	m_velocity = v3d::GetZeroVector();

	command->Execute(CommandKind::kMoveUpCamera,	this);
	command->Execute(CommandKind::kMoveDownCamera,	this);
	command->Execute(CommandKind::kMoveLeftCamera,	this);
	command->Execute(CommandKind::kMoveRightCamera, this);
	command->Execute(CommandKind::kInitAngle,		this);

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
	const VECTOR look_pos	= GetLookPos();
	const VECTOR forward	= m_transform->GetForward(CoordinateKind::kWorld);
	const VECTOR pos		= look_pos - forward * m_distance_to_target;
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

VECTOR Camera::GetLookPos()
{
	if (!m_target_transform) { return v3d::GetZeroVector(); }
	if (!m_target_modeler)	 { return m_target_transform->GetPos(CoordinateKind::kWorld); }

	// ボーンの行列情報を取得
	const int model_handle	= m_target_modeler->GetModelHandle();
	const int frame_num		= MV1SearchFrame(model_handle, m_target_bone.c_str());
	MATRIX	  frame_mat		= MV1GetFrameLocalWorldMatrix(model_handle, frame_num);

	// ボーン自体を追跡すると画面の揺れが強すぎるため
	// 同じ高さの位置を追跡
	const auto distance	= m_target_transform->GetPos(CoordinateKind::kWorld) - MGetTranslateElem(frame_mat);
	auto look_pos = m_target_transform->GetPos(CoordinateKind::kWorld) + m_target_transform->GetUp(CoordinateKind::kWorld) * VSize(distance);

	// カメラの軸をもとに位置を修正
	const auto axes = m_transform->GetAxes(CoordinateKind::kWorld);
	look_pos += axes.x_axis * kLookCorrectPos.x;
	look_pos += axes.y_axis * kLookCorrectPos.y;
	look_pos += axes.z_axis * kLookCorrectPos.z;

	return look_pos;
}

void Camera::SetLookDir()
{
	const VECTOR pos = m_transform->GetPos(CoordinateKind::kWorld);
	const VECTOR look_pos = pos + m_transform->GetForward(CoordinateKind::kWorld);
	SetCameraPositionAndTarget_UpVecY(pos, look_pos);
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
