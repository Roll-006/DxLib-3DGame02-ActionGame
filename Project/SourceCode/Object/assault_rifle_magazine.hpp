#pragma once
#include "../Base/obj_base.hpp"
#include "../Interface/i_loadable_ammo.hpp"

#include "../Accessor/obj_accessor.hpp"
#include "../GameTime/game_time_manager.hpp"

class AssaultRifleMagazine final : public ObjBase, public ILoadableAmmo
{
public:
	AssaultRifleMagazine(const std::shared_ptr<Transform> weapon_load_transform);
	~AssaultRifleMagazine() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void AddToObjManager()			override;
	void RemoveToObjManager()		override;

	void OnStartReload(const std::shared_ptr<Modeler>& owner_modler) override;
	void OnReloaded() override;
	void TrackLoad() override;

	[[nodiscard]] const float	GetDeltaTime() const override;
	[[nodiscard]] const bool	IsReloading()  const override { return m_is_reloading; }

private:
	void TrackOwnerHand() override;

private:
	static constexpr VECTOR kBasicAngle			= { 0.0f, 0.0f, 0.0f };
	static constexpr float  kBasicScale			= 1.5f;

	static constexpr VECTOR kHoldOffsetAngle	= { 0.0f, 90.0f * math::kDegToRad, 90.0f * math::kDegToRad };	// 手に持たれる際のオフセット角度
	static constexpr VECTOR kHoldOffsetPos		= { 5.0f, 5.0f, 10.0f };										// 手に持たれる際のオフセット座標
	static constexpr float  kHoldOffsetScale	= 1.0f;															// 手に持たれる際のオフセットスケール
	static constexpr VECTOR kLoadOffsetPos		= { 0.0f, 10.0f, 0.0f };										// 装填位置のオフセット座標
	static constexpr VECTOR kLoadOffsetAngle	= { 0.0f, 0.0f, 0.0f };											// 装填位置のオフセット角度
	static constexpr float  kLoadOffsetScale	= 1.0f;															// 装填位置のオフセットスケール

	std::shared_ptr<Modeler>	m_modeler;
	std::shared_ptr<Modeler>	m_owner_modeler;
	std::shared_ptr<Transform>  m_weapon_load_transform;
	bool						m_is_reloading;
};
