#include "camera_controller_component.hpp"

CameraControllerComponent::CameraControllerComponent(GameObj* owner_obj) :
	ComponentBase			(owner_obj),
	m_quaternion			(quat::GetZeroQuaternion()),
	m_next_m_quaternion		(quat::GetZeroQuaternion()),
	m_velocity				(v3d::GetZeroVector()),
	m_possible_operation	(true),
	m_is_invert_horizontal	(false),
	m_is_invert_vertical	(false),
	m_transform				(nullptr)
{
	m_transform = m_owner_obj->GetComponent<TransformComponent>();
}

CameraControllerComponent::~CameraControllerComponent()
{
	// 処理なし
}

void CameraControllerComponent::Init()
{
	
}

void CameraControllerComponent::Update()
{
	Move();
}

void CameraControllerComponent::LateUpdate()
{
	// 処理なし
}

void CameraControllerComponent::Draw()const
{
	// 処理なし
}

void CameraControllerComponent::Move()
{
	CalcSpeedByMouse();
	CalcSpeedByPad();

	MoveByMouse();
	MoveByPad();

	//	// 上下の角度制限をつける
	//	if (m_angle.x < kMinVerticalAngle * math::kDegreesToRadian) { m_angle.x = kMinVerticalAngle * math::kDegreesToRadian; }
	//	if (m_angle.x > kMaxVerticalAngle * math::kDegreesToRadian) { m_angle.x = kMaxVerticalAngle * math::kDegreesToRadian; }
	
	//	// カメラが下に行くほど対象との距離が近づいていく
	//	float ratio = math::GetUnitValue<float, float>(kMinVerticalAngle * math::kDegreesToRadian, kMaxVerticalAngle * math::kDegreesToRadian, m_angle.x);
	//	m_distance_to_target = ratio * (kMaxDistanceToTarget - kMinDistanceToTarget) + kMinDistanceToTarget;
	
	//	MATRIX matrix_x = MGetRotX(m_angle.x);
	//	MATRIX matrix_y = MGetRotY(m_angle.y);
	//	MATRIX matrix   = MMult(matrix_x, matrix_y);
	
	//	m_direction = GetUnitVector(VTransform(VGet(0.0f, 0.0f, 1.0f), matrix));
	//	m_pos = m_result_target_pos - m_direction * m_distance_target;
}

void CameraControllerComponent::MoveByMouse()
{
	if (!m_possible_operation) { return; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kKeyboard) { return; }

	//VECTOR axis = v3d::GetZeroVector();

	//if (InputChecker::GetInstance()->IsSlideMouse(MouseSlideDirectionKind::kLeft))	{ m_angle.y += m_velocity.y; }
	//if (InputChecker::GetInstance()->IsSlideMouse(MouseSlideDirectionKind::kRight)) { m_angle.y += m_velocity.y; }
	//if (InputChecker::GetInstance()->IsSlideMouse(MouseSlideDirectionKind::kDown))	{ m_angle.x += m_velocity.x; }
	//if (InputChecker::GetInstance()->IsSlideMouse(MouseSlideDirectionKind::kUp))	{ m_angle.x += m_velocity.x; }

	//m_quaternion = quat::GetQuaternion();
}

void CameraControllerComponent::MoveByPad()
{
	if (!m_possible_operation) { return; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kPad) { return; }

	m_transform->SetPos(VRotQ(m_transform->GetPos(), VGet(0.0f, 1.0f, 0.0f), 1.0f * math::kDegreesToRadian));
	
	//if (InputChecker::GetInstance()->GetPadStickParameter(StickKind::kRSLeft))	{ m_angle.y -= m_velocity.y; }
	//if (InputChecker::GetInstance()->GetPadStickParameter(StickKind::kRSRight)) { m_angle.y += m_velocity.y; }
	//if (InputChecker::GetInstance()->GetPadStickParameter(StickKind::kRSDown))	{ m_angle.x += m_velocity.x; }
	//if (InputChecker::GetInstance()->GetPadStickParameter(StickKind::kRSUp))	{ m_angle.x -= m_velocity.x; }
}

void CameraControllerComponent::CalcSpeedByMouse()
{
	if (!m_possible_operation) { return; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kKeyboard) { return; }

	Vector2D<float> velocity = InputChecker::GetInstance()->GetMouseVelocity(InputChecker::TimeState::kCurrent);
	m_velocity = VGet(velocity.y, velocity.x, 0.0f) * kMouseSpeedRate * FPS::GetDeltaTime();
	
	//	// 上下移動は速く補正する
	//	m_velocity.x *= kMouseVerticalSpeedRate;
	
	//	// 操作反転処理
	//	if (m_is_invert_horizontal) { m_velocity.y *= -1; }
	//	if (m_is_invert_vertical) { m_velocity.x *= -1; }
}

void CameraControllerComponent::CalcSpeedByPad()
{
	if (!m_possible_operation) { return; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kPad) { return; }

	m_velocity = v3d::GetZeroVector();
	
	// スティックの傾きから移動速度を取得
	if (InputChecker::GetInstance()->GetPadStickParameter(StickKind::kRSLeft))
	{
		m_velocity.y = math::GetUnitValue<int, float>(InputChecker::kStickDeadZone, -InputChecker::kStickMinTilt, -InputChecker::GetInstance()->GetPadStickParameter(StickKind::kRSLeft));
	}
	if (InputChecker::GetInstance()->GetPadStickParameter(StickKind::kRSRight))
	{
		m_velocity.y = math::GetUnitValue<int, float>(InputChecker::kStickDeadZone,  InputChecker::kStickMaxTilt,  InputChecker::GetInstance()->GetPadStickParameter(StickKind::kRSRight));
	}
	if (InputChecker::GetInstance()->GetPadStickParameter(StickKind::kRSDown))
	{
		m_velocity.x = math::GetUnitValue<int, float>(InputChecker::kStickDeadZone, -InputChecker::kStickMinTilt, -InputChecker::GetInstance()->GetPadStickParameter(StickKind::kRSDown));
	}
	if (InputChecker::GetInstance()->GetPadStickParameter(StickKind::kRSUp))
	{
		m_velocity.x = math::GetUnitValue<int, float>(InputChecker::kStickDeadZone,  InputChecker::kStickMaxTilt,  InputChecker::GetInstance()->GetPadStickParameter(StickKind::kRSUp));
	}
	
	m_velocity *= kSpeed * FPS::GetDeltaTime();
	
	// 操作反転処理
	if (m_is_invert_horizontal) { m_velocity.y *= -1; }
	if (m_is_invert_vertical)	{ m_velocity.x *= -1; }
}
