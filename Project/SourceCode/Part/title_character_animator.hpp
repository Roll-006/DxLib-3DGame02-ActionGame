#pragma once
#include "../Base/animator_base.hpp"
#include "../Kind/title_character_anim_kind.hpp"

class TitleCharacterAnimator final : public AnimatorBase
{
public:
	TitleCharacterAnimator(const std::shared_ptr<Modeler>& modeler);
	~TitleCharacterAnimator();

	void Init()		override;
	void Update()	override;

private:
	void LoadAnim()		override;
	void ChangeAnim()	override;
};
