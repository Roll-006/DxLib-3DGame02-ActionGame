#pragma once
#include "../transform_component.hpp"

#include "../../Manager/input_checker.hpp"

class CameraControllerComponent : public ComponentBase
{
public:
	CameraControllerComponent(GameObj* owner_obj);
	~CameraControllerComponent()override;

	void Init()		 override;
	void Update()	 override;
	void LateUpdate()override;
	void Draw()const override;

private:
	void Move();
	void MoveByMouse();
	void MoveByPad();
	void CalcSpeedByMouse();
	void CalcSpeedByPad();

private:
	static constexpr float kSpeed					= 2.0f;
	static constexpr float kMouseSpeedRate			= 0.1f;
	static constexpr float kMouseVerticalSpeedRate	= 2.5f;
	static constexpr float kInitAngleSpeed			= 9.0f;
	static constexpr float kMaxDistanceToTarget		= 500.0f;
	static constexpr float kMinDistanceToTarget		= 100.0f;
	static constexpr float kInitVerticalAngle		= 7.0f * math::kDegreesToRadian;
	static constexpr float kMaxVerticalAngle		= 89.0f;
	static constexpr float kMinVerticalAngle		= -70.0f;

	Quaternion	m_quaternion;
	Quaternion	m_next_m_quaternion;
	VECTOR		m_velocity;
	bool		m_possible_operation;		// 操作が可能であるかを判定
	bool		m_is_invert_horizontal;		// 操作時に左右反転を行うかを判定
	bool		m_is_invert_vertical;		// 操作時に上下反転を行うかを判定

	std::shared_ptr<TransformComponent>	m_transform;
};
