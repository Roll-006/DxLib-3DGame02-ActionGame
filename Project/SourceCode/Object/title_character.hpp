#pragma once
#include "../Base/obj_base.hpp"

#include "../Part/title_character_animator.hpp"

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
	[[nodiscard]] float						GetDeltaTime()	const override;
	[[nodiscard]] std::shared_ptr<Modeler>	GetModeler()	const { return m_modeler; }
	#pragma endregion

private:
	static constexpr VECTOR kBasicAngle = { 0.0f, DX_PI_F, 0.0f };
	static constexpr float  kBasicScale = 0.31f;

private:
	std::shared_ptr<Modeler>				m_modeler;
	std::shared_ptr<TitleCharacterAnimator> m_animator;
};
