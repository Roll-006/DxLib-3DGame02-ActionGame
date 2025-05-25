#include "player.hpp"

Player::Player(std::shared_ptr<Camera> camera) :
	CollideObjBase	(ObjName.PLAYER, ObjTag.PLAYER, MassKind::kMedium),
	m_modeler		(nullptr),
	m_animator		(nullptr),
	m_camera		(camera)
{
	m_modeler	= std::make_shared<Modeler> (GetTransform(), ModelPath.CHARA_01);
	m_animator	= std::make_shared<Animator>(m_modeler, 3.0f);

	// TEST : 仮で座標を指定
	m_transform->SetPos(CoordinateKind::kWorld, VGet(100, 100, 100));

	// 各アニメーション追加
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdle), AnimPath.IDLE_01, AnimTag.NONE, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalk), AnimPath.WALK_01, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kJog),  AnimPath.JOG_01,  AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kRun),  AnimPath.RUN_01,  AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kJump), AnimPath.JUMP_01, AnimTag.NONE, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kFall), AnimPath.FALL_01, AnimTag.NONE, 20.0f, true);
	m_animator->AttachAnim	 (static_cast<int>(PlayerAnimKind::kIdle));
}

Player::~Player()
{

}

void Player::Init()
{

}

void Player::Update()
{
	m_animator->Update();
}

void Player::Draw()const
{
	m_modeler->Draw();
}

void Player::OnCollide(const CollideObjBase& check_hit_obj)
{

}
