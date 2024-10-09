// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/Exception.h>
#include <cstdlib>
#include "constants.h"
#include "thread_pool.h"
#include "player_league_manager.h"

namespace pn = Poco::Net;
namespace pj = Poco::JSON;

/// <summary>
/// Fetching API for data of top scorers in specified leagues
/// </summary>
/// <param name="manager">object to store and write leagues' data</param>
/// <param name="leagueName">name of league to fetch for data</param>
/// <param name="leagueEndpoint">endpoint of league</param>
/// <returns>a league object containing top scorers' data</returns>
League fetchScorers(ScorersLeagueManager& manager, const std::string& leagueName, 
    const std::string& leagueEndpoint, const char* apiKey) {

    League league(leagueName);

    try {
        // Create a HTTP client session
        pn::HTTPClientSession session(api::BASE_URL);

        // Create an HTTP request (GET)
        pn::HTTPRequest request(pn::HTTPRequest::HTTP_GET, leagueEndpoint, pn::HTTPMessage::HTTP_1_1);
        request.set(api::HEADER_NAME, apiKey);
        session.sendRequest(request);

        // Get response
        pn::HTTPResponse response;
        std::istream& responseStream = session.receiveResponse(response);

        // Check status of response
        if (response.getStatus() == httpStatus::HTTP_OK) {
            // Convert the response stream to a string
            std::string responseString;
            Poco::StreamCopier::copyToString(responseStream, responseString);
            
            // Parse JSON response
            pj::Parser parser;
            auto scorers = parser.parse(responseString);

            // Extract JSON object
            pj::Object::Ptr scorerList = scorers.extract<pj::Object::Ptr>();
            pj::Array::Ptr scorerArray = scorerList->getArray(api::SCORER_SECTION);

            // Accessing values from JSON object
            for (size_t i = 0; i < scorerArray->size(); ++i) {
                pj::Object::Ptr scorer = scorerArray->getObject(i);

                // Get player's name, goals, team, matches played and nationality
                pj::Object::Ptr playerInfo  = scorer->getObject(api::PLAYER_INFO);
                pj::Object::Ptr teamInfo    = scorer->getObject(api::TEAM_INFO);

                std::string playerName      = playerInfo->getValue<std::string>(api::NAME);
                std::string playerTeam      = teamInfo->getValue<std::string>(api::PLAYER_TEAM);
                std::string nationality     = playerInfo->getValue<std::string>(api::NATIONALITY);
                int goals                   = scorer->getValue<int>(api::GOALS);
                int playedMatches           = scorer->getValue<int>(api::PLAYED_MATCHES);

                // Create new player based on data fetched and add to league's top scorers
                Player newScorer(playerName, goals, playedMatches, playerTeam, nationality);
                league.addTopScorer(newScorer);
            }
            std::cout << httpStatus::SUCCESSFUL_FETCH << leagueName << std::endl;
            std::cout << "Fetched players: " << league.getTopScorers().size() << std::endl;
            return league;

        } else {
            throw Poco::Exception(httpStatus::ERROR_FETCHING_DATA + leagueName + ". " + response.getReason());
        }
    } catch (const Poco::Exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}

int main() {
    char* apiKey = nullptr;
    size_t size = 0;

    ScorersLeagueManager dataManager;
    ThreadPool pool(THREAD_NUM);

    std::vector<std::pair<std::string, std::string>> allLeagues = {
        {league::PREMIER_LEAGUE, api::PL_ENDPOINT},
        {league::LA_LIGA, api::PD_ENDPOINT},
        {league::BUNDESLIGA, api::BL_ENDPOINT}
    };

    if (_dupenv_s(&apiKey, &size, api::API_KEY_ENV_VAR) == 0 && apiKey != nullptr) {

        // Enqueue fetch tasks 
        while (true) {
            std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
            std::time_t nowTimeT = std::chrono::system_clock::to_time_t(now);

            // Initialize nowGmtime to point to epoch time 1/1/1970
            std::tm nowGmtime = {};
            nowGmtime.tm_year = 70;
            nowGmtime.tm_mon = 0;
            nowGmtime.tm_mday = 1;

            errno_t status = gmtime_s(&nowGmtime, &nowTimeT);

            // Calculate fetching interval for weekdays/weekend GMT time
            std::chrono::time_point<std::chrono::system_clock> nextFetch;
            if (status == 0) {
                if (nowGmtime.tm_wday == 0 || nowGmtime.tm_wday == 6) {
                    nextFetch = now + std::chrono::hours(api::FETCH_WEEKEND);
                    std::cout << api::WEEKEND_FETCH_INTERVAL;
                }
                else {
                    nextFetch = now + std::chrono::hours(api::FETCH_WEEKDAY);
                    std::cout << api::WEEKDAY_FETCH_INTERVAL;
                }
            }
            else {
                std::cerr << api::ERROR_FETCH_INTERVAL;
            }

            std::vector<std::future<League>> futures;

            // Enqueue fetching tasks to thread pool and add futures storing leagues' data in vector
            for (const auto& league : allLeagues) {
                std::packaged_task<League()> wrappedTask([&dataManager, leagueName = league.first, endpoint = league.second, apiKey]
                () { return fetchScorers(dataManager, std::move(leagueName), std::move(endpoint), std::move(apiKey));
                    });

                std::future<League> future = wrappedTask.get_future();
                futures.emplace_back(std::move(future));
                pool.enqueue(std::move(wrappedTask));
            }

            // Get league objects from futures
            for (auto& future : futures) {
                try {
                    League newLeague = future.get();
                    dataManager.addLeague(newLeague);
                }
                catch (const std::exception& ex) {
                    std::cerr << httpStatus::ERROR_FETCHING_DATA << ex.what() << std::endl;
                }
            }

            dataManager.writeData(OUTPUT_FILE);

            // Fetch new data periodically
            std::this_thread::sleep_until(nextFetch);
        }
    } else {
        std::cerr << api::ENV_VAR_ERROR;
    }

    return 0;
}

