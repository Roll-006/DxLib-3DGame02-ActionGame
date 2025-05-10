#pragma once
#include "transform_component.hpp"

#include "../Manager/input_checker.hpp"

class ThirdPersonControllerComponent : public ComponentBase
{
public:
	ThirdPersonControllerComponent(GameObj* owner_obj, TransformComponent* camera_transform);
	~ThirdPersonControllerComponent()override;

	void Init()		 override;
	void Update()	 override;
	void LateUpdate()override;
	void Draw()const override;

	void Move();

private:
	TransformComponent&	m_camera_transform;
};
