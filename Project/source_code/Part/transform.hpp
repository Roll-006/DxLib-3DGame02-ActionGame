#pragma once
#include <string>
#include <memory>

#include "../Data/Kind/coordinate_kind.hpp"
#include "../Calculation/math.hpp"

class Transform final
{
public:
	Transform();
	~Transform();

	void Move(const CoordinateKind coord_kind, const VECTOR& velocity);


	#pragma region 親のアタッチ/デタッチ
	void AttachParent(const std::shared_ptr<Transform> parent_transform);
	void AttachParent(const std::string& parent_obj_name);
	void DetachParent();
	#pragma endregion


	#pragma region setter
	void SetMatrix	(const CoordinateKind coord_kind, const MATRIX& matrix);
	void SetPos		(const CoordinateKind coord_kind, const VECTOR&	pos);
	void SetRot		(const CoordinateKind coord_kind, const MATRIX&	rot_matrix);
	void SetRot		(const CoordinateKind coord_kind, const Axes&	axes);

	/// @brief 回転行列を設定
	/// @brief キャラクターの水平な回転しか考慮していない
	/// @param dir Y軸が0である必要あり
	void SetRot		(const CoordinateKind coord_kind, const VECTOR& dir);

	void SetScale	(const CoordinateKind coord_kind, const VECTOR&	scale);
	void SetScale	(const CoordinateKind coord_kind, const float	scale);
	#pragma endregion


	#pragma region getter
	[[nodiscard]] MATRIX GetMatrix			(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR GetPos				(const CoordinateKind coord_kind);
	[[nodiscard]] MATRIX GetRotMatrix		(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR GetScale			(const CoordinateKind coord_kind);
	[[nodiscard]] Axes   GetAxes			(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR GetRight			(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR GetUp				(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR GetForward			(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR GetEulerAngles		(const CoordinateKind coord_kind);
	#pragma endregion


	/// @brief 親がアタッチされているかを判定
	[[nodiscard]] bool HasParent() const { return m_parent_transform != nullptr; }

private:
	MATRIX m_local_matrix;
	std::shared_ptr<Transform> m_parent_transform;
};
