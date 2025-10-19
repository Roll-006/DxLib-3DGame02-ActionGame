#include "title_character_animator.hpp"

TitleCharacterAnimator::TitleCharacterAnimator(const std::shared_ptr<Modeler>& modeler) : 
	AnimatorBase(modeler, ObjName.TITLE_CHARACTER)
{
	LoadAnim();
}

TitleCharacterAnimator::~TitleCharacterAnimator()
{

}

void TitleCharacterAnimator::Init()
{

}

void TitleCharacterAnimator::Update()
{
	DivideBone();
	ChangeAnim();

	BlendAnim();
	PlayAnim();
}

void TitleCharacterAnimator::LoadAnim()
{
	AddAnimHandle(static_cast<int>(TitleCharacterAnimKind::kIdle), AnimPath.ZOMBIE_IDLE_01, 0, AnimTag.NONE, 10.0f, true, true);
}

void TitleCharacterAnimator::ChangeAnim()
{
	AttachResultAnim(static_cast<int>(TitleCharacterAnimKind::kIdle));
}
