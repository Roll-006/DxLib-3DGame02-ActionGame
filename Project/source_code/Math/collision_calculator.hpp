//#pragma once
//#include "math.hpp"
//#include "../Data/IncludeList/shape.hpp"
//
//namespace collision
//{
//    #pragma region 衝突判定
//    /// @brief 点と直線の衝突判定
//    [[nodiscard]] bool IsHitPointAndLine           (const VECTOR&   point,    const Line&     line);
//
//    /// @brief 点と線分の衝突判定
//    [[nodiscard]] bool IsHitPointAndSegment        (const VECTOR&   point,    const Segment&  segment);
//
//    /// @brief 点と平面(無限に広がる面)の衝突判定
//    [[nodiscard]] bool IsHitPointAndPlane          (const VECTOR&   point,    const Plane&    plane);
//
//    /// @brief 点と三角形の衝突判定
//    [[nodiscard]] bool IsHitPointAndTriangle       (const VECTOR&   point,    const Triangle& triangle);
//
//    /// @brief 点と四角形の衝突判定
//    [[nodiscard]] bool IsHitPointAndSquare         (const VECTOR&   point,    const Square&   square);
//
//    /// @brief 点とカプセルの衝突判定
//    [[nodiscard]] bool IsHitPointAndCapsule        (const VECTOR&   point,    const Capsule&  capsule);
//
//    /// @brief 直線と直線の衝突判定
//    [[nodiscard]] bool IsHitLineAndLine            (const Line&     line1,    const Line&     line2);
//
//    /// @brief 直線と平面(無限に広がる面)の衝突判定
//    [[nodiscard]] bool IsHitLineAndPlane           (const Line&     line,     const Plane&    plane);
//
//    /// @brief 線分と線分の衝突判定
//    [[nodiscard]] bool IsHitSegmentAndSegment      (const Segment&  segment1, const Segment&  segment2);
//
//    /// @brief 線分と平面(無限に広がる面)の衝突判定
//    [[nodiscard]] bool IsHitSegmentAndPlane        (const Segment&  segment,  const Plane&    plane);
//    
//    /// @brief 線分と三角形の衝突判定
//    [[nodiscard]] bool IsHitSegmentAndTriangle     (const Segment&  segment,  const Triangle& triangle);
//
//    /// @brief 線分と四角形の衝突判定
//    [[nodiscard]] bool IsHitSegmentAndSquare       (const Segment&  segment,  const Square&   square);
//
//    /// @brief 線分と円周の衝突判定
//    //[[nodiscard]] bool IsHitSegmentAndCircumference(const Segment*  segment,  const Circle*   circle);
//
//    /// @brief 線分とカプセルの衝突判定
//    [[nodiscard]] bool IsHitSegmentAndCapsule      (const Segment&  segment,  const Capsule&  capsule);
//
//    /// @brief 平面(無限に広がる面)とカプセルの衝突判定
//    [[nodiscard]] bool IsHitPlaneAndCapsule        (const Plane&    plane,    const Capsule&  capsule);
//
//    /// @brief 三角形とカプセルの衝突判定
//    [[nodiscard]] bool IsHitTriangleAndCapsule     (const Triangle& triangle, const Capsule&  capsule);
//
//    /// @brief 四角形とカプセルの衝突判定
//    [[nodiscard]] bool IsHitSquareAndCapsule       (const Square&   square,   const Capsule&  capsule);
//
//    /// @brief 球と球の衝突判定
//    [[nodiscard]] bool IsHitSphereAndSphere        (const Sphere&   sphere1,  const Sphere&   sphere2);
//
//    /// @brief 球とカプセルの衝突判定 
//    [[nodiscard]] bool IsHitSphereAndCapsule       (const Sphere&   sphere,   const Capsule&  capsule);
//
//    /// @brief カプセルとカプセルの衝突判定
//    [[nodiscard]] bool IsHitCapsuleAndCapsule      (const Capsule&  capsule1, const Capsule&  capsule2);
//
//    /// @brief 円周とカプセルの衝突判定
//    /// @brief 注意 : 完璧な当たり判定ではない。円周に大して垂直なカプセルのみの使用を推奨。
//    /// @bried TODO : 後に二つの円で円周の判定を行うように変更
//    //[[nodiscard]] bool IsHitCircumferenceAndCapsuleLowPrecision(const Circle* circle, const Capsule* capsule);
//    #pragma endregion
//
//
//    #pragma region 壁ずり
//    /// @brief 壁ずりベクトルを取得
//    /// @param velocity 速度ベクトル
//    /// @param extra_velocity 余分であった速度ベクトル
//    /// @param wall_normal_vector 壁の法線
//    /// @return 壁ずり修正後の速度ベクトル
//    [[nodiscard]] VECTOR GetWallSlideVector(const VECTOR& velocity, const VECTOR& extra_velocity, const VECTOR& wall_normal_vector);
//    #pragma endregion
//
//
//    #pragma region 押し戻し(衝突時の有効な速度ベクトルを取得)
//
//    // TODO : 面の横から入った際の押し戻す方向が正しくない
//
//    /// @brief カプセル(移動オブジェクト)と四角形(固定オブジェクト)が衝突する際の有効な速度ベクトルを取得
//    /// @brief 衝突対象にめり込まないように速度ベクトルを書き換える処理を行う
//    /// @brief 注意点 : この関数は現在衝突対象にめり込んでいないことを前提としている
//    /// @param velocity 速度ベクトル
//    /// @param dynamic_capsule カプセル(移動オブジェクト)
//    /// @param static_square 四角形(固定オブジェクト)
//    /// @return 有効な速度ベクトル
//    [[nodiscard]] VECTOR GetValidVelocityAfterHitCapsuleAndSquare(const VECTOR& velocity, const Capsule& dynamic_capsule, const Square& static_square, bool& is_hit);
//
//    // TODO : 欠陥あり
//
//    /// @brief カプセル(移動オブジェクト)とOBB(固定オブジェクト)が衝突した際の有効な速度ベクトルを取得
//    /// @param velocity 速度ベクトル
//    /// @param dynamic_capsule カプセル(移動オブジェクト)
//    /// @param static_obb OBB(固定オブジェクト)
//    /// @return 有効な速度ベクトル
//    [[nodiscard]] VECTOR GetValidVelocityAfterHitCapsuleAndOBB(const VECTOR& velocity, const Capsule& dynamic_capsule, const OBB* static_obb, bool& is_hit);
//    #pragma endregion
//};
