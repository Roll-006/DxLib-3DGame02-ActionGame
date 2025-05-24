#include "camera.hpp"

Camera::Camera() : 
	CollideObjBase		(ObjName.CAMERA, ObjTag.CAMERA, MassKind::kVeryLight),
	m_target_transform	(nullptr),
	m_move_speed		(0.0f),
	m_distance_to_target(500.0f)
{
	m_target_pos[TimeState::kCurrect] = m_target_pos[TimeState::kNext] = m_transform->GetPos(CoordinateKind::kLocal);

	SetCameraNearFar(kNear, kFar);
	SetupCamera_Perspective(kFOV * math::kDegreesToRadian);
}

Camera::~Camera()
{
	// 処理なし
}

void Camera::Init()
{

}

void Camera::Update()
{
	Move();
	SetCameraPositionAndTarget_UpVecY(GetTransform()->GetPos(CoordinateKind::kWorld), m_target_transform->GetPos(CoordinateKind::kWorld));
	//SetCameraPositionAndTarget_UpVecY(GetTransform()->GetPos(), v3d::GetZeroVector());
}

void Camera::Draw()const
{
	// TEST : 仮で各軸を描画
	DrawLine3D(VGet(0, 0, 0), VGet(10000,	  0,	 0), 0xff0000);
	DrawLine3D(VGet(0, 0, 0), VGet(	   0, 10000,	 0), 0x00ff22);
	DrawLine3D(VGet(0, 0, 0), VGet(    0,	  0, 10000), 0x0077ff);
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


	// 仮でカメラを回転
	if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_0) == InputState::kHold)
	{
		const float		 speed		 = 10.0f * FPS::GetDeltaTime();
		const Quaternion rota_q		 = quat::GetQuaternion(VGet(0.0f,1.0f, 0.0f), speed);
		const VECTOR	 rotated_pos = math::GetRotatedPos(m_transform->GetPos(CoordinateKind::kLocal), rota_q);
		m_transform->SetPos(CoordinateKind::kLocal, rotated_pos);
	}

	// 仮で親を外す
	if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_1) == InputState::kSingle)
	{
		m_transform->DetachParent();
	}

	// 仮で親を外す
	if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_2) == InputState::kSingle)
	{
		m_transform->AttachParent(ObjName.PLAYER);
	}
}

void Camera::CalcSpeedByPad()
{

}

void Camera::CalcSpeedByMouse()
{

}
