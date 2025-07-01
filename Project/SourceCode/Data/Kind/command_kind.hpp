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

	kMoveUpPlayer,		// プレイヤー上移動
	kMoveDownPlayer,	// プレイヤー下移動
	kMoveLeftPlayer,	// プレイヤー左移動
	kMoveRightPlayer,	// プレイヤー右移動
	kRun,				// ダッシュ
	kSquat,				// しゃがむ
	kShot,				// 銃を撃つ
	kAimingGun,			// 銃を構える
	kReloadGun,			// 銃のリロード
	kSilentKill,		// サイレントキル(不意打ち)
	kTurnAround,		// 振り向き

	kMoveUpCamera,		// カメラ上移動
	kMoveDownCamera,	// カメラ下移動
	kMoveLeftCamera,	// カメラ左移動
	kMoveRightCamera,	// カメラ右移動
	kInitAngle,			// 視点リセット
};
