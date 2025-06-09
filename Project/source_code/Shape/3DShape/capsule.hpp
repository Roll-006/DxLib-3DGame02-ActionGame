#pragma once
#include "../1DShape/segment.hpp"

class Capsule final : public ShapeBase
{
public:
    Capsule(const Segment& segment, const float radius);
    Capsule(const VECTOR& segment_begin_pos, const VECTOR& segment_end_pos, const float radius);
    Capsule(const VECTOR& segment_begin_pos, const VECTOR& dir, const float segment_length, const float radius);
    Capsule();
    ~Capsule() override;

    /// @brief 描画
    /// @param is_draw_frame フレームを描画させるかどうか
    /// @param alpha_blend_num 透過値 (0～255 : 0で完全透過)
    /// @param frame_color フレーム色
    void Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const override;

    void Move(const VECTOR& velocity) override;

    void SetPos(const VECTOR& segment_begin_pos) { m_segment.SetPos(segment_begin_pos); }
    void SetLength(const float segment_length) { m_segment.SetLength(segment_length); }

    [[nodiscard]] const Segment& GetSegment() const { return m_segment; }
    [[nodiscard]] float          GetRadius()  const { return m_radius; }

private:
    static constexpr int kPolygonDetailed = 8; // ポリゴンの細かさ

    float   m_radius;
    Segment m_segment;
};
