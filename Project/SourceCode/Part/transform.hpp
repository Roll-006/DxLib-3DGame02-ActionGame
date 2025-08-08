#pragma once
#include <string>
#include <memory>

#include "../Data/Kind/coordinate_kind.hpp"
#include "../Calculation/math.hpp"

class Transform final
{
public:
	Transform();
	Transform(Transform& transform);
	Transform(Transform* transform);
	~Transform();

	void Move(const CoordinateKind coord_kind, const VECTOR& velocity);


	#pragma region 親のアタッチ/デタッチ
	void AttachParent(const std::shared_ptr<Transform> parent_transform);
	void AttachParent(const std::string& parent_obj_name);
	void DetachParent();
	#pragma endregion


	#pragma region Setter
	void SetMatrix	(const CoordinateKind coord_kind, const MATRIX&		matrix);
	void SetPos		(const CoordinateKind coord_kind, const VECTOR&		pos);
	void SetRot		(const CoordinateKind coord_kind, const MATRIX&		rot_matrix);
	void SetRot		(const CoordinateKind coord_kind, const Quaternion& quaternion);

	/// @brief forwardから回転を設定
	/// @biref WARNING : forwardがワールドY軸に平行であった場合は回転が失われる
	void SetRot		(const CoordinateKind coord_kind, const VECTOR&		forward);

	void SetScale	(const CoordinateKind coord_kind, const VECTOR&		scale);
	void SetScale	(const CoordinateKind coord_kind, const float		scale);
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] MATRIX	 GetMatrix		(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR	 GetPos			(const CoordinateKind coord_kind);
	[[nodiscard]] MATRIX	 GetRotMatrix	(const CoordinateKind coord_kind);
	[[nodiscard]] Quaternion GetQuaternion	(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR	 GetScale		(const CoordinateKind coord_kind);
	[[nodiscard]] MATRIX	 GetScaleMatrix	(const CoordinateKind coord_kind);
	[[nodiscard]] Axes		 GetAxes		(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR	 GetRight		(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR	 GetUp			(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR	 GetForward		(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR	 GetEulerAngles	(const CoordinateKind coord_kind);
	#pragma endregion


	/// @brief 親がアタッチされているかを判定
	[[nodiscard]] bool HasParent() const { return m_parent_transform != nullptr; }

private:
	MATRIX m_local_matrix;
	std::shared_ptr<Transform> m_parent_transform;
};
