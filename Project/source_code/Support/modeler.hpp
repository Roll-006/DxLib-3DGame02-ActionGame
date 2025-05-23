#pragma once
#include <DxLib.h>

#include "../Data/Path/model_path.hpp"
#include "transform.hpp"

class Modeler final
{
public:
	Modeler(std::shared_ptr<Transform> transform, const std::string& file_path);
	Modeler(std::shared_ptr<Transform> transform, const int model_handle);
	~Modeler();

	void Draw();

	[[nodiscard]] int GetModelHandle()const { return m_model_handle; }

private:
	int   m_model_handle;
	float m_opacity_rate;	// ƒ‚ƒfƒ‹‚Ì•s“§–¾“x(0.0f`1.0f)

	std::shared_ptr<Transform> m_transform;
};