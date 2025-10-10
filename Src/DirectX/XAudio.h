// =========================================================================================
//
//  ＸＡｕｄｉｏ２　
//																   ver 3.3        2024.3.23
// =========================================================================================

#pragma once

#define _WIN32_DCOM
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <xaudio2.h>
#include <tchar.h>

#define AUDIO_LOOP         1          // ループ再生
#define AUDIO_SOURCE_MAX  10          // 同時に再生する同一オーディオソースの最大数

// 
//  XAudio マスタークラス	
//  アプリに一つ必要
// 
class CXAudio
{
private:
	// メンバ変数
	HWND                    m_hWnd;
	IXAudio2*               m_pXAudio2;
	IXAudio2MasteringVoice* m_pMasteringVoice;

public:
	// メソッド
	HRESULT     InitAudio(HWND);
	CXAudio();
	~CXAudio();
	IXAudio2* XAudio2() { return m_pXAudio2; }
};

// 
//  XAudio ソースボイスクラス	
//  一つのサウンド（ＷＡＶ）に一つ必要
// 
class CXAudioSource
{
private:
	CXAudio*				m_pXAudio;			// XAudio マスタークラスアドレス	
	bool					m_bWav;				// ソースがＷａｖｅファイルか　true:XAudio(WAV)　false:MCI(MP3やMID) 
	TCHAR                   m_szAliasName[256]; // Mciの別名が入る

	DWORD                   m_dwSourceIndex;					//	オーディオソースインデックス
	DWORD                   m_dwSourceNum;						//	オーディオソースの個数	
	IXAudio2SourceVoice*    m_pSourceVoice[AUDIO_SOURCE_MAX];  // オーディオソース
	BYTE*                   m_pWavBuffer[AUDIO_SOURCE_MAX];    // 波形データ（フォーマット等を含まない、純粋に波形データのみ）
	DWORD                   m_dwWavSize[AUDIO_SOURCE_MAX];     // 波形データのサイズ
public:
	HRESULT      Load(const TCHAR* szFileName, DWORD dwNum=1);
	HRESULT      LoadAudio(const TCHAR* szFileName, DWORD dwNum);
	HRESULT      LoadAudioSub(const TCHAR* szFileName, DWORD dwIndex);
	HRESULT      LoadMci(const TCHAR* szFileName);
	void         Play(int loop=0);
	void         PlayAudio(int loop=0);
	void         PlayMci(int loop=0);
	void         Stop();
	void         StopAudio();
	void         StopMci();
	void         Volume(float fVol);
	void         VolumeAudio(float fVol);
	void         VolumeMci(int nVol);
	
	CXAudioSource();
	CXAudioSource(const TCHAR* szFileName, DWORD dwNum = 1);
	CXAudioSource(CXAudio*	pXAudio);
	CXAudioSource(CXAudio*	pXAudio, const TCHAR* szFileName, DWORD dwNum=1);
	~CXAudioSource();

};