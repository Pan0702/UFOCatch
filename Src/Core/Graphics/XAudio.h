// =========================================================================================
//
//  波形オーディオ管理
//                                                  ver 3.3        2024.3.23
// =========================================================================================

#pragma once

#define _WIN32_DCOM
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <xaudio2.h>
#include <tchar.h>

#define AUDIO_LOOP         1          // ループ再生フラグ
#define AUDIO_SOURCE_MAX  10          // 同時に再生可能なオーディオソースの最大数

// 
//  XAudio マスタークラス   
//  アプリに一つ必要
// 
class CXAudio
{
private:
    // メンバ変数
    HWND                    m_hWnd;
    IXAudio2* m_pXAudio2;
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
//  一つのサウンド（音色）に一つ必要
// 
class CXAudioSource
{
private:
    CXAudio* m_pXAudio;         // XAudio マスタークラスのアドレス   
    bool                m_bWav;            // ソースがWAVファイルか否か true:XAudio(WAV) false:MCI(MP3やMID) 
    TCHAR               m_szAliasName[256]; // MCI再生用のデバイス別名

    DWORD                m_dwSourceIndex;               // オーディオソースインデックス
    DWORD                m_dwSourceNum;                 // オーディオソースの個数   
    IXAudio2SourceVoice* m_pSourceVoice[AUDIO_SOURCE_MAX]; // オーディオソース本体
    BYTE* m_pWavBuffer[AUDIO_SOURCE_MAX];   // 波形データ（フォーマットを含まない純粋な波形データ）
    DWORD                m_dwWavSize[AUDIO_SOURCE_MAX];    // 波形データのサイズ
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
    CXAudioSource(CXAudio* pXAudio);
    CXAudioSource(CXAudio* pXAudio, const TCHAR* szFileName, DWORD dwNum=1);
    ~CXAudioSource();

};