//#pragma once
//#include "../Base/virtual_camera_base.hpp"
//#include "../Interface/i_control_virtual_camera.hpp"
//
///// @brief 回転コントロールカメラ
//class RotControlVirtualCamera final : public VirtualCamera, public IControlVirtualCamera
//{
//public:
//	RotControlVirtualCamera();
//	~RotControlVirtualCamera()		override;
//
//	void Init()						override;
//	void Update()					override;
//	void LateUpdate()				override;
//	void DrawToShadowMap()	const	override;
//	void Draw()				const	override;
//
//
//	#pragma region コマンド
//	//void MoveUp()					override;
//	//void MoveDown()				override;
//	//void MoveLeft()				override;
//	//void MoveRight()				override;
//	void InitAim()					override;
//	void InitYawAim()				override;
//	#pragma endregion
//
//
//	[[nodiscard]] bool IsInitAiming() const override { return m_data.is_init_aiming; }
//
//private:
//	void InitMove()					override;
//	void Move()						override;
//
//	void CalcMoveDirFromPad()		override;
//	void CalcMoveDirFromMouse()		override;
//	void CalcMoveDirFromCommand()	override;
//
//	void CalcInputAngle()			override;
//	void CalcInitAim()				override;
//
//private:
//	static constexpr float  kMaxVerticalInputAngle	= 60.0f;
//	static constexpr float  kMinVerticalInputAngle	= -60.0f;
//	static constexpr float  kInitAngleSpeed			= 10.0f;
//	static constexpr float  kInitYawSpeed			= 60.0f;
//	static constexpr float  kInitAngleEndThreshold	= 0.02f;
//	static constexpr float  kInitYawEndThreshold	= 0.3f;
//
//	static constexpr float	kMoveSpeedWithStick		= 2.5f;
//	static constexpr float	kMoveSpeedWithMouse		= 0.1f;
//	static constexpr float	kMoveSpeedWithButton	= 2.0f;
//
//	ControlVirtualCameraData m_data;
//};
