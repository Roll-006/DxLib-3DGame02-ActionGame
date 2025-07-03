#pragma once
#include "../Base/singleton_base.hpp"

class UIManager final : public SingletonBase<UIManager>
{
public:

private:
	UIManager();
	~UIManager() override;

	friend SingletonBase<UIManager>;
};
