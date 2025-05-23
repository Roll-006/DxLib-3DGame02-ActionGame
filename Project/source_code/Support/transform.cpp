#include "transform.hpp"

Transform::Transform(const VECTOR& pos) :
	m_local_matrix(MGetIdent()),
	m_world_matrix(MGetIdent())
{
	m_world_matrix.m[3][0] = pos.x;
	m_world_matrix.m[3][1] = pos.y;
	m_world_matrix.m[3][2] = pos.z;
}

Transform::Transform() :
	m_world_matrix(MGetIdent())
{
	// èàóùÇ»Çµ
}

Transform::~Transform()
{
	// èàóùÇ»Çµ
}

void Transform::Move(const VECTOR& velocity)
{
	m_world_matrix.m[3][0] += velocity.x;
	m_world_matrix.m[3][1] += velocity.y;
	m_world_matrix.m[3][2] += velocity.z;
}

void Transform::SetPos(const VECTOR& pos)
{
	m_world_matrix.m[3][0] = pos.x;
	m_world_matrix.m[3][1] = pos.y;
	m_world_matrix.m[3][2] = pos.z;
}

void Transform::SetScale(const VECTOR& scale)
{
	m_world_matrix.m[0][0] = scale.x;
	m_world_matrix.m[1][1] = scale.y;
	m_world_matrix.m[2][2] = scale.z;
}

VECTOR Transform::GetPos()const
{
	return VECTOR(m_world_matrix.m[3][0], m_world_matrix.m[3][1], m_world_matrix.m[3][2]);
}

MATRIX Transform::GetMatrix(const CoordinateKind coord_kind)const
{
	return m_world_matrix; }
VECTOR Transform::GetPos(const CoordinateKind coord_kind)const;
MATRIX Transform::GetRotationMatrix(const CoordinateKind coord_kind)const { return MGetRotElem(m_world_matrix); }
VECTOR Transform::GetScale(const CoordinateKind coord_kind)const;

VECTOR Transform::GetScale()const
{
	return VECTOR(m_world_matrix.m[0][0], m_world_matrix.m[1][1], m_world_matrix.m[2][2]);
}
