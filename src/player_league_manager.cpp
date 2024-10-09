#include "player_league_manager.h"

// class Player
Player::Player() {
	name = "";
	goals = 0;
	playedMatches = 0;
	team = "";
	nationality = "";
}

Player::Player(std::string newName, int newGoals, int newPlayedMatches, std::string newTeam, std::string newNationality) {
	name = newName;
	goals = newGoals;
	playedMatches = newPlayedMatches;
	team = newTeam;
	nationality = newNationality;
}

// Player's getters
std::string Player::getName() const { 
	return name; 
}
std::string Player::getTeam() const { 
	return team; 
}
std::string Player::getNationality() const { 
	return nationality; 
}
int Player::getGoals() const { 
	return goals; 
}
int Player::getMatchesPlayed() const { 
	return playedMatches; 
}

// Player's setters
void Player::setName(const std::string& newName) { 
	name = newName; 
}
void Player::setTeam(const std::string& newTeam) { 
	team = newTeam; 
}
void Player::setNationality(const std::string& newNationality) { 
	nationality = newNationality; 
}
void Player::setGoals(int newGoals) { 
	goals = newGoals; 
}
void Player::setMatchesPlayed(int newPlayedMatches) { 
	playedMatches = newPlayedMatches; 
}


// class League
League::League()
{
	name = "";
}
League::League(std::string leagueName) { 
	name = leagueName;
}
std::string League::getName() const { 
	return name; 
}
std::vector<Player> League::getTopScorers() const {
	return topScorers;
}
void League::addTopScorer(const Player& scorer) { 
	topScorers.emplace_back(scorer); 
}


// class ScorersLeagueManager
ScorersLeagueManager::ScorersLeagueManager() {}

ScorersLeagueManager::~ScorersLeagueManager() {
	if (scorersFile.is_open()) {
		scorersFile.close();
	}
}

std::vector<League> ScorersLeagueManager::getLeagues() {
	return leagues;
}

void ScorersLeagueManager::addLeague(const League& league) {
	leagues.emplace_back(league);
}

/// <summary>
/// Member function to write fetched data to a csv file with time info
/// </summary>
/// <param name="outFilename">name of csv file to write data to</param>
void ScorersLeagueManager::writeData(const std::string& outFilename) {
	scorersFile.open(outFilename);
	scorersFile << "\xEF\xBB\xBF";
	if (!scorersFile.is_open()) {
		std::cerr << "Unable to open file to write data.\n";
		return;
	}

	// Get current time as a string
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t nowC = std::chrono::system_clock::to_time_t(now);
	char timeStr[100];
	ctime_s(timeStr, sizeof(timeStr), &nowC);

	// Write fetched data of all leagues
	scorersFile << "League,Name,Club,Country,Matches Played,Goals\n";
	for (const auto& league : leagues) {

		for (const Player& player : league.getTopScorers()) {
			scorersFile << league.getName() << ","
				<< player.getName() << ","
				<< player.getTeam() << ","
				<< player.getNationality() << ","
				<< player.getMatchesPlayed() << ","
				<< player.getGoals() << "\n";
		}
	}
	scorersFile << "\n";
	scorersFile << "Last Updated: " << timeStr << std::endl;
	std::cout << "File written successfully.\n";
}
