#pragma once
#include "../Base/component_base.hpp"

#include "../Math/math.hpp"

class TransformComponent : public ComponentBase
{
public:
	TransformComponent(GameObj* owner_obj, const VECTOR& pos);
	~TransformComponent()override;

	void Init()		 override;
	void Update()	 override;
	void LateUpdate()override;
	void Draw()const override;

	void Move(const VECTOR& velocity);

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
