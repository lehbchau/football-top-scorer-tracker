#pragma once

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <cstdlib>

const size_t THREAD_NUM = 3;
const std::string OUTPUT_FILE = "top_scorers.csv";
const int NUMBER_OF_FIELDS = 6;

namespace api {
	const std::string BASE_URL		= "api.football-data.org";
	const std::string PL_ENDPOINT		= "/v4/competitions/PL/scorers/?season=2024";
	const std::string PD_ENDPOINT		= "/v4/competitions/PD/scorers/?season=2024";
	const std::string BL_ENDPOINT		= "/v4/competitions/BL1/scorers/?season=2024";
	const std::string HEADER_NAME		= "X-Auth-Token";
	constexpr const char* API_KEY_ENV_VAR	= "FB_API_KEY";


	const std::string SCORER_SECTION	= "scorers";
	const std::string PLAYER_INFO		= "player";
	const std::string TEAM_INFO		= "team";
	const std::string NAME			= "name";
	const std::string NATIONALITY		= "nationality";
	const std::string GOALS			= "goals";
	const std::string PLAYER_TEAM		= "shortName";
	const std::string PLAYED_MATCHES	= "playedMatches";

	const int FETCH_WEEKDAY = 6;
	const int FETCH_WEEKEND = 3;

	const std::string ENV_VAR_ERROR		 = "FB_API_KEY environment variable not set.\n";
	const std::string ERROR_FETCH_INTERVAL	 = "Error calculating fetch interval.\n";
	const std::string WEEKEND_FETCH_INTERVAL = "Weekend fetch interval: Every 3 hours\n";
	const std::string WEEKDAY_FETCH_INTERVAL = "Weekday fetch interval: Every 6 hours\n";
}

namespace httpStatus {
	const std::string ERROR_FETCHING_DATA = "Failed to fetch data: ";
	const std::string SUCCESSFUL_FETCH = "Data successfully fetched for: ";
	const int HTTP_OK = 200;
}

namespace league {
	const std::string PREMIER_LEAGUE = "Premier League";
	const std::string LA_LIGA = "La Liga";
	const std::string BUNDESLIGA = "Bundesliga";
}

#endif
