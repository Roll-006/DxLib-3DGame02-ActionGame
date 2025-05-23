#pragma once
#include <memory>
#include "../Calculation/math.hpp"

class Transform final
{
public:
	enum class CoordinateKind
	{
		kLocal,
		kWorld,
	};

public:
	Transform(const VECTOR& pos);
	Transform();
	~Transform();

	void Move(const VECTOR& velocity);

	void SetOwnerTransform(std::shared_ptr<Transform> owner);

	void SetPos(const VECTOR& pos);
	//void SetRotatetion();
	void SetScale(const VECTOR& scale);

	[[nodiscard]] MATRIX	 GetMatrix()const { return m_matrix; }
	[[nodiscard]] VECTOR	 GetPos()const;
	//[[nodiscard]] Quaternion GetRotationQuaternion()const;
	[[nodiscard]] MATRIX	 GetRotationMatrix()const { return MGetRotElem(m_matrix); }
	[[nodiscard]] VECTOR	 GetScale()const;

	//[[nodiscard]] VECTOR	 GetRotationVector()const;
	//[[nodiscard]] Quaternion GetRotateQuaternion(const VECTOR& axis, float angle)const;
	//[[nodiscard]] Quaternion GetPosQuaternion(const VECTOR& pos)const;

private:
	MATRIX m_matrix;
};
