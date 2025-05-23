#pragma once
#include <memory>
#include "../Calculation/math.hpp"

class Transform final
{
public:
	Transform(const VECTOR& pos);
	Transform();
	~Transform();

	void Move(const VECTOR& velocity);

	void SetLocalPPos(const VECTOR& pos);
	void SetWorldPPos(const VECTOR& pos);
	//void SetRotatetion();
	void SetScale(const VECTOR& scale);

	void AttachOwner(const std::shared_ptr<Transform> owner) { m_owner_transform = owner; }
	void DetachOwner() { m_owner_transform = nullptr; }

	[[nodiscard]] MATRIX GetLocalMatrix			()const { return m_local_matrix; }
	[[nodiscard]] MATRIX GetWorldMatrix			()const { return m_world_matrix; }
	[[nodiscard]] VECTOR GetLocalPos			()const;
	[[nodiscard]] VECTOR GetWorldPos			()const;
	[[nodiscard]] MATRIX GetLocalRotationMatrix	()const { return MGetRotElem(m_local_matrix); }
	[[nodiscard]] MATRIX GetWorldRotationMatrix	()const { return MGetRotElem(m_world_matrix); }
	[[nodiscard]] VECTOR GetLocalScale			()const;
	[[nodiscard]] VECTOR GetWorldScale			()const;

private:
	MATRIX m_local_matrix;
	std::shared_ptr<Transform> m_owner_transform;
};
