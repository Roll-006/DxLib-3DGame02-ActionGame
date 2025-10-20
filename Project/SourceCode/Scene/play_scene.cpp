#include "play_scene.hpp"
#include "../Manager/scene_manager.hpp"

PlayScene::PlayScene() :
	m_is_active						(true),
	m_scene_kind					(SceneKind::kPlay),
	m_player						(std::make_shared<Player>()),
	m_enemy_manager					(std::make_shared<EnemyManager>()),
	m_house							(std::make_shared<House>()),
	m_ground						(std::make_shared<Ground>()),
	m_skydome						(std::make_shared<Skydome>(ObjManager::GetInstance()->GetObj<MainCamera>(ObjName.MAIN_CAMERA))),
	m_rifle_cartridge_object_pool	(std::make_shared<RifleCartridgeObjectPool>()),
	m_play_scene_effect_object_pool (std::make_shared<PlaySceneEffectObjectPool>()),
	m_player_ui_creator				(std::make_shared<PlayerUICreator>(m_player)),
	m_game_over_tab					(std::make_shared<GameOverTab>())
{
	m_player		->AddToObjManager();
	m_enemy_manager	->AddToObjManager();
	m_house			->AddToObjManager();
	m_ground		->AddToObjManager();

	m_enemy_manager->AttachTarget(m_player);

	const auto pool_holder = ObjectPoolHolder::GetInstance();
	pool_holder->AddObjectPool(m_rifle_cartridge_object_pool);
	pool_holder->AddObjectPool(m_play_scene_effect_object_pool);

	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->AddVirtualCameraController(std::make_shared<ControlVirtualCamerasController>(*m_player.get()));

	// ライトの設定
	const auto light_holder = LightHolder::GetInstance();
	light_holder->CreateLight(std::make_shared<DirectionalLight>(LightName.MOONLIGHT, 0, v3d::GetNormalizedV(VGet(0.5f, -0.5f, 0.5f))));

	const auto game_time_manager = GameTimeManager::GetInstance();
	game_time_manager->InitTimeScale();

	TabDrawer::GetInstance()->AddTab		(m_game_over_tab);
	UIDrawer ::GetInstance()->AddUICreator	(m_player_ui_creator);

	Init();
}

PlayScene::~PlayScene()
{
	m_player		->RemoveToObjManager();
	m_enemy_manager	->RemoveToObjManager();
	m_house			->RemoveToObjManager();
	m_ground		->RemoveToObjManager();

	const auto pool_holder = ObjectPoolHolder::GetInstance();
	pool_holder->RemoveObjectPool(m_rifle_cartridge_object_pool	 ->GetName());
	pool_holder->RemoveObjectPool(m_play_scene_effect_object_pool->GetName());

	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCameraController(VirtualCameraControllerKind::kControl);

	// ライトの削除
	const auto light_holder = LightHolder::GetInstance();
	light_holder->DeleteLight(LightName.MOONLIGHT);

	TabDrawer::GetInstance()->RemoveTab			(typeid(GameOverTab));
	UIDrawer ::GetInstance()->RemoveUICreator	(m_player_ui_creator->GetName());
}

void PlayScene::Init()
{
	const auto game_time_manager = GameTimeManager::GetInstance();
	game_time_manager->InitTimeScale();

	// カメラ初期化
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->SetNear(10.0f);
	cinemachine_brain->SetFar (4000.0f);
	cinemachine_brain->SetFOV (25.0f);

	m_player_ui_creator->Init();
}

void PlayScene::Update()
{
	m_player							->Update();
	m_enemy_manager						->Update();
	RifleCartridgeManager::GetInstance()->Update();
	m_house								->Update();
	m_ground							->Update();
	m_skydome							->Update();
	m_game_over_tab						->Update();
}

void PlayScene::LateUpdate()
{
	m_player							->LateUpdate();
	m_enemy_manager						->LateUpdate();
	RifleCartridgeManager::GetInstance()->LateUpdate();
	m_house								->LateUpdate();
	m_ground							->LateUpdate();
	m_skydome							->LateUpdate();
	m_player_ui_creator					->LateUpdate();
}

void PlayScene::DrawToShadowMap() const
{
	m_player							->DrawToShadowMap();
	m_enemy_manager						->DrawToShadowMap();
	RifleCartridgeManager::GetInstance()->DrawToShadowMap();
	m_house								->DrawToShadowMap();
	m_ground							->DrawToShadowMap();
}

void PlayScene::Draw() const
{
	m_player							->Draw();
	m_enemy_manager						->Draw();
	RifleCartridgeManager::GetInstance()->Draw();
	m_house								->Draw();
	m_ground							->Draw();
	m_skydome							->Draw();

	DrawFormatString(0, 40, 0xffffff, "Current Scene : PLAY");
	DrawFormatString(0, 60, 0xffffff, "TAB / Start でタイトルに戻る");
}

std::shared_ptr<IScene> PlayScene::ChangeScene()
{
	const auto command = CommandHandler::GetInstance();

	// タイトル
	if (command->IsExecute(CommandKind::kPause, TimeKind::kCurrent))
	{
		return std::make_shared<TitleScene>();
	}

	return nullptr;
}
