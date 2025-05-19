#include "modeler.hpp"

Modeler::Modeler(std::shared_ptr<Transform> transform, const std::string& file_path) :
	m_model_handle	(MV1LoadModel(file_path.c_str())),
	m_opacity_rate	(1.0f),
	m_transform		(transform)
{

}

Modeler::Modeler(std::shared_ptr<Transform> transform, const int model_handle) :
	m_model_handle	(model_handle),
	m_opacity_rate	(1.0f),
	m_transform		(transform)
{

}

Modeler::~Modeler()
{
	MV1DeleteModel(m_model_handle);
}

void Modeler::Draw()
{
	MV1SetPosition(m_model_handle, m_transform->GetPos());
	MV1DrawModel(m_model_handle);
}
