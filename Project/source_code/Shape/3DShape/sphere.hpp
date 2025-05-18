#pragma once
#include "../../Base/shape_base.hpp"

class Sphere : public ShapeBase
{
public:
    Sphere(const VECTOR& pos, float radius);
    Sphere();
    ~Sphere();

    /// @brief •`‰æ
    /// @param is_draw_frame ˜g‚ğ•`‰æ‚³‚¹‚é‚©‚Ç‚¤‚©
    /// @param alpha_blend_num “§‰ß’l (0`255 : 0‚ÅŠ®‘S“§‰ß)
    void Draw(bool is_draw_frame, int alpha_blend_num, unsigned int frame_color)const;

    void Move(const VECTOR& velocity);

    void SetPos(const VECTOR& pos) { m_pos = pos; }

    [[nodiscard]] VECTOR GetPos()   const { return m_pos; }
    [[nodiscard]] float  GetRadius()const { return m_radius; }

private:
    static constexpr int kPolygonDetailed = 16;     // ƒ|ƒŠƒSƒ“‚Ì×‚©‚³

    VECTOR m_pos;
    float  m_radius;
};
