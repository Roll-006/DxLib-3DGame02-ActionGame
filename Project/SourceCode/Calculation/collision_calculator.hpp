#pragma once
#include <limits>
#include "math.hpp"
#include "../Data/IncludeList/shape.hpp"

namespace collision
{
    static constexpr int    kRayCapsuleDivisionNum  = 3;        // 光線カプセルの衝突分割数
    static constexpr float  kPointCollisionRadius   = 0.05f;    // 点を球として扱うための半径

    #pragma region 衝突判定
    /// @brief 点と直線の衝突判定
    /// @brief WARNING : 完全な点なため誤差が生じる可能性あり
    /// @param intersection 交点を格納
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitPointAndLine            (const VECTOR&      point,          const Line&         line,           std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitPointAndLine            (const VECTOR&      point,          const Line&         line);

    /// @brief 点と線分の衝突判定
    /// @brief WARNING : 完全な点なため誤差が生じる可能性あり
    /// @param intersection 交点を格納
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitPointAndSegment         (const VECTOR&      point,          const Segment&      segment,        std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitPointAndSegment         (const VECTOR&      point,          const Segment&      segment);

    /// @brief 点と平面(無限に広がる面)の衝突判定
    /// @brief WARNING : 完全な点なため誤差が生じる可能性あり
    /// @param intersection 交点を格納
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitPointAndPlane           (const VECTOR&      point,          const Plane&        plane,          std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitPointAndPlane           (const VECTOR&      point,          const Plane&        plane);

    /// @brief 点と三角形の衝突判定
    /// @brief WARNING : 完全な点なため誤差が生じる可能性あり
    /// @param intersection 交点を格納
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitPointAndTriangle        (const VECTOR&      point,          const Triangle&     triangle,       std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitPointAndTriangle        (const VECTOR&      point,          const Triangle&     triangle);

    /// @brief 点と四角形の衝突判定
    /// @param intersection 交点を格納
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitPointAndSquare          (const VECTOR&      point,          const Square&       square,         std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitPointAndSquare          (const VECTOR&      point,          const Square&       square);

    /// @brief 点とカプセルの衝突判定
    /// @param intersection 交点を格納
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitPointAndCapsule         (const VECTOR&      point,          const Capsule&      capsule,        std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitPointAndCapsule         (const VECTOR&      point,          const Capsule&      capsule);

    /// @brief 直線と直線の衝突判定
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitLineAndLine             (const Line&        line1,          const Line&         line2,          std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitLineAndLine             (const Line&        line1,          const Line&         line2);

    /// @brief 直線と平面(無限に広がる面)の衝突判定
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitLineAndPlane            (const Line&        line,           const Plane&        plane,          std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitLineAndPlane            (const Line&        line,           const Plane&        plane);

    /// @brief 直線と直線の衝突判定
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitSegmentAndSegment       (const Segment&     segment1,       const Segment&      segment2,       std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitSegmentAndSegment       (const Segment&     segment1,       const Segment&      segment2);

    /// @brief 線分と平面(無限に広がる面)の衝突判定
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitSegmentAndPlane         (const Segment&     segment,        const Plane&        plane,          std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitSegmentAndPlane         (const Segment&     segment,        const Plane&        plane);
    
    /// @brief 線分と三角形の衝突判定
    /// @param intersection 衝突していれば、線分の始点に最も近い交点を格納
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitSegmentAndTriangle      (const Segment&     segment,        const Triangle&     triangle,       std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitSegmentAndTriangle      (const Segment&     segment,        const Triangle&     triangle);

    /// @brief 線分と四角形の衝突判定
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitSegmentAndSquare        (const Segment&     segment,        const Square&       square,         std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitSegmentAndSquare        (const Segment&     segment,        const Square&       square);

    /// @brief 線分と円周の衝突判定
    //[[nodiscard]] bool IsHitSegmentAndCircumference(const Segment*  segment,  const Circle*   circle);

    /// @brief 線分と球の衝突判定
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    //[[nodiscard]] bool IsHitSegmentAndSphere        (const Segment&     segment,        const Sphere&       sphere,         std::optional<VECTOR>& intersection);
    //[[nodiscard]] bool IsHitSegmentAndSphere        (const Segment&     segment,        const Sphere&       sphere);
    
    /// @brief 線分とカプセルの衝突判定
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitSegmentAndCapsule       (const Segment&     segment,        const Capsule&      capsule,        std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitSegmentAndCapsule       (const Segment&     segment,        const Capsule&      capsule);

    /// @brief 線分とモデルの衝突判定
    /// @param intersection 衝突していれば、線分の始点に最も近い交点を格納
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitSegmentAndModel         (const Segment&     segment,        const int           model_handle,   std::optional<VECTOR>& intersection, std::vector<Triangle>& hit_triangles);
    [[nodiscard]] bool IsHitSegmentAndModel         (const Segment&     segment,        const int           model_handle,   std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitSegmentAndModel         (const Segment&     segment,        const int           model_handle);

    /// @brief 平面(無限に広がる面)とカプセルの衝突判定
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitPlaneAndCapsule         (const Plane&       plane,          const Capsule&      capsule,        std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitPlaneAndCapsule         (const Plane&       plane,          const Capsule&      capsule);

    /// @brief 三角形と三角形の衝突判定
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitTriangleAndTriangle     (const Triangle&    triangle1,      const Triangle&     triangle2,      std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitTriangleAndTriangle     (const Triangle&    triangle1,      const Triangle&     triangle2);

    /// @brief 三角形と球の衝突判定
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitTriangleAndSphere       (const Triangle&    triangle,       const Sphere&       sphere,         std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitTriangleAndSphere       (const Triangle&    triangle,       const Sphere&       sphere);

    /// @brief 三角形とカプセルの衝突判定
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitTriangleAndCapsule      (const Triangle&    triangle,       const Capsule&      capsule,        std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitTriangleAndCapsule      (const Triangle&    triangle,       const Capsule&      capsule);

    /// @brief 三角形と光線カプセルの衝突判定
    /// @brief 視点カプセルを徐々に移動させて判定を行っている(判定回数は定数[collision::kRayCapsuleDivisionNum]に依存)
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitTriangleAndRayCapsule   (const Triangle&    triangle,       const RayCapsule&   ray_capsule,    std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitTriangleAndRayCapsule   (const Triangle&    triangle,       const RayCapsule&   ray_capsule);

    /// @brief 三角形とモデルの衝突判定
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitTriangleAndModel        (const Triangle&    triangle,       const int           model_handle,   std::optional<VECTOR>& intersection, std::vector<Triangle>& hit_triangles);
    [[nodiscard]] bool IsHitTriangleAndModel        (const Triangle&    triangle,       const int           model_handle,   std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitTriangleAndModel        (const Triangle&    triangle,       const int           model_handle);

    /// @brief 四角形とカプセルの衝突判定
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitSquareAndCapsule        (const Square&      square,         const Capsule&      capsule,        std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitSquareAndCapsule        (const Square&      square,         const Capsule&      capsule);

    /// @brief 四角形と光線カプセルの衝突判定
    /// @brief 視点カプセルを徐々に移動させて判定を行っている(判定回数は定数[collision::kRayCapsuleDivisionNum]に依存)
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitSquareAndRayCapsule     (const Square&      square,         const RayCapsule&   ray_capsule,    std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitSquareAndRayCapsule     (const Square&      square,         const RayCapsule&   ray_capsule);

    /// @brief 球と球の衝突判定
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitSphereAndSphere         (const Sphere&      sphere1,        const Sphere&       sphere2,        std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitSphereAndSphere         (const Sphere&      sphere1,        const Sphere&       sphere2);
    
    /// @brief 球とカプセルの衝突判定 
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitSphereAndCapsule        (const Sphere&      sphere,         const Capsule&      capsule,        std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitSphereAndCapsule        (const Sphere&      sphere,         const Capsule&      capsule);

    /// @brief 球とモデルの衝突判定 
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitSphereAndModel          (const Sphere&      sphere,         const int           model_handle,   std::optional<VECTOR>& intersection, std::vector<Triangle>& hit_triangles);
    [[nodiscard]] bool IsHitSphereAndModel          (const Sphere&      sphere,         const int           model_handle,   std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitSphereAndModel          (const Sphere&      sphere,         const int           model_handle);

    /// @brief カプセルとカプセルの衝突判定
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitCapsuleAndCapsule       (const Capsule&     capsule1,       const Capsule&      capsule2,       std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitCapsuleAndCapsule       (const Capsule&     capsule1,       const Capsule&      capsule2);

    /// @brief カプセルとモデルの衝突判定
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitCapsuleAndModel         (const Capsule&     capsule,        const int           model_handle,   std::optional<VECTOR>& intersection, std::vector<Triangle>& hit_triangles);
    [[nodiscard]] bool IsHitCapsuleAndModel         (const Capsule&     capsule,        const int           model_handle,   std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitCapsuleAndModel         (const Capsule&     capsule,        const int           model_handle,   MV1_COLL_RESULT_POLY_DIM& hit_result);
    [[nodiscard]] bool IsHitCapsuleAndModel         (const Capsule&     capsule,        const int           model_handle);

    /// @brief 光線カプセルとモデルの衝突判定
    /// @brief 視点カプセルを徐々に移動させて判定を行っている(判定回数は定数[collision::kRayCapsuleDivisionNum]に依存)
    /// @param intersection 未実装
    /// @return true : 衝突, false : 非衝突
    [[nodiscard]] bool IsHitRayCapsuleAndModel      (const RayCapsule&  ray_capsule,    const int           model_handle,   std::optional<VECTOR>& intersection);
    [[nodiscard]] bool IsHitRayCapsuleAndModel      (const RayCapsule&  ray_capsule,    const int           model_handle,   MV1_COLL_RESULT_POLY_DIM& hit_result);
    [[nodiscard]] bool IsHitRayCapsuleAndModel      (const RayCapsule&  ray_capsule,    const int           model_handle);

    /// @brief 円周とカプセルの衝突判定
    /// @brief WARNING : 完璧な当たり判定ではない。円周に大して垂直なカプセルのみの使用を推奨。
    /// @bried TODO : 後に二つの円で円周の判定を行うように変更
    //[[nodiscard]] bool IsHitCircumferenceAndCapsuleLowPrecision(const Circle* circle, const Capsule* capsule);
    #pragma endregion


    #pragma region 押し戻し(衝突時の有効な速度ベクトルを取得)
    /// @brief TODO : 正しく機能するか確認していないため要検証
    /// @brief 球(移動オブジェクト)と三角形(固定オブジェクト)が衝突する際の有効な速度ベクトルを取得
    /// @brief 高速移動対応
    /// @brief WARNING : この関数は現在衝突対象にめり込んでいないことを前提としている
    /// @brief WARNING : 法線の裏側を考慮しないものとする
    /// @param velocity 速度ベクトル
    /// @param dynamic_sphere 球(移動オブジェクト)
    /// @param static_triangle 三角形(固定オブジェクト)
    /// @return 有効な速度ベクトル
    [[nodiscard]] VECTOR PushBackSphereAndTriangle(const VECTOR& velocity, const Sphere& dynamic_sphere, const Triangle& static_triangle);

    /// @brief TODO : 正しく機能するか確認していないため要検証
    /// @brief 球(移動オブジェクト)とモデル(固定オブジェクト)が衝突した際の有効な速度ベクトルを取得
    /// @brief 高速移動対応
    /// @brief WARNING : この関数は現在衝突対象にめり込んでいないことを前提としている
    /// @brief WARNING : 法線の裏側を考慮しないものとする
    /// @param velocity 速度ベクトル
    /// @param dynamic_sphere 球(移動オブジェクト)
    /// @param model_handle モデルハンドル(固定オブジェクトなモデルのハンドル)
    /// @return 有効な速度ベクトル
    [[nodiscard]] VECTOR PushBackSphereAndModel(const VECTOR& velocity, const Sphere& dynamic_sphere, const int model_handle);

    [[nodiscard]] VECTOR PushBackCapsuleAndTriangle(const VECTOR& velocity, const Capsule& dynamic_capsule, const Triangle& static_triangle);

    /// @brief カプセル(移動オブジェクト)と四角形(固定オブジェクト)が衝突する際の有効な速度ベクトルを取得
    /// @brief WARNING : この関数は現在衝突対象にめり込んでいないことを前提としている
    /// @brief WARNING : 法線の裏側を考慮しないものとする
    /// @param velocity 速度ベクトル
    /// @param dynamic_capsule カプセル(移動オブジェクト)
    /// @param static_square 四角形(固定オブジェクト)
    /// @return 有効な速度ベクトル
    [[nodiscard]] VECTOR PushBackCapsuleAndSquare(const VECTOR& velocity, const Capsule& dynamic_capsule, const Square& static_square);

    /// @brief FIXME : 欠陥あり。角でバグ発生中。
    /// @brief カプセル(移動オブジェクト)とOBB(固定オブジェクト)が衝突した際の有効な速度ベクトルを取得
    /// @param velocity 速度ベクトル
    /// @param dynamic_capsule カプセル(移動オブジェクト)
    /// @param static_obb OBB(固定オブジェクト)
    /// @return 有効な速度ベクトル
    [[nodiscard]] VECTOR PushBackCapsuleAndOBB(const VECTOR& velocity, const Capsule& dynamic_capsule, const OBB& static_obb);

    /// @brief カプセル(移動オブジェクト)とモデル(固定オブジェクト)が衝突した際の有効な速度ベクトルを取得
    /// @brief WARNING : この関数は現在衝突対象にめり込んでいないことを前提としている
    /// @brief WARNING : 法線の裏側を考慮しないものとする
    /// @param velocity 速度ベクトル
    /// @param dynamic_capsule カプセル(移動オブジェクト)
    /// @param model_handle モデルハンドル(固定オブジェクトなモデルのハンドル)
    /// @return 有効な速度ベクトル
    [[nodiscard]] VECTOR PushBackCapsuleAndModel(const VECTOR& velocity, const Capsule& dynamic_capsule, const int model_handle);
    #pragma endregion
};
