#pragma once

enum class CommandKind
{
	kDecide,			// 決定ボタン
	kBack,				// 戻るボタン
	kSelectUp,			// 選択(上)
	kSelectDown,		// 選択(下)
	kSelectLeft,		// 選択(左)
	kSelectRight,		// 選択(右)
	kPause,				// ポーズ画面切り替え

	kAimGun,			// 銃を構える
	kAttack,			// 攻撃
	kAimKnife,			// ナイフを構える / (パリィ)
	kInvestigate,		// 調べる / (拾う)
	kMelee,				// メレー
	kReload,			// リロード
	kScope,				// スコープ倍率変更
	kMoveUpPlayer,		// プレイヤー上移動
	kMoveDownPlayer,	// プレイヤー下移動
	kMoveLeftPlayer,	// プレイヤー左移動
	kMoveRightPlayer,	// プレイヤー右移動
	kRun,				// ダッシュ
	kCrouch,			// しゃがむ
	kQuickTurn,			// クイックターン

	kMoveUpCamera,		// カメラ上移動
	kMoveDownCamera,	// カメラ下移動
	kMoveLeftCamera,	// カメラ左移動
	kMoveRightCamera,	// カメラ右移動
	kInitAim,			// 視点リセット
};
