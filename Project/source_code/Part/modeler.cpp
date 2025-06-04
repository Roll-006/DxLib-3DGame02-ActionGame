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
                                                                                   
void Modeler::Draw()
{
	// 透過情報を適用
	MV1SetUseZBuffer(m_model_handle, TRUE);
	if (m_opacity != 1.0f) { MV1SetUseZBuffer(m_model_handle, FALSE); }
	MV1SetOpacityRate(m_model_handle, m_opacity);

	// 行列情報を適用
	ApplyMatrix();

	// 最終描画
	MV1DrawModel(m_model_handle);

	//DxLibHelper::DrawModelFrames(m_model_handle);
}

void Modeler::ApplyMatrix()
{
	MV1SetMatrix(m_model_handle, m_transform->GetMatrix(CoordinateKind::kWorld));
	//if (m_is_turn_around) { MV1SetRotationXYZ(m_model_handle, VGet(0.0f, DX_PI_F, 0.0f)); }
}
