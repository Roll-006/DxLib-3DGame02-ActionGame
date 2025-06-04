#pragma once
#include <string>
#include <memory>

#include "../Data/Kind/coordinate_kind.hpp"
#include "../Calculation/math.hpp"

#include "../FPS/fps.hpp"

class Transform final
{
public:
	Transform();
	~Transform();

	void Move(const CoordinateKind coord_kind, const VECTOR& velocity);

	void AttachParent(const std::shared_ptr<Transform> parent_transform);
	void AttachParent(const std::string& parent_obj_name);
	void DetachParent();

	void SetMatrix	(const CoordinateKind coord_kind, const MATRIX&	matrix);
	void SetPos		(const CoordinateKind coord_kind, const VECTOR&	pos);
	void SetRot		(const CoordinateKind coord_kind, const MATRIX&	rotation_matrix);
	void SetRot		(const CoordinateKind coord_kind, const VECTOR& forward);
	void SetRot		(const CoordinateKind coord_kind, const Axes&	axes);
	void SetScale	(const CoordinateKind coord_kind, const VECTOR&	scale);

	[[nodiscard]] MATRIX GetMatrix			(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR GetPos				(const CoordinateKind coord_kind);
	[[nodiscard]] MATRIX GetRotationMatrix	(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR GetScale			(const CoordinateKind coord_kind);
	[[nodiscard]] Axes   GetAxes			(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR GetRight			(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR GetUp				(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR GetForward			(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR GetEulerAngles		(const CoordinateKind coord_kind);

	/// @brief 親がアタッチされているかを判定
	[[nodiscard]] bool HasParent() { return m_parent_transform != nullptr; }

private:
	MATRIX m_local_matrix;
	std::shared_ptr<Transform> m_parent_transform;
};
