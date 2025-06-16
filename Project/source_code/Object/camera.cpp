#include "camera.hpp"
#include "../Manager/command_handler.hpp"

Camera::Camera() : 
	PhysicalObjBase				(ObjName.CAMERA, ObjTag.CAMERA, MassKind::kVeryLight),
	m_target_transform			(nullptr),
	m_target_modeler			(nullptr),
	m_target_bone				(""),
	m_distance_to_target		(kNormalDistance),
	m_init_angle_speed			(0.0f),
	m_init_end_threshold		(0.0f),
	m_is_invert_horizontal		(false),
	m_is_invert_vertical		(false),
	m_is_init_angle				(false), 
	m_is_look_same_dir_target	(false),
	m_dir						(v3d::GetZeroVector()),
	m_velocity					(v3d::GetZeroVector())
{
	SetCameraNearFar(kNear, kFar);
	SetupCamera_Perspective(kFOV * math::kDegreesToRadian);

	m_angle[TimeKind::kCurrent] = m_angle[TimeKind::kNext] = v3d::GetZeroVector();
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
	InitMove();

	const auto command = CommandHandler::GetInstance();
	command->Execute(CommandKind::kInitAngle,		this);
	command->Execute(CommandKind::kMoveUpCamera,	this);
	command->Execute(CommandKind::kMoveDownCamera,	this);
	command->Execute(CommandKind::kMoveLeftCamera,	this);
	command->Execute(CommandKind::kMoveRightCamera, this);

	Move();
	SetLookDir();
	JudgeLookSameDirTarget();
}

void Camera::Draw() const
{

}

void Camera::OnCollide(const PhysicalObjBase& check_hit_obj)
{

}

void Camera::OnGravity()
{

}

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
	m_target_transform	= nullptr;
	m_target_modeler	= nullptr;
	m_target_bone		= "";
}

void Camera::Approach(const float min_distance, const float move_speed)
{
	m_distance_to_target -= move_speed;
	if (std::abs(m_distance_to_target) < min_distance)
	{
		m_distance_to_target = min_distance;
	}
}

void Camera::Depart(const float max_distance, const float move_speed)
{
	m_distance_to_target += move_speed;
	if (std::abs(m_distance_to_target) > max_distance)
	{
		m_distance_to_target = max_distance;
	}
}


#pragma region コマンド
void Camera::MoveUp()
{
	// 視点リセット中は操作不可
	if (m_is_init_angle) { return; }

	m_dir.x = -1;
	m_is_input.at(static_cast<int>(InputDir::kUp)) = true;
}

void Camera::MoveDown()
{
	// 視点リセット中は操作不可
	if (m_is_init_angle) { return; }

	m_dir.x = 1;
	m_is_input.at(static_cast<int>(InputDir::kDown)) = true;
}

void Camera::MoveLeft()
{
	// 視点リセット中は操作不可
	if (m_is_init_angle) { return; }

	m_dir.y = -1;
	m_is_input.at(static_cast<int>(InputDir::kLeft)) = true;
}

void Camera::MoveRight()
{
	// 視点リセット中は操作不可
	if (m_is_init_angle) { return; }

	m_dir.y = 1;
	m_is_input.at(static_cast<int>(InputDir::kRight)) = true;
}

void Camera::InitAngle()
{
	if (!m_target_transform) { return; }
	if (m_is_init_angle)	 { return; }

	// 追跡対象のforwardを目標とする
	const VECTOR forward = m_target_transform->GetForward(CoordinateKind::kWorld);
	m_angle.at(TimeKind::kNext) = math::GetYawRotVector(forward);
	m_angle.at(TimeKind::kNext).y = math::ConnectMinusPiToPi(m_angle.at(TimeKind::kNext).y);

	m_init_angle_speed	 = kInitAngleSpeed;
	m_init_end_threshold = kInitAngleEndThreshold;
	m_is_init_angle		 = true;
}
#pragma endregion


void Camera::InitYaw()
{
	if (!m_target_transform) { return; }
	if (m_is_init_angle)	 { return; }

	// ヨー角以外はカメラ自身の姿勢をそのまま保つ
	const VECTOR forward = m_target_transform->GetForward(CoordinateKind::kWorld);
	const float  yaw	 = math::GetYaw(forward);
	m_angle.at(TimeKind::kNext) = m_angle.at(TimeKind::kCurrent);
	m_angle.at(TimeKind::kNext).y = math::ConnectMinusPiToPi(yaw);

	m_init_angle_speed	 = kInitYawSpeed;
	m_init_end_threshold = kInitYawEndThreshold;
	m_is_init_angle		 = true;
}

void Camera::Move()
{
	if (!m_is_init_angle)
	{
		CalcDirFromPad();
		CalcDirFromMouse();
	}

	// 操作反転処理
	//ApplyInvert();

	CalcAngle();
	CalcDistance();

	// 回転行列を生成
	MATRIX m = MGetIdent();
	CreateRotationXYZMatrix(&m, m_angle.at(TimeKind::kCurrent).x, m_angle.at(TimeKind::kCurrent).y, m_angle.at(TimeKind::kCurrent).z);

	// 結果を反映
	m_transform->SetRot(CoordinateKind::kWorld, MGetRotElem(m));
	const VECTOR look_pos	= GetLookPos();
	const VECTOR forward	= m_transform->GetForward(CoordinateKind::kWorld);
	const VECTOR pos		= look_pos - forward * m_distance_to_target;
	m_transform->SetPos(CoordinateKind::kWorld, pos);

	DrawFormatString(0,  0, 0xffffff, "pos      : %f, %f, %f", pos.x, pos.y, pos.z);
	DrawFormatString(0, 20, 0xffffff, "look_pos : %f, %f, %f", look_pos.x, look_pos.y, look_pos.z);
}

void Camera::InitMove()
{
	for (auto& is_input : m_is_input) { is_input = false; }
	m_dir = v3d::GetZeroVector();
	m_velocity = v3d::GetZeroVector();
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

	// 視点リセット
	CalcInitAngle();

	m_velocity *= FPS::GetDeltaTime();

	// 角度を取得
	if (m_is_input.at(static_cast<int>(InputDir::kUp)))		{ m_angle.at(TimeKind::kCurrent).x += m_velocity.x; }
	if (m_is_input.at(static_cast<int>(InputDir::kDown)))	{ m_angle.at(TimeKind::kCurrent).x += m_velocity.x; }
	if (m_is_input.at(static_cast<int>(InputDir::kLeft)))	{ m_angle.at(TimeKind::kCurrent).y += m_velocity.y; }
	if (m_is_input.at(static_cast<int>(InputDir::kRight)))	{ m_angle.at(TimeKind::kCurrent).y += m_velocity.y; }

	m_angle.at(TimeKind::kCurrent).y = math::ConnectMinusPiToPi(m_angle.at(TimeKind::kCurrent).y);

	// 角度制限
	if (m_angle.at(TimeKind::kCurrent).x < kMinVerticalAngle * math::kDegreesToRadian) { m_angle.at(TimeKind::kCurrent).x = kMinVerticalAngle * math::kDegreesToRadian; }
	if (m_angle.at(TimeKind::kCurrent).x > kMaxVerticalAngle * math::kDegreesToRadian) { m_angle.at(TimeKind::kCurrent).x = kMaxVerticalAngle * math::kDegreesToRadian; }
}

void Camera::CalcDistance()
{
	// 対象を上から見ると離れ、下から見ると近づく
	const float min  = kMinVerticalAngle * math::kDegreesToRadian;
	const float max  = kMaxVerticalAngle * math::kDegreesToRadian;
	const float rate = math::GetUnitValue<float, float>(min, max, m_angle.at(TimeKind::kCurrent).x);

	m_distance_to_target = (kMaxDistanceToTarget - kMinDistanceToTarget) * rate + kMinDistanceToTarget;
}

void Camera::CalcInitAngle()
{
	if (!m_is_init_angle) { return; }
	
	VECTOR distance_v = m_angle.at(TimeKind::kNext) - m_angle.at(TimeKind::kCurrent);
	distance_v.y = math::ConnectMinusPiToPi(distance_v.y);
	VECTOR dir = v3d::GetNormalizedVector(distance_v);

	// 右・左回りから最短経路を取得し、回転方向に反映
	float distance = VSize(distance_v);
	const float shortest = min(distance, DX_TWO_PI_F - distance);
	if (distance != shortest)
	{
		dir.y *= -1;
	}

	// 目的地に遠いほど速く移動させる
	m_angle.at(TimeKind::kCurrent) += dir * distance * m_init_angle_speed * FPS::GetDeltaTime();
		
	// 終了判定
	distance = VSize(m_angle.at(TimeKind::kNext) - m_angle.at(TimeKind::kCurrent));
	if (distance < m_init_end_threshold)
	{
		m_angle.at(TimeKind::kCurrent) = m_angle.at(TimeKind::kNext);

		m_is_init_angle = false;
	}
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

void Camera::JudgeLookSameDirTarget()
{
	const VECTOR forward = m_target_transform->GetForward(CoordinateKind::kWorld);
	const float  yaw = math::GetYaw(forward);
	m_angle.at(TimeKind::kCurrent).y;

	m_is_look_same_dir_target = m_angle.at(TimeKind::kCurrent).y == yaw ? true : false;
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
