#include "transform.hpp"
#include "../Manager/obj_manager.hpp"

Transform::Transform() :
	m_local_matrix		(MGetIdent()),
	m_parent_transform	(nullptr)
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
	m_local_matrix		= m_local_matrix * MInverse(m_parent_transform->GetMatrix(CoordinateKind::kWorld));
}

void Transform::AttachParent(const std::string& parent_obj_name)
{
	// 親がいない場合のみアタッチ
	if (m_parent_transform) { return; }
	
	const auto parent_obj		= ObjManager::GetInstance()->GetObj(parent_obj_name);
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
void Transform::SetMatrix(const CoordinateKind coord_kind, const MATRIX& matrix)
{
	if (coord_kind == CoordinateKind::kWorld)
	{
		if (m_parent_transform)
		{
			m_local_matrix = MInverse(m_parent_transform->GetMatrix(coord_kind)) * matrix;
			return;
		}
	}

	m_local_matrix = matrix;
}

void Transform::SetPos(const CoordinateKind coord_kind, const VECTOR& pos)
{
	VECTOR parent_pos = v3d::GetZeroVector();

	if (coord_kind == CoordinateKind::kWorld)
	{
		if (m_parent_transform)
		{
			parent_pos = m_parent_transform->GetPos(CoordinateKind::kWorld);
		}
	}

	m_local_matrix.m[3][0] = pos.x - parent_pos.x;
	m_local_matrix.m[3][1] = pos.y - parent_pos.y;
	m_local_matrix.m[3][2] = pos.z - parent_pos.z;
}

void Transform::SetRotation(const CoordinateKind coord_kind, const MATRIX& rotation_matrix)
{
	// ローカル行列として格納
	m_local_matrix = rotation_matrix * MInverse(GetRotationMatrix(coord_kind)) * GetMatrix(coord_kind);
}

void Transform::SetRotation(const CoordinateKind coord_kind, const VECTOR& dir)
{
	// dirからオブジェクトのXYZ軸を取得
	Axes axes = math::GetAxes(dir, axis::GetWorldAxes());

	if (coord_kind == CoordinateKind::kLocal)
	{
		if (m_parent_transform)
		{
			// 親がいる場合は親基準のXYZ軸を取得
			axes = math::GetAxes(dir, m_parent_transform->GetAxes(CoordinateKind::kWorld));
		}
	}
	SetRotation(coord_kind, axes);
}

void Transform::SetRotation(const CoordinateKind coord_kind, const Axes& axes)
{
	// XYZ軸からオイラー角へ変換
	VECTOR angle = math::ConvertAxesToEulerAngles(axes, axis::GetWorldAxes());

	if (coord_kind == CoordinateKind::kLocal)
	{
		if (m_parent_transform)
		{
			angle = math::ConvertAxesToEulerAngles(axes, m_parent_transform->GetAxes(CoordinateKind::kWorld));
		}
	}

	// オイラー角から回転行列を生成
	MATRIX mat = MGetIdent();
	CreateRotationXYZMatrix(&mat, angle.x, angle.y, angle.z);

	SetRotation(coord_kind, mat); 
}

void Transform::SetScale(const CoordinateKind coord_kind, const VECTOR& scale)
{
	//m_local_matrix = scale * MInverse(GetRotationMatrix(coord_kind)) * GetMatrix(coord_kind);
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
			return m_local_matrix * m_parent_transform->GetMatrix(CoordinateKind::kWorld);
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
	return GetAxes(coord_kind).x_axis;
}

VECTOR Transform::GetUp(const CoordinateKind coord_kind)
{
	return GetAxes(coord_kind).y_axis;
}

VECTOR Transform::GetForward(const CoordinateKind coord_kind)
{
	return GetAxes(coord_kind).z_axis;
}

Axes Transform::GetAxes(const CoordinateKind coord_kind)
{
	return math::ConvertRotationMatrixToAxes(GetMatrix(coord_kind));
}

VECTOR Transform::GetEulerAngles(const CoordinateKind coord_kind)
{
	if (coord_kind == CoordinateKind::kLocal)
	{
		if (m_parent_transform)
		{
			return math::ConvertRotationMatrixToEulerAngles(GetMatrix(coord_kind), m_parent_transform->GetAxes(coord_kind));
		}
	}
	return math::ConvertRotationMatrixToEulerAngles(GetMatrix(coord_kind), axis::GetWorldAxes());
}
#pragma endregion
