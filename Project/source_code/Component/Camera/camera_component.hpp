#pragma once
#include "../../Math/math.hpp"
#include "../../Component/transform_component.hpp"
#include "../../Component/tracker_component.hpp"

class CameraComponent : public ComponentBase
{
public:
	CameraComponent(GameObj* owner_obj);
	~CameraComponent()override;

	void Init()		 override;
	void Update()	 override;
	void LateUpdate()override;
	void Draw()const override;

private:
	static constexpr float kNear	= 10.0f;
	static constexpr float kFar		= 2500.0f;
	static constexpr float kFOV		= 60.0f;

	std::shared_ptr<TransformComponent>	m_transform;
	std::shared_ptr<TrackerComponent>	m_tracker;
};
