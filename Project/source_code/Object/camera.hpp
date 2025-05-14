#pragma once
#include <memory>

#include "../Support/transform.hpp"

class Camera
{
public:
	Camera();
	~Camera();

	void Init();
	void Update();
	void Draw()const;

private:
	static constexpr float kNear	= 10.0f;
	static constexpr float kFar		= 2500.0f;
	static constexpr float kFOV		= 60.0f;

	std::shared_ptr<Transform> m_transform;
};
