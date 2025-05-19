#pragma once
#include "../1DShape/segment.hpp"

class Capsule : public ShapeBase
{
public:
    Capsule(const Segment* segment, float radius);
    Capsule(const VECTOR& segment_begin_pos, const VECTOR& segment_end_pos, float radius);
    Capsule(const VECTOR& segment_begin_pos, const VECTOR& dir, float length, float radius);
    Capsule();
    ~Capsule();

    /// @brief •`‰æ
    /// @param is_draw_frame ˜g‚ğ•`‰æ‚³‚¹‚é‚©‚Ç‚¤‚©
    /// @param alpha_blend_num “§‰ß’l (0`255 : 0‚ÅŠ®‘S“§‰ß)
    void Draw(bool is_draw_frame, int alpha_blend_num, unsigned int frame_color)const;

    void Move(const VECTOR& velocity);

    void SetPos(const VECTOR& segment_begin_pos) { m_segment.SetPos(segment_begin_pos); }

    [[nodiscard]] const Segment* GetSegment()const { return &m_segment; }
    [[nodiscard]] float          GetRadius() const { return m_radius; }

private:
    static constexpr int kPolygonDetailed = 16; // ƒ|ƒŠƒSƒ“‚Ì×‚©‚³

    float   m_radius;
    Segment m_segment;
};
