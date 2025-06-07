#pragma once
#include "obj_base.hpp"
#include "../Part/modeler.hpp"

class WeaponBase abstract : public ObjBase
{
public:
	WeaponBase(const std::string& name, const std::string& file_path) :
		ObjBase			(name, ObjTag.WEAPON),
		m_modeler		(std::make_shared<Modeler>(GetTransform(), file_path, false)),
		m_owner_modeler	(nullptr),
		m_correct_angle	(v3d::GetZeroVector()),
		m_correct_pos	(v3d::GetZeroVector())
	{ }

	virtual ~WeaponBase() = default;

	/// @brief 装備する持ち主をアタッチする
	void AttachOwner(const std::shared_ptr<Modeler> owner_modeler)
	{
		if (!m_owner_modeler) { m_owner_modeler = owner_modeler; }
	}
	/// @brief 装備していた持ち主をデタッチする
	void DetachOwner()
	{
		if (m_owner_modeler) { m_owner_modeler = nullptr; }
	}

	/// @brief 持ち主を追跡する
	void TrackOwner()
	{
		if (!m_owner_modeler) { return; }

		// アタッチする部位の行列情報を取り出す
		const int    owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), BonePath.RIGHT_HAND);
		const MATRIX owner_attach_frame_mat = MV1GetFrameLocalWorldMatrix(m_owner_modeler->GetModelHandle(), owner_attach_frame_num);

		// 武器をアタッチする部位に合わせて回転し、行列を取得
		MATRIX correct = MGetIdent();
		CreateRotationXYZMatrix(&correct, m_correct_angle.x, m_correct_angle.y, m_correct_angle.z);
		const MATRIX result_mat = correct * owner_attach_frame_mat;

		// 情報を適用
		m_transform->SetMatrix(CoordinateKind::kWorld, result_mat);
		m_transform->SetPos   (CoordinateKind::kLocal, m_transform->GetPos(CoordinateKind::kLocal) + VTransformSR(m_correct_pos, result_mat));
	}

protected:
	/// @brief 補正値を設定
	/// @param angle 補正角度
	/// @param pos 補正座標
	void SetCorrectValue(const VECTOR& angle, const VECTOR& pos)
	{
		m_correct_angle = angle;
		m_correct_pos   = pos;
	}

protected:
	std::shared_ptr<Modeler> m_modeler;
	std::shared_ptr<Modeler> m_owner_modeler;	// 武器の持ち主であるオブジェクトのモデラー
	
	VECTOR m_correct_angle;		// 補正角度
	VECTOR m_correct_pos;		// 補正座標
};
