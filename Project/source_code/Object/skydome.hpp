#pragma once
#include "../Base/obj_base.hpp"
#include "../Manager/command_handler.hpp"

#include "camera.hpp"

class Skydome final : public ObjBase
{
public:
	Skydome(std::shared_ptr<Camera> camera);
	~Skydome() override;

	void Init()		  override;
	void Update()	  override;
	void Draw() const override;
	
private:
	static constexpr float kScale = 30.0f;

	std::shared_ptr<Modeler>   m_modeler;
	std::shared_ptr<Transform> m_camera_transform;
};
