#pragma once
#include <string>
#include <array>
#include "../1DShape/segment.hpp"
#include "../../Base/shape_base.hpp"

/// @brief 板ポリゴン(三角形)
class Triangle : public ShapeBase
{
public:
	/// @brief コンストラクタ
	/// @brief 必ず左回り(反時計回り)で座標を指定する必要あり
	Triangle(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3);
	Triangle();
	~Triangle();

	/// @brief 描画
	/// @param is_draw_normal_vector 法線ベクトルを描画するかどうか
	/// @param is_draw_frame 枠を描画させるかどうか
	/// @param alpha_blend_num 透過値 (0～255 : 0で完全透過)
	void Draw(bool is_draw_normal_vector, bool is_draw_frame, int alpha_blend_num, unsigned int frame_color)const;

	void Move(const VECTOR& velocity);

	/// @brief テクスチャ読み込み
	/// @brief この関数を使用しなかった場合、白い板が描画される
	/// @brief 画像の左上は 「u = 0.0f, v = 0.0f」, 画像の右下は 「u = 1.0f, v = 1.0f」とする
	/// @param file_path 画像のファイルパス
	/// @param u, v 頂点のテクスチャ座標
	void LoadTexture(std::string file_path, float u1, float v1, float u2, float v2, float u3, float v3);

	[[nodiscard]] VECTOR			GetNormalVector	()			const	{ return m_normal_vector; }
	[[nodiscard]] VECTOR			GetCentroid		()			const	{ return m_centroid; }
	[[nodiscard]] VECTOR			GetPos			(int index)	const	{ return m_vertexes[index].pos; }
	[[nodiscard]] const Segment*	GetEdge			(int index)	const	{ return &m_edges.at(index); }
	
private:
	static constexpr int kVertexNum = 3;

	VECTOR					m_normal_vector;	// 法線ベクトル
	VECTOR					m_centroid;			// 重心
    VERTEX3D				m_vertexes[3];		// ポリゴンの各頂点
	std::array<Segment, 3>	m_edges;			// 各辺
	int						m_image_handle;		// 画像ハンドル
};
