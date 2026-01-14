#include "title_scene.hpp"
#include "../Manager/scene_manager.hpp"

TitleScene::TitleScene() : 
	m_is_active						(true),
	m_scene_kind					(SceneKind::kTitle),
	m_title_character				(std::make_shared<TitleCharacter>()),
	m_title_tab						(std::make_shared<TitleTab>()),
	m_title_scene_effect_object_pool(std::make_shared<TitleSceneEffectObjectPool>()),

	m_title_camera					(std::make_shared<VirtualCamera>(ObjName.TITLE_CAMERA, BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_aim_transform					(std::make_shared<Transform>()),
	m_smoke_transform				(std::make_shared<Transform>()),
	m_sin							(90.0f * math::kDegToRad),
	m_smoke_delete_handle			(HandleCreator::GetInstance()->CreateHandle())
{
	// マネージャー登録
	m_title_character->AddToObjManager();

	const auto pool_holder = ObjectPoolHolder::GetInstance();
	pool_holder->AddObjectPool(m_title_scene_effect_object_pool);

	// TODO : JSON化
	
	// ライトの設定
	const auto light_holder			= LightHolder::GetInstance();
	const auto pos					= VGet(-2.6f, 46.0f, -13.f);
	const auto dir					= v3d::GetNormalizedV(VGet(0.0f, 1.5f, 1.0f));
	const auto angle				= 40.0f * math::kDegToRad;
	const auto attenuation_angle	= 17.0f * math::kDegToRad;
	const auto range_attenuation	= LightRangeAttenuationData(100.0f, 0.2f, 0.001f, 0.000001f);
	light_holder->CreateLight(std::make_shared<SpotLight>(LightName.TITLE_LIGHT, 0, pos, dir, angle, attenuation_angle, range_attenuation));

	// カメラの設定
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveAllVirtualCameraController();
	cinemachine_brain->RemoveAllVirtualCamera();
	cinemachine_brain->AddVirtualCamera(m_title_camera, true);
	m_title_camera->SetPriority(0);
	m_title_camera->AttachTarget(m_aim_transform);
	const auto body = m_title_camera->GetBody();
	body->SetFollowOffset({ 5.0f, 5.0f, -30.0f });
	const auto aim = m_title_camera->GetAim();
	aim->SetTrackedObjOffset({ -3.0f, 0.0f, 0.0f });

	TabDrawer::GetInstance()->AddTab(m_title_tab);
	UIDrawer ::GetInstance()->GetUICreator(UICreatorName.SCREEN_FILTER_CREATOR)->Init();

	// タイトルシーンに入ったことを通知
	const OnChangeTitleSceneEvent event{ m_smoke_transform, m_smoke_delete_handle };
	EventSystem::GetInstance()->Publish(event);

	const auto game_time_manager = GameTimeManager::GetInstance();
	game_time_manager->InitTimeScale();

	StartFadeIn();
	Init();
}

TitleScene::~TitleScene()
{
	m_title_character->RemoveToObjManager();

	// エフェクトの登録を解除
	EffectManager::GetInstance()->ForciblyReturnPoolEffect(m_smoke_delete_handle, ObjectPoolName.TITLE_SCENE_EFFECT_POOL);
	const auto pool_holder = ObjectPoolHolder::GetInstance();
	pool_holder->RemoveObjectPool(m_title_scene_effect_object_pool->GetName());

	// ライトの削除
	const auto light_holder = LightHolder::GetInstance();
	light_holder->DeleteLight(LightName.TITLE_LIGHT);

	// タブの登録を解除
	TabDrawer::GetInstance()->RemoveTab(m_title_tab->GetTabHandle());

	// カメラの登録を解除
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->SetBlendTime(1.0f);
	cinemachine_brain->RemoveVirtualCamera(m_title_camera->GetCameraHandle());
}

void TitleScene::Init()
{
	// TODO : リファクタリング
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->SetBlendTime(0.0f);
	cinemachine_brain->SetNear(1.0f);
	cinemachine_brain->SetFar (50.0f);
	cinemachine_brain->SetFOV (25.0f);
}

void TitleScene::Update()
{
	m_title_tab			->Update();
	m_title_character	->Update();

	// TODO : 後に別クラス化

	// 基準となるトランスフォームを設定
	const auto model_handle = m_title_character->GetModeler()->GetModelHandle();
	auto	   spine2_m		= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, FramePath.SPINE_2));
	const auto spine2_pos	= matrix::GetPos(spine2_m);
	const auto spine2_axis	= math::ConvertRotMatrixToAxis(spine2_m);
	m_aim_transform->SetPos(CoordinateKind::kWorld, spine2_pos + spine2_axis.y_axis * 13.0f);
	m_aim_transform->SetRot(CoordinateKind::kWorld, spine2_axis.z_axis);

	// エフェクト
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	math::Increase(m_sin, 0.5f * delta_time, DX_PI_F, true);
	const auto num = sin(m_sin);
	const auto deg = math::ConvertValueNewRange<float, float>(0.0f, 1.0f, -60.0f, -30.0f, num);
	MATRIX rot_m = MGetIdent();
	CreateRotationXYZMatrix(&rot_m, 0.0f, -90.0f * math::kDegToRad, deg * math::kDegToRad);
	m_smoke_transform->SetPos(CoordinateKind::kWorld, VGet(20, 45, 0));
	m_smoke_transform->SetRot(CoordinateKind::kWorld, matrix::GetRotMatrix(rot_m));
}

void TitleScene::LateUpdate()
{

}

void TitleScene::DrawToShadowMap() const
{
	m_title_character->Draw();
}

void TitleScene::Draw() const
{
	m_title_character->Draw();
}

std::shared_ptr<IScene> TitleScene::ChangeScene()
{
	const auto fader = SceneFader::GetInstance();

	// ロード(プレイ)
	if (m_title_tab->IsGameStart())
	{
		return std::make_shared<LoadScene>(SceneKind::kPlay);
	}

	return nullptr;
}

void TitleScene::StartFadeIn()
{
	const auto fader = SceneFader::GetInstance();
	fader->StartFade(0, 70.0f);
}
