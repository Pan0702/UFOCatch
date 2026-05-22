#pragma once
#include <string>
#include "FbxMesh.h"

/// <summary>汎用ユーティリティで使う Animator の情報と処理をまとめる型</summary>
class Animator
{
public:
    /// Animator を初期化する
    Animator();
    /// Animator の終了処理を行う
    ~Animator();
    /// 毎フレームの状態を更新する
    void Update();

    /// Model を設定する
    /// @param mesh mesh に渡す値
    void SetModel(CFbxMesh* mesh);
    /// 再生する
    /// @param id id に渡す値
    /// @param force force に渡す値
    void Play(int id, bool force = false);
    /// Merge Play の処理を行う
    /// @param id id に渡す値
    /// @param time time に渡す値
    void MergePlay(int id, float time = 0.2f);
    /// Play Speed を設定する
    /// @param speed speed に渡す値
    void SetPlaySpeed(float speed = 1.0f);
    /// ing ID を再生する
    /// @return 処理結果の数値
    int PlayingID();
    /// ID を減算する
    /// @return 処理結果の数値
    int SubID();
    /// Rate を返す
    /// @return 計算結果の値
    float Rate();
    /// Current Frame を返す
    /// @return 計算結果の値
    float CurrentFrame();
    /// Frame を減算する
    /// @return 計算結果の値
    float SubFrame();
    /// Finished を返す
    /// @return 成功または条件を満たす場合 true
    bool Finished();
    /// 停止する
    void Stop();
    /// Animation を保持しているか判定する
    /// @param id id に渡す値
    /// @return 成功または条件を満たす場合 true
    bool HasAnimation(int id) const;

private:
    CFbxMesh* base;

    /// <summary>汎用ユーティリティで使う Info の情報と処理をまとめる型</summary>
    struct Info
    {
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
