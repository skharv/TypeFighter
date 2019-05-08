#ifndef LEADERBOARDHANDLER_H
#define LEADERBOARDHANDLER_H

#include <steam\steam_api.h>


class LeaderboardHandler
{
private:
	SteamLeaderboard_t currentLeaderboard;

	bool boardReady = false;

	int min(int a, int b) { return (a < b ? a : b); };
public:
	int leaderboardEntriesSize;
	LeaderboardEntry_t leaderboardEntries[10];

	void FindLeaderboard(const char *LeaderboardName);
	bool UploadScore( int Score);
	bool DownloadScore();

	bool DownloadTopScore();

	bool IsBoardReady();
	void DisplayTopBoard() { boardReady = true; };

	void OnFindLeaderboard(LeaderboardFindResult_t *pResult, bool bIOFailure);
	CCallResult<LeaderboardHandler, LeaderboardFindResult_t> callResultFindLeaderboard;
	void OnUploadScore(LeaderboardScoreUploaded_t *pResult, bool bIOFailure);
	CCallResult<LeaderboardHandler, LeaderboardScoreUploaded_t> callResultUploadScore;
	void OnDownloadScore(LeaderboardScoresDownloaded_t *pResult, bool bIOFailure);
	CCallResult<LeaderboardHandler, LeaderboardScoresDownloaded_t> callResultDownloadScore;

	LeaderboardHandler();
	~LeaderboardHandler();
};

#endif

