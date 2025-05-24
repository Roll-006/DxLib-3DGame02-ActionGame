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

	void SetPos		(const CoordinateKind coord_kind, const VECTOR& pos);
	void SetRotation(const CoordinateKind coord_kind, const MATRIX& rotation_matrix);
	void SetScale	(const CoordinateKind coord_kind, const VECTOR& scale);

	/// @brief 親とするトランスフォームをアタッチする
	void AttachParent(const std::shared_ptr<Transform> parent);
	void AttachParent(const std::string& obj_name);
	/// @brief 親とするトランスフォームをデタッチする
	void DetachParent();

	/// @brief 親がアタッチされているかを判定
	[[nodiscard]] bool HasParent() { return m_parent_transform ? true : false; }

	[[nodiscard]] MATRIX GetMatrix			(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR GetPos				(const CoordinateKind coord_kind);
	[[nodiscard]] MATRIX GetRotationMatrix	(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR GetScale			(const CoordinateKind coord_kind);

private:
	MATRIX m_local_matrix;
	std::shared_ptr<Transform> m_parent_transform;
};
