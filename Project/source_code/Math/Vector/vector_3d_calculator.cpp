#include "vector_3d_calculator.hpp"

VECTOR v3d::GetZeroVector()
{
    return VGet(0.0f, 0.0f, 0.0f);
}

VECTOR v3d::GetNormalizedVector(const VECTOR& v)
{
    return VSize(v) != 0 ? VNorm(v) : v;
}

VECTOR v3d::GetNormalVector(const VECTOR& v)
{
    VECTOR normal_v = GetZeroVector();

    if (v != VGet(1, 0, 0))
    {
        normal_v = GetNormalVector(v, VGet(1, 0, 0));
    }
    else if (v != VGet(0, 1, 0))
    {
        normal_v = GetNormalVector(v, VGet(0, 1, 0));
    }
    else if (v != VGet(0, 0, 1))
    {
        normal_v = GetNormalVector(v, VGet(0, 0, 1));
    }
    return GetNormalizedVector(normal_v);
}

VECTOR v3d::GetNormalVector(const VECTOR& v1, const VECTOR& v2)
{
    // [法線ベクトル = v1とv2の外積]から成分を取得
    return GetNormalizedVector(VCross(v1, v2));
}

float v3d::GetYaw(const VECTOR& vector)
{
    return atan2f(-vector.x, -vector.z);
}

VECTOR v3d::GetYawRotateVector(const VECTOR& vector)
{
    return VGet(0.0f, GetYaw(vector), 0.0f);
}
