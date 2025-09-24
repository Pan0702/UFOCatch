#include "MeshCollider.h"
#include "ResourceManager.h"

namespace {
    // スキンメッシュを変形させるときのフレームポーズ変換リミット
    // (０に近いほど正確だが重くなる)
    const float FrameLimit = 8;
};


MeshCollider::MeshCollider()
{
    parent = nullptr;
}

MeshCollider::MeshCollider(Object3D* object)
{
    parent = object;
}

MeshCollider::~MeshCollider()
{
}

void MeshCollider::MakeFromMesh(CFbxMesh* meshIn, Animator* animatorIn)
{
    mesh = meshIn;
    animator = animatorIn;
    if (mesh->m_nMeshType == 1) animator = nullptr;
    id = -1;
    frame = 0;

    std::vector<PolygonInfo> pi;
    std::vector<Vertex> ve;

    // CFbxMeshからデータを取得する
    // 複数メッシュに別れているので、各メッシュごとに作成する
    for (DWORD m = 0; m < mesh->m_dwMeshNum; m++) {
        vertices.emplace_back(ve);
        polygons.emplace_back(pi);
        CFbxMeshArray* arr = &mesh->m_pMeshArray[m];

        // 頂点情報
        int vNum = arr->m_dwVerticesNum;
        vertices.back().reserve(vertices.size() + vNum);
        for (int v = 0; v < vNum; v++) {
            Vertex vt = {};
            if (mesh->m_nMeshType == 1)  // スタティックメッシュ
            {
                vt.pos = arr->m_vStaticVerticesNormal[v].Pos;
                vertices.back().push_back(vt);
            }
            else {     // スキンメッシュ
                vt.pos = arr->m_vSkinVerticesNormal[v].Pos;
                vt.bone[0] = arr->m_vSkinVerticesNormal[v].ClusterNum[0];
                vt.bone[1] = arr->m_vSkinVerticesNormal[v].ClusterNum[1];
                vt.bone[2] = arr->m_vSkinVerticesNormal[v].ClusterNum[2];
                vt.bone[3] = arr->m_vSkinVerticesNormal[v].ClusterNum[3];
                vt.weits = arr->m_vSkinVerticesNormal[v].Weits;
                vertices.back().push_back(vt);
            }
        }

        // インデックス情報
        int iNum = arr->m_dwIndicesNum / 3;
        polygons.back().reserve(polygons.size() + iNum);
        for (int i = 0; i < iNum; i++) {
            PolygonInfo inf;
            inf.indices[0] = arr->m_nIndices[i * 3 + 0];
            inf.indices[1] = arr->m_nIndices[i * 3 + 1];
            inf.indices[2] = arr->m_nIndices[i * 3 + 2];
            VECTOR3 v0 = vertices.back()[inf.indices[0]].pos;
            VECTOR3 v1 = vertices.back()[inf.indices[1]].pos;
            VECTOR3 v2 = vertices.back()[inf.indices[2]].pos;
            inf.normal = XMVector3Normalize(XMVector3Cross(v1 - v0, v2 - v0));
            polygons.back().push_back(inf);
        }
    }

    // AABBバウンディングボックス
    bBox.min = mesh->m_vMin;
    bBox.max = mesh->m_vMax;

    // バウンディングボール
    bBall.center = (bBox.min + bBox.max) / 2;   // ここがメッシュの中心になる
    bBall.radius = magnitude(bBox.max - bBall.center);    // メッシュの半径(メッシュを全て囲む最大半径)
}

void MeshCollider::MakeFromFile(std::string fileName)
{
    CFbxMesh* meshIn = new CFbxMesh();
    meshIn->Load(fileName.c_str());
    MakeFromMesh(meshIn);
    SAFE_DELETE(meshIn);
}

int MeshCollider::SelectBoneNo(Vertex vt[3])
{
    int bone = 0;

    if (vt[0].bone[0] == vt[1].bone[0] || vt[0].bone[0] == vt[2].bone[0])    // ２頂点以上が同一のボーン番号のとき
    {
        bone = vt[0].bone[0];
    }
    else if (vt[1].bone[0] == vt[2].bone[0])      // ２頂点以上が同一のボーン番号のとき
    {
        bone = vt[1].bone[0];
    }
    else {  // ３頂点ともボーン番号が異なるときはウェイトの高いものを優先する
        if (vt[0].weits.x > vt[1].weits.x)
        {
            if (vt[0].weits.x > vt[2].weits.x) bone = vt[0].bone[0];
            else bone = vt[2].bone[0];
        }
        else {
            if (vt[1].weits.x > vt[2].weits.x) bone = vt[1].bone[0];
            else bone = vt[2].bone[0];
        }
    }

    return bone;
}

// スキンメッシュのときボーン行列による変形を行う
void MeshCollider::transformSkinVertices()
{
    if (animator == nullptr)  return;
    if (animator->PlayingID() == id)
    {
        if (animator->CurrentFrame() <= frame + FrameLimit && animator->CurrentFrame() >= frame - FrameLimit) 
            return;
    }
    id = animator->PlayingID();
    frame = animator->CurrentFrame();

    for (int m = 0; m < mesh->m_dwMeshNum; m++)
    {
        // ボーン行列による頂点の変形
        int i = 0;
        for (Vertex& vt : vertices[m])
        {
            vt.pos = VECTOR3(0, 0, 0);
            if (vt.bone[0] < 254) vt.pos += vt.weits.x * XMVector3TransformCoord(mesh->m_pMeshArray[m].m_vSkinVerticesNormal[i].Pos, XMMatrixTranspose(mesh->m_pMeshArray[m].m_pBoneShader[id][frame].shaderFramePose[vt.bone[0]]));
            if (vt.bone[1] < 254) vt.pos += vt.weits.y * XMVector3TransformCoord(mesh->m_pMeshArray[m].m_vSkinVerticesNormal[i].Pos, XMMatrixTranspose(mesh->m_pMeshArray[m].m_pBoneShader[id][frame].shaderFramePose[vt.bone[1]]));
            if (vt.bone[2] < 254) vt.pos += vt.weits.z * XMVector3TransformCoord(mesh->m_pMeshArray[m].m_vSkinVerticesNormal[i].Pos, XMMatrixTranspose(mesh->m_pMeshArray[m].m_pBoneShader[id][frame].shaderFramePose[vt.bone[2]]));
            if (vt.bone[3] < 254) vt.pos += vt.weits.w * XMVector3TransformCoord(mesh->m_pMeshArray[m].m_vSkinVerticesNormal[i].Pos, XMMatrixTranspose(mesh->m_pMeshArray[m].m_pBoneShader[id][frame].shaderFramePose[vt.bone[3]]));
            i++;
        }

        // 面法線の再計算
        for ( PolygonInfo &inf : polygons[m] )
        {
            VECTOR3 v0 = vertices[m][inf.indices[0]].pos;
            VECTOR3 v1 = vertices[m][inf.indices[1]].pos;
            VECTOR3 v2 = vertices[m][inf.indices[2]].pos;
            inf.normal = XMVector3Normalize(XMVector3Cross(v1 - v0, v2 - v0));
        }
    }
}

bool MeshCollider::CheckCollisionLine(const MATRIX4X4& trans, const VECTOR3& from, const VECTOR3& to, MeshCollider::CollInfo* hitOut)
{
    MATRIX4X4 invTrans = XMMatrixInverse(nullptr, trans);
    VECTOR3 invFrom = from * invTrans;
    VECTOR3 invTo = to * invTrans;

    // バウンディングボールで判定
    //VECTOR3 center = (invTo - invFrom) / 2.0f;   // ??????????????
    VECTOR3 center = (invTo + invFrom) / 2.0f;
    float radius = (invTo - invFrom).Length() / 2.0f;
    float radiusSq = (radius + bBall.radius) * (radius + bBall.radius);
    if ((center - bBall.center).LengthSquare() > radiusSq) {
        return false;
    }
    // AABBバウンディングボックスで判定(スタティックメッシュのみ)
    if (animator == nullptr)
    {
        if (bBox.min.x > invFrom.x && bBox.min.x > invTo.x) return false;
        if (bBox.max.x < invFrom.x && bBox.max.x < invTo.x) return false;
        if (bBox.min.y > invFrom.y && bBox.min.y > invTo.y) return false;
        if (bBox.max.y < invFrom.y && bBox.max.y < invTo.y) return false;
        if (bBox.min.z > invFrom.z && bBox.min.z > invTo.z) return false;
        if (bBox.max.z < invFrom.z && bBox.max.z < to.z) return false;
    }

    // スキンメッシュのときボーン行列による変形を行う
    transformSkinVertices();

    // ポリゴンとの接触判定
    float maxLengthSq = (to - from).LengthSquare();
    float minLengthSq = maxLengthSq;
    CollInfo minColl;
    int m = 0;
    for( const std::vector<PolygonInfo> &pi : polygons) { // 全メッシュのポリゴンとの接触判定
        for (const PolygonInfo& pol : pi) {   // １つのメッシュの全ポリゴンとの接触判定
            CollInfo coll;
            if (checkPolygonToLine(m, pol, invFrom, invTo, &coll)) {     // １つのポリゴンとの接触判定
                float lenSq = (coll.hitPosition - invFrom).LengthSquare();
                if (minLengthSq > lenSq) {    // より近くで接触したポリゴンがあるかどうか調べる
                    minColl = coll;
                    minLengthSq = lenSq;   // より近い接触位置
                }
            }
        }
        m++;
    }
    if (minLengthSq < maxLengthSq) { // 1つ以上見つかっている
        if (hitOut != nullptr) {
            hitOut->hitPosition = minColl.hitPosition * trans;   // 接触したポリゴンの情報を返す
            hitOut->triangle[0] = minColl.triangle[0];
            hitOut->triangle[0].pos = minColl.triangle[0].pos * trans;
            hitOut->triangle[1] = minColl.triangle[1];
            hitOut->triangle[1].pos = minColl.triangle[1].pos * trans;
            hitOut->triangle[2] = minColl.triangle[2];
            hitOut->triangle[2].pos = minColl.triangle[2].pos * trans;
            VECTOR4 n = VECTOR4(minColl.normal);
            n.w = 0.0f;
            hitOut->normal = XMVector4Transform(n, trans);
            hitOut->meshNo = minColl.meshNo;
        }
        return true;          // 接触している
    }
    return false;             // 接触していない
}

bool MeshCollider::CheckCollisionSphere(const MATRIX4X4& trans, const VECTOR3& center, float radius, CollInfo* hitOut)
{
    MATRIX4X4 invTrans = XMMatrixInverse(nullptr, trans);
    VECTOR3 invCenter = center * invTrans;

    // バウンディングボール
    if ((invCenter - bBall.center).LengthSquare() > (bBall.radius + radius) * (bBall.radius + radius)) return false;

    // AABBバウンディングボックスで判定(スタティックメッシュのみ)
    if (animator == nullptr)
    {
        if (bBox.min.x > invCenter.x + radius) return false;
        if (bBox.max.x < invCenter.x - radius) return false;
        if (bBox.min.y > invCenter.y + radius) return false;
        if (bBox.max.y < invCenter.y - radius) return false;
        if (bBox.min.z > invCenter.z + radius) return false;
        if (bBox.max.z < invCenter.z - radius) return false;
    }

    // スキンメッシュのときボーン行列による変形を行う
    transformSkinVertices();

    // 球の中心から、平面に下した垂線を求める
    float minLengthSq = radius*radius;
    CollInfo minColl;
    bool found = false;
    int m = 0;
    for (const std::vector<PolygonInfo>& pi : polygons) { // 全メッシュのポリゴンとの接触判定
        for (const PolygonInfo& pol : pi) {   // １つのメッシュの全ポリゴンとの接触判定
            CollInfo coll;
            if (checkPolygonToSphere(m, pol, invCenter, radius, &coll)) {           // １つのポリゴンとの接触判定  // -- 2024.9.27
                float lenSq = (coll.hitPosition - invCenter).LengthSquare();
                if (lenSq < minLengthSq) {         // より近くで接触したポリゴンがあるかどうか調べる
                    minColl = coll;
                    found = true;
                }
            }
        }
        m++;
    }
    if (!found) return false;

    if (hitOut != nullptr) {
        hitOut->hitPosition = minColl.hitPosition * trans;       // 接触したポリゴンの情報を返す
        hitOut->triangle[0] = minColl.triangle[0];
        hitOut->triangle[0].pos = minColl.triangle[0].pos * trans;
        hitOut->triangle[1] = minColl.triangle[1];
        hitOut->triangle[1].pos = minColl.triangle[1].pos * trans;
        hitOut->triangle[2] = minColl.triangle[2];
        hitOut->triangle[2].pos = minColl.triangle[2].pos * trans;
        VECTOR4 n = VECTOR4(minColl.normal);
        n.w = 0.0f;
        hitOut->normal = XMVector4Transform(n, trans);
        hitOut->meshNo = minColl.meshNo;
    }
    return true;
}

std::list<MeshCollider::CollInfo> MeshCollider::CheckCollisionSphereList(const MATRIX4X4& trans, const VECTOR3& center, float radius)
{
    MATRIX4X4 invTrans = XMMatrixInverse(nullptr, trans);
    VECTOR3 invCenter = center * invTrans;
    std::list<CollInfo> meshes;

    // バウンディングボール
    if ((invCenter - bBall.center).LengthSquare() > (bBall.radius + radius) * (bBall.radius + radius)) return meshes;

    // AABBバウンディングボックスで判定(スタティックメッシュのみ)
    if (animator == nullptr)
    {
        if (bBox.min.x > invCenter.x + radius) return meshes;
        if (bBox.max.x < invCenter.x - radius) return meshes;
        if (bBox.min.y > invCenter.y + radius) return meshes;
        if (bBox.max.y < invCenter.y - radius) return meshes;
        if (bBox.min.z > invCenter.z + radius) return meshes;
        if (bBox.max.z < invCenter.z - radius) return meshes;
    }

    // スキンメッシュのときボーン行列による変形を行う
    transformSkinVertices();

    // 球の中心から、平面に下した垂線を求める
    int m = 0;
    for (const std::vector<PolygonInfo>& pi : polygons) { // 全メッシュのポリゴンとの接触判定
        for (const PolygonInfo& pol : pi) {   // １つのメッシュの全ポリゴンとの接触判定
            CollInfo coll;
            if (checkPolygonToSphere(m, pol, invCenter, radius, &coll)) {
                coll.hitPosition = coll.hitPosition * trans;
                VECTOR4 n = VECTOR4(coll.normal);
                n.w = 0.0f;
                coll.normal = XMVector4Transform(n, trans);
                meshes.push_back(coll);
            }
        }
        m++;
    }
    return meshes;
}

//bool MeshCollider::CheckCollisionCapsule(const MATRIX4X4& trans, const VECTOR3& p1, const VECTOR3& p2, float radius, MeshCollider::CollInfo* info)
//{
//    return false;
//}

//std::list<MeshCollider::CollInfo> MeshCollider::CheckCollisionCapsuleList(const MATRIX4X4& trans, const VECTOR3& p1, const VECTOR3& p2, float radius)
//{
//    return std::list<MeshCollider::CollInfo>();
//}

//bool MeshCollider::CheckCollisionTriangle(const MATRIX4X4& trans, const VECTOR3* vertexes, CollInfo* info)
//{
//    // triangleの３つのエッジが交点を持つか、お互いを調べればOK
//    VECTOR3 pos[4];
//    MATRIX4X4 invTrans = XMMatrixInverse(nullptr, trans);
//    for (int i = 0; i < 3; i++) {
//        pos[i] = vertexes[i];
//    }
//    pos[3] = pos[0];
//    float minX = min(min(pos[0].x, pos[1].x), pos[2].x);
//    float maxX = max(max(pos[0].x, pos[1].x), pos[2].x);
//    float minY = min(min(pos[0].y, pos[1].y), pos[2].y);
//    float maxY = max(max(pos[0].y, pos[1].y), pos[2].y);
//    float minZ = min(min(pos[0].z, pos[1].z), pos[2].z);
//    float maxZ = max(max(pos[0].z, pos[1].z), pos[2].z);
//
//    // バウンディングボックスで判定
//    if (bBox.min.x > maxX) return false;
//    if (bBox.max.x < minX) return false;
//    if (bBox.min.y > maxY) return false;
//    if (bBox.max.y < minY) return false;
//    if (bBox.min.z > maxZ) return false;
//    if (bBox.max.z < minZ) return false;
//
//    for (int i = 0; i < 3; i++) {
//        float maxLengthSq = (pos[i + 1] - pos[i]).LengthSquare();
//        float minLengthSq = maxLengthSq;
//        for (const PolygonInfo& pol : polygons) {
//            CollInfo coll;
//            if (checkPolygonToLine(pol, pos[i], pos[i + 1], &coll)) {
//                return true;
//            }
//        }
//    }
//    return false;
//}

bool MeshCollider::CheckBoundingLine(const MATRIX4X4& trans, const VECTOR3& from, const VECTOR3& to)
{
    MATRIX4X4 invTrans = XMMatrixInverse(nullptr, trans);
    VECTOR3 invFrom = from * invTrans;
    VECTOR3 invTo = to * invTrans;

    // バウンディングボールで判定
    //VECTOR3 center = (invTo - invFrom) / 2.0f;      // ????????????????
    VECTOR3 center = (invTo + invFrom) / 2.0f;
    float radius = (invTo - invFrom).Length() / 2.0f;
    float radiusSq = (radius + bBall.radius) * (radius + bBall.radius);
    if ((center - bBall.center).LengthSquare() > radiusSq) {
        return false;
    }
    // バウンディングボックスで判定
    if (bBox.min.x > invFrom.x && bBox.min.x > invTo.x) return false;
    if (bBox.max.x < invFrom.x && bBox.max.x < invTo.x) return false;
    if (bBox.min.y > invFrom.y && bBox.min.y > invTo.y) return false;
    if (bBox.max.y < invFrom.y && bBox.max.y < invTo.y) return false;
    if (bBox.min.z > invFrom.z && bBox.min.z > invTo.z) return false;
    if (bBox.max.z < invFrom.z && bBox.max.z < to.z) return false;

    return true;
}

bool MeshCollider::checkPolygonToLine(const int m, const PolygonInfo& info, const VECTOR3& from, const VECTOR3& to, CollInfo* hit)
{
    // 法線が0だと、線か点なので、交点は求めない
    if (info.normal.LengthSquare() == 0.0f)
        return false;
    // ポリゴンを含む面との交差があるかを求める
    VECTOR3 v0 = vertices[m][info.indices[0]].pos;
    VECTOR3 v1 = vertices[m][info.indices[1]].pos;
    VECTOR3 v2 = vertices[m][info.indices[2]].pos;
    float nFrom = Dot(from - v0, info.normal);
    float nTo = Dot(to - v0, info.normal);
    if (nFrom * nTo > 0.0f) // ベクトルが同じ向きなので交差していない
        return false;
    nFrom = fabsf(nFrom);
    nTo = fabsf(nTo);
    // 交点の座標を求める
    VECTOR3 pos = from + (to - from) * nFrom / (nFrom + nTo);
    // 三角形の中かを調べる（外積ベクトルが法線と同じ向き）
    float n = Dot(XMVector3Cross(v1 - v0, pos - v0), info.normal);
    if (n < 0.0f)
        return false;
    n = Dot(XMVector3Cross(v2 - v1, pos - v1), info.normal);
    if (n < 0.0f)
        return false;
    n = Dot(XMVector3Cross(v0 - v2, pos - v2), info.normal);
    if (n < 0.0f)
        return false;
    hit->hitPosition = pos;
    hit->normal = info.normal;
    hit->triangle[0] = vertices[m][info.indices[0]];
    hit->triangle[1] = vertices[m][info.indices[1]];
    hit->triangle[2] = vertices[m][info.indices[2]];
    hit->meshNo = m;
    return true;
}

bool MeshCollider::checkPolygonToSphere(const int m, const PolygonInfo& info, const VECTOR3& center, float radius, CollInfo* hit)
{

#define EDGE_HIT 0    // エッジのヒットを計算する場合１

    // 法線が0だと、線か点
    if (info.normal.LengthSquare() == 0.0f)
        return false;

    // 中心からポリゴンを含む平面への垂線の長さを求める
    VECTOR3 v[3];
    v[0] = vertices[m][info.indices[0]].pos;
    v[1] = vertices[m][info.indices[1]].pos;
    v[2] = vertices[m][info.indices[2]].pos;
    float len = Dot(center - v[0], info.normal);

#if EDGE_HIT>0
    
    if (len < 0 || len > radius) return false; // 垂線の長さが半径より大きい
    VECTOR3 pos = center - info.normal * len; // 垂線との交点（ここが交点）
    if (Dot(center - pos, info.normal) < 0) // 法線が後ろなら当たらない
        return false;
#else
    if (fabs(len) > radius) return false; // 垂線の長さが半径より大きい。中心がポリゴンの内側でも対象とする
    VECTOR3 pos = center - info.normal * len; // 垂線との交点（ここが交点）
#endif

    // 交点がポリゴンの内側か調べる（外積の向きが法線と同じになるはず）
#if EDGE_HIT>0
    int hitIdx[3];
#endif
    int hitNum = 0;
    for (int vi = 0; vi < 3; vi++) {
        VECTOR3& v0 = v[vi];
        VECTOR3& v1 = v[(vi + 1) % 3];
        float n = Dot(XMVector3Cross(v1 - v0, pos - v0), info.normal);
        if (n < 0.0f)       // 外積の向きが法線と異なるとマイナスになる
        {                   // 辺の上に交点があると、外積が0なので、nも0になる
#if EDGE_HIT>0
            hitIdx[hitNum++] = vi;
#else
            return false;
#endif
        }
    }
    if (hitNum == 0) {
        if (hit != nullptr) {
            hit->hitPosition = pos;
            hit->normal = info.normal;
            hit->triangle[0] = vertices[m][info.indices[0]];
            hit->triangle[1] = vertices[m][info.indices[1]];
            hit->triangle[2] = vertices[m][info.indices[2]];
            hit->meshNo = m;
        }
        return true;
    }
#if EDGE_HIT>0
    else if (hitNum==1) { // １つだけ外側なので、そのエッジに寄せる
        int vi = hitIdx[0];
        VECTOR3 edge = XMVector3Normalize(v[(vi + 1) % 3] - v[vi]); // 当たったエッジの向き
        float len = Dot(center - v[vi], edge);
        VECTOR3 hitPos = edge * len + v[vi];
        if ((hitPos-center).LengthSquare() > radius*radius)
            return false;
        if (hit != nullptr) {
            hit->hitPosition = hitPos;
            hit->normal = info.normal;
            hit->triangle[0] = vertices[m][info.indices[0]];
            hit->triangle[1] = vertices[m][info.indices[1]];
            hit->triangle[2] = vertices[m][info.indices[2]];
            hit->meshNo = m;
        }
        return true;
    } else { // ２つの外側なので、その頂点に当たる
        // 0-1と1-2に当たっていれば1、1-2と2-0に当たっていれば2、2-0と0-1に当たっていれば0の頂点を使うという計算
        int n = hitIdx[0] + hitIdx[1];
        n = 2 - (n % 3);

        float lenSq = (center - v[n]).LengthSquare();
        if (lenSq > radius * radius)
            return false;
        if (hit != nullptr) {
            hit->hitPosition = v[n];
            hit->normal = info.normal;
            hit->triangle[0] = vertices[m][info.indices[0]];
            hit->triangle[1] = vertices[m][info.indices[1]];
            hit->triangle[2] = vertices[m][info.indices[2]];
            hit->meshNo = m;
        }
        return true;
    }
#endif
    return false;
}

