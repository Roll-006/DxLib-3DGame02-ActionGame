#include "title_character.hpp"
#include "../Manager/obj_manager.hpp"

TitleCharacter::TitleCharacter() : 
	ObjBase		(ObjName.TITLE_CHARACTER, ObjTag.OBJ_CHARACTER),
	m_modeler	(std::make_shared<Modeler>(m_transform, ModelPath.TITLE_CHARACTER, kBasicAngle, kBasicScale)),
	m_animator	(std::make_shared<TitleCharacterAnimator>(m_modeler))
{
	m_transform->SetRot(CoordinateKind::kWorld, VGet(0.0f, 0.0f, -1.0f));
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
	ObjManager::GetInstance()->AddObj(shared_from_this());
}

void TitleCharacter::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	ObjManager::GetInstance()->RemoveObj(obj_handle);
}

float TitleCharacter::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}
