#pragma once
#include "../Base/physical_obj_base.hpp"

#include "../Manager/command_handler.hpp"

class Ground final : public PhysicalObjBase
{
public:
	Ground();
	~Ground() override;

	void Init()			override;
	void Update()		override;
	void Draw() const	override;

	void OnCollide(const PhysicalObjBase& check_hit_obj) override;

private:
	static constexpr float  kModelScale = 500.0f;
	static constexpr VECTOR kPos		= VECTOR(0.0f, -100.0f, 0.0f);

	std::shared_ptr<Modeler> m_modeler;
	std::shared_ptr<Modeler> m_modeler2;
};