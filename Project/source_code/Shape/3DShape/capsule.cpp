#include "../../../Vector/vector.hpp"
#include "capsule.hpp"

Capsule::Capsule(const Segment* segment, float radius):
    ShapeBase(ShapeKind::kCapsule),
    m_radius        (radius)
{
    m_segment = *segment;
    Init(&m_segment, m_radius);
}

Capsule::Capsule(const VECTOR& begin_pos, const VECTOR& end_pos, float radius) :
    ShapeBase(ShapeKind::kCapsule),
    m_radius        (radius)
{
    m_segment = Segment(begin_pos, end_pos);
    Init(begin_pos, end_pos, m_radius);
}

Capsule::Capsule(const VECTOR& begin_pos, const VECTOR& dir, float length, float radius) :
    ShapeBase(ShapeKind::kCapsule),
    m_radius        (radius)
{
    m_segment = Segment(begin_pos, dir, length);
    Init(begin_pos, dir, length, m_radius);
}

Capsule::Capsule() :
    ShapeBase(ShapeKind::kCapsule),
    m_radius        (0.0f)
{

}

Capsule::~Capsule()
{

}

void Capsule::Init(const Segment* segment, float radius)
{
    m_segment = *segment;
    m_radius  = radius;
}

void Capsule::Init(const VECTOR& begin_pos, const VECTOR& end_pos, float radius)
{
    m_segment.Init(begin_pos, end_pos);
    m_radius = radius;
}

void Capsule::Init(const VECTOR& begin_pos, const VECTOR& dir, float length, float radius)
{
    m_segment.Init(begin_pos, dir, length);
    m_radius = radius;
}

void Capsule::Update(const VECTOR& pos)
{
    m_segment.Update(pos);
}

void Capsule::Draw(bool is_draw_frame, int alpha_blend_num)const
{
    if (is_draw_frame)
    {
        DrawCapsule3D(m_segment.GetBeginPos(), m_segment.GetEndPos(), m_radius, kPolygonDetailed, 0xff0000, 0xff0000, FALSE);
    }

    if (alpha_blend_num > 0)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_blend_num);
        DrawCapsule3D(m_segment.GetBeginPos(), m_segment.GetEndPos(), m_radius, kPolygonDetailed, 0xffffff, 0xff0000, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}

void Capsule::InitOffset(const VECTOR& offset)
{
    m_segment.InitOffset(offset);
}
