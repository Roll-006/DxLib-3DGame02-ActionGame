#pragma once
#include <queue>
#include "../Base/obj_base.hpp"

#include "../Data/Kind/mass_kind.hpp"
#include "../Part/collider.hpp"
#include "../Data/collider_pair_data.hpp"
#include "../FPS/fps.hpp"

/// @brief 物理的な挙動を行うオブジェクトの基底クラス
class PhysicalObjBase abstract : public ObjBase
{
public:
	PhysicalObjBase(const std::string& name, const std::string& tag, MassKind mass_level_kind);
	virtual ~PhysicalObjBase() = default;

	virtual void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) abstract;

	/// @brief 衝突した三角形の登録を解除する
	void RemoveHitTriangles();

	/// @brief 重力を与える(適用させる)
	/// @brief 物理管理クラスから適用される
	/// @param gravity_acceleration 重力加速度 (デルタタイム適用前)
	/// @param max_gravity 最大重力 (デルタタイム適用後の最大値)
	void ApplyGravity(const float gravity_acceleration, const float max_gravity);

	/// @brief 速度ベクトルに落下速度ベクトルを加算
	void AddFallVelocity()
	{
		m_velocity += m_fall_velocity;
	}

	/// @brief 速度ベクトルをトランスフォームおよびコライダーに適用
	void ApplyVelocity();

	/// @brief 速度ベクトルを接している面に投影する
	void ProjectionVelocity();
	/// @brief 座標を着地面に投影する
	void ProjectionPos();

	/// @brief 着地判定を解除する
	void ReleaseLanding() { m_is_landing = false; }

	void SetModelHandle	(const int model_handle) { m_model_handle = model_handle; }
	void SetVelocity	(const VECTOR& velocity) { m_velocity = velocity; }

	[[nodiscard]] int		GetModelHandle()	const { return m_model_handle; }
	[[nodiscard]] VECTOR	GetVelocity()		const { return m_velocity; }
	[[nodiscard]] VECTOR	GetFallVelocity()	const { return m_fall_velocity; }
	[[nodiscard]] MassKind	GetMassKind()		const { return m_mass_kind; }
	[[nodiscard]] std::shared_ptr<Collider> GetCollider(const ColliderKind kind) const;
	[[nodiscard]] std::vector<std::shared_ptr<Collider>> GetColliderAll() const { return m_collider; }

protected:
	/// @brief コライダーを追加
	/// @brief 異なる種類であれば複数のコライダーの追加を許可
	/// @brief 同種類コライダーであっても図形が異なれば上書きを許可
	void AddCollider(const std::shared_ptr<Collider> collider);

protected:
	float	 m_fall_speed;
	VECTOR	 m_fall_velocity;
	VECTOR	 m_velocity;
	bool	 m_is_landing;
	MassKind m_mass_kind;

	std::vector<std::shared_ptr<Collider>> m_collider;

private:
	int m_model_handle;
};
