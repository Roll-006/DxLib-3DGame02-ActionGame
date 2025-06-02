#pragma once
#include <DxLib.h>
#include <dxlib_helper.h>

#include "../Data/Path/model_path.hpp"
#include "../Data/Path/bone_path.hpp"
#include "transform.hpp"

class Modeler final
{
public:
	Modeler(const std::shared_ptr<Transform> transform, const std::string& file_path);
	Modeler(const std::shared_ptr<Transform> transform, const int model_handle);
	~Modeler();

	void Draw();

	[[nodiscard]] int GetModelHandle() const { return m_model_handle; }

private:
	int   m_model_handle;
	float m_opacity;		// ƒ‚ƒfƒ‹‚Ì•s“§–¾“x(0.0f`1.0f)

	std::shared_ptr<Transform> m_transform;
};
