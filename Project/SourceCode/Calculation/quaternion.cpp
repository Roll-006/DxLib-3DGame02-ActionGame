#include "quaternion.hpp"

Quaternion quat::ConvertFloat4ToQuaternion(const FLOAT4& f)
{
	return Quaternion(f.x, f.y, f.z, f.w);
}

FLOAT4 quat::ConvertQuaternionToFloat4(const Quaternion& q)
{
	return FLOAT4(q.x, q.y, q.z, q.w);
}

Quaternion quat::GetZeroQuaternion()
{
	return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
}

Quaternion quat::GetIdentityQuaternion()
{
	return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

Quaternion quat::CreateQuaternion(const VECTOR& axis, const float angle)
{
	return ConvertFloat4ToQuaternion(QTRot(v3d::GetNormalizedV(axis), angle));
}

float quat::GetSize(const Quaternion& q)
{
	return sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

float quat::GetSquareSize(const Quaternion& q)
{
	return GetSize(q) * GetSize(q);
}

Quaternion quat::GetNormalizedQuaternion(const Quaternion& q)
{
	const float size = GetSize(q);
	return size != 0 ? Quaternion(q.x / size, q.y / size, q.z / size, q.w / size) : q;
}

Quaternion quat::GetConjugateQuaternion(const Quaternion& q)
{
	return Quaternion(-q.x, -q.y, -q.z, q.w);
}

Quaternion quat::GetInverseQuaternion(const Quaternion& q)
{
	return GetConjugateQuaternion(q) * (1.0f / GetSquareSize(q));
}

Quaternion quat::GetSlerpQuaternion(const Quaternion& begin_q, const Quaternion& end_q, const float t)
{
    // 角度算出
    const float len1 = sqrt(begin_q.x * begin_q.x + begin_q.y * begin_q.y + begin_q.z * begin_q.z + begin_q.w * begin_q.w);
    const float len2 = sqrt(end_q.x   * end_q.x   + end_q.y   * end_q.y   + end_q.z   * end_q.z   + end_q.w   * end_q.w  );

    // 不正なクォータニオンは処理を中断
    if (len1 == 0.0f || len2 == 0.0f)
    {
        return begin_q; 
    }

    const float cos_val = (begin_q.x * end_q.x + begin_q.y * end_q.y + begin_q.z * end_q.z + begin_q.w * end_q.w) / (len1 * len2);
    const float w       = acos(cos_val);

    // 球面線形補間
    const float sin_w       = sin(w);
    const float sin_t_w     = sin(t * w);
    const float sin_inv_t_w = sin((1.0f - t) * w);
    const float mult_q1     = sin_inv_t_w / sin_w;
    const float mult_q2     = sin_t_w / sin_w;

    return Quaternion
    {
        mult_q1 * begin_q.x + mult_q2 * end_q.x,
        mult_q1 * begin_q.y + mult_q2 * end_q.y,
        mult_q1 * begin_q.z + mult_q2 * end_q.z,
        mult_q1 * begin_q.w + mult_q2 * end_q.w
    };
}
