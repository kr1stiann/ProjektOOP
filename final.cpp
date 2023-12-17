#include "newfinal.hpp"


int main(int argc, char *argv[10]) {
    
    std::string name = argv[1];
    std::vector<Athlete> athletes = fileInput(name);

    createFullDaySchedule(athletes);
    return 0;
}


