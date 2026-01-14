#pragma once
#include "../Part/transform.hpp"
#include "../Path/effect_path.hpp"
#include "../Data/effect_data.hpp"
#include "../Handle/handle_keeper.hpp"

class IEffect abstract
{
public:
	virtual ~IEffect() = default;

	#pragma region Attach / Detach
	virtual void AttachOwnerTransform(const std::shared_ptr<Transform>& owner_transform) abstract;
	virtual void DetachOwnerTransform() abstract;
	#pragma endregion


	#pragma region 登録 / 削除
	virtual void AddTimeScaleOwner(const std::string& owner_name) abstract;
	virtual void RemoveTimeScaleOwner() abstract;

	/// @brief 強制的にプールに返却させるためのハンドルを追加する
	/// @brief 主にオーナーとしてアタッチしたトランスフォームのオブジェクトハンドルを追加する
	virtual void AddReturnPoolTriggerHandle(const int return_trigger_handle) abstract;
	virtual void RemoveReturnPoolTriggerHandle() abstract;
	#pragma endregion


	#pragma region Setter
	virtual void SetOffsetPos  (const VECTOR& offset_pos)   abstract;
	virtual void SetOffsetAngle(const VECTOR& offset_angle) abstract;
	virtual void SetOffsetScale(const VECTOR& offset_scale) abstract;
	virtual void SetOffsetScale(const float   offset_scale) abstract;
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] const virtual int GetOriginEffectHandle()			const abstract;
	[[nodiscard]] const virtual int GetPlayingEffectHandle()		const abstract;
	[[nodiscard]] const virtual int GetReturnPoolTriggerHandle()	const abstract;
	#pragma endregion

protected:
	virtual void ApplyMatrix()    const abstract;
	virtual void ApplyPlaySpeed() const abstract;

	virtual void PlayEffect() abstract;
};
