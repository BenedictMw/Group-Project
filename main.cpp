#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;
    struct  team {
        string name ;
        string localtown;
        string stadium;
    };
    struct Match {
        string hometeam;
        string awayteam;
        string localtown;
        string stadium;
        int leg;
        int weekend;
    };
vector<team> readTeamsFromCSV(const string& filename) {
    vector<team> teams;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Failed to open file!" << endl;
        return teams;
    }

    string line, name, localTown, stadium;
    getline(file, line);

    while (getline(file, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);

        name = line.substr(0, pos1);
        localTown = line.substr(pos1 + 1, pos2 - pos1 - 1);
        stadium = line.substr(pos2 + 1);

        teams.push_back({name, localTown, stadium});
    }

    file.close();
    return teams;
}
vector<Match> generateFixtures(vector<team>& teams) {
    vector<Match> fixtures; // Holds the generated fixtures
    int numTeams =teams.size();
    int weekendCounter = 1;

    for (int i = 0; i < numTeams; ++i) {
        for (int j = i + 1; j < numTeams; ++j) {
            fixtures.push_back({teams[i].name, teams[j].name, teams[i].stadium, teams[i].localtown, 1, weekendCounter});
            fixtures.push_back({teams[j].name, teams[i].name, teams[j].stadium, teams[j].localtown, 2, weekendCounter + (numTeams - 1)});

            if (fixtures.size() % 2 == 0) {
                weekendCounter++;
            }
        }
    }

    return fixtures;
}
void saveFixturesToCSV(const string& filename, const vector<Match>& fixtures) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Failed to open file for writing!" << endl;
        return;
    }

    file << "Home Team,Away Team,Stadium,Local Town,Leg,Weekend\n";
    for (const auto& fixture : fixtures) {
        file << fixture.hometeam << "," << fixture.awayteam << ","
             << fixture.stadium << "," << fixture.localtown << ","
             << fixture.leg << "," << fixture.weekend << "\n";
    }

    file.close();
}
int main() {
    string filename = "../teams.csv";
    vector<team> teams = readTeamsFromCSV(filename);  // Read teams from the CSV file

    if (teams.empty()) {
        cerr << "No teams loaded from the CSV file." << endl;
        return 1;
    }

    vector<Match> fixtures = generateFixtures(teams);  // Generate the fixtures

    // Display the generated fixtures
    cout << "Generated Fixtures:\n";
    for (const auto& fixture : fixtures) {
        cout << "Weekend #" << fixture.weekend << ": " << fixture.hometeam
             << " vs " << fixture.awayteam << " at " << fixture.stadium
             << " (" << fixture.localtown << "), Leg: " << fixture.leg << "\n";
    }


    saveFixturesToCSV("fixtures.csv", fixtures);

    return 0;
}



