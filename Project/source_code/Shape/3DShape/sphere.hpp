#pragma once
#include <DxLib.h>
#include "../../Base/shape_base.hpp"

class Sphere : public ShapeBase
{
public:
    Sphere(float radius);
    Sphere();
    ~Sphere();

    void Init(float radius);
    void Update(const VECTOR& pos);

    /// @brief 描画
    /// @param is_draw_frame 枠を描画させるかどうか
    /// @param alpha_blend_num 透過値 (0～255 : 0で完全透過)
    void Draw(bool is_draw_frame, int alpha_blend_num)const;

    void InitOffset(const VECTOR& offset);

    [[nodiscard]] VECTOR GetPos()const { return m_pos; }
    [[nodiscard]] float  GetRadius()const   { return m_radius; }

private:
    static constexpr int kPolygonDetailed   = 16;       // ポリゴンの細かさ

    VECTOR m_pos;
    VECTOR m_offset;    // 座標修正用オフセット
    float  m_radius;
};
