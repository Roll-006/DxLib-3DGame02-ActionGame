#pragma once
#include "../../Base/component_base.hpp"

#include "../../Data/Path/model_path.hpp"

#include "../transform_component.hpp"

class ModelerComponent : public ComponentBase
{
public:
	ModelerComponent(GameObj* owner_obj, std::string file_path);
	ModelerComponent(GameObj* owner_obj, int model_handle);
	~ModelerComponent()override;

	void Init()		 override;
	void Update()	 override;
	void LateUpdate()override;
	void Draw()const override;

	[[nodiscard]] int GetModelHandle() { return m_model_handle; }

private:
	int   m_model_handle;
	float m_opacity_rate;	// ÉÇÉfÉãÇÃïsìßñæìx(0.0fÅ`1.0f)

	std::shared_ptr<TransformComponent> m_transform;
};
