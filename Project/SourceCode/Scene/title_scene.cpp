#include "title_scene.hpp"
#include "../Manager/scene_manager.hpp"

TitleScene::TitleScene() : 
	m_is_active						(true),
	m_scene_kind					(SceneKind::kTitle),
	m_title_character				(std::make_shared<TitleCharacter>()),
	m_title_tab						(std::make_shared<TitleTab>()),
	m_warning_tab					(std::make_shared<WarningTab>()),
	m_title_scene_effect_object_pool(std::make_shared<TitleSceneEffectObjectPool>()),

	m_title_camera					(std::make_shared<VirtualCamera>(ObjName.TITLE_CAMERA, BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_aim_transform					(std::make_shared<Transform>()),
	m_smoke_transform				(std::make_shared<Transform>()),
	m_sin							(90.0f * math::kDegToRad)
{
	// マネージャー登録
	m_title_character->AddToObjManager();

	const auto pool_holder = ObjectPoolHolder::GetInstance();
	pool_holder->AddObjectPool(m_title_scene_effect_object_pool);

	// カメラの設定
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->AddVirtualCamera(m_title_camera, true);
	m_title_camera->SetPriority(0);
	m_title_camera->AttachTarget(m_aim_transform);
	const auto body = m_title_camera->GetBody();
	body->SetFollowOffset({ 5.0f, 5.0f, -30.0f });
	const auto aim = m_title_camera->GetAim();
	aim->SetTrackedObjOffset({ -3.0f, 0.0f, 0.0f });

	// タブの登録
	TabDrawer::GetInstance()->AddTab(m_title_tab);
	TabDrawer::GetInstance()->AddTab(m_warning_tab);

	// タイトルシーンに入ったことを通知
	const OnChangeTitleSceneEvent event{ m_smoke_transform };
	EventSystem::GetInstance()->Publish(event);

	Init();
}

TitleScene::~TitleScene()
{
	m_title_character->RemoveToObjManager();

	const auto pool_holder = ObjectPoolHolder::GetInstance();
	pool_holder->RemoveObjectPool(m_title_scene_effect_object_pool->GetName());

	// タブの登録を解除
	TabDrawer::GetInstance()->RemoveTab(typeid(TitleTab));
	TabDrawer::GetInstance()->RemoveTab(typeid(WarningTab));

	// カメラの登録を解除
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCamera(m_title_camera->GetCameraHandle());
}

void TitleScene::Init()
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->SetNear(1.0f);
	cinemachine_brain->SetFar (100.0f);
	cinemachine_brain->SetFOV (25.0f);
}

void TitleScene::Update()
{
	m_title_tab			->Update();
	m_warning_tab		->Update();
	m_title_character	->Update();

	if (m_title_tab->IsActivateWarningTab())
	{
		m_warning_tab	->Activate();
		m_title_tab		->StopSelect();
		m_title_tab		->InitActivateWarningTab();
	}

	if (m_warning_tab->IsBack())
	{
		m_warning_tab	->InitBack();
		m_warning_tab	->Deactivate();
		m_title_tab		->AllowSelect();
	}

	// 基準となるトランスフォームを設定
	const auto model_handle = m_title_character->GetModeler()->GetModelHandle();
	auto	   spine2_m		= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.SPINE_2));
	const auto spine2_pos	= MGetTranslateElem(spine2_m);
	const auto spine2_axes	= math::ConvertRotMatrixToAxes(spine2_m);
	m_aim_transform->SetPos(CoordinateKind::kWorld, spine2_pos + spine2_axes.y_axis * 13.0f);
	m_aim_transform->SetRot(CoordinateKind::kWorld, spine2_axes.z_axis);

	// エフェクト
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	math::Increase(m_sin, 0.1f * delta_time, DX_PI_F, true);
	const auto num = sin(m_sin);
	const auto deg = math::ConvertValueNewRange<float, float>(0.0f, 1.0f, -60.0f, -30.0f, num);
	MATRIX rot_m = MGetIdent();
	CreateRotationXYZMatrix(&rot_m, 0.0f, -90.0f * math::kDegToRad, deg * math::kDegToRad);
	m_smoke_transform->SetPos(CoordinateKind::kWorld, VGet(40, 25, 0));
	m_smoke_transform->SetRot(CoordinateKind::kWorld, MGetRotElem(rot_m));
}

void TitleScene::LateUpdate()
{

}

void TitleScene::DrawToShadowMap() const
{
	m_title_character->DrawToShadowMap();
}

void TitleScene::Draw() const
{
	m_title_character->Draw();

	DrawFormatString(0, 40, 0xffffff, "Current Scene : TITLE");
	DrawFormatString(0, 60, 0xffffff, "SPACE / A でゲームスタート");
}

std::shared_ptr<IScene> TitleScene::ChangeScene()
{
	const auto command = CommandHandler::GetInstance();

	// プレイ
	if (m_title_tab->IsGameStart())
	{
		return std::make_shared<PlayScene>();
	}

	return nullptr;
}
