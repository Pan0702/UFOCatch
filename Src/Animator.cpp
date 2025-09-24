#include "Animator.h"
#include <fstream>
#include "GameObject.h"

Animator::Animator()
{
	base = nullptr;
	main.id = 0;
	main.frame = 0.0f;
	sub.id = 0;
	sub.frame = 0.0f;
	time = 0.0f;
	changeTime = 0.0f;
	rate = 0.0f;

	playSpeed = 1.0f;
	finished = true;
}

Animator::~Animator()
{
}

void Animator::Update()
{
	if (base==nullptr)
		return;

	finished = false;
	float Dt = 60 * SceneManager::DeltaTime();	  // -- 2024.12.26
	if (Dt > playSpeed)
	{
		main.frame += playSpeed;
	}
	else {
		main.frame += playSpeed * Dt;
	}
	const ANIMATION &info = base->m_Animation[main.id];
	if (main.frame >= info.endFrame) {
		if (info.loop) {
			main.frame -= info.endFrame - info.startFrame;
		}
		else {
			main.frame = info.endFrame;
			finished = true;
		}
	}
	if (changeTime > 0.0f) {
		time += SceneManager::DeltaTime();
		float rat = time / changeTime;
		if (rat >= 1.0f) {
			rat = 1.0f;
			changeTime = 0.0;
			time = 0.0f;
		}
		rate = rat;

	}
	else {
		rate = 1.0f;
	}
}

void Animator::SetModel(CFbxMesh* mesh)
{
	base = mesh;
}

void Animator::Play(int id, bool force)
{
//	playSpeed = 1.0f;
	if (base == nullptr)
		return;

	const ANIMATION &info = base->m_Animation[id];
	if (!info.used) {
		MessageBox(nullptr, "Animator::Play()", _T("■□■ アニメーションがLoadされていません ■□■"), MB_OK);
	}
	if (main.id == id && !Finished()) {
		if (force) {
			main.frame = info.startFrame;
		}
	}
	else {
		main.id = id;
		main.frame = info.startFrame;
	}
}

void Animator::MergePlay(int id, float time)
{
	//	playSpeed = 1.0f;
	if (base == nullptr)
		return;

	const ANIMATION& info = base->m_Animation[id];
	if (!info.used) {
		MessageBox(nullptr, "Animator::Play()", _T("■□■ アニメーションがLoadされていません ■□■"), MB_OK);
	}
	if (main.id != id) {
		changeTime = time;
		sub.id = main.id;
		sub.frame = main.frame;
		main.id = id;
		main.frame = info.startFrame;
	}
}

void Animator::SetPlaySpeed(float speed)
{
	playSpeed = speed;
}

int Animator::PlayingID()
{
	return main.id;
}

float Animator::CurrentFrame()
{
	return main.frame;
}

int Animator::SubID()
{
	return sub.id;
}

float Animator::SubFrame()
{
	return sub.frame;
}

bool Animator::Finished()
{
	const ANIMATION &info = base->m_Animation[main.id];
	return !info.loop && main.frame >= info.endFrame;
}
float Animator::Rate()
{
	return rate;
}