#include "assault_rifle_magazine.hpp"

AssaultRifleMagazine::AssaultRifleMagazine(const std::shared_ptr<Transform> weapon_load_transform) :
	ObjBase					(ObjName.ASSAULT_RIFLE_MAGAZINE, ObjTag.MAGAZINE),
	m_modeler				(std::make_shared<Modeler>(m_transform, ModelPath.ASSAULT_RIFLE_MAGAZINE, kBasicAngle, kBasicScale)),
	m_owner_modeler			(nullptr),
	m_weapon_load_transform	(weapon_load_transform),
	m_is_reloading			(false)
{

}

AssaultRifleMagazine::~AssaultRifleMagazine()
{

}

void AssaultRifleMagazine::Init()
{

}

void AssaultRifleMagazine::Update()
{
	if (!IsActive()) { return; }
}

void AssaultRifleMagazine::LateUpdate()
{
	if (!IsActive()) { return; }

	TrackOwnerHand();
	TrackLoad();
}

void AssaultRifleMagazine::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
}

void AssaultRifleMagazine::AddToObjManager()
{
	ObjAccessor::GetInstance()->AddObj(shared_from_this());
}

void AssaultRifleMagazine::RemoveToObjManager()
{
	ObjAccessor::GetInstance()->RemoveObj(GetObjHandle());
}

void AssaultRifleMagazine::OnStartReload(const std::shared_ptr<Modeler>& owner_modler)
{
	m_is_reloading	= true;
	m_owner_modeler = owner_modler;
}

void AssaultRifleMagazine::OnReloaded()
{
	m_is_reloading = false;
}

const float AssaultRifleMagazine::GetDeltaTime() const
{
	return GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kWorld);
}

void AssaultRifleMagazine::TrackOwnerHand()
{
	if (!m_is_reloading)	{ return; }
	if (!m_owner_modeler)	{ return; }

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

void AssaultRifleMagazine::TrackLoad()
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
