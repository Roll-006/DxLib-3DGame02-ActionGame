#pragma once
#include <string>
#include <array>

#include "../../Data/Kind/texture_kind.hpp"
#include "triangle.hpp"

class Square : public ShapeBase
{
public:
	/// @brief コンストラクタ
	/// @brief 基本的に平面的な図形にする必要あり
	/// @brief 平面的でなかった場合、衝突判定に不具合が発生する
	/// @param pos1 右下座標
	/// @param pos2 右上座標
	/// @param pos3 左上座標
	/// @param pos4 左下座標
	Square(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3, const VECTOR& pos4);
	Square();
	~Square();

	/// @brief 描画
	/// @param is_draw_normal_vector 法線ベクトルを描画するかどうか
	/// @param is_draw_frame 枠を描画させるかどうか
	/// @param alpha_blend_num 透過値 (0～255 : 0で完全透過)
	void Draw(bool is_draw_normal_vector, bool is_draw_frame, int alpha_blend_num, unsigned int frame_color);

	void Move(const VECTOR& velocity);

	/// @brief テクスチャ読み込み
	/// @brief この関数を使用しなかった場合、白い板が描画される
	/// @brief 画像の左上は 「u = 0.0f, v = 0.0f」, 画像の右下は 「u = 1.0f, v = 1.0f」とする
	/// @brief テクスチャを張り付ける座標を自ら設定する
	/// @param file_path 画像のファイルパス
	/// @param u, v 頂点のテクスチャ座標
	void LoadTexture(std::string file_path, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4);

	/// @brief テクスチャ読み込み
	/// @brief この関数を使用しなかった場合、白い板が描画される
	/// @param file_path 画像のファイルパス
	/// @param texture_dir テクスチャを張り付ける向き
	void LoadTexture(std::string file_path, TextureDirKind texture_dir);

	[[nodiscard]] const Triangle*	GetTriangle		(const int index)	const { return &m_triangles.at(index); }
	[[nodiscard]] VECTOR			GetNormalVector	()					const { return m_triangles.at(0).GetNormalVector(); }
	[[nodiscard]] VECTOR			GetCentroid		()					const { return m_centroid; }
	[[nodiscard]] VECTOR			GetPos			(const int index)	const;
	[[nodiscard]] const Segment*	GetEdge			(const int index)	const;

private:
	VECTOR m_centroid;	// 重心
	std::array<Triangle, 2> m_triangles;
};
