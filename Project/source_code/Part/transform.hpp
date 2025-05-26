#pragma once
#include <string>
#include <memory>

#include "../Data/Kind/coordinate_kind.hpp"
#include "../Calculation/math.hpp"

class Transform final
{
public:
	Transform(const VECTOR& local_pos);
	Transform();
	~Transform();

	void Move(const CoordinateKind coord_kind, const VECTOR& velocity);

	void AttachParent(const std::shared_ptr<Transform> parent_transform);
	void AttachParent(const std::string& parent_obj_name);
	void DetachParent();

	void SetPos		(const CoordinateKind coord_kind, const VECTOR&		pos);
	void SetRotation(const CoordinateKind coord_kind, const MATRIX&		rotation_matrix);
	void SetRotation(const CoordinateKind coord_kind, const Quaternion& quaternion);
	void SetScale	(const CoordinateKind coord_kind, const VECTOR&		scale);

	[[nodiscard]] MATRIX GetMatrix			(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR GetPos				(const CoordinateKind coord_kind);
	[[nodiscard]] MATRIX GetRotationMatrix	(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR GetScale			(const CoordinateKind coord_kind);

	/// @brief 親がアタッチされているかを判定
	[[nodiscard]] bool HasParent() { return m_parent_transform ? true : false; }

private:
	MATRIX m_local_matrix;
	std::shared_ptr<Transform> m_parent_transform;
};
