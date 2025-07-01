//#pragma once
//#include <array>
//#include "../../Base/shape_base.hpp"
//
//class Circle : public ShapeBase
//{
//public:
//	/// @brief コンストラクタ
//	/// @param has_internal 内部があるかを判定(true : 円, false : 円周)
//	/// @param normal_vector 法線ベクトル
//	/// @param radius 半径
//	Circle(bool has_internal, const VECTOR& normal_vector, float radius);
//	Circle(bool has_internal);
//	~Circle();
//
//	void SetPos(const VECTOR& pos);
//
//	/// @brief 描画
//	/// @param is_draw_normal_vector 法線ベクトルを描画するかどうか
//	/// @param is_draw_frame 枠を描画させるかどうか
//	/// @param alpha_blend_num 透過値 (0～255 : 0で完全透過)
//	void Draw(bool is_draw_normal_vector, bool is_draw_frame, int alpha_blend_num) const;
//
//	[[nodiscard]] VECTOR GetPos() const		{ return m_pos; }
//	[[nodiscard]] VECTOR GetNormalVector() const { return m_normal_vector; }
//	[[nodiscard]] float  GetRadius() const		{ return m_radius; }
//
//private:
//	static constexpr float kNormalVectorLength	= 100.0f;
//	static constexpr int   kVertexNum			= 40;		// 頂点の数(実際には正円だが、描画するために頂点を設定)
//
//	std::array<Triangle*, kVertexNum> m_triangle;	// 三角形
//
//	VECTOR m_pos;				// 中心座標
//	VECTOR m_normal_vector;		// 法線ベクトル
//	float  m_radius;			// 半径
//	float  m_interval_andle;	// 頂点間の間隔
//};