#include "non_collide_rocket_bomb.hpp"

NonCollildeRocketBomb::NonCollildeRocketBomb(const std::shared_ptr<Transform> weapon_load_transform) :
	ObjBase					(ObjName.NON_COLLIDE_ROCKET_BOMB, ObjTag.MAGAZINE),
	m_modeler				(std::make_shared<Modeler>(m_transform, ModelPath.ROCKET_BOMB, kBasicAngle, kBasicScale)),
	m_owner_modeler			(nullptr),
	m_weapon_load_transform	(weapon_load_transform),
	m_is_reloading			(false)
{

}

NonCollildeRocketBomb::~NonCollildeRocketBomb()
{

}

void NonCollildeRocketBomb::Init()
{

}

void NonCollildeRocketBomb::Update()
{
	if (!IsActive()) { return; }
}

void NonCollildeRocketBomb::LateUpdate()
{
	if (!IsActive()) { return; }

	ApplyMoveDirToRot();
	TrackOwnerHand();
	TrackLoad();
}

void NonCollildeRocketBomb::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
}

void NonCollildeRocketBomb::AddToObjManager()
{
	ObjAccessor::GetInstance()->AddObj(shared_from_this());
}

void NonCollildeRocketBomb::RemoveToObjManager()
{
	ObjAccessor::GetInstance()->RemoveObj(GetObjHandle());
}

void NonCollildeRocketBomb::OnStartReload(const std::shared_ptr<Modeler>& owner_modler)
{
	m_is_reloading	= true;
	m_owner_modeler = owner_modler;
}

void NonCollildeRocketBomb::OnReloaded()
{
	m_is_reloading	= false;
}

const float NonCollildeRocketBomb::GetDeltaTime() const
{
	return GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kWorld);
}

void NonCollildeRocketBomb::ApplyMoveDirToRot()
{

}

void NonCollildeRocketBomb::TrackOwnerHand()
{
	if (!m_is_reloading)  { return; }
	if (!m_owner_modeler) { return; }

	m_owner_modeler->ApplyMatrix();

	// アタッチする部位の行列情報を取り出す
	const auto owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), FramePath.LEFT_HAND);
	const auto owner_attach_frame_mat = MV1GetFrameLocalWorldMatrix(m_owner_modeler->GetModelHandle(), owner_attach_frame_num);

	// 武器をアタッチする部位に合わせて回転し、行列を取得
	const auto offset_angle_mat = math::ConvertEulerAnglesToXYZRotMatrix(kHoldOffsetAngle);
	const auto result_mat		= offset_angle_mat * owner_attach_frame_mat;

	// 情報を適用
	m_transform->SetMatrix	(CoordinateKind::kWorld, result_mat);
	m_transform->SetPos		(CoordinateKind::kLocal, m_transform->GetPos(CoordinateKind::kLocal) + VTransformSR(kHoldOffsetPos, result_mat));
	m_transform->SetScale	(CoordinateKind::kWorld, kHoldOffsetScale);
}

void NonCollildeRocketBomb::TrackLoad()
{
	if (m_is_reloading) { return; }

	// 武器をアタッチする部位に合わせて回転し、行列を取得
	const auto offset_angle_mat = math::ConvertEulerAnglesToXYZRotMatrix(kLoadOffsetAngle);
	const auto result_mat		= offset_angle_mat * m_weapon_load_transform->GetMatrix(CoordinateKind::kWorld);

	// 情報を適用
	m_transform->SetMatrix	(CoordinateKind::kWorld, result_mat);
	m_transform->SetPos		(CoordinateKind::kLocal, m_transform->GetPos(CoordinateKind::kLocal) + VTransformSR(kLoadOffsetPos, result_mat));
	m_transform->SetScale	(CoordinateKind::kWorld, kLoadOffsetScale);
}
