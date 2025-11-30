#include "item_creator.hpp"

ItemCreator::ItemCreator(const std::shared_ptr<Player>& player) :
	m_player(player)
{
	// ÉCÉxÉìÉgìoò^
	EventSystem::GetInstance()->Subscribe<DeadEnemyEvent>	(this, &ItemCreator::CreateDeadEnemyItem);
	EventSystem::GetInstance()->Subscribe<GotItemEvent>		(this, &ItemCreator::RemoveItem);
}

ItemCreator::~ItemCreator()
{
	// ÉCÉxÉìÉgÇÃìoò^âèú
	EventSystem::GetInstance()->Unsubscribe<DeadEnemyEvent>	(this, &ItemCreator::CreateDeadEnemyItem);
	EventSystem::GetInstance()->Unsubscribe<GotItemEvent>	(this, &ItemCreator::RemoveItem);

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
	std::shared_ptr<IItem> item = nullptr;

	// âºÇ≈íeÇã≠êßÇ≈ê∂ê¨
	
	if (event.enemy_id == "zombie0")
	{
		item = std::make_shared<RocketBombBox>(1);
	}
	else
	{
		const auto num = RandomGenerator::GetInstance()->GetRandClosedOpen(10, 30);
		item = std::make_shared<AssaultRifleAmmoBox>(num);
	}

	const auto obj = std::dynamic_pointer_cast<ObjBase>(item);
	obj->AddToObjManager();
	obj->GetTransform()		->SetPos(CoordinateKind::kWorld, hips_pos);
	item->GetItemTransform()->SetPos(CoordinateKind::kWorld, hips_pos);
	
	m_items.emplace_back(obj);
	EventSystem::GetInstance()->Publish(DropItemEvent(item->GetItemTransform(), obj->GetObjHandle(), item->GetItemKind()));
}

void ItemCreator::RemoveItem(const GotItemEvent& event)
{
	for (auto itr = m_items.begin(); itr != m_items.end(); )
	{
		if (itr->get()->GetObjHandle() == event.obj_handle)
		{
			EffectManager::GetInstance()->ForciblyReturnPoolEffect(event.obj_handle, ObjectPoolName.PLAY_SCENE_EFFECT_POOL);
			(*itr)->RemoveToObjManager();
			itr = m_items.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}
