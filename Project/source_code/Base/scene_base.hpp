#pragma once
#include <memory>

#include "../Data/Kind/scene_kind.hpp"

class SceneBase
{
public:
	SceneBase() : m_is_active(true){}
	virtual ~SceneBase() = default;

	virtual void Init()		 = 0;
	virtual void Update()	 = 0;
	virtual void Draw()const = 0;

	/// @brief アクティブ化する
	void Activate(){ m_is_active = true; }

	/// @brief 非アクティブ化する
	/// @brief オブジェクトを削除せずに処理を停止
	/// @brief Init関数を通さずにシーンを再開したい場合に有効
	void Deactivate(){ m_is_active = false; }

	[[nodiscard]] bool IsActive()const { return m_is_active; }

private:
	bool m_is_active;
};
