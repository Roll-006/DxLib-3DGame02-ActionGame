#pragma once
#include <unordered_map>
#include "../Base/singleton_base.hpp"

class HandleCreator final : public SingletonBase<HandleCreator>
{
public:
	enum class Kind
	{
		kNone,		// 区別なし
		kEnemy,
	};

public:
	/// @brief ステートハンドルを生成
	[[nodiscard]] int CreateHandle(const Kind kind = Kind::kNone) { return ++m_handle_create_count[kind]; }

private:
	HandleCreator() {}
	~HandleCreator() override {}

private:
	std::unordered_map<Kind, int> m_handle_create_count;	// ハンドル生成用カウント

	friend SingletonBase<HandleCreator>;
};
