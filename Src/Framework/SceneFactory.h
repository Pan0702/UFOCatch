#pragma once

#include <memory>
#include <string>

/// <summary>各シーンが継承する共通インターフェース</summary>
class SceneBase;

/// <summary>
/// SceneManagerから呼び出され、Sceneを作成するクラス
/// SceneManagerが肥大化しないように、作成ロジックをここに分離している
/// 各シーンは、文字列（名前）で指定して作成できるようにする
/// </summary>
class SceneFactory
{
public:
    /// First を作成する
    /// @return 処理結果
    static std::unique_ptr<SceneBase> CreateFirst();

    /// 作成する
    /// @param name 名前
    /// @return 処理結果
    static std::unique_ptr<SceneBase> Create(const std::string& name);
};
