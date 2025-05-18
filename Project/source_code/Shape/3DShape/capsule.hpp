#pragma once
#include <DxLib.h>
#include "../1DShapes/segment.hpp"
#include "../../Base/shape_base.hpp"

class Capsule : public ShapeBase
{
public:
    Capsule(const Segment* segment, float radius);
    Capsule(const VECTOR& begin_pos, const VECTOR& end_pos, float radius);
    Capsule(const VECTOR& begin_pos, const VECTOR& dir, float length, float radius);
    Capsule();
    ~Capsule();

    void Init(const Segment* segment, float radius);
    void Init(const VECTOR& begin_pos, const VECTOR& end_pos, float radius);
    void Init(const VECTOR& begin_pos, const VECTOR& dir, float length, float radius);
    void Update(const VECTOR& pos);

    /// @brief •`‰æ
    /// @param is_draw_frame ˜g‚ğ•`‰æ‚³‚¹‚é‚©‚Ç‚¤‚©
    /// @param alpha_blend_num “§‰ß’l (0`255 : 0‚ÅŠ®‘S“§‰ß)
    void Draw(bool is_draw_frame, int alpha_blend_num)const;

    void InitOffset(const VECTOR& offset);

    // FIXME: Œ³‚Ìd—l‚É‰ˆ‚¤‚½‚ß const_cast ‚Ìg—p
    [[nodiscard]] Segment* GetSegment()const { return const_cast<Segment*>(&m_segment); }
    [[nodiscard]] float    GetRadius()const  { return m_radius; }

private:
    static constexpr int kPolygonDetailed   = 16;       // ƒ|ƒŠƒSƒ“‚Ì×‚©‚³

    Segment  m_segment;
    float    m_radius;
};
