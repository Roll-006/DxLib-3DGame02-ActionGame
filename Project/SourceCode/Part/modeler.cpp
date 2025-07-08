#include "modeler.hpp"

Modeler::Modeler(const std::shared_ptr<Transform> transform, const std::string& file_path, const VECTOR& basic_angle) :
	m_model_handle(HandleKeeper::GetInstance()->LoadHandle(HandleKind::kModel, file_path)),
	m_opacity(1.0f),
	m_transform(transform),
	m_basic_angle(basic_angle)
{
	MV1SetupCollInfo(m_model_handle);
}

Modeler::Modeler(const std::shared_ptr<Transform> transform, const std::string& file_path) :
	m_model_handle(HandleKeeper::GetInstance()->LoadHandle(HandleKind::kModel, file_path)),
	m_opacity(1.0f),
	m_transform(transform),
	m_basic_angle(v3d::GetZeroV())
{
	MV1SetupCollInfo(m_model_handle);
}

Modeler::Modeler(const std::shared_ptr<Transform> transform, const int model_handle, const VECTOR& basic_angle) :
	m_model_handle(model_handle),
	m_opacity(1.0f),
	m_transform(transform),
	m_basic_angle(basic_angle)
{
	MV1SetupCollInfo(m_model_handle);
}

Modeler::Modeler(const std::shared_ptr<Transform> transform, const int model_handle) :
	m_model_handle(model_handle),
	m_opacity(1.0f),
	m_transform(transform),
	m_basic_angle(v3d::GetZeroV())
{
	MV1SetupCollInfo(m_model_handle);
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
	//DxLibHelper::DrawModelFrames(m_model_handle, "model", 0.0325f);
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
	const MATRIX rot_m = math::ConvertEulerAnglesToRotMatrix(m_basic_angle);
	MV1SetMatrix(m_model_handle, rot_m * m_transform->GetMatrix(CoordinateKind::kWorld));

	// 行列情報の更新と同時に衝突情報も更新
	MV1RefreshCollInfo(m_model_handle);
}
