#include "modeler_component.hpp"

ModelerComponent::ModelerComponent(GameObj* owner_obj, std::string file_path) :
	ComponentBase	(owner_obj),
	m_model_handle	(MV1LoadModel(file_path.c_str())),
	m_opacity_rate	(1.0f),
	m_transform		(nullptr)
{
	m_transform = m_owner_obj->GetComponent<TransformComponent>();
	MV1SetScale(m_model_handle, m_transform->GetScale());
	MV1SetUseZBuffer(m_model_handle, TRUE);
	MV1SetOpacityRate(m_model_handle, m_opacity_rate);
}

ModelerComponent::ModelerComponent(GameObj* owner_obj, int model_handle) :
	ComponentBase	(owner_obj),
	m_model_handle	(model_handle),
	m_opacity_rate	(1.0f),
	m_transform		(nullptr)
{
	m_transform = m_owner_obj->GetComponent<TransformComponent>();
	MV1SetScale(m_model_handle, m_transform->GetScale());
	MV1SetUseZBuffer(m_model_handle, TRUE);
	MV1SetOpacityRate(m_model_handle, m_opacity_rate);
}

ModelerComponent::~ModelerComponent()
{
	MV1DeleteModel(m_model_handle);
}

void ModelerComponent::Init()
{
	// ˆ—‚È‚µ
}

void ModelerComponent::Update()
{
	// ˆ—‚È‚µ
}

void ModelerComponent::LateUpdate()
{

}

void ModelerComponent::Draw()const
{
	MV1SetPosition(m_model_handle, m_transform->GetPos());
	MV1DrawModel(m_model_handle);
}
