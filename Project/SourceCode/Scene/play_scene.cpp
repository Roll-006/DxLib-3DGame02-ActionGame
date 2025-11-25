#include "play_scene.hpp"
#include "../Manager/scene_manager.hpp"

PlayScene::PlayScene() :
	m_is_active						(true),
	m_scene_kind					(SceneKind::kPlay),
	m_elapsed_time					(0.0f),
	m_can_fade_in					(true),
	m_player						(std::make_shared<Player>()),
	m_enemy_manager					(std::make_shared<EnemyManager>()),
	m_houses						(std::make_shared<Houses>()),
	m_ground						(std::make_shared<Ground>()),
	//m_trees							(std::make_shared<Trees>()),
	m_skydome						(std::make_shared<Skydome>()),
	m_stealth_kill_target_searcher	(std::make_shared<StealthKillTargetSearcher>(m_player)),
	m_melee_target_searcher			(std::make_shared<MeleeTargetSearcher>(m_player)),
	m_item_creator					(std::make_shared<ItemCreator>(m_player)),
	m_rifle_cartridge_object_pool	(std::make_shared<RifleCartridgeObjectPool>()),
	m_play_scene_effect_object_pool (std::make_shared<PlaySceneEffectObjectPool>()),
	m_player_ui_creator				(std::make_shared<PlayerUICreator>(m_player)),
	m_game_clear_tab				(std::make_shared<GameClearTab>()),
	m_game_over_tab					(std::make_shared<GameOverTab>()),
	m_pause_tab						(std::make_shared<PauseTab>())
{
	m_player->AddToObjManager();
	m_ground->AddToObjManager();
	m_houses->AddToObjManager();

	m_enemy_manager->AttachTarget(m_player);

	const auto pool_holder = ObjectPoolHolder::GetInstance();
	pool_holder->AddObjectPool(m_rifle_cartridge_object_pool);
	pool_holder->AddObjectPool(m_play_scene_effect_object_pool);

	// カメラ設定
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveAllVirtualCameraController();
	cinemachine_brain->RemoveAllVirtualCamera();
	cinemachine_brain->AddVirtualCameraController(std::make_shared<ControlVirtualCamerasController>(*m_player.get()));

	// ライトの設定
	const auto light_holder = LightHolder::GetInstance();
	light_holder->CreateLight(std::make_shared<DirectionalLight>(LightName.MOONLIGHT, 0, v3d::GetNormalizedV(VGet(0.5f, -0.5f, 0.5f))));

	const auto game_time_manager = GameTimeManager::GetInstance();
	game_time_manager->InitTimeScale();

	TabDrawer::GetInstance()->AddTab		(m_game_clear_tab);
	TabDrawer::GetInstance()->AddTab		(m_game_over_tab);
	TabDrawer::GetInstance()->AddTab		(m_pause_tab);
	UIDrawer ::GetInstance()->AddUICreator	(m_player_ui_creator);
	UIDrawer ::GetInstance()->GetUICreator	(UICreatorName.SCREEN_FILTER_CREATOR)->Init();

	Init();
}

PlayScene::~PlayScene()
{
	m_item_creator = nullptr;

	m_player->Init();
	m_enemy_manager->Init();

	m_player->RemoveToObjManager();
	m_ground->RemoveToObjManager();
	m_houses->RemoveToObjManager();

	m_enemy_manager->DetachTarget();
	m_enemy_manager = nullptr;

	const auto pool_holder = ObjectPoolHolder::GetInstance();
	pool_holder->RemoveObjectPool(m_rifle_cartridge_object_pool	 ->GetName());
	pool_holder->RemoveObjectPool(m_play_scene_effect_object_pool->GetName());

	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCameraController(VirtualCameraControllerKind::kControl);

	// ライトの削除
	const auto light_holder = LightHolder::GetInstance();
	light_holder->DeleteLight(LightName.MOONLIGHT);

	TabDrawer::GetInstance()->RemoveTab			(m_game_clear_tab	->GetTabHandle());
	TabDrawer::GetInstance()->RemoveTab			(m_game_over_tab	->GetTabHandle());
	TabDrawer::GetInstance()->RemoveTab			(m_pause_tab		->GetTabHandle());
	UIDrawer ::GetInstance()->RemoveUICreator	(m_player_ui_creator->GetName());
}

void PlayScene::Init()
{
	const auto game_time_manager = GameTimeManager::GetInstance();
	game_time_manager->InitTimeScale();

	// カメラ初期化
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->SetNear(10.0f);
	cinemachine_brain->SetFar (2000.0f);
	cinemachine_brain->SetFOV (25.0f);

	m_player_ui_creator->Init();
}

void PlayScene::Update()
{
	StartFadeIn();

	if (SceneFader::GetInstance()->GetAlphaBlendNum() >= UCHAR_MAX) { return; }

	m_player							->Update();
	m_enemy_manager						->Update();
	RifleCartridgeManager::GetInstance()->Update();
	m_ground							->Update();
	m_houses							->Update();
	m_skydome							->Update();
	m_stealth_kill_target_searcher		->Update();
	m_melee_target_searcher				->Update();
	m_item_creator						->Update();
	m_game_clear_tab					->Update();
	m_game_over_tab						->Update();
	m_pause_tab							->Update();
}

void PlayScene::LateUpdate()
{
	if (SceneFader::GetInstance()->GetAlphaBlendNum() >= UCHAR_MAX) { return; }

	m_player							->LateUpdate();
	m_enemy_manager						->LateUpdate();
	RifleCartridgeManager::GetInstance()->LateUpdate();
	m_ground							->LateUpdate();
	m_houses							->LateUpdate();
	m_skydome							->LateUpdate();
	m_item_creator						->LateUpdate();
	m_player_ui_creator					->LateUpdate();
}

void PlayScene::DrawToShadowMap() const
{
	if (SceneFader::GetInstance()->GetAlphaBlendNum() >= UCHAR_MAX) { return; }

	m_player							->Draw();
	m_enemy_manager						->Draw();
	RifleCartridgeManager::GetInstance()->Draw();
	m_ground							->Draw();
	m_houses							->Draw();
	m_item_creator						->Draw();
}

void PlayScene::Draw() const
{
	if (SceneFader::GetInstance()->GetAlphaBlendNum() >= UCHAR_MAX) { return; }

	m_player							->Draw();
	m_enemy_manager						->Draw();
	RifleCartridgeManager::GetInstance()->Draw();
	m_ground							->Draw();
	m_houses							->Draw();
	m_skydome							->Draw();
	m_item_creator						->Draw();
}

std::shared_ptr<IScene> PlayScene::ChangeScene()
{
	// タイトル
	if (m_game_over_tab->IsQuitGame() || m_pause_tab->IsQuitGame() || m_game_clear_tab->IsQuitGame())
	{
		return std::make_shared<TitleScene>();
	}
	// ロード(プレイ)
	if (m_game_over_tab->IsContinue() || m_pause_tab->IsRestart()  || m_game_clear_tab->IsRetry())
	{
		return std::make_shared<LoadScene>(SceneKind::kPlay);
	}

	return nullptr;
}

void PlayScene::StartFadeIn()
{
	if (!m_can_fade_in) { return; }

	m_elapsed_time += GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kNoneScale);
	if (m_elapsed_time > 25.0f)
	{
		const auto fader = SceneFader::GetInstance();
		fader->StartFade(0, 120.0f);
		m_can_fade_in = false;
	}
}
