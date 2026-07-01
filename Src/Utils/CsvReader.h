#pragma once
#include <string>
#include <vector>

/// <summary>
/// CSVを読むクラス
/// </summary>
class CsvReader
{
public:
    /// CsvReader を初期化する
    /// @param filename ファイル名
    CsvReader(std::string filename);
    /// CsvReader の終了処理を行う
    ~CsvReader();

    /// Lines を取得する
    /// @return 処理結果
    unsigned int GetLines();

    /// Columns を取得する
    /// @param line line に渡す値
    /// @return 処理結果
    unsigned int GetColumns(unsigned int line);

    /// String を取得する
    /// @param line line に渡す値
    /// @param column column に渡す値
    /// @return 取得した文字列
    std::string GetString(unsigned int line, unsigned int column);

    /// Int を取得する
    /// @param line line に渡す値
    /// @param column column に渡す値
    /// @return 処理結果の数値
    int GetInt(unsigned int line, unsigned int column);

    /// Float を取得する
    /// @param line line に渡す値
    /// @param column column に渡す値
    /// @return 計算結果の値
    float GetFloat(unsigned int line, unsigned int column);

private:
    /// <summary>汎用ユーティリティで使う LINEREC の情報と処理をまとめる型</summary>
    struct LINEREC
    {
        std::vector<std::string> record;
    };

    std::vector<LINEREC> all;
};
