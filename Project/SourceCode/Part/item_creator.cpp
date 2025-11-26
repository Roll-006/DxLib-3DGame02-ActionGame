#include "item_creator.hpp"

ItemCreator::ItemCreator(const std::shared_ptr<Player>& player) :
	m_player(player)
{
	// ƒCƒxƒ“ƒg“o˜^
	EventSystem::GetInstance()->Subscribe<DeadEnemyEvent>(this, &ItemCreator::CreateDeadEnemyItem);
}

ItemCreator::~ItemCreator()
{
	// ƒCƒxƒ“ƒg‚Ì“o˜^‰ğœ
	EventSystem::GetInstance()->Unsubscribe<DeadEnemyEvent>(this, &ItemCreator::CreateDeadEnemyItem);

	const auto effect_manager = EffectManager::GetInstance();
	for (const auto& item : m_items)
	{
		effect_manager->ForciblyReturnPoolEffect(item->GetObjHandle(), ObjectPoolName.PLAY_SCENE_EFFECT_POOL);
		item->RemoveToObjManager();
	}
	m_items.clear();
}

void ItemCreator::Init()
{
	for (const auto& item : m_items)
	{
		item->Init();
	}
}

void ItemCreator::Update()
{
	for (const auto& item : m_items)
	{
		item->Update();
	}
}

void ItemCreator::LateUpdate()
{
	for (const auto& item : m_items)
	{
		item->LateUpdate();
	}
}

void ItemCreator::Draw() const
{
	for (const auto& item : m_items)
	{
		item->Draw();
	}
}

void ItemCreator::CreateDeadEnemyItem(const DeadEnemyEvent& event)
{
	auto	   hips_m	= MV1GetFrameLocalWorldMatrix(event.model_handle, MV1SearchFrame(event.model_handle, FramePath.HIPS));
	const auto hips_pos	= MGetTranslateElem(hips_m);

	// ‰¼‚Å’e‚ğ‹­§‚Å¶¬
	// TODO : ‚Ì‚¿‚É•ÏX
	const auto item	= std::make_shared<AssaultRifleAmmoBox>(40);
	item->AddToObjManager();
	item->GetTransform()	->SetPos(CoordinateKind::kWorld, hips_pos);
	item->GetItemTransform()->SetPos(CoordinateKind::kWorld, hips_pos);
	m_items.emplace_back(item);

	EventSystem::GetInstance()->Publish(DropItemEvent(item->GetItemTransform(), item->GetObjHandle(), item->GetItemKind()));
}
