#include "../../../Calculator/math.hpp"
#include "sphere.hpp"

Sphere::Sphere(float radius):
    ShapeBase(ShapeKind::kSphere),
    m_pos      (math::GetZeroVector()),
    m_offset        (math::GetZeroVector()),
    m_radius        (radius)
{

}

Sphere::Sphere() :
    ShapeBase(ShapeKind::kSphere),
    m_pos      (math::GetZeroVector()),
    m_offset        (math::GetZeroVector()),
    m_radius        (0.0f)
{

}

Sphere::~Sphere()
{

}

void Sphere::Init(float radius)
{
    m_radius = radius;
}

void Sphere::Update(const VECTOR& pos)
{
    m_pos = pos + m_offset;
}

void Sphere::Draw(bool is_draw_frame, int alpha_blend_num)const
{
    if (is_draw_frame)
    {
        DrawSphere3D(m_pos, m_radius, kPolygonDetailed, 0xff0000, 0xff0000, FALSE);
    }

    if (alpha_blend_num > 0)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_blend_num);
        DrawSphere3D(m_pos, m_radius, kPolygonDetailed, 0xffffff, 0xffffff, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}

void Sphere::InitOffset(const VECTOR& offset)
{
    m_offset = offset;
}
