//#include "grab_virtual_camera_controller.hpp"
//#include "../VirtualCamera/cinemachine_brain.hpp"
//#include "../Object/player.hpp"
//
//GrabVirtualCameraController::GrabVirtualCameraController(Player& player) :
//	m_virtual_camera_controller_kind(VirtualCameraControllerKind::kRocketLauncherCutscene),
//	m_controller_handle				(HandleCreator::GetInstance()->CreateHandle()),
//	m_is_active						(true),
//	m_player						(player),
//	//m_subject						(std::make_shared<Subject<GrabVirtualCameraController>>()),
//	m_camera						(std::make_shared<VirtualCamera>(ObjName.ROCKET_LAUNCHER_ENTER_ROT_VIRTUAL_CAMERA,	BlendActivationPolicyKind::kDeactivateAllCamera)),
//	m_aim_transform					(std::make_shared<Transform>())
//{
//	// パラメータ設定
//	SetupCamera();
//
//	const auto cinemachine_brain = CinemachineBrain::GetInstance();
//
//	const auto control_camera = cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl);
//	control_camera->Deactivate();
//	cinemachine_brain->SetBlendTime(1.0f);
//	cinemachine_brain->AddVirtualCamera(m_camera, true);
//
//	//// オブザーバー登録
//	//const auto screen_filter = UIDrawer::GetInstance()->GetUICreator(UICreatorName.SCREEN_FILTER_CREATOR);
//	//m_subject->AddObserver(std::dynamic_pointer_cast<IObserver>(screen_filter));
//	//GameTimeManager::GetInstance()->GetTimeScaleController()->AddToSubject(m_subject);
//}
//
//GrabVirtualCameraController::~GrabVirtualCameraController()
//{
//	const auto cinemachine_brain = CinemachineBrain::GetInstance();
//	cinemachine_brain->RemoveVirtualCamera(m_camera->GetObjHandle());
//}
//
//void GrabVirtualCameraController::Init()
//{
//	
//}
//
//void GrabVirtualCameraController::Update()
//{
//	if (!IsActive()) { return; }
//}
//
//void GrabVirtualCameraController::LateUpdate()
//{
//	if (!IsActive()) { return; }
//
//	m_player.GetModeler()->ApplyMatrix();
//
//	CalcAimTransform();
//}
//
//VirtualCameraControllerKind GrabVirtualCameraController::GetVirtualCameraControllerKind() const
//{
//	return m_virtual_camera_controller_kind;
//}
//
//std::shared_ptr<VirtualCameraBase> GrabVirtualCameraController::GetHaveVirtualCamera(const std::string& name) const
//{
//	const auto cinemachine_brain = CinemachineBrain::GetInstance();
//	const auto camera = cinemachine_brain->GetVirtualCamera(name);
//
//	if (camera == m_camera)
//	{
//		return camera;
//	}
//
//	return nullptr;
//}
//
//std::vector<std::shared_ptr<VirtualCameraBase>> GrabVirtualCameraController::GetHaveAllVirtualCamera() const
//{
//	return std::vector<std::shared_ptr<VirtualCameraBase>>{ m_camera };
//}
//
//
//#pragma region カメラ設定
//void GrabVirtualCameraController::SetupCamera()
//{
//	m_camera->SetPriority(8);
//	m_camera->AttachTarget(m_aim_transform);
//	m_camera->GetBody()->SetFollowOffset(kFollowOffset);
//	m_camera->GetAim()->SetTrackedObjOffset(kTrackedObjOffset);
//}
//#pragma endregion
//
//
//#pragma region 起点トランスフォームの計算
//void GrabVirtualCameraController::CalcAimTransform()
//{
//	if (!m_camera->IsActive()) { return; }
//
//	// TODO : 後に弾丸そのものを追尾するよう変更
//	// 追跡するボーンから行列を取得
//	const auto	model_handle	= m_player.GetModeler()->GetModelHandle();
//	const auto	hand_index		= MV1SearchFrame(model_handle, BonePath.RIGHT_HAND);
//	auto		hand_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, hand_index);
//	const auto	aim_pos			= MGetTranslateElem(hand_world_m);
//	const auto  offset_rot		= math::ConvertEulerAnglesToXYZRotMatrix(VGet(-90.0f * math::kDegToRad, -90.0f * math::kDegToRad, 0.0f));
//	auto		aim_rot			= math::ConvertEulerAnglesToXYZRotMatrix(m_rot_camera_angle) * offset_rot * MGetRotElem(hand_world_m);
//
//	// カメラの追跡対象となるトランスフォームの情報を更新
//	m_rot_camera_aim_transform->SetRot(CoordinateKind::kWorld, aim_rot);
//	m_rot_camera_aim_transform->SetPos(CoordinateKind::kWorld, aim_pos);
//
//	// 回転量を計算
//	const float acc = kEnterRotAcceleration * m_enter_rot_camera->GetDeltaTime();
//	math::Decrease(m_rot_camera_angle.y, acc, -DX_PI_F);
//}
//#pragma endregion
