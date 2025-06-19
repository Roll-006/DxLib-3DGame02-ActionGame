#pragma once
#include "../Base/physical_obj_base.hpp"

#include "../Manager/command_handler.hpp"

class House final : public PhysicalObjBase
{
public:
	House();
	~House() override;

	void Init()			override;
	void Update()		override;
	void Draw() const	override;

	void OnCollide(const PhysicalObjBase& check_hit_obj) override;
	void OnGravity() override;

private:
	static constexpr float  kModelScale = 10.0f;
	static constexpr VECTOR kPos		= VECTOR(0.0f, 0.0f, 1000.0f);

	std::shared_ptr<Modeler> m_modeler;
};