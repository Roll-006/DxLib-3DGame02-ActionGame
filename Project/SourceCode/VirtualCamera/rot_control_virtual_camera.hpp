#pragma once
#include "../Base/control_virtual_camera_base.hpp"

/// @brief 回転コントロールカメラ
/// @brief 対象の周りを回転する
class RotControlVirtualCamera final : public ControlVirtualCameraBase
{
public:
	RotControlVirtualCamera();
	~RotControlVirtualCamera()		override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw() const				override;

	#pragma region コマンド
	void MoveUp()					override;
	void MoveDown()					override;
	void MoveLeft()					override;
	void MoveRight()				override;
	void InitAim()					override;
	void InitYawAim()				override;
	#pragma endregion

private:
	void InitMove()					override;
	void Move()						override;

	void CalcMoveDirFromPad()		override;
	void CalcMoveDirFromMouse()		override;
	void CalcMoveDirFromCommand()	override;

	void CalcInputAngle()			override;
	void CalcInitAim()				override;

	//void CalcPos();

	//[[nodiscard]] MATRIX GetRotMatrix() override;
	//void InitMove();
	//void JudgeLookSameDirTarget();
	//void ApplyInvert();

private:
	static constexpr int	kPriority				= 10;		// バーチャルカメラの優先度

	static constexpr float  kMaxVerticalInputAngle	= 89.0f;
	static constexpr float  kMinVerticalInputAngle	= -89.0f;
	static constexpr float  kInitAngleSpeed			= 10.0f;
	static constexpr float  kInitYawSpeed			= 60.0f;
	static constexpr float  kInitAngleEndThreshold	= 0.01f;
	static constexpr float  kInitYawEndThreshold	= 0.3f;

	static constexpr float	kMoveSpeedWithStick		= 2.5f;
	static constexpr float	kMoveSpeedWithMouse		= 0.1f;
	static constexpr float	kMoveSpeedWithButton	= 2.0f;
};
