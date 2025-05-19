#pragma once
#include "../2DShape/square.hpp"
#include "../../Data/box_data.hpp"
#include "../../Data/Kind/box_kind.hpp"

class OBB : public ShapeBase
{
public:
	OBB(const VECTOR& pos, const VECTOR& dir, const VECTOR& length);
	OBB();
	~OBB();

	void Draw(bool is_draw_normal_vector, bool is_draw_frame, int alpha_blend_num, unsigned int frame_color)const;

	/// @brief テクスチャ読み込み
	/// @brief この関数を使用しなかった場合、白い板が描画される
	/// @param file_path 画像のファイルパス
	/// @param paste_square テクスチャを張り付ける面
	/// @param texture_dir テクスチャを張り付ける向き
	void LoadTexture(std::string file_path, SquareKind paste_square, TextureDirKind texture_dir);

	/// @brief 移動
	/// @param is_sync_dir 移動方向とOBBの向きを同期させるかどうか
	void Move(const VECTOR& velocity, bool is_sync_dir);

	void SetPos(const VECTOR& pos);
	void SetDir(const VECTOR& dir);

	[[nodiscard]] VECTOR		 GetPos		()						const	{ return m_pos; }
	[[nodiscard]] VECTOR		 GetDir		()						const	{ return m_dir; }
	[[nodiscard]] VECTOR		 GetVertex	(VertexKind vertex_kind)const	{ return m_box.vertexes.at(static_cast<int>(vertex_kind)); }
	[[nodiscard]] const Square*  GetPlane	(SquareKind square_kind)const	{ return &m_box.squares.at(static_cast<int>(square_kind)); }
	[[nodiscard]] const Segment* GetEdge	(EdgeKind edge_kind)	const;

private:
	void CalcVertexPos();
	void CalcPlanePos();

private:
	VECTOR  m_pos;		// 中心座標
	VECTOR  m_dir;
	VECTOR  m_length;	// 各軸の長さ
	BoxData m_box;
};
