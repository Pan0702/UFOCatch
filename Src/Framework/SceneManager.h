#pragma once
/// <summary>
/// �V�[���̐؂�ւ����Ǘ�����N���X
/// ���ۂ̃V�[���̍쐬�����́ASceneFactory�ɏ����Ă�������
/// </summary>
/// <author>N.Hanai</author>
#include <string>
#include <list>
#include <memory>

class SceneFactory;
class SceneBase;
class CScreenTransition;

/// <summary>
/// ���݂̃V�[�����Ăяo���Ă���B
/// Scene��؂�ւ��鎞�́AChangeScene()���Ăяo���΂悭�A
/// �Ă񂾎��ɐ؂�ւ���̂ł͂Ȃ��A�����Update()�Ő؂�ւ���悤�ɂ��Ă���B
/// </summary>
namespace SceneManager {
	void Start();
	void Update();
	void Draw();
	void Release();

	/// <summary>
	/// ���݂̃V�[�����擾����
	/// </summary>
	SceneBase* CurrentScene();

	/// <summary>
	/// ���݂̃V�[���Ƃ��ēo�^����
	/// </summary>
	/// <param name="scene"></param>
	void SetCurrentScene(SceneBase* scene);

	/// <summary>
	/// �V�[����؂�ւ���B
	/// ���ڂ����ł͐؂�ւ����A����Update()���Ă΂ꂽ�Ƃ��AScene�������ĂԑO�ɐ؂�ւ��܂��B
	/// �V�[���̖��O�ƃN���X�̊֌W�́ASceneFactory()�ɋL�q���Ă��������B
	/// </summary>
	/// <param name="sceneName">�V�[����</param>
	void ChangeScene(const std::string& sceneName);

	/// <summary>
	/// トランジション付きでシーンを切り替える
	/// </summary>
	void ChangeSceneWithTransition(const std::string& sceneName);

	/// <summary>
	/// トランジション中かどうか
	/// </summary>
	bool IsTransitioning();

	/// <summary>
	/// �O�̃t���[������̌o�ߎ��ԁi�b�j
	/// �ʏ�A60�t���[���̏ꍇ�́A0.01667�ɂȂ邪�A�O�̏�����2�t���[������������0.0333�ɂȂ�
	/// ����10�t���[�����x�ŁA��ƂȂ鎞�Ԍv�������Ă���̂ŁA���肷��܂ł́A�l���s����ɂȂ�܂��B
	/// </summary>
	/// <returns></returns>
	float DeltaTime();
	
	void Exit();
};
