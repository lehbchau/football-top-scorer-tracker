#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <mutex>
#include <sstream>
#include <chrono>
#include <ctime>
#include "constants.h"

class Player {
private:
	std::string name;
	int goals;
	int playedMatches;
	std::string team;
	std::string nationality;

public:
	Player();
	Player(std::string newName, int newGoals, int newPlayedMatches, std::string newTeam, std::string newNationality);

	// Getters
	std::string getName() const;
	std::string getTeam() const;
	std::string getNationality() const;
	int getGoals() const;
	int getMatchesPlayed() const;

	// Setters
	void setName(const std::string& newName);
	void setTeam(const std::string& newTeam);
	void setNationality(const std::string& newNationality);
	void setGoals(int newGoals);
	void setMatchesPlayed(int newPlayedMatches);
};

class League {
private:
	std::string name;
	std::vector<Player> topScorers;

public:
	League();
	League(std::string leagueName);
	std::string getName() const;
	std::vector<Player> getTopScorers() const;
	void addTopScorer(const Player& scorer);
};

class ScorersLeagueManager {
private:
	std::ofstream scorersFile;
	std::vector<League> leagues;

public:
	ScorersLeagueManager();
	~ScorersLeagueManager();

	std::vector<League> getLeagues();
	void addLeague(const League& league);
	void writeData(const std::string& outFilename);
};