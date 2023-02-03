#include<iostream>
#include<unistd.h>
#include<vector>

class Shell{
    public:
        Shell();
        ~Shell();
    private:
        void shellLoop();

        std::string readLine(); //read input
        std::vector<std::string> parseLine(std::string line); //parse
        int runLine(std::vector<std::string> vec); //execute

        int exec(const std::vector<std::string> vec); 
        int exec(const std::string fName);
        std::string getUserName();
        
        bool alive;
};
