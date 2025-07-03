#pragma once
#include <vector>
#include <tuple>
#include <DxLib.h>
#include <EffekseerForDXLib.h>

#include "../Data/Path/model_path.hpp"
#include "../Data/Path/animation_path.hpp"
#include "../Base/singleton_base.hpp"

enum class HandleKind
{
	kImage,
	kModel,
	kAnim,
	kEffect,
};

/// @brief ハンドルの再ロードを避けるためのハンドル保管クラス
class HandleKeeper : public SingletonBase<HandleKeeper>
{
public:
	/// @brief ハンドル読み込み
	/// @param scale エフェクトの場合のみ有効(未指定の場合は1.0倍)
	/// @return ハンドル
	int  LoadHandle  (const HandleKind handle_kind, const std::string& file_path, const float scale = 1.0f);
	/// @brief ハンドルの破棄
	void DeleteHandle(const HandleKind handle_kind, const std::string& file_path);
	void DeleteHandle(const HandleKind handle_kind, const int handle);

	[[nodiscard]] int GetHandle(const HandleKind handle_kind, const std::string& file_path);

private:
	HandleKeeper();
	~HandleKeeper() override;

private:
	std::vector<std::tuple<HandleKind, std::string, int>> m_handles;

	friend SingletonBase<HandleKeeper>;
};
