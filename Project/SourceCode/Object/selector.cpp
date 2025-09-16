#include "selector.hpp"

Selector::Selector() : 
	ObjBase(ObjName.SELECTOR, ObjTag.SYSTEM)
{

}

Selector::~Selector()
{

}

void Selector::Init()
{

}

void Selector::Update()
{
	if (!IsActive()) { return; }
}

void Selector::LateUpdate()
{
	if (!IsActive()) { return; }
}

void Selector::DrawToShadowMap() const
{

}

void Selector::Draw() const
{
	if (!IsActive()) { return; }
}

void Selector::AddToObjManager()
{

}

void Selector::RemoveToObjManager()
{

}

float Selector::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleController::LayerKind::kNoneScale);
}
