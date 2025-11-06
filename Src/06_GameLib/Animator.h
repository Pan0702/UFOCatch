#pragma once
#include <string>
#include "FbxMesh.h"

class Animator {
public:
	Animator();
	~Animator();
	void Update();

	/// <summary>
	/// �A�j���[�V���������郂�f�����w�肷��
	/// </summary>
	/// <param name="mesh">���f���f�[�^</param>
	void SetModel(CFbxMesh* mesh);

	void Play(int id, bool force = false);
	void MergePlay(int id, float time = 0.2f);
	void SetPlaySpeed(float speed = 1.0f);
	int PlayingID();
	int SubID();
	float Rate();
	float CurrentFrame();
	float SubFrame();
	bool Finished();
	void Stop();
private:
	CFbxMesh* base;
	struct Info {
		int id;
		float frame;
	};
	Info main;
	Info sub;
	float time;
	float changeTime;
	float rate;

	float playSpeed;
	bool finished;
};