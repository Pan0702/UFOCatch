#include "Object3D.h"
#include "../Utils/MeshCollider.h"
#include "../Utils/Animator.h"

Object3D::Object3D()
{
	m_pAnimator = nullptr;
	m_pMesh = nullptr;
	m_pMeshCol = nullptr;
}

Object3D::~Object3D()
{
	if (m_pAnimator != nullptr) {
		delete m_pAnimator;
		m_pAnimator = nullptr;
	}
	// mesh と meshCol は各クラスで管理されるため削除しない
}

void Object3D::Update()
{
	if (m_pAnimator != nullptr)
	{
		m_pAnimator->Update();
	}
}

void Object3D::Draw()
{
	if (m_pMesh == nullptr)  return;
	if (m_pSpriteImage != nullptr)
	{
		DrawObjectShadow();
	}
	if (m_pAnimator == nullptr)
	{
		m_pMesh->Render(transform.matrix());
	}
	else {
		m_pMesh->Render(m_pAnimator, transform.matrix());
	}

}

SphereCollider Object3D::Collider()
{
	SphereCollider sphere;
	sphere.center = transform.position + VECTOR3(0, 1, 0);
	sphere.radius = 0.5f;
	return sphere;
}

bool Object3D::HitSphereToMeshPush(const SphereCollider& sphere, VECTOR3* push)
{
	if (m_pMeshCol == nullptr)
		return false;
	MATRIX4X4 mat = transform.matrix();
	std::list<MeshCollider::CollInfo> meshes = m_pMeshCol->CheckCollisionSphereList(mat, sphere.center, sphere.radius);
	if (meshes.size() == 0)
		return false;
	if (push != nullptr) { // ���W���K�v�Ȃ̂ł����
		VECTOR3 pushVec = VECTOR3(0, 0, 0); // �ŏI�I�ɉ����x�N�g��
		for (const MeshCollider::CollInfo& m : meshes) { // �������Ă��邷�ׂẴ|���S����
			VECTOR3 move = sphere.center - m.hitPosition;
			float len = move.Length(); // ���������_���璆�S�ւ̋���
			move = move * ((sphere.radius - len) / len);
			VECTOR3 push = m.normal * Dot(move, m.normal); // �����Ԃ������x�N�g��
			// ����pushVec�ƍ�������
			VECTOR3 pushVecNorm = XMVector3Normalize(pushVec); // �����ς݃x�N�g���̌���
			float dot = Dot(push, pushVecNorm);	// ���̐����̒���
			if (dot < pushVec.Length()) {
				pushVec += push - pushVecNorm * dot; // ���̐��������炵�Ă���
			}
			else {
				pushVec = push;
			}
		}
		*push = pushVec;
	}
	return true;
}


bool Object3D::HitSphereToMesh(const SphereCollider& sphere, MeshCollider::CollInfo* collOut)
{
	if (m_pMeshCol == nullptr)
		return false;
	MATRIX4X4 mat = transform.matrix();
	MeshCollider::CollInfo coll;
	bool ret = m_pMeshCol->CheckCollisionSphere(mat, sphere.center, sphere.radius, &coll);
	if (ret) {
		if (collOut != nullptr) {
			*collOut = coll;
		}
		return true;
	}
	return false;
}

bool Object3D::HitLineToMesh(const VECTOR3& from, const VECTOR3& to, MeshCollider::CollInfo* collOut)
{
	if (m_pMeshCol == nullptr)
		return false;
	MATRIX4X4 mat = transform.matrix();
	MeshCollider::CollInfo coll;
	bool ret = m_pMeshCol->CheckCollisionLine(mat, from, to, &coll);
	if (ret) {
		if (collOut != nullptr) {
			*collOut = coll;
		}
		return true;
	}
	return false;
}

float Object3D::HitSphereToSphere(const SphereCollider& target, bool withY)
{
	SphereCollider my = Collider();
	VECTOR3 diff = target.center - my.center;
	if (withY == false)
		diff.y = 0.0f;
	float rsum = target.radius + my.radius;
	if (diff.LengthSquare() < rsum * rsum) {
		return rsum - diff.Length();
	}
	return 0.0f;
}

bool Object3D::HitSphereToSpherePush(const SphereCollider& target, bool withY, VECTOR3* push)
{
	SphereCollider my = Collider();
	VECTOR3 pushVec = my.center - target.center;
	if (withY == false)
		pushVec.y = 0.0f;
	float rsum = my.radius + target.radius;
	if (pushVec.LengthSquare() < rsum * rsum) {	  // ���̓����蔻��
		// �������Ă�Ƃ�
		if (push != nullptr) {
			// �����o��������pushVec  �����o��������pushLen
			// �����o���x�N�g��push�����߂�
			float pushLen = rsum - pushVec.Length();
			pushVec = XMVector3Normalize(pushVec); // pushVec�̒������P�ɂ���
			*push = pushVec * pushLen;
		}
		return true;
	}
	return false;
}

SphereCollider  Object3D::GetSphereCollider()
{
	SphereCollider col;
	if (m_pMeshCol != nullptr)
	{
		m_pMeshCol->GetBall(&col.center, &col.radius);
	}
	return col;
}

void Object3D::DrawObjectShadow()
{
	static constexpr float GROUND_OFFSET        = 0.1f;
	static const VECTOR2 IMAGE_SCALE            = VECTOR2(2.0f, 1.65f);
	static constexpr float FORWARD_STRETCH      = 1.5f;  // 進行方向への伸び率
	static constexpr float CIRCLE_DEPTH         = 1.0f;
	static constexpr float GROUND_ROTATION      = -XM_PI / 2.0f;
	static constexpr float CIRCLE_ALPHA         = 0.5f;
	static constexpr float SPRITE_SIZE          = 1.0f;

	CSprite spr;

	const VECTOR2 objectSize     = ToVec2XZ(m_pMesh->m_vMax);
	const VECTOR3 groundPos      = VECTOR3(transform.position.x, GROUND_OFFSET, transform.position.z);
	const MATRIX4X4 mScale       = XMMatrixScaling(objectSize.x * IMAGE_SCALE.x,
													objectSize.y * IMAGE_SCALE.y,
													CIRCLE_DEPTH);
	ImGui::Begin("a");
	ImGui::SliderFloat("1", &a.x, -0, 5);
	ImGui::SliderFloat("2", &a.y, -0, 5);
	ImGui::SliderFloat("3", &a.z, -0, 5);
	const float forwardOffset    = - (objectSize.y * IMAGE_SCALE.y * (FORWARD_STRETCH - 0.75f) / 6.0f);
	ImGui::End();
	const MATRIX4X4 mForwardShift = XMMatrixTranslation(0, forwardOffset, 0);
	const MATRIX4X4 mRotX        = XMMatrixRotationX(GROUND_ROTATION);
	const MATRIX4X4 mRotY        = XMMatrixRotationY(transform.rotation.y);
	const MATRIX4X4 mTranslation = XMMatrixTranslation(groundPos.x, groundPos.y, groundPos.z);
	const MATRIX4X4 mWorld       = mScale * mForwardShift * mRotX * mRotY * mTranslation;

	const MATRIX4X4 mView = GameDevice()->m_mView;
	const MATRIX4X4 mProj = GameDevice()->m_mProj;

	const DWORD texWidth  = m_pSpriteImage->m_dwImageWidth;
	const DWORD texHeight = m_pSpriteImage->m_dwImageHeight;

	spr.Draw3DWithWorldMatrix(
		m_pSpriteImage,
		mWorld, mView, mProj,
		VECTOR2(SPRITE_SIZE, SPRITE_SIZE),
		VECTOR2(0, 0),
		VECTOR2(static_cast<float>(texWidth), static_cast<float>(texHeight)),
		CIRCLE_ALPHA
	);
}
