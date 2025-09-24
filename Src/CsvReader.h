#pragma once
#include <string>
#include <vector>

/// <summary>
/// CSVを読むクラス
/// </summary>
class CsvReader {
public:
	CsvReader(std::string filename);
	~CsvReader();

	/// <summary>
	/// CSVファイルの行数を返す
	/// </summary>
	/// <returns>行数</returns>
	unsigned int GetLines();

	/// <summary>
	/// CSVの桁数を返す
	/// 行によって桁数が変わる可能性があるので、引数で行を取得する
	/// 行番号は、Excelの表示行ではなく、0から指定する
	/// </summary>
	/// <param name="line">行番号（Excelの表示行数-1)</param>
	/// <returns>その行の桁数</returns>
	unsigned int GetColumns(unsigned int line);

	/// <summary>
	/// 指定位置の文字列を取得する
	/// </summary>
	/// <param name="line">行</param>
	/// <param name="column">桁</param>
	/// <returns>文字列</returns>
	std::string GetString(unsigned int line, unsigned int column);

	/// <summary>
	/// 指定位置の内容をintで取得する
	/// </summary>
	/// <param name="line">行</param>
	/// <param name="column">桁</param>
	/// <returns>整数値</returns>
	int GetInt(unsigned int line, unsigned int column);

	/// <summary>
	/// 指定位置の内容をfloatで取得する
	/// </summary>
	/// <param name="line">行</param>
	/// <param name="column">桁</param>
	/// <returns>小数値</returns>
	float GetFloat(unsigned int line, unsigned int column);
private:
	struct LINEREC {
		std::vector<std::string> record;
	};
	std::vector<LINEREC> all;
};
