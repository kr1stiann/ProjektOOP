#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <queue>
#include <iomanip>

//Nicola start
class Athlete {
public:
    std::string club;
    std::string name;
    std::string surname;
    char sex;
    int age;

    // Binary indicators for event participation 1 if they are participating 0 if not
    int sprint60m;
    int sprint200m;
    int middle800m;
    int middle1500m;
    int long3000m;
    int hurdles60m;
    int longJump;
    int tripleJump;
    int highJump;
    int poleVault;
    int shotPut;

    //Nikola stop

    //Stefan start
    // Constructor
    Athlete(std::string c, std::string n, std::string s, char sx, int a,
            int s60m, int s200m, int m800m, int m1500m, int l3000m, int h60m,
            int lj, int tj, int hj, int pv, int sp)
        : club(c), name(n), surname(s), sex(sx), age(a),
          sprint60m(s60m), sprint200m(s200m), middle800m(m800m),
          middle1500m(m1500m), long3000m(l3000m), hurdles60m(h60m),
          longJump(lj), tripleJump(tj), highJump(hj), poleVault(pv), shotPut(sp) {}

       
    //Function to add an event for an athlete.
    void addEvent(const std::string& eventName, int startTime, int endTime) {
        scheduledEvents.emplace_back(startTime, endTime);
    }

    // Method to check for schedule conflicts
    bool hasScheduleConflict(int newStartTime, int newEndTime) {
        for (const auto& event : scheduledEvents) {
            if (newStartTime < event.second && newEndTime > event.first) {
                return true; // There is a conflict
            }
        }
        return false; // No conflict
    }
    std::vector<std::pair<int, int>> scheduledEvents;

    
        bool isAvailable(int startTime, int endTime) {
        return !hasScheduleConflict(startTime, endTime);
    }
};

std::vector<std::string> sports = {"Sprint60", "Sprint200","Middle800","Middle1500","Long3000","Hurdles60","LongJump","TripleJump","HighJump","PoleVault","ShotPut"}; // List all sports here

//Stefan stop

//Arman start
std::map<std::string, int> maxPersonsPerHeat = {
    {"Sprint60", 8},    // Max 8 athletes per heat for the 60m sprint
    {"Sprint200", 8},   // Max 8 athletes per heat for the 200m sprint
    {"Middle800", 6},   // Max 6 athletes per heat for the 800m middle distance
    {"Middle1500", 6},  // Max 6 athletes per heat for the 1500m middle distance
    {"Long3000", 6},   // Max  athletes per heat for the 3000m long distance
    {"Hurdles60", 6},   // Max 6 athletes per heat for the 60m hurdles
    {"LongJump", 2},    // Max 2 athletes per heat for the long jump
    {"TripleJump", 2},  // Max 2 athletes per heat for the triple jump
    {"HighJump", 2},    // Max 2 athletes per heat for the high jump
    {"PoleVault", 1},   // Max 1 athletes per heat for the pole vault
    {"ShotPut", 2}      // Max 2 athletes per heat for the shot put
    };


std::string getAgeDivision(int age) {
    if (age <= 8) return "8 & Under";
    if (age >= 9 && age <= 10) return "9 - 10";
    if (age >= 11 && age <= 12) return "11 - 12";
    if (age >= 13 && age <= 14) return "13 - 14";
    if (age >= 15 && age <= 16) return "15 - 16";
    if (age >= 17 && age <= 18) return "17 - 18";
    return "Over 18"; // Or handle ages over 18 in another way
}

// Helper function to check if an athlete is participating in a given sport
bool isParticipating(const Athlete& athlete, const std::string& sportName) {
    if (sportName == "Sprint60" && athlete.sprint60m) return true;
    if (sportName == "Sprint200" && athlete.sprint200m) return true;
    if (sportName == "Middle800" && athlete.middle800m) return true;
    if (sportName == "Middle1500" && athlete.middle1500m) return true;
    if (sportName == "Long3000" && athlete.long3000m) return true;
    if (sportName == "Hurdles60" && athlete.hurdles60m) return true;
    if (sportName == "LongJump" && athlete.longJump) return true;
    if (sportName == "TripleJump" && athlete.tripleJump) return true;
    if (sportName == "HighJump" && athlete.highJump) return true;
    if (sportName == "PoleVault" && athlete.poleVault) return true;
    if (sportName == "ShotPut" && athlete.shotPut) return true;   
    return false;
}
//Arman stop

//Majdi start

std::vector<Athlete> fileInput(std::string name) {
     
    std::ifstream file(name);
     std::string line;
     std::vector<Athlete> athletes;

     
     while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::string club, name, surname;
        char sex;
        int age;
        int values[11] = {0}; // Array to hold binary values for event participation

        std::getline(ss, club, ',');
        std::getline(ss, name, ',');
        std::getline(ss, surname, ',');
        ss >> sex; ss.ignore();
        ss >> age; ss.ignore();

        for (int i = 0; i < 11; ++i) { // Assuming there are 11 events
            if (std::getline(ss, cell, ',') && !cell.empty()) {
                values[i] = 1;
            }
        }

        // Create an Athlete object with the parsed data
        Athlete athlete(club, name, surname, sex, age,
                        values[0], values[1], values[2], values[3], values[4],
                        values[5], values[6], values[7], values[8], values[9], values[10]);
        athletes.push_back(athlete);
    }

    file.close();

    return athletes;

}
//Majdi stop


//Kristian start

void createFullDaySchedule(std::vector<Athlete>& allAthletes) {
    //Makes a csv file and tries to open it.
    std::ofstream scheduleFile("full_day_schedule.csv");
    if (!scheduleFile.is_open()) {
        std::cerr << "Failed to open full_day_schedule.csv for writing." << std::endl;
        return;
    }

    scheduleFile << "Sport, Age Group, Gender, Start Time, Athlete Names\n"; // CSV Header

    // Maps to track the next available time for each sport
    std::map<std::string, int> nextAvailableTime;
    for (const std::string& sport : sports) {
        nextAvailableTime[sport] = 8 * 60; // Initialize to 8 AM for each sport
    }

    const int MAX_REQUEUE_LIMIT = 3; // Limit for the number of times an athlete can be re-queued
    const int BREAK_DURATION = 10; // Duration for breaks between events (in minutes)

    // Iterate through each sport and schedule athletes
    for (const std::string& sportName : sports) {
        int athletesPerHeat = maxPersonsPerHeat[sportName];

        // Group athletes by age division and gender
        std::map<std::string, std::map<char, std::vector<Athlete*>>> groupedAthletes;
        for (Athlete& athlete : allAthletes) {
            if (isParticipating(athlete, sportName)) {
                std::string ageGroup = getAgeDivision(athlete.age);
                groupedAthletes[ageGroup][athlete.sex].push_back(&athlete);
            }
        }

        // Schedule athletes by age division and gender
        for (const auto& ageGroup : groupedAthletes) {
            for (const auto& genderGroup : ageGroup.second) {
                std::queue<Athlete*> participantsQueue;
                std::map<Athlete*, int> requeueCounts; // Track the number of times an athlete is re-queued

                for (Athlete* athlete : genderGroup.second) {
                    participantsQueue.push(athlete);
                    requeueCounts[athlete] = 0;
                }
//Hit
                bool isFirstEvent = true; // Flag to check if it's the first event for the sport
                while (!participantsQueue.empty()) {
                    std::string athleteInfo;
                    int currentTime = nextAvailableTime[sportName];

                    std::vector<Athlete*> athletesScheduledForThisHeat;
                    while (!participantsQueue.empty() && athletesScheduledForThisHeat.size() < athletesPerHeat) {
                        Athlete* athlete = participantsQueue.front();
                        participantsQueue.pop();

                        if (requeueCounts[athlete] >= MAX_REQUEUE_LIMIT) {
                            continue; // Skip athletes that have been re-queued too many times
                        }

                        // Check if the athlete is available at this time
                        if (athlete->isAvailable(currentTime, currentTime + 10)) {
                            athletesScheduledForThisHeat.push_back(athlete);
                            athleteInfo += athlete->name + " " + athlete->surname + ", ";
                        } else {
                            // Re-queue the athlete for later consideration
                            participantsQueue.push(athlete);
                            requeueCounts[athlete]++;
                        }
                    }
                    
                    // Schedule the athletes for this heat
                    for (Athlete* athlete : athletesScheduledForThisHeat) {
                        athlete->addEvent(sportName, currentTime, currentTime + 10);
                    }

                    if (!athleteInfo.empty()) {
                        athleteInfo.pop_back(); // Remove trailing comma
                        athleteInfo.pop_back(); // Remove trailing space

                        scheduleFile << sportName << ", "
                                     << ageGroup.first << ", "
                                     << genderGroup.first << ", "
                                     << std::setw(2) << std::setfill('0') << currentTime / 60 << ":" 
                                     << std::setw(2) << std::setfill('0') << currentTime % 60 << ", "
                                     << athleteInfo << "\n";

                        // Write the break to the file only if it's not the first event
                        if (!isFirstEvent) {
                            int breakEndTime = currentTime + 10 + BREAK_DURATION;
                            scheduleFile << "Break, , , "
                                         << std::setw(2) << std::setfill('0') << (currentTime + 10) / 60 << ":" 
                                         << std::setw(2) << std::setfill('0') << (currentTime + 10) % 60 << " - "
                                         << std::setw(2) << std::setfill('0') << breakEndTime / 60 << ":" 
                                         << std::setw(2) << std::setfill('0') << breakEndTime % 60 << ", "
                                         << "Break Time\n";
                        } else {
                            isFirstEvent = false;
                        }

                        // Update the next available time for this sport with an additional break
                        nextAvailableTime[sportName] = currentTime + 10 + (isFirstEvent ? 0 : BREAK_DURATION);
                    }
                }
            }
        }
    }

    scheduleFile.close();
}


//Kristian Stop