#pragma once
/// <summary>
/// �Q�[���I�u�W�F�N�g�̊��N���X
/// </summary>
/// <author>N.Hanai</author>

#include <string>
#include "SceneBase.h"
#include "ObjectManager.h"
#include "../03_GameMain/GameMain.h"

class GameObject {
public:
	GameObject() : pParent(nullptr), tag("") { ObjectManager::Push(this); }
	GameObject(GameObject* object) : pParent(object), tag("") { ObjectManager::Push(this); }
	virtual ~GameObject() {}

	/// <summary>
	/// �I�u�W�F�N�g�𐶐���A�ŏ���Update()�̑O�ɌĂ΂�܂�
	/// </summary>
	virtual void Start() {}

	/// <summary>
	/// ���t���[���̍X�V�����̂��߂ɌĂ΂�܂�
	/// </summary>
	virtual void Update() {}

	/// <summary>
	/// ���t���[���̕`�揈���̂��߂ɌĂ΂�܂�
	/// </summary>
	virtual void Draw() {}

	/// <summary>
	/// �����̃C���X�^���X���폜���鎞�ɌĂяo���Ă�������
	/// �����Update�̑O�ɍ폜����܂�
	/// </summary>
	virtual void DestroyMe() {
		ObjectManager::Destroy(this);
	}

	/// <summary>
	/// �V�[�����ς��Ƃ��ɍ폜���Ȃ��悤�ɂ��܂�
	/// </summary>
	void DontDestroyMe() {
		ObjectManager::DontDestroy(this);
	}

	/// <summary>
	/// �����̗D�揇�ʂ��w�肵�܂�
	/// ���l���������Ɏ��s����܂�
	/// �����w�肵�Ȃ���΁A�O�ł�
	/// </summary>
	/// <param name="pri">�D�揇��</param>
	void SetPriority(int pri) {
		ObjectManager::SetPriority(this, pri);
	}

	/// <summary>
	/// �`��̗D�揇�ʂ��w�肵�܂�
	/// ���l�������قǁA�ォ��`�悳���̂ŁA
	/// 2D�ł���΁A��O�ɕ\������܂�
	/// �����w�肵�Ȃ���΂O�ł�
	/// </summary>
	/// <param name="odr">�`�揇��</param>
	void SetDrawOrder(int odr) {
		ObjectManager::SetDrawOrder(this, odr);
	}

	/// <summary>
	/// �^�O������
	/// �^�O�͂P�����t���邱�Ƃ��ł��܂�
	/// </summary>
	/// <param name="_tag">�^�O</param>
	void SetTag(std::string _tag) { tag = _tag; }

	/// <summary>
	/// �w�肳�ꂽ�^�O�Ɠ�������Ԃ�
	/// </summary>
	/// <param name="_tag">�^�O</param>
	/// <returns>�����ł����true</returns>
	bool IsTag(std::string _tag) const { return tag == _tag; }

	/// <summary>
	/// �e�̃I�u�W�F�N�g��Ԃ�
	/// </summary>
	/// <returns>�e�I�u�W�F�N�g�̃|�C���^�[</returns>
	GameObject* Parent() const { return pParent; }

	/// <summary>
	/// �e�̃I�u�W�F�N�g��ݒ肷��
	/// </summary>
	/// <returns>�e�I�u�W�F�N�g�̃|�C���^�[</returns>
	void SetParent(GameObject* _parent) { pParent = _parent; }

private:
	GameObject* pParent; // �e�̃I�u�W�F�N�g
	std::string tag;     // �^�O
};

template <class C> C* Instantiate()
{
	C* obj = new C;
	return obj;
};

template <class C> C* Instantiate(GameObject* parent)
{
	C* obj = new C(parent);
	return obj;
};

template <class C> C* SingleInstantiate()
{
	C* obj = ObjectManager::FindGameObject<C>();
	if (obj == nullptr) {
		obj = Instantiate<C>();
	}
	return obj;
}
