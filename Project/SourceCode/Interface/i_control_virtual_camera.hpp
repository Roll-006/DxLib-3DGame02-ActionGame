#pragma once
#include "../Data/control_virtual_camera_data.hpp"
#include "../Kind/input_kind.hpp"

class IControlVirtualCamera abstract
{
public:
	virtual ~IControlVirtualCamera() = default;

	#pragma region コマンド
	//virtual void MoveUp()		abstract;
	//virtual void MoveDown()	abstract;
	//virtual void MoveLeft()	abstract;
	//virtual void MoveRight()	abstract;
	virtual void InitAim()		abstract;
	virtual void InitYawAim()	abstract;
	#pragma endregion

	/// @brief 視点リセット中かを取得
	[[nodiscard]] virtual bool IsInitAiming() const abstract;

protected:
	virtual void InitMove()					abstract;
	virtual void Move()						abstract;

	virtual void CalcMoveDirFromPad()		abstract;
	virtual void CalcMoveDirFromMouse()		abstract;
	virtual void CalcMoveDirFromCommand()	abstract;

	/// @brief 入力角度の計算を行う
	virtual void CalcInputAngle()			abstract;
	/// @brief 視点リセットの計算を行う
	virtual void CalcInitAim()				abstract;
};
