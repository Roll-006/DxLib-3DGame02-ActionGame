#include "transform.hpp"
#include "../Manager/obj_manager.hpp"

Transform::Transform(const VECTOR& local_pos) :
	m_local_matrix(MGetIdent())
{
	m_local_matrix.m[3][0] = local_pos.x;
	m_local_matrix.m[3][1] = local_pos.y;
	m_local_matrix.m[3][2] = local_pos.z;
}

Transform::Transform() :
	m_local_matrix(MGetIdent())
{
	// 処理なし
}

Transform::~Transform()
{
	// 処理なし
}

void Transform::Move(const CoordinateKind coord_kind, const VECTOR& velocity)
{
	SetPos(coord_kind, GetPos(coord_kind) + velocity);
}


#pragma region Attach / Detach
void Transform::AttachParent(const std::shared_ptr<Transform> parent_transform)
{
	// 親がいない場合のみアタッチ
	if (m_parent_transform) { return; }

	m_parent_transform	= parent_transform;
	m_local_matrix		= MMult(m_local_matrix, MInverse(m_parent_transform->GetMatrix(CoordinateKind::kWorld)));
}

void Transform::AttachParent(const std::string& parent_obj_name)
{
	// 親がいない場合のみアタッチ
	if (m_parent_transform) { return; }
	
	const auto parent_obj = ObjManager::GetInstance()->GetObj(parent_obj_name);
	const auto parent_transform = parent_obj->GetTransform();

	AttachParent(parent_transform);
}

void Transform::DetachParent()
{
	// 親がいる場合のみデタッチ
	if (!m_parent_transform) { return; }

	m_local_matrix = GetMatrix(CoordinateKind::kWorld);
	m_parent_transform = nullptr;
}
#pragma endregion


#pragma region Setter
void Transform::SetPos(const CoordinateKind coord_kind, const VECTOR& pos)
{
	// ワールド座標
	if (coord_kind == CoordinateKind::kWorld)
	{
		if (m_parent_transform)
		{
			m_local_matrix.m[3][0] = pos.x - m_parent_transform->GetPos(CoordinateKind::kWorld).x;
			m_local_matrix.m[3][1] = pos.y - m_parent_transform->GetPos(CoordinateKind::kWorld).y;
			m_local_matrix.m[3][2] = pos.z - m_parent_transform->GetPos(CoordinateKind::kWorld).z;
			return;
		}
	}

	// ローカル座標 / 親を持たないワールド座標
	m_local_matrix.m[3][0] = pos.x;
	m_local_matrix.m[3][1] = pos.y;
	m_local_matrix.m[3][2] = pos.z;
}

void Transform::SetRotation(const CoordinateKind coord_kind, const MATRIX& rotation_matrix)
{
	const MATRIX prev_rot = MGetRotElem(GetMatrix(coord_kind));

	m_local_matrix = MMult(m_local_matrix, MInverse(prev_rot));
	m_local_matrix = MMult(m_local_matrix, rotation_matrix);
}

void Transform::SetScale(const CoordinateKind coord_kind, const VECTOR& scale)
{

}
#pragma endregion


#pragma region Getter
MATRIX Transform::GetMatrix(const CoordinateKind coord_kind)
{
	// ワールド座標
	if (coord_kind == CoordinateKind::kWorld)
	{
		if (m_parent_transform)
		{
			return MMult(m_local_matrix, m_parent_transform->GetMatrix(CoordinateKind::kWorld));
		}
	}

	// ローカル座標 / 親を持たないワールド座標
	return m_local_matrix;
}

VECTOR Transform::GetPos(const CoordinateKind coord_kind)
{
	const MATRIX mat = GetMatrix(coord_kind);
	return VECTOR(mat.m[3][0], mat.m[3][1], mat.m[3][2]);
}

MATRIX Transform::GetRotationMatrix(const CoordinateKind coord_kind)
{
	return MGetRotElem(GetMatrix(coord_kind));
}

VECTOR Transform::GetScale(const CoordinateKind coord_kind)
{
	const MATRIX mat = GetMatrix(coord_kind);
	return VECTOR(mat.m[0][0], mat.m[1][1], mat.m[2][2]);
}

VECTOR Transform::GetRight(const CoordinateKind coord_kind)
{
	return math::ConvertRotationMatrixToAxes(GetMatrix(coord_kind)).x;
}

VECTOR Transform::GetUp(const CoordinateKind coord_kind)
{
	return math::ConvertRotationMatrixToAxes(GetMatrix(coord_kind)).y;
}

VECTOR Transform::GetForward(const CoordinateKind coord_kind)
{
	return math::ConvertRotationMatrixToAxes(GetMatrix(coord_kind)).z;
}
#pragma endregion
