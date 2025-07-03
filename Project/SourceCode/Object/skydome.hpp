#pragma once
#include "../Base/obj_base.hpp"

#include "camera.hpp"

class Skydome final : public ObjBase
{
public:
	Skydome(std::shared_ptr<Camera> camera);
	~Skydome() override;

	void Init()		  override;
	void Update()	  override;
	void LateUpdate() override;
	void Draw() const override;

	[[nodiscard]] std::shared_ptr<Modeler> GetModeler() { return m_modeler; }
	
private:
	static constexpr VECTOR kScale = { 30.0f, 30.0f, 30.0f };

private:
	std::shared_ptr<Modeler>   m_modeler;
	std::shared_ptr<Transform> m_camera_transform;
};
