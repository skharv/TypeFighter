#include "LeaderboardHandler.h"

void LeaderboardHandler::FindLeaderboard(const char * LeaderboardName)
{
	currentLeaderboard = NULL;

	SteamAPICall_t hSteamAPICall = SteamUserStats()->FindOrCreateLeaderboard(LeaderboardName, k_ELeaderboardSortMethodDescending, k_ELeaderboardDisplayTypeNumeric);
	callResultFindLeaderboard.Set(hSteamAPICall, this, &LeaderboardHandler::OnFindLeaderboard );
}

bool LeaderboardHandler::UploadScore(int Score)
{
	if(!currentLeaderboard)
		return false;

	SteamAPICall_t hSteamAPICall = SteamUserStats()->UploadLeaderboardScore(currentLeaderboard, k_ELeaderboardUploadScoreMethodKeepBest, (int)Score, NULL, 0);

	callResultUploadScore.Set(hSteamAPICall, this, &LeaderboardHandler::OnUploadScore);

	return true;
}

bool LeaderboardHandler::DownloadScore()
{
	if (!currentLeaderboard)
		return false;

	SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(currentLeaderboard, k_ELeaderboardDataRequestGlobalAroundUser, -4, 5);

	callResultDownloadScore.Set(hSteamAPICall, this, &LeaderboardHandler::OnDownloadScore);

	return true;
}


bool LeaderboardHandler::DownloadTopScore()
{
	if (!currentLeaderboard)
		return false;

	SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(currentLeaderboard, k_ELeaderboardDataRequestGlobal, 0, 9);

	callResultDownloadScore.Set(hSteamAPICall, this, &LeaderboardHandler::OnDownloadScore);

	return true;
}

bool LeaderboardHandler::IsBoardReady()
{
	if (boardReady)
	{
		boardReady = false;
		return true;
	}
	else
		return boardReady;
}

void LeaderboardHandler::OnFindLeaderboard(LeaderboardFindResult_t * pResult, bool bIOFailure)
{
	if (!pResult->m_bLeaderboardFound || bIOFailure)
		return;

	currentLeaderboard = pResult->m_hSteamLeaderboard;
	boardReady = true;
}

void LeaderboardHandler::OnUploadScore(LeaderboardScoreUploaded_t * pResult, bool bIOFailure)
{
	bool b = pResult->m_bScoreChanged;
	int s = pResult->m_nScore;
	int r = pResult->m_nGlobalRankNew;
	int p = pResult->m_nGlobalRankPrevious;

	if (!pResult->m_bSuccess || bIOFailure)
	{
		throw("Unable to upload score to Steam");
	}
}

void LeaderboardHandler::OnDownloadScore(LeaderboardScoresDownloaded_t * pResult, bool bIOFailure)
{
	if (!bIOFailure)
	{
		leaderboardEntriesSize = min(pResult->m_cEntryCount, 10);

		for (int index = 0; index < leaderboardEntriesSize; index++)
		{
			SteamUserStats()->GetDownloadedLeaderboardEntry(pResult->m_hSteamLeaderboardEntries, index, &leaderboardEntries[index], NULL, 0);
		}
	}
}

LeaderboardHandler::LeaderboardHandler()
{
	boardReady = false;
	currentLeaderboard = NULL;
}

LeaderboardHandler::~LeaderboardHandler()
{
}
