#include "modeler.hpp"

Modeler::Modeler(const std::shared_ptr<Transform> transform, const std::string& file_path, const bool is_turn_around) :
	m_model_handle	(HandleKeeper::GetInstance()->LoadHandle(HandleKind::kModel, file_path)),
	m_opacity		(1.0f),
	m_transform		(transform),
	m_is_turn_around(is_turn_around)
{
	
}

Modeler::Modeler(const std::shared_ptr<Transform> transform, const int model_handle, const bool is_turn_around) :
	m_model_handle	(model_handle),
	m_opacity		(1.0f),
	m_transform		(transform),
	m_is_turn_around(is_turn_around)
{

}

Modeler::~Modeler()
{
	MV1DeleteModel(m_model_handle);
}
                                                                                   
void Modeler::Draw() const
{
	ApplyOpacity();
	ApplyMatrix();

	MV1DrawModel(m_model_handle);
}

void Modeler::ApplyOpacity() const
{
	if (m_opacity == 1.0f)
	{
		MV1SetUseZBuffer(m_model_handle, TRUE);
	}
	else
	{
		MV1SetUseZBuffer(m_model_handle, FALSE);
		MV1SetOpacityRate(m_model_handle, m_opacity);
	}
}

void Modeler::ApplyMatrix() const
{
	const MATRIX rot_y = m_is_turn_around ? MGetRotY(DX_PI_F) : MGetIdent();
	MV1SetMatrix(m_model_handle, rot_y * m_transform->GetMatrix(CoordinateKind::kWorld));
}
