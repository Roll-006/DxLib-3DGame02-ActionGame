#pragma once

enum class CommandKind
{
	kDecide,		// 決定ボタン
	kBack,			// 戻るボタン
	kSelect,		// 選択
	kPause,			// ポーズ画面切り替え
	kMovePlayer,	// プレイヤーの移動
	kRun,			// ダッシュ
	kSquat,			// しゃがむ
	kMoveCamera,	// カメラの移動
	kInitAngle,		// 視点リセット
	kShot,			// 銃を撃つ
	kReloadGun,		// 銃のリロード
	kSilentKill,	// サイレントキル(不意打ち)
};
