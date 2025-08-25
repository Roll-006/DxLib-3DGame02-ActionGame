#include "collision_calculator.hpp"

#pragma region 衝突判定
/// @brief 点と直線の衝突判定
bool collision::IsHitPointAndLine           (const VECTOR&      point,          const Line&         line,           std::optional<VECTOR>& intersection)
{
    const bool is_hit = math::GetDistancePointToLine(point, line) < math::kEpsilonLow;

    intersection = is_hit ? std::optional<VECTOR>(point) : std::nullopt;

    return is_hit;
}
bool collision::IsHitPointAndLine           (const VECTOR&      point,          const Line&         line)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitPointAndLine(point, line, intersection);
}

/// @brief 点と線分の衝突判定
bool collision::IsHitPointAndSegment        (const VECTOR&      point,          const Segment&      segment,        std::optional<VECTOR>& intersection)
{
    const bool is_hit = math::GetDistancePointToSegment(point, segment) < math::kEpsilonLow;

    intersection = is_hit ? std::optional<VECTOR>(point) : std::nullopt;

    return is_hit;
}
bool collision::IsHitPointAndSegment        (const VECTOR&      point,          const Segment&      segment)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitPointAndSegment(point, segment, intersection);
}

/// @brief 点と平面(無限に広がる面)の衝突判定
bool collision::IsHitPointAndPlane          (const VECTOR&      point,          const Plane&        plane,          std::optional<VECTOR>& intersection)
{
    const bool is_hit = math::GetDistancePointToPlane(point, plane) < math::kEpsilonLow;

    intersection = is_hit ? std::optional<VECTOR>(point) : std::nullopt;

    return is_hit;
}
bool collision::IsHitPointAndPlane          (const VECTOR&      point,          const Plane&        plane)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitPointAndPlane(point, plane, intersection);
}

/// @brief 点と三角形の衝突判定
bool collision::IsHitPointAndTriangle       (const VECTOR&      point,          const Triangle&     triangle,       std::optional<VECTOR>& intersection)
{
    const Plane plane = Plane(triangle.GetCentroid(), triangle.GetNormalVector());
    if (!IsHitPointAndPlane(point, plane))
    {
        intersection = std::nullopt;
        return false;
    }

    const VECTOR v1 = triangle.GetPos(0) - triangle.GetPos(1);
    const VECTOR v2 = triangle.GetPos(1) - triangle.GetPos(2);
    const VECTOR v3 = triangle.GetPos(2) - triangle.GetPos(0);

    const VECTOR n_v1 = math::GetNormalVector(v1, point - triangle.GetPos(1));
    const VECTOR n_v2 = math::GetNormalVector(v2, point - triangle.GetPos(2));
    const VECTOR n_v3 = math::GetNormalVector(v3, point - triangle.GetPos(0));

    const bool is_hit = (triangle.GetNormalVector() != n_v1
                      && triangle.GetNormalVector() != n_v2
                      && triangle.GetNormalVector() != n_v3);

    intersection = is_hit ? std::optional<VECTOR>(point) : std::nullopt;

    return is_hit;
}
bool collision::IsHitPointAndTriangle       (const VECTOR&      point,          const Triangle&     triangle)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitPointAndTriangle(point, triangle, intersection);
}

/// @brief 点と四角形の衝突判定
bool collision::IsHitPointAndSquare         (const VECTOR&      point,          const Square&       square,         std::optional<VECTOR>& intersection)
{
    const bool is_hit = IsHitPointAndTriangle(point, square.GetTriangle(0)) || IsHitPointAndTriangle(point, square.GetTriangle(1));

    intersection = is_hit ? std::optional<VECTOR>(point) : std::nullopt;

    return is_hit;
}
bool collision::IsHitPointAndSquare         (const VECTOR&      point,          const Square&       square)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitPointAndSquare(point, square, intersection);
}

bool collision::IsHitPointAndCapsule        (const VECTOR&      point,          const Capsule&      capsule,        std::optional<VECTOR>& intersection)
{
    const float distance = math::GetDistancePointToSegment(point, capsule.GetSegment());
    const bool  is_hit   = distance <= capsule.GetRadius();

    intersection = is_hit ? std::optional<VECTOR>(point) : std::nullopt;

    return is_hit;
}
bool collision::IsHitPointAndCapsule        (const VECTOR&      point,          const Capsule&      capsule)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitPointAndCapsule(point, capsule, intersection);
}

/// @brief 直線と直線の衝突判定
bool collision::IsHitLineAndLine            (const Line&        line1,          const Line&         line2,          std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    return math::GetDistanceLineToLine(line1, line2) < math::kEpsilonLow;
}
bool collision::IsHitLineAndLine            (const Line&        line1,          const Line&         line2)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitLineAndLine(line1, line2, intersection);
}

/// @brief 直線と直線の衝突判定
bool collision::IsHitLineAndPlane           (const Line&        line,           const Plane&        plane,          std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    const VECTOR v = line.GetPos() - plane.GetPos();

    const bool is_vertical = math::IsVertical(line.GetDir(), plane.GetNormalVector());
    const bool is_contain_plane = math::IsVertical(v, plane.GetNormalVector());

    return is_contain_plane || !is_vertical;
}
bool collision::IsHitLineAndPlane           (const Line&        line,           const Plane&        plane)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitLineAndPlane(line, plane, intersection);
}

/// @brief 直線と直線の衝突判定
bool collision::IsHitSegmentAndSegment      (const Segment&     segment1,       const Segment&      segment2,       std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    return math::GetDistanceSegmentToSegment(segment1, segment2) < math::kEpsilonLow;
}
bool collision::IsHitSegmentAndSegment      (const Segment&     segment1,       const Segment&      segment2)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitSegmentAndSegment(segment1, segment2, intersection);
}

/// @brief 線分と平面(無限に広がる面)の衝突判定
bool collision::IsHitSegmentAndPlane        (const Segment&     segment,        const Plane&        plane,          std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    return math::GetDistanceSegmentToPlane(segment, plane) < math::kEpsilonLow;
}
bool collision::IsHitSegmentAndPlane        (const Segment&     segment,        const Plane&        plane)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitSegmentAndPlane(segment, plane, intersection);
}

/// @brief 線分と三角形の衝突判定
bool collision::IsHitSegmentAndTriangle     (const Segment&     segment,        const Triangle&     triangle,       std::optional<VECTOR>& intersection)
{
    // 三角形を平面に拡張
    const Plane plane = Plane(triangle.GetCentroid(), triangle.GetNormalVector());

    // 平面と衝突していない場合は衝突なし
    if (!IsHitSegmentAndPlane(segment, plane))
    {
        intersection = std::nullopt;
        return false;
    }

    const VECTOR v1 = segment.GetBeginPos() - triangle.GetPos(0);
    const VECTOR v2 = segment.GetEndPos()   - triangle.GetPos(0);
    const float  d1 = math::GetDistancePointToPlane(segment.GetBeginPos(), plane);
    const float  d2 = math::GetDistancePointToPlane(segment.GetEndPos(),   plane);
    const float  a  = d1 / (d1 + d2);
    const VECTOR v3 = (1 - a) * v1 + a * v2;

    // 平面との交点
    intersection = triangle.GetPos(0) + v3;

    // 球に拡張した交点が三角形と衝突しているか
    const Sphere sphere(*intersection, kPointCollisionRadius);
    return IsHitTriangleAndSphere(triangle, sphere);
}
bool collision::IsHitSegmentAndTriangle     (const Segment&     segment,        const Triangle&     triangle)
{
    std::optional<VECTOR> intersection = std::nullopt;
    return IsHitSegmentAndTriangle(segment, triangle, intersection);
}

/// @brief 線分と四角形の衝突判定
bool collision::IsHitSegmentAndSquare       (const Segment&     segment,        const Square&       square,         std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    return IsHitSegmentAndTriangle(segment, square.GetTriangle(0)) || IsHitSegmentAndTriangle(segment, square.GetTriangle(1));
}
bool collision::IsHitSegmentAndSquare       (const Segment&     segment,        const Square&       square)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitSegmentAndSquare(segment, square, intersection);
}

//bool collision::IsHitSegmentAndCircumference(const Segment* segment, const Circle* circle)
//{
//    Plane plane = Plane(circle->GetPos(), circle->GetNormalVector(), 0); // 円がある平面
//    auto intersection = math::GetIntersectionSegmentAndPlane(segment, &plane); // 交点
//
//    // 線分が平面と交点を持つ場合
//    if (intersection)
//    {
//        float minus_nan_ind = intersection->x;
//
//        // 一点で交わっている場合
//        if (std::isfinite(minus_nan_ind))
//        {
//            float distance = VSize(*intersection - circle->GetPos());
//
//            if (distance == circle->GetRadius())
//            {
//                return true;
//            }
//        }
//        // 線分と平面が同一平面上にある場合
//        else
//        {
//            float begin_distance = VSize(segment->GetBeginPosition() - circle->GetPos());
//            float end_distance = VSize(segment->GetEndPosition() - circle->GetPos());
//
//            // 始点から円の中心の距離と、終点から円の中心の距離が、半径より短い場合 ➡ 非衝突
//            if (begin_distance < circle->GetRadius() && end_distance < circle->GetRadius())
//            {
//                return false;
//            }
//
//            // どちらか片方の距離が半径より短く、もう片方が半径より長い場合 ➡ 衝突
//            if ((begin_distance < circle->GetRadius() && end_distance > circle->GetRadius())
//                || (begin_distance > circle->GetRadius() && end_distance < circle->GetRadius()))
//            {
//                return true;
//            }
//
//            // 始点と終点がともに円の外にある場合
//            // 線分と円の中心の最短距離が、半径以下の場合 ➡ 衝突
//            if (math::GetDistancePointToSegment(circle->GetPos(), segment) <= circle->GetRadius())
//            {
//                return true;
//            }
//            return false;
//        }
//    }
//    return false;
//}

///// @brief 線分と球の衝突判定
//bool collision::IsHitSegmentAndSphere       (const Segment&     segment,        const Sphere&       sphere,         std::optional<VECTOR>& intersection)
//{
//    return false;
//}
//bool collision::IsHitSegmentAndSphere       (const Segment&     segment,        const Sphere&       sphere)
//{
//    return false;
//}

/// @brief 線分とカプセルの衝突判定
bool collision::IsHitSegmentAndCapsule      (const Segment&     segment,        const Capsule&      capsule,        std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    const float distance = math::GetDistanceSegmentToSegment(segment, capsule.GetSegment());
    return distance <= capsule.GetRadius();
}
bool collision::IsHitSegmentAndCapsule      (const Segment&     segment,        const Capsule&      capsule)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitSegmentAndCapsule(segment, capsule, intersection);
}

/// @brief 線分とモデルの衝突判定
bool collision::IsHitSegmentAndModel        (const Segment&     segment,        const int           model_handle,   std::optional<VECTOR>& intersection, std::vector<Triangle>& hit_triangles)
{
    hit_triangles.clear();

    const auto hit_result = MV1CollCheck_LineDim(model_handle, -1, segment.GetBeginPos(), segment.GetEndPos());
    if (!hit_result.HitNum)
    {
        intersection = std::nullopt;
        return false;
    }

    // ヒットしたポリゴンから三角形を生成
    // 三角形との距離を取得
    std::unordered_map<int, Triangle>   triangles;
    std::vector<std::pair<int, float>>	distance;
    for (int i = 0; i < hit_result.HitNum; ++i)
    {
        Triangle triangle(hit_result.Dim[i].Position[0], hit_result.Dim[i].Position[2], hit_result.Dim[i].Position[1]);

        triangles[i] = triangle;
        distance.emplace_back(std::make_pair(i, math::GetDistancePointToTriangle(segment.GetBeginPos(), triangle)));
    }

    // 距離が最も近い三角形との交点を取得
    distance = algorithm::Sort(distance, SortKind::kAscending);
    int loop_count = 0;
    for (const auto& dist : distance)
    {
        hit_triangles.emplace_back(triangles.at(dist.first));
        ++loop_count;
    }

    MV1CollResultPolyDimTerminate(hit_result);
    return collision::IsHitSegmentAndTriangle(segment, hit_triangles.at(0), intersection);
}
bool collision::IsHitSegmentAndModel        (const Segment&     segment,        const int           model_handle,   std::optional<VECTOR>& intersection)
{
    std::vector<Triangle> hit_triangles;
    return IsHitSegmentAndModel(segment, model_handle, intersection, hit_triangles);
}
bool collision::IsHitSegmentAndModel        (const Segment&     segment,        const int           model_handle)
{
    std::optional<VECTOR> intersection = std::nullopt;
    return IsHitSegmentAndModel(segment, model_handle, intersection);
}

/// @brief 平面(無限に広がる面)とカプセルの衝突判定
bool collision::IsHitPlaneAndCapsule        (const Plane&       plane,          const Capsule&      capsule,        std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    return math::GetDistancePlaneToCapsule(plane, capsule) < math::kEpsilonLow;
}
bool collision::IsHitPlaneAndCapsule        (const Plane&       plane,          const Capsule&      capsule)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitPlaneAndCapsule(plane, capsule, intersection);
}

/// @brief 三角形と球の衝突判定
bool collision::IsHitTriangleAndSphere      (const Triangle&    triangle,       const Sphere&       sphere,         std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    return math::GetDistancePointToTriangle(sphere.GetPos(), triangle) <= sphere.GetRadius();
}
bool collision::IsHitTriangleAndSphere      (const Triangle&    triangle,       const Sphere&       sphere)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitTriangleAndSphere(triangle, sphere, intersection);
}

/// @brief 三角形と三角形の衝突判定
bool collision::IsHitTriangleAndTriangle    (const Triangle&    triangle1,      const Triangle&     triangle2,      std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    return math::GetDistanceTriangleToTriangle(triangle1, triangle2) < math::kEpsilonLow;
}
bool collision::IsHitTriangleAndTriangle    (const Triangle&    triangle1,      const Triangle&     triangle2)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitTriangleAndTriangle(triangle1, triangle2, intersection);
}

/// @brief 三角形とカプセルの衝突判定
bool collision::IsHitTriangleAndCapsule     (const Triangle&    triangle,       const Capsule&      capsule,        std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    return math::GetDistanceSegmentToTriangle(capsule.GetSegment(), triangle) <= capsule.GetRadius();
}
bool collision::IsHitTriangleAndCapsule     (const Triangle&    triangle,       const Capsule&      capsule)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitTriangleAndCapsule(triangle, capsule, intersection);
}

/// @brief 三角形と光線カプセルの衝突判定
bool collision::IsHitTriangleAndRayCapsule  (const Triangle&    triangle,       const RayCapsule&   ray_capsule,    std::optional<VECTOR>& intersection)
{
    intersection    = std::nullopt;
    Capsule capsule = ray_capsule.GetBeginCapsule();

    // 速度を分割
    VECTOR velocity = ray_capsule.GetDir() * ray_capsule.GetRayLength();
    velocity *= (1.0f / kRayCapsuleDivisionNum);

    for (int i = 0; i < kRayCapsuleDivisionNum + 1; ++i)
    {
        if (IsHitTriangleAndCapsule(triangle, capsule, intersection))
        {
            return true;
        }

        capsule.Move(velocity);
    }

    return false;
}
bool collision::IsHitTriangleAndRayCapsule  (const Triangle&    triangle,       const RayCapsule&   ray_capsule)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitTriangleAndRayCapsule(triangle, ray_capsule, intersection);
}

/// @brief 三角形とモデルの衝突判定
bool collision::IsHitTriangleAndModel       (const Triangle&    triangle,       const int           model_handle,   std::optional<VECTOR>& intersection, std::vector<Triangle>& hit_triangles)
{
    hit_triangles.clear();

    const auto hit_result = MV1CollCheck_Triangle(model_handle, -1, triangle.GetPos(0), triangle.GetPos(1), triangle.GetPos(2));
    if (!hit_result.HitNum)
    {
        intersection = std::nullopt;
        return false;
    }

    // ヒットしたポリゴンから三角形を生成
    // 三角形との距離を取得
    std::unordered_map<int, Triangle>   triangles;
    std::vector<std::pair<int, float>>	distance;
    for (int i = 0; i < hit_result.HitNum; ++i)
    {
        hit_triangles.emplace_back(Triangle(hit_result.Dim[i].Position[0], hit_result.Dim[i].Position[2], hit_result.Dim[i].Position[1]));

        triangles[i] = hit_triangles.at(i);
        distance.emplace_back(std::make_pair(i, math::GetDistanceTriangleToTriangle(triangle, hit_triangles.at(i))));
    }

    // 距離が最も近い三角形との交点を取得
    distance = algorithm::Sort(distance, SortKind::kAscending);
    int loop_count = 0;
    for (const auto& dist : distance)
    {
        hit_triangles.at(loop_count) = triangles.at(dist.first);
        ++loop_count;
    }

    MV1CollResultPolyDimTerminate(hit_result);
    return collision::IsHitTriangleAndTriangle(triangle, hit_triangles.at(0), intersection);
}
bool collision::IsHitTriangleAndModel       (const Triangle&    triangle,       const int           model_handle,   std::optional<VECTOR>& intersection)
{
    std::vector<Triangle> hit_triangles{};
    return IsHitTriangleAndModel(triangle, model_handle, intersection, hit_triangles);
}
bool collision::IsHitTriangleAndModel       (const Triangle&    triangle,       const int           model_handle)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitTriangleAndModel(triangle, model_handle, intersection);
}

/// @brief 四角形とカプセルの衝突判定
bool collision::IsHitSquareAndCapsule       (const Square&      square,         const Capsule&      capsule,        std::optional<VECTOR>& intersection)
{
   intersection = std::nullopt;

    const float distance1 = math::GetDistanceSegmentToTriangle(capsule.GetSegment(), square.GetTriangle(0));
    const float distance2 = math::GetDistanceSegmentToTriangle(capsule.GetSegment(), square.GetTriangle(1));

    return distance1 <= capsule.GetRadius() || distance2 <= capsule.GetRadius();
}
bool collision::IsHitSquareAndCapsule       (const Square&      square,         const Capsule&      capsule)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitSquareAndCapsule(square, capsule, intersection);
}

/// @brief 四角形と光線カプセルの衝突判定
bool collision::IsHitSquareAndRayCapsule    (const Square&      square,         const RayCapsule&   ray_capsule,    std::optional<VECTOR>& intersection)
{
    intersection    = std::nullopt;
    Capsule capsule = ray_capsule.GetBeginCapsule();

    // 速度を分割
    VECTOR velocity = ray_capsule.GetDir() * ray_capsule.GetRayLength();
    velocity *= (1.0f / kRayCapsuleDivisionNum);

    for (int i = 0; i < kRayCapsuleDivisionNum + 1; ++i)
    {
        if (IsHitSquareAndCapsule(square, capsule, intersection))
        {
            return true;
        }

        capsule.Move(velocity);
    }

    return false;
}
bool collision::IsHitSquareAndRayCapsule    (const Square&      square,         const RayCapsule&   ray_capsule)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitSquareAndRayCapsule(square, ray_capsule, intersection);
}

/// @brief 球と球の衝突判定
bool collision::IsHitSphereAndSphere        (const Sphere&      sphere1,        const Sphere&       sphere2,        std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    const VECTOR p1       = sphere1.GetPos();
    const VECTOR p2       = sphere2.GetPos();
    const float  distance = VSize(p2 - p1);

    return distance <= sphere1.GetRadius() + sphere2.GetRadius();
}
bool collision::IsHitSphereAndSphere        (const Sphere&      sphere1,        const Sphere&       sphere2)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitSphereAndSphere(sphere1, sphere2, intersection);
}

/// @brief 球とカプセルの衝突判定 
bool collision::IsHitSphereAndCapsule       (const Sphere&      sphere,         const Capsule&      capsule,        std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    const VECTOR p = sphere.GetPos();
    const float distance = math::GetDistancePointToSegment(p, capsule.GetSegment());

    return distance <= sphere.GetRadius() + capsule.GetRadius();
}
bool collision::IsHitSphereAndCapsule       (const Sphere&      sphere,         const Capsule&      capsule)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitSphereAndCapsule(sphere, capsule, intersection);
}

/// @brief 球とモデルの衝突判定 
bool collision::IsHitSphereAndModel         (const Sphere&      sphere,         const int           model_handle,   std::optional<VECTOR>& intersection, std::vector<Triangle>& hit_triangles)
{
    hit_triangles.clear();

    const auto hit_result = MV1CollCheck_Sphere(model_handle, -1, sphere.GetPos(), sphere.GetRadius());
    if (!hit_result.HitNum)
    {
        intersection = std::nullopt;
        return false;
    }

    // ヒットしたポリゴンから三角形を生成
    // 三角形との距離を取得
    std::unordered_map<int, Triangle>   triangles;
    std::vector<std::pair<int, float>>	distance;
    for (int i = 0; i < hit_result.HitNum; ++i)
    {
        hit_triangles.emplace_back(Triangle(hit_result.Dim[i].Position[0], hit_result.Dim[i].Position[2], hit_result.Dim[i].Position[1]));

        triangles[i] = hit_triangles.at(i);
        distance.emplace_back(std::make_pair(i, math::GetDistanceTriangleToSphere(hit_triangles.at(i), sphere)));
    }

    // 距離が最も近い三角形との交点を取得
    distance = algorithm::Sort(distance, SortKind::kAscending);
    int loop_count = 0;
    for (const auto& dist : distance)
    {
        hit_triangles.at(loop_count) = triangles.at(dist.first);
        ++loop_count;
    }

    MV1CollResultPolyDimTerminate(hit_result);
    return collision::IsHitTriangleAndSphere(hit_triangles.at(0), sphere, intersection);
}
bool collision::IsHitSphereAndModel         (const Sphere&      sphere,         const int           model_handle,   std::optional<VECTOR>& intersection)
{
    std::vector<Triangle> hit_triangles{};
    return IsHitSphereAndModel(sphere, model_handle, intersection, hit_triangles);
}
bool collision::IsHitSphereAndModel         (const Sphere&      sphere,         const int           model_handle)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitSphereAndModel(sphere, model_handle, intersection);
}

/// @brief カプセルとカプセルの衝突判定
bool collision::IsHitCapsuleAndCapsule      (const Capsule&     capsule1,       const Capsule&      capsule2,       std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    const float distance = math::GetDistanceSegmentToSegment(capsule1.GetSegment(), capsule2.GetSegment());
    return distance <= capsule1.GetRadius() + capsule2.GetRadius();
}
bool collision::IsHitCapsuleAndCapsule      (const Capsule&     capsule1,       const Capsule&      capsule2)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitCapsuleAndCapsule(capsule1, capsule2, intersection);
}

/// @brief カプセルとモデルの衝突判定
bool collision::IsHitCapsuleAndModel        (const Capsule&     capsule,        const int           model_handle,   std::optional<VECTOR>& intersection, std::vector<Triangle>& hit_triangles)
{
    hit_triangles.clear();

    const auto hit_result = MV1CollCheck_Capsule(model_handle, -1, capsule.GetSegment().GetBeginPos(), capsule.GetSegment().GetEndPos(), capsule.GetRadius());
    if (!hit_result.HitNum)
    {
        intersection = std::nullopt;
        return false;
    }

    // ヒットしたポリゴンから三角形を生成
    // 三角形との距離を取得
    std::unordered_map<int, Triangle>   triangles;
    std::vector<std::pair<int, float>>	distance;
    for (int i = 0; i < hit_result.HitNum; ++i)
    {
        hit_triangles.emplace_back(Triangle(hit_result.Dim[i].Position[0], hit_result.Dim[i].Position[2], hit_result.Dim[i].Position[1]));

        triangles[i] = hit_triangles.at(i);
        distance.emplace_back(std::make_pair(i, math::GetDistanceTriangleToCapsule(hit_triangles.at(i), capsule)));
    }

    // 三角形を距離が近い順にソート
    distance = algorithm::Sort(distance, SortKind::kAscending);
    int loop_count = 0;
    for (const auto& dist : distance)
    {
        hit_triangles.at(loop_count) = triangles.at(dist.first);
        ++loop_count;
    }

    MV1CollResultPolyDimTerminate(hit_result);
    return collision::IsHitTriangleAndCapsule(hit_triangles.at(0), capsule, intersection);
}
bool collision::IsHitCapsuleAndModel        (const Capsule&     capsule,        const int           model_handle,   std::optional<VECTOR>& intersection)
{
    std::vector<Triangle> hit_triangles{};
    return IsHitCapsuleAndModel(capsule, model_handle, intersection, hit_triangles);
}
bool collision::IsHitCapsuleAndModel        (const Capsule&     capsule,        const int           model_handle,   MV1_COLL_RESULT_POLY_DIM& hit_result)
{
    const Segment segment = capsule.GetSegment();
    hit_result = MV1CollCheck_Capsule(model_handle, -1, segment.GetBeginPos(), segment.GetEndPos(), capsule.GetRadius());
    return hit_result.HitNum;
}
bool collision::IsHitCapsuleAndModel        (const Capsule&     capsule,        const int           model_handle)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitCapsuleAndModel(capsule, model_handle, intersection);
}

/// @brief 光線カプセルとモデルの衝突判定
bool collision::IsHitRayCapsuleAndModel     (const RayCapsule&  ray_capsule,    const int           model_handle,   std::optional<VECTOR>& intersection)
{
    intersection    = std::nullopt;
    Capsule capsule = ray_capsule.GetBeginCapsule();

    // 速度を分割
    VECTOR velocity = ray_capsule.GetDir() * ray_capsule.GetRayLength();
    velocity *= (1.0f / kRayCapsuleDivisionNum);

    for (int i = 0; i < kRayCapsuleDivisionNum + 1; ++i)
    {
        if (MV1CollCheck_Capsule(model_handle, -1, capsule.GetSegment().GetBeginPos(), capsule.GetSegment().GetEndPos(), capsule.GetRadius()).HitNum)
        {
            return true;
        }
        
        capsule.Move(velocity);
    }

    return false;
}
bool collision::IsHitRayCapsuleAndModel     (const RayCapsule&  ray_capsule,    const int           model_handle,   MV1_COLL_RESULT_POLY_DIM& hit_result)
{
    Capsule capsule = ray_capsule.GetBeginCapsule();

    // 速度を分割
    VECTOR velocity = ray_capsule.GetDir() * ray_capsule.GetRayLength();
    velocity *= (1.0f / kRayCapsuleDivisionNum);

    for (int i = 0; i < kRayCapsuleDivisionNum + 1; ++i)
    {
        hit_result = MV1CollCheck_Capsule(model_handle, -1, capsule.GetSegment().GetBeginPos(), capsule.GetSegment().GetEndPos(), capsule.GetRadius());
        if (hit_result.HitNum)
        {
            return true;
        }

        capsule.Move(velocity);
    }

    return false;
}
bool collision::IsHitRayCapsuleAndModel     (const RayCapsule&  ray_capsule,    const int           model_handle)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsHitRayCapsuleAndModel(ray_capsule, model_handle, intersection);
}

//bool collision::IsHitCircumferenceAndCapsuleLowPrecision(const Circle* circle, const Capsule* capsule)
//{
//    Plane plane = Plane(circle->GetPos(), circle->GetNormalVector(), 0);
//
//    if (!IsHitPlaneAndCapsule(&plane, capsule)) { return false; }
//
//    // 始点から平面の最短距離座標を取得
//    float distance1 = math::GetDistancePointToPlane(capsule->GetSegment()->GetBeginPosition(), &plane);
//    VECTOR v = plane.GetNormalVector() * distance1;
//    if (math::IsPointAheadOfPlane(capsule->GetSegment()->GetBeginPosition(), &plane))
//    {
//        v *= -1;
//    }
//    VECTOR p = capsule->GetSegment()->GetBeginPosition() + v;
//
//    // 斜辺の長さを取得
//    VECTOR hypotenuse_direction = capsule->GetSegment()->GetDir();
//    float angle1 = math::GetAngleBetweenTwoVector(capsule->GetSegment()->GetDir(), v);
//    float angle2 = math::GetAngleBetweenTwoVector(-capsule->GetSegment()->GetDir(), v);
//    float hypotenuse_length = distance1 / cos(angle1);
//    if (angle1 > angle2)
//    {
//        hypotenuse_length = distance1 / cos(angle2);
//        hypotenuse_direction *= -1;
//    }
//
//    // TODO : 欠陥あり
//    // カプセルの上部が少しだけ平面に衝突した場合や、カプセルが傾いて衝突した場合に計算がずれる
//    // 交点を取得し、半径を考慮し衝突判定を行う
//    VECTOR intersection = capsule->GetSegment()->GetBeginPosition() + hypotenuse_direction * hypotenuse_length;
//    float distance2 = VSize(circle->GetPos() - intersection);
//
//    return distance2 >= circle->GetRadius() - capsule->GetRadius()
//        && distance2 <= circle->GetRadius() + capsule->GetRadius();
//}
#pragma endregion


#pragma region 押し戻し(衝突時の有効な速度ベクトルを取得)
VECTOR collision::PushBackSphereAndTriangle (const VECTOR& velocity, const Sphere&  dynamic_sphere,  const Triangle& static_triangle,
    const float slope_difficulty_angle_threshold, const float max_slope_angle)
{
    // 未来の球を取得
    Sphere future_sphere = dynamic_sphere;
    future_sphere.Move(velocity);

    //// 未来の座標と衝突しているかを判定
    //if (!IsHitTriangleAndSphere(static_triangle, dynamic_sphere))
    //{
    //    return velocity;
    //}

    // 未来の座標から生成されたカプセルと衝突しているかを判定
    if (!IsHitTriangleAndCapsule(static_triangle, Capsule(dynamic_sphere.GetPos(), future_sphere.GetPos(), dynamic_sphere.GetRadius())))
    {
        // 衝突していない場合、そのまま返す
        return velocity;
    }

    // 未来の座標と平面の距離を取得
    const Plane plane = Plane(static_triangle.GetCentroid(), static_triangle.GetNormalVector());
    VECTOR future_pos = future_sphere.GetPos();
    const float future_distance_to_square = math::GetDistancePointToTriangle(future_pos, static_triangle);

    // 線分の位置からどちら側に位置修正するべきか
    VECTOR closest_dir = plane.GetNormalVector();
    if (math::IsPointAheadOfPlane(future_pos, plane))
    {
        closest_dir *= -1;
    }

    // 本来の到達地点までのvelocityを取得
    future_pos += closest_dir * future_distance_to_square;
    future_pos += plane.GetNormalVector() * dynamic_sphere.GetRadius();
    return future_pos - dynamic_sphere.GetPos();
}

VECTOR collision::PushBackSphereAndModel    (const VECTOR& velocity, const Sphere&  dynamic_sphere,  const int       model_handle,
    const float slope_difficulty_angle_threshold, const float max_slope_angle)
{
    VECTOR valid_velocity = velocity;

    // 未来の球を取得
    Sphere future_sphere = dynamic_sphere;
    future_sphere.Move(velocity);

    // 未来の座標から生成されたカプセルと衝突しているかを判定
    const auto hit_result = MV1CollCheck_Capsule(model_handle, -1, dynamic_sphere.GetPos(), dynamic_sphere.GetPos(), future_sphere.GetRadius());
    if (!hit_result.HitNum)
    {
        return velocity;
    }

    // ヒットしたポリゴンから三角形を生成
    // 三角形との現在の距離を取得
    std::unordered_map<int, Triangle>   triangles;
    std::vector<std::pair<int, float>>	current_distance;
    for (int i = 0; i < hit_result.HitNum; ++i)
    {
        Triangle triangle(hit_result.Dim[i].Position[0], hit_result.Dim[i].Position[2], hit_result.Dim[i].Position[1]);

        triangles[i] = triangle;
        current_distance.emplace_back(std::make_pair(i, math::GetDistanceTriangleToSphere(triangle, dynamic_sphere)));
    }

    // 距離が近い順に押し戻す
    current_distance = algorithm::Sort(current_distance, SortKind::kAscending);
    for (const auto& distance : current_distance)
    {
        valid_velocity = collision::PushBackSphereAndTriangle(valid_velocity, dynamic_sphere, triangles.at(distance.first), slope_difficulty_angle_threshold, max_slope_angle);
    }

    MV1CollResultPolyDimTerminate(hit_result);
    return valid_velocity;
}

VECTOR collision::PushBackCapsuleAndTriangle(const VECTOR& velocity, const Capsule& dynamic_capsule, const Triangle& static_triangle,
    const float slope_difficulty_angle_threshold, const float max_slope_angle)
{
    //// 未来のカプセルを取得
    //Capsule future_capsule = dynamic_capsule;
    //future_capsule.Move(velocity);
    //
    //// 未来の座標と衝突しているかを判定
    //if (!IsHitTriangleAndCapsule(static_triangle, future_capsule))
    //{
    //    return velocity;
    //}
    //
    //// 三角形を平面に拡張
    //const Plane  plane = Plane(static_triangle.GetCentroid(), static_triangle.GetNormalVector());
    //const float  angle = math::GetAngleBetweenTwoVector(axis::GetWorldYAxis(), plane.GetNormalVector());
    //VECTOR       future_begin_pos = future_capsule.GetSegment().GetBeginPos();
    //VECTOR       future_origin_pos = future_begin_pos;
    //
    //// 三角形が下を向いている場合、終点を基準に押し戻す
    //if (angle > 90.0f * math::kDegToRad)
    //{
    //    future_origin_pos = future_capsule.GetSegment().GetEndPos();
    //}
    //
    //// 線分の位置からどちら側に位置修正するべきか
    //VECTOR closest_dir = plane.GetNormalVector();
    //if (math::IsPointAheadOfPlane(future_origin_pos, plane))
    //{
    //    closest_dir *= -1;
    //}
    //
    //// 本来の到達地点を取得
    //const float future_distance_to_plane = math::GetDistancePointToPlane(future_origin_pos, plane);
    //future_begin_pos += closest_dir * future_distance_to_plane;
    //future_begin_pos += plane.GetNormalVector() * dynamic_capsule.GetRadius();
    //
    //// 本来の到達地点までのvelocityを取得
    //return future_begin_pos - dynamic_capsule.GetSegment().GetBeginPos();

    // 未来のカプセルを取得
    Capsule future_capsule = dynamic_capsule;
    future_capsule.Move(velocity);
    
    // 未来の座標と衝突しているかを判定
    if (!IsHitTriangleAndCapsule(static_triangle, future_capsule))
    {
        return velocity;
    }
    
    const auto  plane                           = Plane(static_triangle.GetCentroid(), static_triangle.GetNormalVector());
    const auto  future_begin_pos                = future_capsule.GetSegment().GetBeginPos();
    const auto  future_end_pos                  = future_capsule.GetSegment().GetEndPos();
    const auto  plane_to_begin_pos_distance     = math::GetDistancePointToPlane(future_begin_pos, plane);
    const auto  plane_to_end_pos_distance       = math::GetDistancePointToPlane(future_end_pos,   plane);
    const auto  is_begin_pos_ahead_of_plane     = math::IsPointAheadOfPlane(future_begin_pos, plane);
    const auto  is_end_pos_ahead_of_plane       = math::IsPointAheadOfPlane(future_end_pos,   plane);
    float       penetration_depth               = 0.0f;
    bool        is_ahead_of_plane               = false;
    
    // めり込んだ距離を取得
    // 始点・終点がどちらも平面の前方にある場合は近い方を採用
    if (is_begin_pos_ahead_of_plane && is_end_pos_ahead_of_plane)
    {
        penetration_depth = min(plane_to_begin_pos_distance, plane_to_end_pos_distance);
        is_ahead_of_plane = true;
    }
    // 始点・終点がどちらも平面の後方にある場合は遠い方を採用
    else if (!is_begin_pos_ahead_of_plane && !is_end_pos_ahead_of_plane)
    {
        penetration_depth = max(plane_to_begin_pos_distance, plane_to_end_pos_distance);
    }
    // 始点が平面の前面にあり、終点が後方にある場合は終点からの距離を採用
    else if (is_begin_pos_ahead_of_plane && !is_end_pos_ahead_of_plane)
    {
        penetration_depth = plane_to_end_pos_distance;
    }
    // 終点が平面の前面にあり、始点が後方にある場合は視点からの距離を採用
    else
    {
        penetration_depth = plane_to_begin_pos_distance;
    }
    
    // velocityと三角形に沿ったベクトルの角度を取得
    const auto  plane_cross_v1      = math::GetNormalVector(plane.GetNormalVector(), axis::GetWorldYAxis());
    auto        plane_cross_v2      = math::GetNormalVector(plane.GetNormalVector(), plane_cross_v1);
    const auto  angle               = 90.0f * math::kDegToRad - math::GetAngleBetweenTwoVector(-plane.GetNormalVector(), v3d::GetNormalizedV(velocity));
    const auto  sub_length          = math::GetHypotenuseLengthRightTriangleFromOpposite(penetration_depth, angle);
    auto        add_length          = math::GetRatio<float, float>(sub_length, penetration_depth, dynamic_capsule.GetRadius());
    const auto  push_back_length    = sub_length + add_length;

    //if (is_ahead_of_plane) { add_length -= sub_length; }
    
    const auto  sub_v               = v3d::GetNormalizedV(velocity) * push_back_length;
    const auto  valid_velocity      = velocity - sub_v;
    auto        fix_capsule         = dynamic_capsule;
    fix_capsule.Move(valid_velocity);
    
    const auto  wall_slide_velocity = math::GetProjectionVector(sub_v, plane_cross_v2);
    auto        result_capsule      = fix_capsule;
    result_capsule.Move(wall_slide_velocity);
    
    const auto  fix_velocity        = result_capsule.GetSegment().GetBeginPos() - future_begin_pos;
    return velocity + fix_velocity;
}

VECTOR collision::PushBackCapsuleAndSquare  (const VECTOR& velocity, const Capsule& dynamic_capsule, const Square&   static_square,
    const float slope_difficulty_angle_threshold, const float max_slope_angle)
{
    // 未来のカプセルを取得
    Capsule future_capsule = dynamic_capsule;
    future_capsule.Move(velocity);

    // 未来の座標と衝突しているかを判定
    if (!IsHitSquareAndCapsule(static_square, future_capsule))
    {
        return velocity;
    }

    // 未来の座標と平面の距離を取得
    const Plane plane = Plane(static_square.GetCentroid(), static_square.GetNormalVector());
    VECTOR future_pos = future_capsule.GetSegment().GetBeginPos();
    const float future_distance_to_square = math::GetDistancePointToSquare(future_pos, static_square);

    // 線分の位置からどちら側に位置修正するべきか
    VECTOR closest_dir = plane.GetNormalVector();
    if (math::IsPointAheadOfPlane(future_pos, plane))
    {
        closest_dir *= -1;
    }

    // 本来の到達地点までのvelocityを取得
    future_pos += closest_dir * future_distance_to_square;
    future_pos += plane.GetNormalVector() * dynamic_capsule.GetRadius();
    return future_pos - dynamic_capsule.GetSegment().GetBeginPos();
}

VECTOR collision::PushBackCapsuleAndOBB     (const VECTOR& velocity, const Capsule& dynamic_capsule, const OBB&      static_obb,
    const float slope_difficulty_angle_threshold, const float max_slope_angle)
{
    VECTOR valid_velocity = velocity;
    std::vector<std::pair<box::SquareKind, float>> current_distance;
    std::vector<std::pair<box::SquareKind, float>> future_distance;

    // 未来のカプセルを取得
    Capsule future_capsule = dynamic_capsule;
    future_capsule.Move(velocity);

    for (int i = 0; i < BoxData::kSquareNum; ++i)
    {
        // 現在の座標とすべての四角形の距離を取得
        const auto current_dist = math::GetDistancePointToSquare(
            dynamic_capsule.GetSegment().GetBeginPos(), static_obb.GetSquare(static_cast<box::SquareKind>(i)));
        current_distance.emplace_back(std::make_pair(static_cast<box::SquareKind>(i), current_dist));

        // 未来の座標とすべての四角形の距離を取得
        const auto future_dist = math::GetDistancePointToSquare(
            future_capsule.GetSegment().GetBeginPos(), static_obb.GetSquare(static_cast<box::SquareKind>(i)));
        future_distance.emplace_back(std::make_pair(static_cast<box::SquareKind>(i), future_dist));
    }

    // 現在の距離が近い順にソート
    // 距離が同じ場合は、同じもの同士で未来の座標が近い順にソート
    current_distance = algorithm::Sort(current_distance, future_distance, SortKind::kAscending, SortKind::kAscending);

    // 移動前の座標と距離が近い四角形から順番に押し戻す
    for (const auto& dist : current_distance)
    {
        valid_velocity = PushBackCapsuleAndSquare(valid_velocity, dynamic_capsule, static_obb.GetSquare(static_cast<box::SquareKind>(dist.first)), slope_difficulty_angle_threshold, max_slope_angle);
    }

    return valid_velocity;
}

VECTOR collision::PushBackCapsuleAndModel   (const VECTOR& velocity, const Capsule& dynamic_capsule, const int       model_handle,
    const float slope_difficulty_angle_threshold, const float max_slope_angle)
{
    VECTOR valid_velocity = velocity;
    MV1_COLL_RESULT_POLY_DIM hit_result;

    // 未来のカプセルを取得
    Capsule future_capsule = dynamic_capsule;
    future_capsule.Move(velocity);

    // 未来の座標と衝突しているかを判定
    if (!IsHitCapsuleAndModel(future_capsule, model_handle, hit_result))
    {
        return velocity;
    }

    // ヒットしたポリゴンから三角形を生成
    // 三角形との現在の距離を取得
    std::unordered_map<int, Triangle>   triangles;
    std::vector<std::pair<int, float>>  current_distance;
    std::vector<std::pair<int, float>>  current_angle;
    for (int i = 0; i < hit_result.HitNum; ++i)
    {
        Triangle triangle(hit_result.Dim[i].Position[0], hit_result.Dim[i].Position[2], hit_result.Dim[i].Position[1]);

        triangles[i] = triangle;
        current_distance.emplace_back(std::make_pair(i, math::GetDistanceTriangleToCapsule(triangle, dynamic_capsule)));
        current_angle   .emplace_back(std::make_pair(i, math::GetAngleBetweenTwoVector(v3d::GetNormalizedV(velocity), triangle.GetNormalVector())));
    }

    // 距離が近い順に押し戻す
    // 距離が同じであればvelocityと法線の角度が大きい順に押し出す
    current_distance = algorithm::Sort(current_distance, current_angle, SortKind::kAscending, SortKind::kDescending);
    for (const auto& distance : current_distance)
    {
        valid_velocity = collision::PushBackCapsuleAndTriangle(valid_velocity, dynamic_capsule, triangles.at(distance.first), slope_difficulty_angle_threshold, max_slope_angle);
    }

    return valid_velocity;
}
#pragma endregion
