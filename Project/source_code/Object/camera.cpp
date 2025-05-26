#include "camera.hpp"

Camera::Camera() : 
	CollideObjBase			(ObjName.CAMERA, ObjTag.CAMERA, MassKind::kVeryLight),
	m_target_transform		(nullptr),
	m_move_speed			(0.0f),
	m_distance_to_target	(kNormalDistance),
	m_is_invert_horizontal	(false),
	m_is_invert_vertical	(false),
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
	m_transform->SetPos(CoordinateKind::kWorld, m_target_transform->GetPos(CoordinateKind::kWorld) - VGet(0.0f, 0.0f, -kNormalDistance));
}

void Camera::Update()
{
	Move();
	SetLookDir();
}

void Camera::Draw()const
{
	// TEST : 仮で各軸を描画
	DrawLine3D(v3d::GetZeroVector(), axis::GetWorldXAxis() * 10000, 0xff0000);
	DrawLine3D(v3d::GetZeroVector(), axis::GetWorldYAxis() * 10000, 0x00ff22);
	DrawLine3D(v3d::GetZeroVector(), axis::GetWorldZAxis() * 10000, 0x0077ff);

	const Axes axes = math::ConvertRotationMatrixToAxes(m_transform->GetRotationMatrix(CoordinateKind::kWorld));
	DrawLine3D(v3d::GetZeroVector(), axes.x * 100, 0xff0000);
	DrawLine3D(v3d::GetZeroVector(), axes.y * 100, 0x00ff22);
	DrawLine3D(v3d::GetZeroVector(), axes.z * 100, 0x0077ff);

	matrix::Draw(m_transform->GetMatrix(CoordinateKind::kWorld), VGet(50, 0, 0));
}

void Camera::OnCollide(const CollideObjBase& check_hit_obj)
{

}

void Camera::AttachTarget(const std::shared_ptr<ObjBase> obj)
{
	// ターゲットを親オブジェクトとする
	m_target_transform = obj->GetTransform();
	m_transform->AttachParent(obj->GetTransform());
}

void Camera::AttachTarget(const std::string& obj_name)
{
	// ターゲットを親オブジェクトとする
	auto target_obj = ObjManager::GetInstance()->GetObj(obj_name);
	m_target_transform = target_obj->GetTransform();
	m_transform->AttachParent(target_obj->GetTransform());
}

void Camera::DetachTarget()
{
	m_target_transform = nullptr;
	m_transform->DetachParent();
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

Axes Camera::GetAxes()const
{
	VECTOR target_pos = v3d::GetZeroVector();

	if (m_target_transform)
	{
		target_pos = m_target_transform->GetPos(CoordinateKind::kWorld);
	}

	return math::GetLookTargetAxes(m_transform->GetPos(CoordinateKind::kWorld), target_pos);
}

void Camera::Move()
{
	if (InputChecker::GetInstance()->IsInput(pad::ButtonKind::kA)) { AttachTarget(ObjName.PLAYER); }
	if (InputChecker::GetInstance()->IsInput(pad::ButtonKind::kB)) { DetachTarget(); }

	const Axes axes = GetAxes();

	// 回転量を取得z
	CalcAngleFromPad  (axes.x);
	CalcAngleFromMouse(axes.x);

	// 角度制限
	if (m_angle.x < kMinVerticalAngle * math::kDegreesToRadian) { m_angle.x = kMinVerticalAngle * math::kDegreesToRadian; }
	if (m_angle.x > kMaxVerticalAngle * math::kDegreesToRadian) { m_angle.x = kMaxVerticalAngle * math::kDegreesToRadian; }

	MATRIX m = MGetIdent();
	CreateRotationZXYMatrix(&m, m_angle.x, m_angle.y, m_angle.z);

	m_transform->SetRotation(CoordinateKind::kWorld, MGetRotElem(m));
	const VECTOR target_pos = m_target_transform ? m_target_transform->GetPos(CoordinateKind::kWorld) : v3d::GetZeroVector();
	const VECTOR pos		= target_pos - m_transform->GetForward(CoordinateKind::kWorld) * m_distance_to_target;
	m_transform->SetPos(CoordinateKind::kWorld, pos);
}

VECTOR Camera::ApplyInvert(VECTOR& velocity)
{
	if (m_is_invert_horizontal) { velocity.y *= -1; }
	if (m_is_invert_vertical)	{ velocity.x *= -1; }

	return velocity;
}

void Camera::CalcAngleFromPad(const VECTOR& x_axis)
{
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kPad) { return; }

	m_velocity = v3d::GetZeroVector();

	// 各方向のパラメーターを取得
	const int up_param		= InputChecker::GetInstance()->GetInputParameter(pad::StickKind::kRSUp);
	const int down_param	= InputChecker::GetInstance()->GetInputParameter(pad::StickKind::kRSDown);
	const int left_param	= InputChecker::GetInstance()->GetInputParameter(pad::StickKind::kRSLeft);
	const int right_param	= InputChecker::GetInstance()->GetInputParameter(pad::StickKind::kRSRight);

	// 入力値を置換
	if (up_param)	{ m_velocity.x = math::GetUnitValue<int, float>(InputChecker::kStickDeadZone,  InputChecker::kStickMaxTilt,  up_param);    }
	if (down_param) { m_velocity.x = math::GetUnitValue<int, float>(InputChecker::kStickDeadZone, -InputChecker::kStickMinTilt, -down_param);  }
	if (left_param) { m_velocity.y = math::GetUnitValue<int, float>(InputChecker::kStickDeadZone, -InputChecker::kStickMinTilt, -left_param);  }
	if (right_param){ m_velocity.y = math::GetUnitValue<int, float>(InputChecker::kStickDeadZone,  InputChecker::kStickMaxTilt,  right_param); }

	// 移動速度を取得
	m_velocity *= kSpeedWithPad * FPS::GetDeltaTime();
	m_velocity = ApplyInvert(m_velocity);

	// 角度を取得
	if (up_param)	{ m_angle.x -= m_velocity.x; }
	if (down_param) { m_angle.x += m_velocity.x; }
	if (left_param) { m_angle.y -= m_velocity.y; }
	if (right_param){ m_angle.y += m_velocity.y; }
}

void Camera::CalcAngleFromMouse(const VECTOR& x_axis)
{
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kKeyboard) { return; }

	// 移動速度を取得
	Vector2D<float> velocity_2d = InputChecker::GetInstance()->GetMouseVelocity(InputChecker::TimeState::kCurrent);
	m_velocity = VGet(velocity_2d.y, velocity_2d.x, 0.0f) * FPS::GetDeltaTime();
	m_velocity = ApplyInvert(m_velocity);

	// 角度を取得
	if (    InputChecker::GetInstance()->IsInput(mouse::SlideDirKind::kUp)
	     || InputChecker::GetInstance()->IsInput(mouse::SlideDirKind::kDown)
	     || InputChecker::GetInstance()->IsInput(mouse::SlideDirKind::kLeft)
	     || InputChecker::GetInstance()->IsInput(mouse::SlideDirKind::kRight))
	{
		m_angle += m_velocity;
	}
}
