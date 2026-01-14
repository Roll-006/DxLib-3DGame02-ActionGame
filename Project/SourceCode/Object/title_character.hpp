#pragma once
#include "../Base/obj_base.hpp"
#include "../Animator/animator.hpp"

class TitleCharacter final : public ObjBase
{
public:
	TitleCharacter();
	~TitleCharacter() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void AddToObjManager()			override;
	void RemoveToObjManager()		override;


	#pragma region Getter
	[[nodiscard]] const  float						GetDeltaTime()	const override;
	[[nodiscard]] const std::shared_ptr<const Modeler>	GetModeler()	const { return m_modeler; }
	#pragma endregion

private:
	static constexpr VECTOR kBasicAngle = { 0.0f, DX_PI_F, 0.0f };
	static constexpr float  kBasicScale = 0.31f;

private:
	std::shared_ptr<Modeler>	m_modeler;
	std::shared_ptr<Animator>	m_animator;
};
