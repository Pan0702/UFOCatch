#include "MeshCollider.h"
#include "../Framework/ResourceManager.h"

namespace {
    // �X�L�����b�V����ό`������Ƃ��̃t���[���|�[�Y�ϊ����~�b�g
    // (�O�ɋ߂��قǐ��m�����d���Ȃ�)
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

    // CFbxMesh����f�[�^���擾����
    // �������b�V���ɕʂ�Ă���̂ŁA�e���b�V�����Ƃɍ쐬����
    for (DWORD m = 0; m < mesh->m_dwMeshNum; m++) {
        vertices.emplace_back(ve);
        polygons.emplace_back(pi);
        CFbxMeshArray* arr = &mesh->m_pMeshArray[m];

        // ���_���
        int vNum = arr->m_dwVerticesNum;
        vertices.back().reserve(vertices.size() + vNum);
        for (int v = 0; v < vNum; v++) {
            Vertex vt = {};
            if (mesh->m_nMeshType == 1)  // �X�^�e�B�b�N���b�V��
            {
                vt.pos = arr->m_vStaticVerticesNormal[v].Pos;
                vertices.back().push_back(vt);
            }
            else {     // �X�L�����b�V��
                vt.pos = arr->m_vSkinVerticesNormal[v].Pos;
                vt.bone[0] = arr->m_vSkinVerticesNormal[v].ClusterNum[0];
                vt.bone[1] = arr->m_vSkinVerticesNormal[v].ClusterNum[1];
                vt.bone[2] = arr->m_vSkinVerticesNormal[v].ClusterNum[2];
                vt.bone[3] = arr->m_vSkinVerticesNormal[v].ClusterNum[3];
                vt.weits = arr->m_vSkinVerticesNormal[v].Weits;
                vertices.back().push_back(vt);
            }
        }

        // �C���f�b�N�X���
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

    // AABB�o�E���f�B���O�{�b�N�X
    bBox.min = mesh->m_vMin;
    bBox.max = mesh->m_vMax;

    // �o�E���f�B���O�{�[��
    bBall.center = (bBox.min + bBox.max) / 2;   // ���������b�V���̒��S�ɂȂ�
    bBall.radius = magnitude(bBox.max - bBall.center);    // ���b�V���̔��a(���b�V����S�Ĉ͂ލő唼�a)
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

    if (vt[0].bone[0] == vt[1].bone[0] || vt[0].bone[0] == vt[2].bone[0])    // �Q���_�ȏオ����̃{�[���ԍ��̂Ƃ�
    {
        bone = vt[0].bone[0];
    }
    else if (vt[1].bone[0] == vt[2].bone[0])      // �Q���_�ȏオ����̃{�[���ԍ��̂Ƃ�
    {
        bone = vt[1].bone[0];
    }
    else {  // �R���_�Ƃ��{�[���ԍ����قȂ�Ƃ��̓E�F�C�g�̍������̂�D�悷��
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

// �X�L�����b�V���̂Ƃ��{�[���s��ɂ��ό`���s��
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
        // �{�[���s��ɂ�钸�_�̕ό`
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

        // �ʖ@���̍Čv�Z
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

    // �o�E���f�B���O�{�[���Ŕ���
    //VECTOR3 center = (invTo - invFrom) / 2.0f;   // ??????????????
    VECTOR3 center = (invTo + invFrom) / 2.0f;
    float radius = (invTo - invFrom).Length() / 2.0f;
    float radiusSq = (radius + bBall.radius) * (radius + bBall.radius);
    if ((center - bBall.center).LengthSquare() > radiusSq) {
        return false;
    }
    // AABB�o�E���f�B���O�{�b�N�X�Ŕ���(�X�^�e�B�b�N���b�V���̂�)
    if (animator == nullptr)
    {
        if (bBox.min.x > invFrom.x && bBox.min.x > invTo.x) return false;
        if (bBox.max.x < invFrom.x && bBox.max.x < invTo.x) return false;
        if (bBox.min.y > invFrom.y && bBox.min.y > invTo.y) return false;
        if (bBox.max.y < invFrom.y && bBox.max.y < invTo.y) return false;
        if (bBox.min.z > invFrom.z && bBox.min.z > invTo.z) return false;
        if (bBox.max.z < invFrom.z && bBox.max.z < to.z) return false;
    }

    // �X�L�����b�V���̂Ƃ��{�[���s��ɂ��ό`���s��
    transformSkinVertices();

    // �|���S���Ƃ̐ڐG����
    float maxLengthSq = (to - from).LengthSquare();
    float minLengthSq = maxLengthSq;
    CollInfo minColl;
    int m = 0;
    for( const std::vector<PolygonInfo> &pi : polygons) { // �S���b�V���̃|���S���Ƃ̐ڐG����
        for (const PolygonInfo& pol : pi) {   // �P�̃��b�V���̑S�|���S���Ƃ̐ڐG����
            CollInfo coll;
            if (checkPolygonToLine(m, pol, invFrom, invTo, &coll)) {     // �P�̃|���S���Ƃ̐ڐG����
                float lenSq = (coll.hitPosition - invFrom).LengthSquare();
                if (minLengthSq > lenSq) {    // ���߂��ŐڐG�����|���S�������邩�ǂ������ׂ�
                    minColl = coll;
                    minLengthSq = lenSq;   // ���߂��ڐG�ʒu
                }
            }
        }
        m++;
    }
    if (minLengthSq < maxLengthSq) { // 1�ȏ㌩�����Ă���
        if (hitOut != nullptr) {
            hitOut->hitPosition = minColl.hitPosition * trans;   // �ڐG�����|���S���̏���Ԃ�
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
        return true;          // �ڐG���Ă���
    }
    return false;             // �ڐG���Ă��Ȃ�
}

bool MeshCollider::CheckCollisionSphere(const MATRIX4X4& trans, const VECTOR3& center, float radius, CollInfo* hitOut)
{
    MATRIX4X4 invTrans = XMMatrixInverse(nullptr, trans);
    VECTOR3 invCenter = center * invTrans;

    // �o�E���f�B���O�{�[��
    if ((invCenter - bBall.center).LengthSquare() > (bBall.radius + radius) * (bBall.radius + radius)) return false;

    // AABB�o�E���f�B���O�{�b�N�X�Ŕ���(�X�^�e�B�b�N���b�V���̂�)
    if (animator == nullptr)
    {
        if (bBox.min.x > invCenter.x + radius) return false;
        if (bBox.max.x < invCenter.x - radius) return false;
        if (bBox.min.y > invCenter.y + radius) return false;
        if (bBox.max.y < invCenter.y - radius) return false;
        if (bBox.min.z > invCenter.z + radius) return false;
        if (bBox.max.z < invCenter.z - radius) return false;
    }

    // �X�L�����b�V���̂Ƃ��{�[���s��ɂ��ό`���s��
    transformSkinVertices();

    // ���̒��S����A���ʂɉ��������������߂�
    float minLengthSq = radius*radius;
    CollInfo minColl;
    bool found = false;
    int m = 0;
    for (const std::vector<PolygonInfo>& pi : polygons) { // �S���b�V���̃|���S���Ƃ̐ڐG����
        for (const PolygonInfo& pol : pi) {   // �P�̃��b�V���̑S�|���S���Ƃ̐ڐG����
            CollInfo coll;
            if (checkPolygonToSphere(m, pol, invCenter, radius, &coll)) {           // �P�̃|���S���Ƃ̐ڐG����  // -- 2024.9.27
                float lenSq = (coll.hitPosition - invCenter).LengthSquare();
                if (lenSq < minLengthSq) {         // ���߂��ŐڐG�����|���S�������邩�ǂ������ׂ�
                    minColl = coll;
                    found = true;
                }
            }
        }
        m++;
    }
    if (!found) return false;

    if (hitOut != nullptr) {
        hitOut->hitPosition = minColl.hitPosition * trans;       // �ڐG�����|���S���̏���Ԃ�
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

    // �o�E���f�B���O�{�[��
    if ((invCenter - bBall.center).LengthSquare() > (bBall.radius + radius) * (bBall.radius + radius)) return meshes;

    // AABB�o�E���f�B���O�{�b�N�X�Ŕ���(�X�^�e�B�b�N���b�V���̂�)
    if (animator == nullptr)
    {
        if (bBox.min.x > invCenter.x + radius) return meshes;
        if (bBox.max.x < invCenter.x - radius) return meshes;
        if (bBox.min.y > invCenter.y + radius) return meshes;
        if (bBox.max.y < invCenter.y - radius) return meshes;
        if (bBox.min.z > invCenter.z + radius) return meshes;
        if (bBox.max.z < invCenter.z - radius) return meshes;
    }

    // �X�L�����b�V���̂Ƃ��{�[���s��ɂ��ό`���s��
    transformSkinVertices();

    // ���̒��S����A���ʂɉ��������������߂�
    int m = 0;
    for (const std::vector<PolygonInfo>& pi : polygons) { // �S���b�V���̃|���S���Ƃ̐ڐG����
        for (const PolygonInfo& pol : pi) {   // �P�̃��b�V���̑S�|���S���Ƃ̐ڐG����
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
//    // triangle�̂R�̃G�b�W����_�������A���݂��𒲂ׂ��OK
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
//    // �o�E���f�B���O�{�b�N�X�Ŕ���
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

    // �o�E���f�B���O�{�[���Ŕ���
    //VECTOR3 center = (invTo - invFrom) / 2.0f;      // ????????????????
    VECTOR3 center = (invTo + invFrom) / 2.0f;
    float radius = (invTo - invFrom).Length() / 2.0f;
    float radiusSq = (radius + bBall.radius) * (radius + bBall.radius);
    if ((center - bBall.center).LengthSquare() > radiusSq) {
        return false;
    }
    // �o�E���f�B���O�{�b�N�X�Ŕ���
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
    // �@����0���ƁA�����_�Ȃ̂ŁA��_�͋��߂Ȃ�
    if (info.normal.LengthSquare() == 0.0f)
        return false;
    // �|���S�����܂ޖʂƂ̌��������邩�����߂�
    VECTOR3 v0 = vertices[m][info.indices[0]].pos;
    VECTOR3 v1 = vertices[m][info.indices[1]].pos;
    VECTOR3 v2 = vertices[m][info.indices[2]].pos;
    float nFrom = Dot(from - v0, info.normal);
    float nTo = Dot(to - v0, info.normal);
    if (nFrom * nTo > 0.0f) // �x�N�g�������������Ȃ̂Ō������Ă��Ȃ�
        return false;
    nFrom = fabsf(nFrom);
    nTo = fabsf(nTo);
    // ��_�̍��W�����߂�
    VECTOR3 pos = from + (to - from) * nFrom / (nFrom + nTo);
    // �O�p�`�̒����𒲂ׂ�i�O�σx�N�g�����@���Ɠ��������j
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

#define EDGE_HIT 0    // �G�b�W�̃q�b�g���v�Z����ꍇ�P

    // �@����0���ƁA�����_
    if (info.normal.LengthSquare() == 0.0f)
        return false;

    // ���S����|���S�����܂ޕ��ʂւ̐����̒��������߂�
    VECTOR3 v[3];
    v[0] = vertices[m][info.indices[0]].pos;
    v[1] = vertices[m][info.indices[1]].pos;
    v[2] = vertices[m][info.indices[2]].pos;
    float len = Dot(center - v[0], info.normal);

#if EDGE_HIT>0
    
    if (len < 0 || len > radius) return false; // �����̒��������a���傫��
    VECTOR3 pos = center - info.normal * len; // �����Ƃ̌�_�i��������_�j
    if (Dot(center - pos, info.normal) < 0) // �@�������Ȃ瓖����Ȃ�
        return false;
#else
    if (fabs(len) > radius) return false; // �����̒��������a���傫���B���S���|���S���̓����ł��ΏۂƂ���
    VECTOR3 pos = center - info.normal * len; // �����Ƃ̌�_�i��������_�j
#endif

    // ��_���|���S���̓��������ׂ�i�O�ς̌������@���Ɠ����ɂȂ�͂��j
#if EDGE_HIT>0
    int hitIdx[3];
#endif
    int hitNum = 0;
    for (int vi = 0; vi < 3; vi++) {
        VECTOR3& v0 = v[vi];
        VECTOR3& v1 = v[(vi + 1) % 3];
        float n = Dot(XMVector3Cross(v1 - v0, pos - v0), info.normal);
        if (n < 0.0f)       // �O�ς̌������@���ƈقȂ�ƃ}�C�i�X�ɂȂ�
        {                   // �ӂ̏�Ɍ�_������ƁA�O�ς�0�Ȃ̂ŁAn��0�ɂȂ�
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
    else if (hitNum==1) { // �P�����O���Ȃ̂ŁA���̃G�b�W�Ɋ񂹂�
        int vi = hitIdx[0];
        VECTOR3 edge = XMVector3Normalize(v[(vi + 1) % 3] - v[vi]); // ���������G�b�W�̌���
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
    } else { // �Q�̊O���Ȃ̂ŁA���̒��_�ɓ�����
        // 0-1��1-2�ɓ������Ă����1�A1-2��2-0�ɓ������Ă����2�A2-0��0-1�ɓ������Ă����0�̒��_���g���Ƃ����v�Z
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

