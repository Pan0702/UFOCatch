// =========================================================================================
//
//  豕｢蠖｢繧ｪ繝ｼ繝・ぅ繧ｪ邂｡逅・
//                                                  ver 3.3        2024.3.23
// =========================================================================================

#pragma once

#define _WIN32_DCOM
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <xaudio2.h>
#include <tchar.h>

#define AUDIO_LOOP         1          // 繝ｫ繝ｼ繝怜・逕溘ヵ繝ｩ繧ｰ
#define AUDIO_SOURCE_MAX  10          // 蜷梧凾縺ｫ蜀咲函蜿ｯ閭ｽ縺ｪ繧ｪ繝ｼ繝・ぅ繧ｪ繧ｽ繝ｼ繧ｹ縺ｮ譛螟ｧ謨ｰ

// 
//  XAudio 繝槭せ繧ｿ繝ｼ繧ｯ繝ｩ繧ｹ   
//  繧｢繝励Μ縺ｫ荳縺､蠢・ｦ・
// 
class CXAudio
{
private:
    // 繝｡繝ｳ繝仙､画焚
    HWND                    m_hWnd;
    IXAudio2* m_pXAudio2;
    IXAudio2MasteringVoice* m_pMasteringVoice;

public:
    // 繝｡繧ｽ繝・ラ
    HRESULT     InitAudio(HWND);
    CXAudio();
    ~CXAudio();
    IXAudio2* XAudio2() { return m_pXAudio2; }
};

// 
//  XAudio 繧ｽ繝ｼ繧ｹ繝懊う繧ｹ繧ｯ繝ｩ繧ｹ   
//  荳縺､縺ｮ繧ｵ繧ｦ繝ｳ繝会ｼ磯浹濶ｲ・峨↓荳縺､蠢・ｦ・
// 
class CXAudioSource
{
private:
    CXAudio* m_pXAudio;         // XAudio 繝槭せ繧ｿ繝ｼ繧ｯ繝ｩ繧ｹ縺ｮ繧｢繝峨Ξ繧ｹ   
    bool                m_bWav;            // 繧ｽ繝ｼ繧ｹ縺係AV繝輔ぃ繧､繝ｫ縺句凄縺・true:XAudio(WAV) false:MCI(MP3繧МID) 
    TCHAR               m_szAliasName[256]; // MCI蜀咲函逕ｨ縺ｮ繝・ヰ繧､繧ｹ蛻･蜷・

    DWORD                m_dwSourceIndex;               // 繧ｪ繝ｼ繝・ぅ繧ｪ繧ｽ繝ｼ繧ｹ繧､繝ｳ繝・ャ繧ｯ繧ｹ
    DWORD                m_dwSourceNum;                 // 繧ｪ繝ｼ繝・ぅ繧ｪ繧ｽ繝ｼ繧ｹ縺ｮ蛟区焚   
    IXAudio2SourceVoice* m_pSourceVoice[AUDIO_SOURCE_MAX]; // 繧ｪ繝ｼ繝・ぅ繧ｪ繧ｽ繝ｼ繧ｹ譛ｬ菴・
    BYTE* m_pWavBuffer[AUDIO_SOURCE_MAX];   // 豕｢蠖｢繝・・繧ｿ・医ヵ繧ｩ繝ｼ繝槭ャ繝医ｒ蜷ｫ縺ｾ縺ｪ縺・ｴ皮ｲ九↑豕｢蠖｢繝・・繧ｿ・・
    DWORD                m_dwWavSize[AUDIO_SOURCE_MAX];    // 豕｢蠖｢繝・・繧ｿ縺ｮ繧ｵ繧､繧ｺ
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