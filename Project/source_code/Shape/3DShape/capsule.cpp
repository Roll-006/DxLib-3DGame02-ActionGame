#include "capsule.hpp"

Capsule::Capsule(const Segment* segment, float radius):
    ShapeBase   (ShapeKind::kCapsule),
    m_radius    (radius),
    m_segment   (*segment)
{
    // ˆ—‚È‚µ
}

Capsule::Capsule(const VECTOR& segment_begin_pos, const VECTOR& segment_end_pos, float radius) :
    ShapeBase   (ShapeKind::kCapsule),
    m_radius    (radius),
    m_segment   (Segment(segment_begin_pos, segment_end_pos))
{
    // ˆ—‚È‚µ
}

Capsule::Capsule(const VECTOR& segment_begin_pos, const VECTOR& dir, float length, float radius) :
    ShapeBase   (ShapeKind::kCapsule),
    m_radius    (radius),
    m_segment   (Segment(segment_begin_pos, dir, length))
{
    // ˆ—‚È‚µ
}

Capsule::Capsule() :
    ShapeBase   (ShapeKind::kCapsule),
    m_radius    (0.0f),
    m_segment   (Segment())
{
    // ˆ—‚È‚µ
}

Capsule::~Capsule()
{
    // ˆ—‚È‚µ
}

void Capsule::Draw(bool is_draw_frame, int alpha_blend_num, unsigned int frame_color)const
{
    if (alpha_blend_num > 0)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_blend_num);
        DrawCapsule3D(m_segment.GetBeginPos(), m_segment.GetEndPos(), m_radius, kPolygonDetailed, 0xffffff, 0xffffff, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    if (is_draw_frame)
    {
        DrawCapsule3D(m_segment.GetBeginPos(), m_segment.GetEndPos(), m_radius, kPolygonDetailed, frame_color, 0xffffff, FALSE);
    }
}

void Capsule::Move(const VECTOR& velocity)
{
    m_segment.Move(velocity);
}
