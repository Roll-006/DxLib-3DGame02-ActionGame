#include "item_acquisition_searcher.hpp"

ItemAcquisitionSearcher::ItemAcquisitionSearcher(const std::weak_ptr<IItemCollectable>& item_collectable) :
	m_item_collectable(item_collectable)
{

}

ItemAcquisitionSearcher::~ItemAcquisitionSearcher()
{

}

void ItemAcquisitionSearcher::Update()
{
	// 追加が許可されてない場合はすべて削除して早期return
	if (!m_item_collectable.lock()->CanAddAcquirableItem())
	{
		m_item_collectable.lock()->RemoveGetCandidateItems();
		return;
	}

	m_item_collectable.lock()->RemoveAcquirableItem();

	SearchItemAcquisition();

	m_item_collectable.lock()->RemoveGetCandidateItems();
}

void ItemAcquisitionSearcher::SearchItemAcquisition()
{
	if (m_item_collectable.lock()->GetCandidateItems().empty()) { return; }

	std::vector<SpottedObjData> distance;
	std::vector<SpottedObjData> angle;

	for (const auto& candidate : m_item_collectable.lock()->GetCandidateItems())
	{
		if (candidate.camera_diff_angle < kPrioritySwitchAngle * math::kDegToRad)
		{
			distance.emplace_back(candidate);
		}
		else
		{
			angle.emplace_back(candidate);
		}
	}

	// 距離でソート
	if (!distance.empty())
	{
		std::ranges::sort(distance,	{}, &SpottedObjData::distance_to_camera);
		const auto candidate_obj = ObjManager::GetInstance()->GetObj<ObjBase>(distance.front().target_obj_handle);
		const auto target = std::dynamic_pointer_cast<IItem>(candidate_obj);
		m_item_collectable.lock()->AddAcquirableItem(target);
		return;
	}

	// 角度でソート
	std::ranges::sort(angle, {}, &SpottedObjData::camera_diff_angle);
	const auto candidate_obj = ObjManager::GetInstance()->GetObj<ObjBase>(angle.front().target_obj_handle);
	const auto target = std::dynamic_pointer_cast<IItem>(candidate_obj);
	m_item_collectable.lock()->AddAcquirableItem(target);
}
