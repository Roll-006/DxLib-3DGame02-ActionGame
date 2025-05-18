#pragma once
#include <DxLib.h>
#include "../2DShapes/square.hpp"
#include "../box_data.hpp"
#include "../../Base/shape_base.hpp"

class OBB : public ShapeBase
{
public:
	OBB(const VECTOR& pos, const VECTOR& dir, const VECTOR& length);
	OBB();
	~OBB();

	void Init(const VECTOR& pos, const VECTOR& dir, const VECTOR& length);
	void Update(const VECTOR& pos, const VECTOR& dir);
	void Draw(bool is_draw_normal_vector, bool is_draw_frame, int alpha_blend_num)const;

	/// @brief テクスチャ読み込み
	/// @brief この関数を使用しなかった場合、白い板が描画される
	/// @param image_name 画像名
	/// @param paste_plane テクスチャを張り付ける面
	/// @param paste_dir テクスチャを張り付ける向き
	void LoadTexture(std::string image_name, BoxData::PlaneKind paste_plane, TextureData::PasteDir paste_dir);

	[[nodiscard]] VECTOR   GetDir()const								{ return m_dir; }
	[[nodiscard]] VECTOR   GetPos()const								{ return m_pos; }
	[[nodiscard]] VECTOR   GetVertex(BoxData::VertexKind vertex_kind)const	{ return m_box.vertex.at(static_cast<int>(vertex_kind)); }
	[[nodiscard]] Square*  GetPlane(BoxData::PlaneKind plane_kind)const		{ return m_box.plane.at(static_cast<int>(plane_kind)); }
	[[nodiscard]] Segment* GetEdge(BoxData::EdgeKind edge_kind)const;

private:
	void CalcVertexPos();
	void CalcPlanePos();

private:
	VECTOR m_pos;	// 中心座標
	VECTOR m_dir;	// 向きベクトル
	VECTOR m_length;	// 各軸の長さ

	BoxData m_box;
};