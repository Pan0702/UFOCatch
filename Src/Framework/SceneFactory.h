#pragma once
/// <summary>
/// SceneManager����Ăяo�����Scene���쐬����N���X
/// SceneManager��G��Ȃ��Ă����悤�ɁA�쐬�����������ɕ������Ă���
/// ���V�[���́A������Ŏw�肵�Ă��炤�悤�ɂ���
/// </summary>
#include <string>

class SceneBase;

class SceneFactory {
public:
	/// <summary>
	/// �ŏ��ɃV�[�������
	/// </summary>
	/// <returns>�쐬�����C���X�^���X</returns>
	SceneBase* CreateFirst();

	/// <summary>
	/// �w�肳�ꂽ�V�[�����쐬����
	/// </summary>
	/// <param name="name">�V�[���̖���</param>
	/// <returns>�쐬�����C���X�^���X</returns>
	SceneBase* Create(const std::string& name);
};
