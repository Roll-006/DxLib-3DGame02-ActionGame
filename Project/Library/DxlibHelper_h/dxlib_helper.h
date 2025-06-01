// SPDX-License-Identifier: MIT-0

#pragma once
#include "DxLib.h"

namespace DxLibHelper
{
/**
 * 3D空間に文字列を描画します。
 *
 * 文字列は指定された位置にビルボード形式で表示されます。
 *
 * @param pos 文字列を表示する3D座標 
 * @param str 表示する文字列
 * @param color 文字色(0xRRGGBB形式)
 * @param edge_color エッジの色(0xAARRGGBB形式)。デフォルトは0(エッジなし)
 * @param size 描画するビルボードのサイズ。デフォルトは24
 */
static void DrawString3D(const VECTOR &pos, const char *str,
                         const unsigned int color, const unsigned int edge_color = 0, const float size = 24)
{
    int size_x, size_y, line_count;
    GetDrawStringSize(&size_x, &size_y, &line_count, str, static_cast<int>(strlen(str)));
    const int draw_screen = MakeScreen(size_x, size_y, TRUE);
    {
        const auto prev_draw_screen_setting = GetUseSetDrawScreenSettingReset();
        SetUseSetDrawScreenSettingReset(FALSE);
        {
            SetDrawScreen(draw_screen);
            DrawString(0, 0, str, color, edge_color);
            SetDrawScreen(DX_SCREEN_BACK);
        }
        SetUseSetDrawScreenSettingReset(prev_draw_screen_setting);
    }

    DrawBillboard3D(pos, 0.5F, 0.5f, size, 0, draw_screen, TRUE);
    DeleteGraph(draw_screen);
}


/**
 * 3D空間に文字列をフォントハンドルを使用して描画します。
 *
 * 文字列は指定された位置にビルボード形式で表示されます。
 * 指定されたフォントハンドルを使用して文字列を描画します。
 *
 * @param pos 文字列を表示する3D座標 
 * @param str 表示する文字列
 * @param color 文字色(0xRRGGBB形式)
 * @param font_handle 使用するフォントのハンドル
 * @param edge_color エッジの色(0xAARRGGBB形式)。デフォルトは0(エッジなし)
 * @param size 描画するビルボードのサイズ。デフォルトは24
 */
static void DrawString3DToHandle(const VECTOR &pos, const char *str,
                                 const unsigned int color, const int font_handle,
                                 const unsigned int edge_color = 0, const float size = 24)
{
    int size_x, size_y, line_count;
    GetDrawStringSizeToHandle(&size_x, &size_y, &line_count, str, static_cast<int>(strlen(str)), font_handle);
    const int draw_screen = MakeScreen(size_x, size_y, TRUE);
    {
        const auto prev_draw_screen_setting = GetUseSetDrawScreenSettingReset();
        SetUseSetDrawScreenSettingReset(FALSE);
        {
            SetDrawScreen(draw_screen);
            DrawStringToHandle(0, 0, str, color, font_handle, edge_color);
            SetDrawScreen(DX_SCREEN_BACK);
        }
        SetUseSetDrawScreenSettingReset(prev_draw_screen_setting);
    }

    DrawBillboard3D(pos, 0.5F, 0.5f, size, 0, draw_screen, TRUE);
    DeleteGraph(draw_screen);
}

/**
 * 3D空間に座標軸を描画します。
 * 
 * X軸は赤、Y軸は緑、Z軸は青で表示されます。
 * 原点から各軸方向に指定された長さの線が描画されます。
 *  
 * @param m 座標軸の変換行列(位置、回転、スケール)
 * @param len 座標軸の長さ
 */
static void DrawAxis3D(const MATRIX &m, const float len = 5)
{
    const auto origin = VTransform({0, 0, 0}, m);
    DrawLine3D(origin, VTransform({len, 0, 0}, m), GetColor(255, 0, 0));
    DrawLine3D(origin, VTransform({0, len, 0}, m), GetColor(0, 255, 0));
    DrawLine3D(origin, VTransform({0, 0, len}, m), GetColor(0, 0, 255));
}

/**
 * オブジェクトの名前と座標軸を3D空間に描画します。
 *
 * オブジェクトの名前は原点付近に表示され、座標軸は与えられた
 * 変換行列に基づいて描画されます。
 *
 * @param object_name 表示するオブジェクトの名前。TCHARで表現される
 *                    NULL終端文字列である必要があります。
 * @param object_matrix オブジェクトのローカル変換(位置、回転、拡大縮小)を
 *                      表す3D空間上の変換行列。
 */
static void DrawObjectInfo(const TCHAR *object_name, const MATRIX &object_matrix)
{
    const auto pos = VTransform({0, 0, 0}, object_matrix);
    DrawString3D(VSub(pos, {0, 1, 0}), object_name, GetColor(255, 255, 255));
    DrawAxis3D(object_matrix);
}

/**
 * 3Dモデルのフレームと階層関係を描画します。
 *
 * このメソッドはモデルと各フレーム(子フレームを含む)の名前と座標軸を表示します。 
 * 親フレームと子フレーム間に線を引いて階層関係を可視化します。
 * 座標情報はモデルハンドルを使用して取得されます。
 *
 * @param model_handle 描画するフレームを持つ3Dモデルのハンドル
 * @param model_name モデルの原点に表示する名前。デフォルトは"Model Origin"
 */
static void DrawModelFrames(const int model_handle, const TCHAR *model_name = "Model Origin")
{
    DrawObjectInfo(model_name, MV1GetMatrix(model_handle));

    const int frame_num = MV1GetFrameNum(model_handle);
    for (int frame_index = 0; frame_index < frame_num; ++frame_index)
    {
        MATRIX frame_mat = MV1GetFrameLocalWorldMatrix(model_handle, frame_index);
        const TCHAR *frame_name = MV1GetFrameName(model_handle, frame_index);
        DrawObjectInfo(frame_name, frame_mat);

        const int child_frame_num = MV1GetFrameChildNum(model_handle, frame_index);
        for (int j = 0; j < child_frame_num; ++j)
        {
            const int child_frame_index = MV1GetFrameChild(model_handle, frame_index, j);
            MATRIX child_frame_mat = MV1GetFrameLocalWorldMatrix(model_handle, child_frame_index);
            DrawLine3D(MGetTranslateElem(child_frame_mat), MGetTranslateElem(frame_mat), 0xFFFFFFFF);
        }
    }
}
}