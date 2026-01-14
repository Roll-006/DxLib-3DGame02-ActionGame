#include "title_character.hpp"
#include "../Kind/title_character_anim_kind.hpp"
#include "../Accessor/obj_accessor.hpp"

TitleCharacter::TitleCharacter() : 
	ObjBase		(ObjName.TITLE_CHARACTER, ObjTag.OBJ_CHARACTER),
	m_modeler	(std::make_shared<Modeler>(m_transform, ModelPath.TITLE_CHARACTER, kBasicAngle, kBasicScale)),
	m_animator	(std::make_shared<Animator>(m_modeler, "title_character"))
{
	m_transform->SetRot(CoordinateKind::kWorld, VGet(0.0f, 0.0f, -1.0f));
	m_animator->AttachResultAnim(static_cast<int>(TitleCharacterAnimKind::kIdle));
}

TitleCharacter::~TitleCharacter()
{

}

void TitleCharacter::Init()
{

}

void TitleCharacter::Update()
{
	m_animator->Update();
}

void TitleCharacter::LateUpdate()
{

}

void TitleCharacter::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
}

void TitleCharacter::AddToObjManager()
{
	ObjAccessor::GetInstance()->AddObj(shared_from_this());
}

void TitleCharacter::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	ObjAccessor::GetInstance()->RemoveObj(obj_handle);
}

const float TitleCharacter::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}
