// =========================================================================================
//
//  ＸＡｕｄｉｏ２　
//																   ver 3.3        2024.3.23
// =========================================================================================

//ヘッダーファイルのインクルード
#include "XAudio.h"
#include "GameMain.h"


//------------------------------------------------------------------------
//
//	XAudio2のコンストラクタ	
//
//  引数　なし
//
//------------------------------------------------------------------------
CXAudio::CXAudio()
{
	ZeroMemory(this, sizeof(CXAudio));
}
//------------------------------------------------------------------------
//
//	XAudio2のデストラクタ	
//
//------------------------------------------------------------------------
CXAudio::~CXAudio()
{
	SAFE_RELEASE(m_pXAudio2);
}

//------------------------------------------------------------------------
//
//	 XAudio2の初期化	
//
//  HWND hWnd         ウィンドウハンドル
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CXAudio::InitAudio(HWND hWnd)
{
	UINT32 flags = 0;

/*    // -- 2018.5.25  DEBUG_ENGINEをやめる
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif
*/

	m_hWnd    = hWnd;

	if (FAILED(XAudio2Create( &m_pXAudio2, flags)))
	{

		// -- 2018.7.8 XAudio2のDLLがなくて初期化ができなくても、ＭＣＩで再生ができるようにしました

		// MessageBox(0, _T("XAudio2　が初期化できないため、ＭＣＩで再生をします"), 0, MB_OK);   // エラーダイアログ

		OutputDebugString(_T("■□■　XAudio2　が初期化できないため、ＭＣＩで再生をします。 ■□■\n"));
		SAFE_RELEASE(m_pXAudio2);
		return S_OK;

	}

	if (FAILED(m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice)))
	{
		MessageBox(0, _T("XAudio2 マスターボイス作成失敗"), 0, MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

//------------------------------------------------------------------------
//
//	XAudioSourceのコンストラクタ	
//
//  引数　CXAudio*	pXAudio    XAudio マスタークラスアドレス
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
//	XAudioSourceのコンストラクタ	
//
//　CXAudio*	pXAudio XAudio マスタークラスアドレス
//  TCHAR* szFileName    音源のファイル名
//  DWORD dwNum         同一音源を幾つ読み込むか（省略値は１）
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
//	XAudioSourceのデストラクタ	
//
//------------------------------------------------------------------------
CXAudioSource::~CXAudioSource()
{
	if (m_bWav) {
		for (DWORD i = 0; i < m_dwSourceNum; i++)  // 全てのオーディオソースを削除
		{
			if (m_pSourceVoice[i]) m_pSourceVoice[i]->DestroyVoice();
			SAFE_DELETE(m_pWavBuffer[i]);
		}
	}
	else {
		TCHAR AllStr[512];
		_tcscpy_s(AllStr, _T("close "));
		_tcscat_s(AllStr, m_szAliasName);

		mciSendString(AllStr, nullptr, 0, nullptr);  // なお、openしたファイルは必ずcloseする必要がある。alias名でcloseできる
		//mciSendString(_T("close all"), nullptr, 0, nullptr);　は、全ての曲をclose
	}
}

//------------------------------------------------------------------------
//
//	音源のロード	
//
//  TCHAR* szFileName    音源のファイル名
//  DWORD dwNum         同一音源を幾つ読み込むか（省略値は１）
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CXAudioSource::Load(const TCHAR* szFileName, DWORD dwNum)
{
	TCHAR ext[50];

	// _tsplitpath_s関数でファイル名から拡張子を得る
	// なお、各項目の長さとは、バイト数ではなく文字数である
	_tsplitpath_s(szFileName, NULL, NULL, NULL, NULL, NULL, NULL, ext, sizeof(ext)/sizeof(TCHAR));

	if ( m_pXAudio->XAudio2() && ( _tcscmp(ext, _T(".wav")) == 0 || _tcscmp(ext, _T(".WAV")) == 0 ) ) // -- 2018.7.8 XAudioが初期化されていて、ＷＡＶ形式のとき
	{
		m_bWav = true;				// WAVファイルのときはXAudio2を使用する
		LoadAudio(szFileName, dwNum);
	}
	else {
		m_bWav = false;				// WAVファイル以外のときはMciを使用する
		LoadMci(szFileName);
	}
	return S_OK;
}
//------------------------------------------------------------------------
//
//	XAudio音源のロード	
// ・XAudio音源は、WAVファイルである
// ・同一ソースを複数個読み込むことによって、重ね合わせ再生を可能とする）
//
//  TCHAR* szFileName    音源のファイル名
//  DWORD dwSourceNum   同一音源を幾つ読み込むか（省略値は１）
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CXAudioSource::LoadAudio(const TCHAR* szFileName, DWORD dwSourceNum)
{
	m_dwSourceNum = dwSourceNum;
	if (m_dwSourceNum < 1) m_dwSourceNum = 1;
	if (m_dwSourceNum > AUDIO_SOURCE_MAX) m_dwSourceNum = AUDIO_SOURCE_MAX;

	for (DWORD i = 0; i < m_dwSourceNum; i++)  // 同一ソースを複数個読み込むとき
	{
		LoadAudioSub( szFileName, i );  // 指定のインデックス位置にロードする
	}
	m_dwSourceIndex = 0;
	return S_OK;
}
//------------------------------------------------------------------------
//
//	XAudioのロード　サブ関数	
//
//  TCHAR* szFileName    音源のファイル名
//  DWORD dwIndex       ロードするインデックス位置
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CXAudioSource::LoadAudioSub(const TCHAR* szFileName, DWORD dwIndex)
{
	HMMIO            hMmio = nullptr;  //WindowsマルチメディアAPIのハンドル(WindowsマルチメディアAPIはWAVファイル関係の操作用のAPI)
	DWORD            dwWavSize = 0; //WAVファイル内　WAVデータのサイズ（WAVファイルはWAVデータで占められているので、ほぼファイルサイズと同一）
	WAVEFORMATEX*    pwfex;         //WAVのフォーマット 例）16ビット、44100Hz、ステレオなど
	MMCKINFO         ckInfo;        //　チャンク情報
	MMCKINFO         riffckInfo = { 0 };    // 最上部チャンク（RIFFチャンク）保存用
	PCMWAVEFORMAT    pcmWaveForm;
	MMIOINFO         mmioInfo;

	TCHAR FName[256];
	_tcscpy_s(FName, szFileName);		// -- 2024.4.20

	//WAVファイル内のヘッダー情報（音データ以外）の確認と読み込み
	ZeroMemory(&mmioInfo, sizeof(MMIOINFO));								// -- 2017.1.22
	hMmio = mmioOpen(FName, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ);	// -- 2024.4.20
	if ( hMmio == nullptr )
	{
		MessageBox(0, _T("XAudio2 サウンドデータ　読み込み失敗"), nullptr, MB_OK);
		return E_FAIL;
	}

	//ファイルポインタをRIFFチャンクの先頭にセットする
	mmioDescend(hMmio, &riffckInfo, nullptr, 0);

	// ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);

	//フォーマットを読み込む
	mmioRead(hMmio, (HPSTR)&pcmWaveForm, sizeof(pcmWaveForm));
	pwfex = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
	memcpy(pwfex, &pcmWaveForm, sizeof(pcmWaveForm));
	pwfex->cbSize = 0;
	mmioAscend(hMmio, &ckInfo, 0);

	// WAVファイル内の音データの読み込み	
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);  //データチャンクにセット
	dwWavSize = ckInfo.cksize;
	m_pWavBuffer[dwIndex] = new BYTE[dwWavSize];
	DWORD dwOffset = ckInfo.dwDataOffset;
	mmioRead(hMmio, (HPSTR)m_pWavBuffer[dwIndex], dwWavSize);

	//ソースボイスにデータを入れる	
	if (FAILED(m_pXAudio->XAudio2()->CreateSourceVoice(&m_pSourceVoice[dwIndex], pwfex)))
	{
		MessageBox(0, _T("XAudio2 ソースボイス作成失敗"), 0, MB_OK);
		return E_FAIL;
	}
	m_dwWavSize[dwIndex] = dwWavSize;

	SAFE_DELETE_ARRAY(pwfex);

	return S_OK;
}


//------------------------------------------------------------------------
//
//	ＭＣＩのロード	
// ・対応の音源は、MP3、WMV、MIDI等である
// ・ＭＣＩインターフェイスは、高レベルＡＰＩである
//
//  TCHAR* szFileName    音源のファイル名
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CXAudioSource::LoadMci(const TCHAR* szFileName)
{
	TCHAR AllStr[512];
	TCHAR exe[265];

	// _tsplitpath_s関数で拡張子を取り去り、ファイル名のみを取りだして、別名m_szAliasNameとする
	// なお、各項目の長さとは、バイト数ではなく文字数である
	_tsplitpath_s(szFileName, NULL, NULL, NULL, NULL, m_szAliasName, sizeof(m_szAliasName)/sizeof(TCHAR), exe, 256 );     // ファイル名のみを得る

	_tcscpy_s( AllStr, _T("open "));
	_tcscat_s( AllStr, szFileName);
	_tcscat_s( AllStr, _T(" type mpegvideo alias "));
	_tcscat_s( AllStr, m_szAliasName);

	mciSendString( AllStr , nullptr, 0, nullptr);

	// 
	// mciSendStringの例
	// 
	// なお、mciSendStringはUnicode版　mciSendStringAはマルチバイト対応版
	// 
	//mciSendString(_T("open Sound/bgm1.mp3 type mpegvideo alias BGM01"), nullptr, 0, nullptr);
	//mciSendString(_T("open Sound/B003B.MID type mpegvideo alias BGM01"), nullptr, 0, nullptr);
	//mciSendString(_T("open Sound/bgm1.mp3 type mpegvideo"), nullptr, 0, nullptr);
	//mciSendString(_T("stop all"), nullptr, 0, nullptr);
	//mciSendString(_T("play BGM01 from 0 repeat"), nullptr, 0, nullptr);
	//mciSendString(_T("close all"), nullptr, 0, nullptr);　は、全ての曲をclose

	return S_OK;
}

//------------------------------------------------------------------------
//
//	再生	
//
//  int loop    ループ指定
//				ループしないとき：0又は省略
//				ループのとき    ：AUDIO_LOOP
//
//	戻り値  なし
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
//	XAudioの再生	
//
//  int loop    ループ指定
//				ループしないとき：0又は省略
//				ループのとき    ：AUDIO_LOOP
//
//	戻り値  なし
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
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;	// ループ再生
	}
	else {
		buffer.LoopCount = 0;
	}

	if (FAILED(m_pSourceVoice[m_dwSourceIndex]->SubmitSourceBuffer(&buffer)))
	{
		MessageBox(0,_T("XAudio2 ソースボイスにサブミット失敗"), 0, MB_OK);
		return;
	}
	m_pSourceVoice[m_dwSourceIndex]->Start(0, XAUDIO2_COMMIT_NOW);

	// 重ね合わせ再生のインデックスを進める
	m_dwSourceIndex++;
	if (m_dwSourceIndex >= m_dwSourceNum) m_dwSourceIndex = 0;
}
//------------------------------------------------------------------------
//
//	Mciの再生	
//
//  int loop    ループ指定
//				ループしないとき：0又は省略
//				ループのとき    ：AUDIO_LOOP
//
//	戻り値  なし
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

	// MPEGビデオを再生するとき、標準指定では別ウィンドウとなる
	// 自ウィンドウで再生するときは、事前に下記処理を行う
	// 但し、DirectX描画を一時的に停止する必要がある
	//TCHAR str[256];
	//_stprintf_s(str, _T("window %s handle %d"), m_szAliasName, (int)m_pXAudio->m_hWnd);
	//mciSendString( str , nullptr, 0, nullptr);
	//_stprintf_s(str, _T("put %s destination at 0 0 640 480"), m_szAliasName);
	//mciSendString( str , nullptr, 0, nullptr);


}

//------------------------------------------------------------------------
//
//	停止	
//
//  引数　なし
//
//	戻り値  なし
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
//	XAudioの停止	
//
//  引数　なし
//
//	戻り値  なし
//
//------------------------------------------------------------------------
void  CXAudioSource::StopAudio()
{
	// 重ね合わせの全てのソースを停止する
	for (DWORD i = 0; i < m_dwSourceNum; i++) {
		m_pSourceVoice[i]->Stop(0, 0);
		m_pSourceVoice[i]->FlushSourceBuffers();
	}
	m_dwSourceIndex = 0;	// インデックスを０に戻す	
}
//------------------------------------------------------------------------
//
//	Mciの停止	
//
//  引数　なし
//
//	戻り値  なし
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
//	ボリューム	
//
//  引数　ボリューム（基準値は1.0f）
//
//	戻り値  なし
//
//------------------------------------------------------------------------
void  CXAudioSource::Volume(float fVol)
{
	if (m_bWav)
	{
		VolumeAudio(fVol);
	}
	else {
		// 1000倍してＭＣＩに渡す
		VolumeMci((int)(fVol*1000));
	}
}
//------------------------------------------------------------------------
//
//	XAudioのボリューム	
//
//  引数　ボリューム（基準値は1.0f）
//
//	戻り値  なし
//
//------------------------------------------------------------------------
void  CXAudioSource::VolumeAudio(float fVol)
{

	//m_pSourceVoice[m_dwSourceIndex]->SetVolume(1.0f);  // マスターボリュームの設定　標準値が1.0、0.0は無音
	//   ボリューム レベルは、 - 224 ～ 224 の浮動小数点振幅逓倍として表現され、最大ゲインは 144.5 dB です。
	//   ボリューム レベル 1.0 は減衰またはゲインがないことを意味し、0 は無音を意味します。
	//   負のレベルは、オーディオのフェーズを反転させるために使用できます。

	// 重ね合わせの全てのソースのボリュームを設定する
	for (DWORD i = 0; i < m_dwSourceNum; i++) {
		m_pSourceVoice[i]->SetVolume(fVol);
	}

}
//------------------------------------------------------------------------
//
//	Mciのボリューム	
//
//  引数　ボリューム（基準値は1000。Volumeメソッドで1000倍して渡されてくるため）
//
//	戻り値  なし
//
//------------------------------------------------------------------------
void  CXAudioSource::VolumeMci(int nVol)
{
	// ボリュームの設定　　　ボリュームレベル（0～1000）100%が1000

	TCHAR str[512];

	_stprintf_s(str, _T("setaudio %s volume to %d"), m_szAliasName, nVol);
	mciSendString( str , nullptr, 0, nullptr);

}
