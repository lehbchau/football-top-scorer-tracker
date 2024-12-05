# Football Top Scorer Tracker

## Description
**Football Top Scorer Tracker** is a C++ application designed to periodically fetch and update the latest statistics of top scorers across various European football leagues. By utilizing a **RESTful API** and leveraging **multi-threading**, it provides valuable insights into top scorers, including their nationality, goals scored, and matches played for the 2024-2025 season. The application generates outputs in **CSV format** for easy analysis and sharing of data. 

## Key Features
- Fetches top scorer data from multiple football leagues (currently including the **Premier League, La Liga, and Bundesliga**)
- **Multi-threaded** design for efficient data retrieval
- **Configurable API settings** for flexibility
- Outputs data in **CSV format** for easy sharing and analysis in spreadsheet applications 
- Periodically updates player statistics

## Technologies Used
- **C++**
- Libraries:
  - **Poco C++ Libraries** for **HTTP requests** and **JSON parsing**
  - **Standard C++ Libraries** for **data structure** and **multi-threading** (used in building custom **thread pool**)
- **API Integration**

## Getting Started

### Prerequisites
- C++ compiler
- Poco C++ Libraries installed
- An API key (sign up at [Football-Data.org](https://www.football-data.org/) to get one)

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/lehbchau/football-top-scorer-tracker.git
   ```
2. Navigate to the project directory:
   ```bash
   cd football-top-scorer-tracker
   ```
3. Set API key as environment variable:
   ```bash
   setx FB_API_KEY "your_api_key"
   ```
4. Compile project:
   ```bash
   make
   ```

### Usage
1. Run the application:
```bash
   ./football-top_scorer_tracker
   ```
2. The application will fetch and process data from API, generating a file named ```top_scorers.csv ```

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments
Special thanks to:
- Football-Data.org: For providing the reliable and comprehensive API for football data
- Poco C++ Libraries: For their efficient network and JSON handling capabilities, essential for this project
- The open-source community: For their contributions to the development of tools and libraries used in this project
- ChatGPT: For introducing me to the Poco C++ Libraries, which proved to be invaluable for this project
