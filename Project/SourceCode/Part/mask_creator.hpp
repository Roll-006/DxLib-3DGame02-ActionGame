#pragma once
#include <DxLib.h>

class MaskCreator final
{
public:
	/// @brief マスクを使用する
	/// @brief この関数を呼び出した後にスクリーンに反映する画像・図形を描画する
	/// @param is_invert_mask マスクをかける位置を反転させるかどうか
	void UseMask(const int graphic_handle, const bool is_invert_mask);

	/// @brief マスクの使用を終了する
	/// @brief スクリーンに反映する画像・図形を描画した後にこの関数を呼び出す
	void UnuseMask();

private:
	static bool m_is_invert_mask;
};
