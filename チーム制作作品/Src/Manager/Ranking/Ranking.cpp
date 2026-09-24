#include "Ranking.h"

#include<fstream>
#include<iostream>
#include <filesystem>

#include"../../Utility/Utility.h"

Ranking* Ranking::ins = nullptr;

Ranking::Ranking() : 
	rankingList(),
	lastAddScoreRankIndex(-1)
{
}

void Ranking::Load(void)
{
	// 最大ランキング数分の領域を確保
	for (int bossType = 0; bossType < (int)BOSS_TYPE::Max; bossType++) { rankingList[bossType].resize(RANKING_MAX); }

#pragma region ランキングデータを読み込む
	// CSVファイルを開く
	std::ifstream ifs = std::ifstream("Data/Ranking/Ranking.csv");

	// 例外処理：ファイルが開けなかった場合
	if (!ifs) {
		// エラーメッセージを表示
		std::cerr << "ランキングデータの読み込みに失敗しました。" << '\n';

		return;
	}

	for (int bossType = 0; bossType < (int)BOSS_TYPE::Max; bossType++) {

		// ファイルから2行読み込み
		std::string scoreLine, nameLine;
		getline(ifs, scoreLine); getline(ifs, nameLine);

		// 読み込んだ行をカンマ区切りで分割し、配列に格納
		std::vector<std::string> scores = Split(scoreLine, ',');
		std::vector<std::string> names = Split(nameLine, ',');

		// 配列に格納されたデータをランキングリストに格納
		for (size_t i = 0; i < scores.size() && i < names.size() && i < rankingList[bossType].size(); i++) {
			rankingList[bossType][i].score = std::stof(scores[i]);
			rankingList[bossType][i].name = names[i];
		}
	}

	// ファイルを閉じる
	ifs.close();
#pragma endregion
}

void Ranking::Save(void)
{
	// 保存先
	const std::filesystem::path filePath = "Data/Ranking/Ranking.csv";

	// 保存先のフォルダが存在しない場合は作成する
	std::error_code ec;
	std::filesystem::create_directories(filePath.parent_path(), ec);

	// フォルダの作成に失敗した場合
	if (ec) {
		std::cerr << "ランキングデータの保存先フォルダの作成に失敗しました。" << '\n';
		return;
	}

	// CSVファイルを開く
	// ファイルが存在しない場合は自動的に新規作成される
	std::ofstream ofs(filePath);

	// 例外処理：ファイルが開けなかった場合
	if (!ofs) {
		std::cerr << "ランキングデータの保存に失敗しました。" << '\n';
		return;
	}

	for (int bossType = 0; bossType < (int)BOSS_TYPE::Max; bossType++) {

		// ランキングリストのスコアの内容をカンマ区切りで1行にまとめて保存する
		for (auto& score : rankingList[bossType]) { ofs << score.score << ","; }
		// 最後のカンマを削除する
		ofs.seekp(-1, std::ios_base::end);
		// 改行を入れる
		ofs << '\n';

		// 次にランキングリストの名前の内容をカンマ区切りで1行にまとめて保存する
		for (auto& score : rankingList[bossType]) { ofs << score.name << ","; }
		// 最後のカンマを削除する
		ofs.seekp(-1, std::ios_base::end);
		// 改行を入れる
		ofs << '\n';
	}

	// ファイルを閉じる
	ofs.close();
}

void Ranking::Release(void)
{
	// ランキング保存
	Save();

	// 保存を終えたらリストをクリア
	for (int bossType = 0; bossType < (int)BOSS_TYPE::Max; bossType++) { rankingList[bossType].clear(); }
}

void Ranking::AddScore(BOSS_TYPE bossType, float score)
{
	// 最終追加スコアランクインデックスをリセット
	lastAddScoreRankIndex = -1;

	std::vector<RankingData>& ranking = rankingList[(int)bossType];

	// 上から探索し、スコアを挿入する位置を見つける
	for (size_t i = 0; i < ranking.size(); i++) {

		// 未設定を発見した場合、シンプルにそこに挿入して終了する
		if (ranking[i].score == -1) {
			// スコアを挿入する
			ranking[i].score = score;

			// スコア追加位置を記録する
			lastAddScoreRankIndex = (int)i;

			// ソートは必要ないのでそのまま処理を終了
			break;
		}

		if (score < ranking[i].score) {
			// 現在スコアを一時変数に保持しておく
			RankingData work;
			work = ranking[i];

			// 現在スコアを新規スコアで上書きする
			ranking[i] = {};
			ranking[i].score = score;

			// スコア追加位置を記録する
			lastAddScoreRankIndex = (int)i;

			// 以降のスコアを1つずつ後ろにずらして並べ替える
			for (size_t j = ranking.size() - 1; j > i; j--) { ranking[j] = ranking[j - 1]; }

			// 一時変数に保持しておいたスコアを挿入する
			if (i < ranking.size() - 2) { ranking[i + 1] = work; }

			// 終了
			break;
		}

	}
}

void Ranking::SetLastAddScoreName(BOSS_TYPE bossType, const std::string& name)
{
	// 最後に追加したスコアのインデックスが有効な場合のみ名前を設定する
	if (lastAddScoreRankIndex != -1 && lastAddScoreRankIndex < (int)(rankingList[(int)bossType].size())) {
		rankingList[(int)bossType][lastAddScoreRankIndex].name = name;
	}
}

void Ranking::AllDeleteRankingList(void)
{
	// 全て未設定(-1,--)にする
	for (int bossType = 0; bossType < (int)BOSS_TYPE::Max; bossType++) {
		for (auto& score : rankingList[bossType]) { score.score = -1; score.name = "--"; }
	}
}

void Ranking::DeleteRankingList(BOSS_TYPE bossType, int rankIndex)
{
	// 指定されたランクインデックスが範囲内であるか確認（例外処理）
	if (rankIndex < 0 || (int)(rankingList[(int)bossType].size()) <= rankIndex) { return; }

	// 指定されたランクインデックスのスコアを削除（未設定にする）
	rankingList[(int)bossType][rankIndex].score = -1;
	rankingList[(int)bossType][rankIndex].name = "--";

	// 削除後、ランキングリストを詰める
	for (size_t i = rankIndex; i < rankingList[(int)bossType].size() - 1; i++) { rankingList[(int)bossType][i] = rankingList[(int)bossType][i + 1]; }

	// 最後の要素を未設定にする
	rankingList[(int)bossType][rankingList[(int)bossType].size() - 1].score = -1;
	rankingList[(int)bossType][rankingList[(int)bossType].size() - 1].name = "--";
}
