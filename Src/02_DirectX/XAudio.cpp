// =========================================================================================
//
//  �w�`���������Q�@
//																   ver 3.3        2024.3.23
// =========================================================================================

//�w�b�_�[�t�@�C���̃C���N���[�h
#include "XAudio.h"
#include "../03_GameMain/GameMain.h"


//------------------------------------------------------------------------
//
//	XAudio2�̃R���X�g���N�^	
//
//  �����@�Ȃ�
//
//------------------------------------------------------------------------
CXAudio::CXAudio()
{
	ZeroMemory(this, sizeof(CXAudio));
}
//------------------------------------------------------------------------
//
//	XAudio2�̃f�X�g���N�^	
//
//------------------------------------------------------------------------
CXAudio::~CXAudio()
{
	SAFE_RELEASE(m_pXAudio2);
}

//------------------------------------------------------------------------
//
//	 XAudio2�̏�����	
//
//  HWND hWnd         �E�B���h�E�n���h��
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CXAudio::InitAudio(HWND hWnd)
{
	UINT32 flags = 0;

/*    // -- 2018.5.25  DEBUG_ENGINE����߂�
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif
*/

	m_hWnd    = hWnd;

	if (FAILED(XAudio2Create( &m_pXAudio2, flags)))
	{

		// -- 2018.7.8 XAudio2��DLL���Ȃ��ď��������ł��Ȃ��Ă��A�l�b�h�ōĐ����ł���悤�ɂ��܂���

		// MessageBox(0, _T("XAudio2�@���������ł��Ȃ����߁A�l�b�h�ōĐ������܂�"), 0, MB_OK);   // �G���[�_�C�A���O

		OutputDebugString(_T("�������@XAudio2�@���������ł��Ȃ����߁A�l�b�h�ōĐ������܂��B ������\n"));
		SAFE_RELEASE(m_pXAudio2);
		return S_OK;

	}

	if (FAILED(m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice)))
	{
		MessageBox(0, _T("XAudio2 �}�X�^�[�{�C�X�쐬���s"), 0, MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

//------------------------------------------------------------------------
//
//	XAudioSource�̃R���X�g���N�^	
//
//  �����@CXAudio*	pXAudio    XAudio �}�X�^�[�N���X�A�h���X
//
//------------------------------------------------------------------------
CXAudioSource::CXAudioSource() : CXAudioSource(GameDevice()->m_pXAudio) {}
CXAudioSource::CXAudioSource(const TCHAR* szFileName, DWORD dwNum) : CXAudioSource(GameDevice()->m_pXAudio, szFileName, dwNum) {}

//------------------------------------------------------------------------
CXAudioSource::CXAudioSource(CXAudio*	pXAudio)
{
	ZeroMemory(this, sizeof(CXAudioSource));
	m_pXAudio = pXAudio;
}
//------------------------------------------------------------------------
//
//	XAudioSource�̃R���X�g���N�^	
//
//�@CXAudio*	pXAudio XAudio �}�X�^�[�N���X�A�h���X
//  TCHAR* szFileName    �����̃t�@�C����
//  DWORD dwNum         ���ꉹ������ǂݍ��ނ��i�ȗ��l�͂P�j
//
//------------------------------------------------------------------------
CXAudioSource::CXAudioSource(CXAudio*	pXAudio, const TCHAR* szFileName, DWORD dwNum)
{
	ZeroMemory(this, sizeof(CXAudioSource));
	m_pXAudio = pXAudio;
	Load(szFileName, dwNum);
}
//------------------------------------------------------------------------
//
//	XAudioSource�̃f�X�g���N�^	
//
//------------------------------------------------------------------------
CXAudioSource::~CXAudioSource()
{
	if (m_bWav) {
		for (DWORD i = 0; i < m_dwSourceNum; i++)  // �S�ẴI�[�f�B�I�\�[�X���폜
		{
			if (m_pSourceVoice[i]) m_pSourceVoice[i]->DestroyVoice();
			SAFE_DELETE(m_pWavBuffer[i]);
		}
	}
	else {
		TCHAR AllStr[512];
		_tcscpy_s(AllStr, _T("close "));
		_tcscat_s(AllStr, m_szAliasName);

		mciSendString(AllStr, nullptr, 0, nullptr);  // �Ȃ��Aopen�����t�@�C���͕K��close����K�v������Balias����close�ł���
		//mciSendString(_T("close all"), nullptr, 0, nullptr);�@�́A�S�Ă̋Ȃ�close
	}
}

//------------------------------------------------------------------------
//
//	�����̃��[�h	
//
//  TCHAR* szFileName    �����̃t�@�C����
//  DWORD dwNum         ���ꉹ������ǂݍ��ނ��i�ȗ��l�͂P�j
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CXAudioSource::Load(const TCHAR* szFileName, DWORD dwNum)
{
	TCHAR ext[50];

	// _tsplitpath_s�֐��Ńt�@�C��������g���q�𓾂�
	// �Ȃ��A�e���ڂ̒����Ƃ́A�o�C�g���ł͂Ȃ��������ł���
	_tsplitpath_s(szFileName, NULL, NULL, NULL, NULL, NULL, NULL, ext, sizeof(ext)/sizeof(TCHAR));

	if ( m_pXAudio->XAudio2() && ( _tcscmp(ext, _T(".wav")) == 0 || _tcscmp(ext, _T(".WAV")) == 0 ) ) // -- 2018.7.8 XAudio������������Ă��āA�v�`�u�`���̂Ƃ�
	{
		m_bWav = true;				// WAV�t�@�C���̂Ƃ���XAudio2���g�p����
		LoadAudio(szFileName, dwNum);
	}
	else {
		m_bWav = false;				// WAV�t�@�C���ȊO�̂Ƃ���Mci���g�p����
		LoadMci(szFileName);
	}
	return S_OK;
}
//------------------------------------------------------------------------
//
//	XAudio�����̃��[�h	
// �EXAudio�����́AWAV�t�@�C���ł���
// �E����\�[�X�𕡐��ǂݍ��ނ��Ƃɂ���āA�d�ˍ��킹�Đ����\�Ƃ���j
//
//  TCHAR* szFileName    �����̃t�@�C����
//  DWORD dwSourceNum   ���ꉹ������ǂݍ��ނ��i�ȗ��l�͂P�j
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CXAudioSource::LoadAudio(const TCHAR* szFileName, DWORD dwSourceNum)
{
	m_dwSourceNum = dwSourceNum;
	if (m_dwSourceNum < 1) m_dwSourceNum = 1;
	if (m_dwSourceNum > AUDIO_SOURCE_MAX) m_dwSourceNum = AUDIO_SOURCE_MAX;

	for (DWORD i = 0; i < m_dwSourceNum; i++)  // ����\�[�X�𕡐��ǂݍ��ނƂ�
	{
		LoadAudioSub( szFileName, i );  // �w��̃C���f�b�N�X�ʒu�Ƀ��[�h����
	}
	m_dwSourceIndex = 0;
	return S_OK;
}
//------------------------------------------------------------------------
//
//	XAudio�̃��[�h�@�T�u�֐�	
//
//  TCHAR* szFileName    �����̃t�@�C����
//  DWORD dwIndex       ���[�h����C���f�b�N�X�ʒu
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CXAudioSource::LoadAudioSub(const TCHAR* szFileName, DWORD dwIndex)
{
	HMMIO            hMmio = nullptr;  //Windows�}���`���f�B�AAPI�̃n���h��(Windows�}���`���f�B�AAPI��WAV�t�@�C���֌W�̑���p��API)
	DWORD            dwWavSize = 0; //WAV�t�@�C�����@WAV�f�[�^�̃T�C�Y�iWAV�t�@�C����WAV�f�[�^�Ő�߂��Ă���̂ŁA�قڃt�@�C���T�C�Y�Ɠ���j
	WAVEFORMATEX*    pwfex;         //WAV�̃t�H�[�}�b�g ��j16�r�b�g�A44100Hz�A�X�e���I�Ȃ�
	MMCKINFO         ckInfo;        //�@�`�����N���
	MMCKINFO         riffckInfo = { 0 };    // �ŏ㕔�`�����N�iRIFF�`�����N�j�ۑ��p
	PCMWAVEFORMAT    pcmWaveForm;
	MMIOINFO         mmioInfo;

	TCHAR FName[256];
	_tcscpy_s(FName, szFileName);		// -- 2024.4.20

	//WAV�t�@�C�����̃w�b�_�[���i���f�[�^�ȊO�j�̊m�F�Ɠǂݍ���
	ZeroMemory(&mmioInfo, sizeof(MMIOINFO));								// -- 2017.1.22
	hMmio = mmioOpen(FName, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ);	// -- 2024.4.20
	if ( hMmio == nullptr )
	{
		MessageBox(0, _T("XAudio2 �T�E���h�f�[�^�@�ǂݍ��ݎ��s"), nullptr, MB_OK);
		return E_FAIL;
	}

	//�t�@�C���|�C���^��RIFF�`�����N�̐擪�ɃZ�b�g����
	mmioDescend(hMmio, &riffckInfo, nullptr, 0);

	// �t�@�C���|�C���^��'f' 'm' 't' ' ' �`�����N�ɃZ�b�g����
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);

	//�t�H�[�}�b�g��ǂݍ���
	mmioRead(hMmio, (HPSTR)&pcmWaveForm, sizeof(pcmWaveForm));
	pwfex = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
	memcpy(pwfex, &pcmWaveForm, sizeof(pcmWaveForm));
	pwfex->cbSize = 0;
	mmioAscend(hMmio, &ckInfo, 0);

	// WAV�t�@�C�����̉��f�[�^�̓ǂݍ���	
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);  //�f�[�^�`�����N�ɃZ�b�g
	dwWavSize = ckInfo.cksize;
	m_pWavBuffer[dwIndex] = new BYTE[dwWavSize];
	DWORD dwOffset = ckInfo.dwDataOffset;
	mmioRead(hMmio, (HPSTR)m_pWavBuffer[dwIndex], dwWavSize);

	//�\�[�X�{�C�X�Ƀf�[�^������	
	if (FAILED(m_pXAudio->XAudio2()->CreateSourceVoice(&m_pSourceVoice[dwIndex], pwfex)))
	{
		MessageBox(0, _T("XAudio2 �\�[�X�{�C�X�쐬���s"), 0, MB_OK);
		return E_FAIL;
	}
	m_dwWavSize[dwIndex] = dwWavSize;

	SAFE_DELETE_ARRAY(pwfex);

	return S_OK;
}


//------------------------------------------------------------------------
//
//	�l�b�h�̃��[�h	
// �E�Ή��̉����́AMP3�AWMV�AMIDI���ł���
// �E�l�b�h�C���^�[�t�F�C�X�́A�����x���`�o�h�ł���
//
//  TCHAR* szFileName    �����̃t�@�C����
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CXAudioSource::LoadMci(const TCHAR* szFileName)
{
	TCHAR AllStr[512];
	TCHAR exe[265];

	// _tsplitpath_s�֐��Ŋg���q����苎��A�t�@�C�����݂̂���肾���āA�ʖ�m_szAliasName�Ƃ���
	// �Ȃ��A�e���ڂ̒����Ƃ́A�o�C�g���ł͂Ȃ��������ł���
	_tsplitpath_s(szFileName, NULL, NULL, NULL, NULL, m_szAliasName, sizeof(m_szAliasName)/sizeof(TCHAR), exe, 256 );     // �t�@�C�����݂̂𓾂�

	_tcscpy_s( AllStr, _T("open "));
	_tcscat_s( AllStr, szFileName);
	_tcscat_s( AllStr, _T(" type mpegvideo alias "));
	_tcscat_s( AllStr, m_szAliasName);

	mciSendString( AllStr , nullptr, 0, nullptr);

	// 
	// mciSendString�̗�
	// 
	// �Ȃ��AmciSendString��Unicode�Ł@mciSendStringA�̓}���`�o�C�g�Ή���
	// 
	//mciSendString(_T("open Sound/bgm1.mp3 type mpegvideo alias BGM01"), nullptr, 0, nullptr);
	//mciSendString(_T("open Sound/B003B.MID type mpegvideo alias BGM01"), nullptr, 0, nullptr);
	//mciSendString(_T("open Sound/bgm1.mp3 type mpegvideo"), nullptr, 0, nullptr);
	//mciSendString(_T("stop all"), nullptr, 0, nullptr);
	//mciSendString(_T("play BGM01 from 0 repeat"), nullptr, 0, nullptr);
	//mciSendString(_T("close all"), nullptr, 0, nullptr);�@�́A�S�Ă̋Ȃ�close

	return S_OK;
}

//------------------------------------------------------------------------
//
//	�Đ�	
//
//  int loop    ���[�v�w��
//				���[�v���Ȃ��Ƃ��F0���͏ȗ�
//				���[�v�̂Ƃ�    �FAUDIO_LOOP
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void  CXAudioSource::Play(int loop)
{
	if (m_bWav) {
		PlayAudio(loop);
	}
	else {
		PlayMci(loop);
	}
}

//------------------------------------------------------------------------
//
//	XAudio�̍Đ�	
//
//  int loop    ���[�v�w��
//				���[�v���Ȃ��Ƃ��F0���͏ȗ�
//				���[�v�̂Ƃ�    �FAUDIO_LOOP
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void  CXAudioSource::PlayAudio(int loop)
{

	m_pSourceVoice[m_dwSourceIndex]->Stop(0, 0);
	m_pSourceVoice[m_dwSourceIndex]->FlushSourceBuffers();


	XAUDIO2_BUFFER buffer;
	ZeroMemory( &buffer, sizeof(XAUDIO2_BUFFER));

	buffer.pAudioData = m_pWavBuffer[m_dwSourceIndex];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = m_dwWavSize[m_dwSourceIndex];
	if (loop == AUDIO_LOOP) {
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;	// ���[�v�Đ�
	}
	else {
		buffer.LoopCount = 0;
	}

	if (FAILED(m_pSourceVoice[m_dwSourceIndex]->SubmitSourceBuffer(&buffer)))
	{
		MessageBox(0,_T("XAudio2 �\�[�X�{�C�X�ɃT�u�~�b�g���s"), 0, MB_OK);
		return;
	}
	m_pSourceVoice[m_dwSourceIndex]->Start(0, XAUDIO2_COMMIT_NOW);

	// �d�ˍ��킹�Đ��̃C���f�b�N�X��i�߂�
	m_dwSourceIndex++;
	if (m_dwSourceIndex >= m_dwSourceNum) m_dwSourceIndex = 0;
}
//------------------------------------------------------------------------
//
//	Mci�̍Đ�	
//
//  int loop    ���[�v�w��
//				���[�v���Ȃ��Ƃ��F0���͏ȗ�
//				���[�v�̂Ƃ�    �FAUDIO_LOOP
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void  CXAudioSource::PlayMci(int loop)
{
	TCHAR AllStr[512];

	_tcscpy_s(AllStr, _T("play "));
	_tcscat_s(AllStr, m_szAliasName);
	_tcscat_s(AllStr, _T(" from 0"));

	if (loop == AUDIO_LOOP)
	{
		_tcscat_s(AllStr, _T(" repeat"));
	}
	mciSendString(AllStr, nullptr, 0, nullptr);

	// MPEG�r�f�I���Đ�����Ƃ��A�W���w��ł͕ʃE�B���h�E�ƂȂ�
	// ���E�B���h�E�ōĐ�����Ƃ��́A���O�ɉ��L�������s��
	// �A���ADirectX�`����ꎞ�I�ɒ�~����K�v������
	//TCHAR str[256];
	//_stprintf_s(str, _T("window %s handle %d"), m_szAliasName, (int)m_pXAudio->m_hWnd);
	//mciSendString( str , nullptr, 0, nullptr);
	//_stprintf_s(str, _T("put %s destination at 0 0 640 480"), m_szAliasName);
	//mciSendString( str , nullptr, 0, nullptr);


}

//------------------------------------------------------------------------
//
//	��~	
//
//  �����@�Ȃ�
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void  CXAudioSource::Stop()
{
	if (m_bWav)
	{
		StopAudio();
	}
	else {
		StopMci();
	}
}
//------------------------------------------------------------------------
//
//	XAudio�̒�~	
//
//  �����@�Ȃ�
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void  CXAudioSource::StopAudio()
{
	// �d�ˍ��킹�̑S�Ẵ\�[�X���~����
	for (DWORD i = 0; i < m_dwSourceNum; i++) {
		m_pSourceVoice[i]->Stop(0, 0);
		m_pSourceVoice[i]->FlushSourceBuffers();
	}
	m_dwSourceIndex = 0;	// �C���f�b�N�X���O�ɖ߂�	
}
//------------------------------------------------------------------------
//
//	Mci�̒�~	
//
//  �����@�Ȃ�
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void  CXAudioSource::StopMci()
{
	TCHAR AllStr[512];

	_tcscpy_s(AllStr, _T("stop "));
	_tcscat_s(AllStr, m_szAliasName);

	mciSendString(AllStr, nullptr, 0, nullptr);
}

//------------------------------------------------------------------------
//
//	�{�����[��	
//
//  �����@�{�����[���i��l��1.0f�j
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void  CXAudioSource::Volume(float fVol)
{
	if (m_bWav)
	{
		VolumeAudio(fVol);
	}
	else {
		// 1000�{���Ăl�b�h�ɓn��
		VolumeMci((int)(fVol*1000));
	}
}
//------------------------------------------------------------------------
//
//	XAudio�̃{�����[��	
//
//  �����@�{�����[���i��l��1.0f�j
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void  CXAudioSource::VolumeAudio(float fVol)
{

	//m_pSourceVoice[m_dwSourceIndex]->SetVolume(1.0f);  // �}�X�^�[�{�����[���̐ݒ�@�W���l��1.0�A0.0�͖���
	//   �{�����[�� ���x���́A - 224 �` 224 �̕��������_�U�����{�Ƃ��ĕ\������A�ő�Q�C���� 144.5 dB �ł��B
	//   �{�����[�� ���x�� 1.0 �͌����܂��̓Q�C�����Ȃ����Ƃ��Ӗ����A0 �͖������Ӗ����܂��B
	//   ���̃��x���́A�I�[�f�B�I�̃t�F�[�Y�𔽓]�����邽�߂Ɏg�p�ł��܂��B

	// �d�ˍ��킹�̑S�Ẵ\�[�X�̃{�����[����ݒ肷��
	for (DWORD i = 0; i < m_dwSourceNum; i++) {
		m_pSourceVoice[i]->SetVolume(fVol);
	}

}
//------------------------------------------------------------------------
//
//	Mci�̃{�����[��	
//
//  �����@�{�����[���i��l��1000�BVolume���\�b�h��1000�{���ēn����Ă��邽�߁j
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void  CXAudioSource::VolumeMci(int nVol)
{
	// �{�����[���̐ݒ�@�@�@�{�����[�����x���i0�`1000�j100%��1000

	TCHAR str[512];

	_stprintf_s(str, _T("setaudio %s volume to %d"), m_szAliasName, nVol);
	mciSendString( str , nullptr, 0, nullptr);

}
