#pragma once
#include "transform_component.hpp"

#include "../Manager/game_obj_manager.hpp"

class TrackerComponent : public ComponentBase
{
public:
	enum class TargetKind
	{
		kMain,
		kSub,
	};

	enum class TimeState
	{
		kCurrect,
		kNext,
	};

	TrackerComponent(GameObj* owner_obj);
	~TrackerComponent()override;

	void Init()		 override;
	void Update()	 override;
	void LateUpdate()override;
	void Draw()const override;

	void AttachMainTarget(const std::shared_ptr<GameObj> obj);
	void AttachMainTarget(std::string& obj_name);
	void DetachMainTarget();

	void SetDistanceToTarget(float distance){ m_distance_to_target = distance; }
	void SetTrackSpeed		(float speed)	{ m_speed = speed; }

	[[nodiscard]] VECTOR			  GetCurrentTargetPos()	  const;
	[[nodiscard]] VECTOR			  GetNextTargetPos()	  const;
	[[nodiscard]] TransformComponent* GetMainTargetTransform()const;
	//[[nodiscard]] TransformComponent* GetSubTargetTransform() const;

private:
	void CalcTargetPos();

private:
	float m_speed;
	float m_distance_to_target;
	bool  m_sync_pos;

	std::unordered_map<TargetKind, std::shared_ptr<TransformComponent>> m_target_transform;
	std::unordered_map<TimeState, VECTOR>								m_target_pos;
};
