// =========================================================================================
//
//                                                  ver 3.3        2024.3.23
// =========================================================================================

#pragma once

#define _WIN32_DCOM
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <xaudio2.h>
#include <tchar.h>

#define AUDIO_LOOP         1
#define AUDIO_SOURCE_MAX  10

// 
//  XAudio マスタークラス   
// 
class CXAudio
{
private:
    // メンバ変数
    HWND m_hWnd;
    IXAudio2* m_pXAudio2;
    IXAudio2MasteringVoice* m_pMasteringVoice;

public:
    /// Audio を初期化する
    /// @return 処理結果
    HRESULT InitAudio(HWND);
    /// CXAudio を初期化する
    CXAudio();
    /// CXAudio の終了処理を行う
    ~CXAudio();
    /// XAudio2 を返す
    /// @return 対象のポインタ
    IXAudio2* XAudio2() { return m_pXAudio2; }
};

// 
//  XAudio ソースボイスクラス   
// 
class CXAudioSource
{
private:
    CXAudio* m_pXAudio; // XAudio マスタークラスのアドレス   
    bool m_bWav;
    TCHAR m_szAliasName[256];

    DWORD m_dwSourceIndex;
    DWORD m_dwSourceNum;
    IXAudio2SourceVoice* m_pSourceVoice[AUDIO_SOURCE_MAX];
    BYTE* m_pWavBuffer[AUDIO_SOURCE_MAX];
    DWORD m_dwWavSize[AUDIO_SOURCE_MAX];

public:
    /// 読み込む
    /// @param szFileName 名前
    /// @param dwNum dwNum に渡す値
    /// @return 処理結果
    HRESULT Load(const TCHAR* szFileName, DWORD dwNum = 1);
    /// Audio を読み込む
    /// @param szFileName 名前
    /// @param dwNum dwNum に渡す値
    /// @return 処理結果
    HRESULT LoadAudio(const TCHAR* szFileName, DWORD dwNum);
    /// Audio Sub を読み込む
    /// @param szFileName 名前
    /// @param dwIndex インデックス
    /// @return 処理結果
    HRESULT LoadAudioSub(const TCHAR* szFileName, DWORD dwIndex);
    /// Mci を読み込む
    /// @param szFileName 名前
    /// @return 処理結果
    HRESULT LoadMci(const TCHAR* szFileName);
    /// 再生する
    /// @param loop ループ再生するか
    void Play(int loop = 0);
    /// Audio を再生する
    /// @param loop ループ再生するか
    void PlayAudio(int loop = 0);
    /// Mci を再生する
    /// @param loop ループ再生するか
    void PlayMci(int loop = 0);
    /// 停止する
    void Stop();
    /// Audio を停止する
    void StopAudio();
    /// Mci を停止する
    void StopMci();
    /// Volume の処理を行う
    /// @param fVol fVol に渡す値
    void Volume(float fVol);
    /// Volume Audio の処理を行う
    /// @param fVol fVol に渡す値
    void VolumeAudio(float fVol);
    /// Volume Mci の処理を行う
    /// @param nVol nVol に渡す値
    void VolumeMci(int nVol);

    /// CXAudioSource を初期化する
    CXAudioSource();
    /// CXAudioSource を初期化する
    /// @param szFileName 名前
    /// @param dwNum dwNum に渡す値
    CXAudioSource(const TCHAR* szFileName, DWORD dwNum = 1);
    /// CXAudioSource を初期化する
    /// @param pXAudio pXAudio に渡す値
    CXAudioSource(CXAudio* pXAudio);
    /// CXAudioSource を初期化する
    /// @param pXAudio pXAudio に渡す値
    /// @param szFileName 名前
    /// @param dwNum dwNum に渡す値
    CXAudioSource(CXAudio* pXAudio, const TCHAR* szFileName, DWORD dwNum = 1);
    /// CXAudioSource の終了処理を行う
    ~CXAudioSource();
};
