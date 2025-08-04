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
