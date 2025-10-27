#pragma once
#include "../Base/obj_base.hpp"
#include "../Part/modeler.hpp"

#include "main_camera.hpp"

class Skydome final : public ObjBase
{
public:
	Skydome();
	~Skydome() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void AddToObjManager()			override;
	void RemoveToObjManager()		override;

	[[nodiscard]] float	GetDeltaTime() const override;
	[[nodiscard]] std::shared_ptr<Modeler> GetModeler() { return m_modeler; }
	
private:
	static constexpr VECTOR kBasicAngle = { 0.0f, DX_PI_F, 0.0f };
	static constexpr float  kBasicScale = 17.0f;

private:
	std::shared_ptr<Modeler> m_modeler;
};
