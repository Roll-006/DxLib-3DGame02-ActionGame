#include "character_collider_creator.hpp"

void CharacterColliderCreator::CreateCapsuleCollider(PhysicalObjBase* physical_obj, const std::shared_ptr<Modeler> modeler, const float capsule_radius)
{
	const auto transform		= physical_obj->GetTransform();
	const auto begin_pos		= transform->GetPos(CoordinateKind::kWorld) + VGet(0.0f, capsule_radius, 0.0f);
	const auto capsule_collider = std::make_shared<Capsule>(begin_pos, transform->GetUp(CoordinateKind::kWorld), 0.0f, capsule_radius);

	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, capsule_collider, physical_obj));

	CalcCapsuleColliderLength(physical_obj, modeler);
}

void CharacterColliderCreator::CreateLandingTrigger(PhysicalObjBase* physical_obj, const float sphere_radius)
{
	// TODO : カプセルのサイズの比率によってずらし量を自動で設定させるように変更
	const auto capsule		= std::static_pointer_cast<Capsule>(physical_obj->GetCollider(ColliderKind::kCollider)->GetShape());
	const auto pos			= capsule->GetSegment().GetBeginPos() - VGet(0.0f, 5.0f, 0.0f);

	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kLandingTrigger, std::make_shared<Sphere>(pos, sphere_radius), physical_obj));
}

void CharacterColliderCreator::CreateLegTrigger(PhysicalObjBase* physical_obj, const std::shared_ptr<Modeler> modeler, const float up_leg_capsule_radius, const float down_leg_capsule_radius)
{
	const auto left_up_leg_trigger		= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), up_leg_capsule_radius);
	const auto left_down_leg_trigger	= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), down_leg_capsule_radius);
	const auto right_up_leg_trigger		= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), up_leg_capsule_radius);
	const auto right_down_leg_trigger	= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), down_leg_capsule_radius);

	// 登録
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kLeftUpLegTrigger,	 left_up_leg_trigger,    physical_obj));
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kLeftDownLegTrigger,  left_down_leg_trigger,  physical_obj));
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kRightUpLegTrigger,	 right_up_leg_trigger,   physical_obj));
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kRightDownLegTrigger, right_down_leg_trigger, physical_obj));

	CalcLegTriggerPos(modeler, physical_obj->GetColliderAll());
}

void CharacterColliderCreator::CreateHeadTrigger(PhysicalObjBase* physical_obj, const std::shared_ptr<Modeler> modeler, const float sphere_radius)
{
	auto head_mat		= MV1GetFrameLocalWorldMatrix(modeler->GetModelHandle(), MV1SearchFrame(modeler->GetModelHandle(), BonePath.HEAD));
	const auto head_pos = MGetTranslateElem(head_mat);

	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kHeadTrigger, std::make_shared<Sphere>(head_pos, sphere_radius), physical_obj));
}

void CharacterColliderCreator::CreateBodyTrigger(PhysicalObjBase* physical_obj, const std::shared_ptr<Modeler> modeler, const float capsule_radius)
{
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kBodyTrigger, std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), capsule_radius), physical_obj));

	CalcBodyTriggerPos(modeler, physical_obj->GetColliderAll());
}

void CharacterColliderCreator::CreateMeshTrigger(PhysicalObjBase* phsyical_obj, const std::shared_ptr<Modeler> modeler)
{
	phsyical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kMeshTrigger, modeler->GetModelHandle(), phsyical_obj));
}

void CharacterColliderCreator::CalcLegTriggerPos(const std::shared_ptr<Modeler> modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>> collider)
{
	modeler->ApplyMatrix();
	const auto model_handle		= modeler->GetModelHandle();

	// フレームの行列情報を取得
	auto left_up_leg_mat		= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.LEFT_UP_LEG));
	auto left_leg_mat			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.LEFT_LEG));
	auto left_foot_mat			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.LEFT_FOOT));
	auto right_up_leg_mat		= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.RIGHT_UP_LEG));
	auto right_leg_mat			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.RIGHT_LEG));
	auto right_foot_mat			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.RIGHT_FOOT));

	// 位置を取得
	const auto left_up_leg_pos	= MGetTranslateElem(left_up_leg_mat);
	const auto left_leg_pos		= MGetTranslateElem(left_leg_mat);
	const auto left_foot_pos	= MGetTranslateElem(left_foot_mat);
	const auto right_up_leg_pos = MGetTranslateElem(right_up_leg_mat);
	const auto right_leg_pos	= MGetTranslateElem(right_leg_mat);
	const auto right_foot_pos	= MGetTranslateElem(right_foot_mat);

	// 位置を適用
	std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kLeftUpLegTrigger)	 ->GetShape())->SetSegmentBeginPos	(left_leg_pos,		true);
	std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kLeftUpLegTrigger)	 ->GetShape())->SetSegmentEndPos	(left_up_leg_pos,	true);
	std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kLeftDownLegTrigger) ->GetShape())->SetSegmentBeginPos	(left_foot_pos,		true);
	std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kLeftDownLegTrigger) ->GetShape())->SetSegmentEndPos	(left_leg_pos,		true);
	std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kRightUpLegTrigger)	 ->GetShape())->SetSegmentBeginPos	(right_leg_pos,		true);
	std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kRightUpLegTrigger)	 ->GetShape())->SetSegmentEndPos	(right_up_leg_pos,	true);
	std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kRightDownLegTrigger)->GetShape())->SetSegmentBeginPos	(right_foot_pos,	true);
	std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kRightDownLegTrigger)->GetShape())->SetSegmentEndPos	(right_leg_pos,		true);
}

void CharacterColliderCreator::CalcBodyTriggerPos(const std::shared_ptr<Modeler> modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>> collider)
{
	modeler->ApplyMatrix();
	const auto model_handle = modeler->GetModelHandle();

	// フレームの行列情報を取得
	auto spine2_mat			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.SPINE_2));
	auto hips_mat			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.HIPS));

	// 位置を取得
	const auto spine2_pos	= MGetTranslateElem(spine2_mat);
	const auto hips_pos		= MGetTranslateElem(hips_mat);

	// 位置を適用
	std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kBodyTrigger)->GetShape())->SetSegmentBeginPos(spine2_pos,	true);
	std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kBodyTrigger)->GetShape())->SetSegmentEndPos  (hips_pos,	true);
}

void CharacterColliderCreator::CalcCapsuleColliderLength(PhysicalObjBase* physical_obj, std::shared_ptr<Modeler> modeler)
{
	modeler->ApplyMatrix();

	// 頭部ボーンの行列情報を取得
	const auto model_handle		= modeler->GetModelHandle();
	const auto frame_num		= MV1SearchFrame(model_handle, BonePath.HEAD_TOP_END);
	auto	   frame_mat		= MV1GetFrameLocalWorldMatrix(model_handle, frame_num);

	// 始点から頭部までの長さを取得
	const auto capsule_length	= VSize(physical_obj->GetTransform()->GetPos(CoordinateKind::kWorld) - MGetTranslateElem(frame_mat));
	const auto capsule			= std::static_pointer_cast<Capsule>(physical_obj->GetCollider(ColliderKind::kCollider)->GetShape());
	capsule->SetCapsuleLength(capsule_length);
}
