#include "shell.hpp"
#include <cstdio>
#include <pwd.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <cstring>


Shell::Shell(){
    shellLoop();
}

Shell::~Shell(){}

void Shell::shellLoop(){
    alive = true;
    std::string line;
    std::vector<std::string> vec;

    while(alive){
        std::cout<< getUserName() << ">";  //fix this dont run function every time

        line = readLine();

        if(line == ""){
            perror("string empty try again");
            continue;
        }
        
        vec = parseLine(line);
        
        runLine(vec);
    }

    std::cout<<std::endl;
}

int Shell::runLine(std::vector<std::string> vec){
    int stat;
    
    if(vec.size() <= 1){
        stat = exec(vec[0]);  //overload if put std::string assume no args and that std::string is fileName else if we get vector then assume vector[0] is file name and the rest are args
    }else {
       stat = exec(vec);
    }

    if(stat == -1){
        perror("unable to read binary");
    }
    
    return stat;
}

int Shell::exec(const std::vector<std::string> vec){
    pid_t pid = fork();
    char** argv = new char*[vec.size() + 1];
    
    for(int i = 0; i < vec.size(); i++){
        argv[i] = new char[vec[i].length() + 1];
        strcpy(argv[i], vec[i].c_str());
    }
    
    argv[vec.size()] = NULL;

    if(pid == -1){
        perror("FORK");
        return -1;
    
    }else if(pid > 0){ //child
        int status;
        wait(&status);
    
    }else{ 
        return execv(vec[0].c_str(), argv);
        //_exit(EXIT_FAILURE);
    }

    std::cout<<std::endl;

    return 1;
}

int Shell::exec(const std::string fName){
    pid_t pid = fork();

    if(pid == -1){
        perror("FORK");
        return -1;
    
    }else if(pid > 0){ //child
        int status;
        wait(&status);
    
    }else{ 
        return execl(fName.c_str(), NULL);
        //_exit(EXIT_FAILURE);
    }

    std::cout<<std::endl;

    return 1;
}


std::vector<std::string> Shell::parseLine(std::string line){ //todo 
    std::vector<std::string> vec;
    std::string str = "";
    
    for(int i=0; i < line.length(); i++){      //turns std::string into std::vector
        if(line[i] != ' '){
            str += line[i];
        }else{
            vec.push_back(str);
            str = "";
        }
    }
    if(!str.empty()) vec.push_back(str);

    return vec;
}

std::string Shell::readLine(){
    std::string line;

    if(!std::getline(std::cin, line)){
        perror("End of File");

        alive = false; //stops the loop 
    }

    return line;
}

std::string Shell::getUserName(){
    std::string buf;

    if(getlogin_r(buf.data(), buf.length()) == 0){
        return buf;
    }
    
    //if getlogin_r doesnt work we do this instead
    struct passwd *pw;

    pw = getpwuid(geteuid());
    
    return pw->pw_name; 
}

