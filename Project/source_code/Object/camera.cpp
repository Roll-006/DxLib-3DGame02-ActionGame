#include "camera.hpp"

Camera::Camera() : 
	CollideObjBase			(ObjName.CAMERA, ObjTag.CAMERA, MassKind::kVeryLight),
	m_target_transform		(nullptr),
	m_move_speed			(0.0f),
	m_distance_to_target	(kNormalDistance),
	m_is_invert_horizontal	(false),
	m_is_invert_vertical	(false),
	m_quaternion			(quat::GetIdentityQuaternion())
{
	//m_target_pos[TimeState::kCurrect] = m_target_pos[TimeState::kNext] = m_transform->GetPos(CoordinateKind::kLocal);

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
	SetCameraPositionAndTarget_UpVecY(GetTransform()->GetPos(CoordinateKind::kWorld), GetTargetPos());
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

void Camera::Move()
{
	const Axes axes	  = math::GetLookTargetAxes(m_transform->GetPos(CoordinateKind::kWorld), GetTargetPos());
	Quaternion rota_q = quat::GetIdentityQuaternion();

	// 回転量を取得
	rota_q = GetRotationFromPad  (rota_q, axes.x);
	rota_q = GetRotationFromMouse(rota_q, axes.x);

	// 回転を反映
	m_quaternion *= rota_q;
	MATRIX rota_m = math::ConvertQuaternionToRotationMatrix(m_transform->GetRotationMatrix(CoordinateKind::kWorld), m_quaternion);
	if (rota_q != quat::GetIdentityQuaternion())
	{
		m_transform->SetRotation(CoordinateKind::kWorld, rota_m);
	}

	// 座標を反映
	const VECTOR rotated_pos = math::GetRotatedPos(m_transform->GetPos(CoordinateKind::kLocal), rota_q);
	m_transform->SetPos(CoordinateKind::kLocal, rotated_pos);
}

VECTOR Camera::ApplyInvert(VECTOR& velocity)
{
	if (m_is_invert_horizontal) { velocity.y *= -1; }
	if (m_is_invert_vertical)	{ velocity.x *= -1; }

	return velocity;
}

Quaternion Camera::GetRotationFromPad(Quaternion& rota_q, const VECTOR& x_axis)
{
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kPad) { return rota_q; }

	VECTOR velocity = v3d::GetZeroVector();

	// 各方向のパラメーターを取得
	const int up_param		= InputChecker::GetInstance()->GetInputParameter(pad::StickKind::kRSUp);
	const int down_param	= InputChecker::GetInstance()->GetInputParameter(pad::StickKind::kRSDown);
	const int left_param	= InputChecker::GetInstance()->GetInputParameter(pad::StickKind::kRSLeft);
	const int right_param	= InputChecker::GetInstance()->GetInputParameter(pad::StickKind::kRSRight);

	// 入力値を置換
	if (up_param)	{ velocity.x = math::GetUnitValue<int, float>(InputChecker::kStickDeadZone,  InputChecker::kStickMaxTilt,  up_param);    }
	if (down_param) { velocity.x = math::GetUnitValue<int, float>(InputChecker::kStickDeadZone, -InputChecker::kStickMinTilt, -down_param);  }
	if (left_param) { velocity.y = math::GetUnitValue<int, float>(InputChecker::kStickDeadZone, -InputChecker::kStickMinTilt, -left_param);  }
	if (right_param){ velocity.y = math::GetUnitValue<int, float>(InputChecker::kStickDeadZone,  InputChecker::kStickMaxTilt,  right_param); }

	velocity *= kSpeedWithPad * FPS::GetDeltaTime();
	velocity = ApplyInvert(velocity);

	// クォータニオンを生成
	if (up_param)	{ rota_q *= quat::MakeQuaternion(x_axis,			    -velocity.x); }
	if (down_param) { rota_q *= quat::MakeQuaternion(x_axis,				 velocity.x); }
	if (left_param) { rota_q *= quat::MakeQuaternion(axis::GetWorldYAxis(), -velocity.y); }
	if (right_param){ rota_q *= quat::MakeQuaternion(axis::GetWorldYAxis(),	 velocity.y); }

	return rota_q;
}

Quaternion Camera::GetRotationFromMouse(Quaternion& rota_q, const VECTOR& x_axis)
{
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kKeyboard) { return rota_q; }

	Vector2D<float> velocity_2d = InputChecker::GetInstance()->GetMouseVelocity(InputChecker::TimeState::kCurrent);
	VECTOR velocity = VGet(velocity_2d.y, velocity_2d.x, 0.0f) * FPS::GetDeltaTime();
	m_velocity = velocity = ApplyInvert(velocity);

	// クォータニオンを生成
	if (InputChecker::GetInstance()->IsInput(mouse::SlideDirKind::kUp))		{ rota_q *= quat::MakeQuaternion(x_axis,				velocity.x); }
	if (InputChecker::GetInstance()->IsInput(mouse::SlideDirKind::kDown))	{ rota_q *= quat::MakeQuaternion(x_axis,				velocity.x); }
	if (InputChecker::GetInstance()->IsInput(mouse::SlideDirKind::kLeft))	{ rota_q *= quat::MakeQuaternion(axis::GetWorldYAxis(),	velocity.y); }
	if (InputChecker::GetInstance()->IsInput(mouse::SlideDirKind::kRight))	{ rota_q *= quat::MakeQuaternion(axis::GetWorldYAxis(),	velocity.y); }

	return rota_q;
}

VECTOR Camera::GetTargetPos()
{
	if (m_target_transform)
	{
		return m_target_transform->GetPos(CoordinateKind::kWorld);
	}
	return v3d::GetZeroVector();
}
