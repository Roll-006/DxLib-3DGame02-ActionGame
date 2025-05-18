#include "transform.hpp"

Transform::Transform(const VECTOR& pos) :
	m_matrix(MGetIdent())
{
	m_matrix.m[3][0] = pos.x;
	m_matrix.m[3][1] = pos.y;
	m_matrix.m[3][2] = pos.z;
}

Transform::~Transform()
{

}

void Transform::Move(const VECTOR& velocity)
{
	m_matrix.m[3][0] += velocity.x;
	m_matrix.m[3][1] += velocity.y;
	m_matrix.m[3][2] += velocity.z;
}

void Transform::SetPos(const VECTOR& pos)
{
	m_matrix.m[3][0] = pos.x;
	m_matrix.m[3][1] = pos.y;
	m_matrix.m[3][2] = pos.z;
}

void Transform::SetScale(const VECTOR& scale)
{
	m_matrix.m[0][0] = scale.x;
	m_matrix.m[1][1] = scale.y;
	m_matrix.m[2][2] = scale.z;
}

VECTOR Transform::GetPos()const
{
	return VECTOR(m_matrix.m[3][0], m_matrix.m[3][1], m_matrix.m[3][2]);
}

//Quaternion TransformComponent::GetRotationQuaternion()const
//{
//	return math::ConvertMatrixToQuaternion(m_matrix);
//}

//Quaternion TransformComponent::GetRotateQuaternion(const VECTOR& axis, float angle)const
//{
//	return ConvertFloat4ToQuaternion(QTRot(axis, angle));
//}
//
//Quaternion TransformComponent::GetPosQuaternion(const VECTOR& pos)const
//{
//	return Quaternion(pos.x, pos.y, pos.z, 0.0f);
//}

VECTOR Transform::GetScale()const
{
	return VECTOR(m_matrix.m[0][0], m_matrix.m[1][1], m_matrix.m[2][2]);
}
