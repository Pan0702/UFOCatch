#include <cassert>

#include "sceneManager.h"
#include "sceneBase.h"
#include "SceneFactory.h"
#include "Time.h"
#include <Windows.h>

namespace {
	std::string currentName; // ���݂̃V�[���̖���
	std::string nextName;    // ���̃V�[���̖���
	SceneBase* currentScene; // ���̃V�[���̃C���X�^���X��ێ�
	SceneFactory* factory;   // �V�[���؂�ւ��p��Factory�̃|�C���^�[

	// DeltaTime�p
	LARGE_INTEGER freq;
	LARGE_INTEGER current;
	float deltaTime;
	static const int REC_SIZE = 60;
	float record[REC_SIZE]; // 60�񕪂̃����O�o�b�t�@
	int recCount = 0;

	void timeInit()
	{
		bool ret = QueryPerformanceFrequency(&freq);
		assert(ret);
		QueryPerformanceCounter(&current);
	}

	void timeUpdate()
	{
		LARGE_INTEGER last = current;
		QueryPerformanceCounter(&current);
		float t = static_cast<float>(current.QuadPart - last.QuadPart) / freq.QuadPart;
		float t2 = t;
		// deltaTimeは、平均フレームレートの2倍を超えないようにする
		if (recCount >= REC_SIZE) {
			float sum = 0;
			for (int i = 0; i < REC_SIZE; i++)
				sum += record[i];
			sum /= REC_SIZE;
			if (t2 > sum * 3.0f)
				t2 = sum * 3.0f;
//			if (t2 <= sum * 1.1f)
//				t2 = sum;
		}
		record[recCount % REC_SIZE] = t;
		recCount++;
		deltaTime = t2;
	}
};

void SceneManager::Start()
{
	timeInit();
	nextName = "";
	currentName = "";

	factory = new SceneFactory();
	//最初に動くシーンは、SceneFactoryに作ってもらう
	currentScene = factory->CreateFirst();
}

void SceneManager::Update()
{
	timeUpdate();
	if (nextName != currentName) { // シーン切り替えの指示があったので
		if (currentScene != nullptr) { //今までのシーンを削除
			delete currentScene;
			currentScene = nullptr;
		}
		currentName = nextName;
		currentScene = factory->Create(nextName); //新しいシーンを作成
	}
	if (currentScene != nullptr)
		currentScene->Update();

}

void SceneManager::Draw()
{
	if (currentScene != nullptr)
		currentScene->Draw();
}

void SceneManager::Release()
{
	if (currentScene != nullptr) {
		delete currentScene;
		currentScene = nullptr;
	}
	delete factory;
}

SceneBase* SceneManager::CurrentScene()
{
	return currentScene;
}

void SceneManager::SetCurrentScene(SceneBase* scene)
{
	currentScene = scene;
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	nextName = sceneName;
}

float SceneManager::DeltaTime()
{
	return deltaTime;
}



void SceneManager::Exit()
{
	PostQuitMessage(0);
}
