#pragma once
#include "physical_obj_base.hpp"

class WeaponBase abstract : public PhysicalObjBase
{
public:
	WeaponBase(const std::string& name, const std::string& file_path) :
		PhysicalObjBase	(name, ObjTag.WEAPON, MassKind::kLight),
		m_owner_modeler	(nullptr),
		m_correct_pos	(v3d::GetZeroV()),
		m_correct_angle	(v3d::GetZeroV()),
		m_correct_scale	(v3d::GetZeroV())
	{
		m_modeler = std::make_shared<Modeler>(m_transform, file_path, VGet(0.0f, DX_PI_F, 0.0f));
	}

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

protected:
	/// @brief 補正値を設定
	/// @param angle 補正角度
	/// @param pos 補正座標
	/// @param scale 補正倍率
	void SetCorrectValue(const VECTOR& pos, const VECTOR& angle, const VECTOR& scale)
	{
		m_correct_pos   = pos;
		m_correct_angle = angle;
		m_correct_scale = scale;
	}

	void SetCorrectValue(const VECTOR& pos, const VECTOR& angle, const float scale)
	{
		m_correct_pos	= pos;
		m_correct_angle = angle;
		m_correct_scale = VGet(scale, scale, scale);
	}

	/// @brief 持ち主を追跡する
	void TrackOwner();

protected:
	std::shared_ptr<Modeler> m_owner_modeler;	// 武器の持ち主であるオブジェクトのモデラー
	
private:
	VECTOR m_correct_pos;		// 補正座標
	VECTOR m_correct_angle;		// 補正角度
	VECTOR m_correct_scale;		// 補正倍率
};
