#pragma once

enum class TimeScaleLayerKind
{
	kNoneScale,	// 等倍
	kWorld,		// ゲーム全体(敵, 環境, 物理)
	kPlayer,	// プレイヤー
	kUI,		// UI
	kEffect,	// エフェクト
	kCamera,	// カメラ
};
