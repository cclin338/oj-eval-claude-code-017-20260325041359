#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sstream>

#include "hash_table.h"
#include "dynamic_array.h"

using namespace std;

// User structure
struct User {
    string username;
    string password;
    string name;
    string mailAddr;
    int privilege;
    bool loggedIn;

    User() : privilege(0), loggedIn(false) {}
    User(const string& u, const string& p, const string& n, const string& m, int g)
        : username(u), password(p), name(n), mailAddr(m), privilege(g), loggedIn(false) {}
};

// Train structure (simplified for now)
struct Train {
    string trainID;
    int stationNum;
    int seatNum;
    string type;
    bool released;

    Train() : stationNum(0), seatNum(0), released(false) {}
    Train(const string& id, int n, int m, const string& y)
        : trainID(id), stationNum(n), seatNum(m), type(y), released(false) {}
};

// Global data structures
HashTable<User> users;
HashTable<Train> trains;

// Command parsing utilities
void trim(string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == string::npos) {
        s.clear();
        return;
    }
    size_t end = s.find_last_not_of(" \t\n\r");
    s = s.substr(start, end - start + 1);
}

DynamicArray<string> split(const string& s, char delimiter) {
    DynamicArray<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        trim(token);
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

// Command handlers
int handle_add_user(const DynamicArray<string>& args) {
    // Parse arguments
    string cur_user, username, password, name, mailAddr;
    int privilege = 0;

    for (int i = 1; i < args.size(); i++) {
        if (args[i] == "-c" && i + 1 < args.size()) {
            cur_user = args[++i];
        } else if (args[i] == "-u" && i + 1 < args.size()) {
            username = args[++i];
        } else if (args[i] == "-p" && i + 1 < args.size()) {
            password = args[++i];
        } else if (args[i] == "-n" && i + 1 < args.size()) {
            name = args[++i];
        } else if (args[i] == "-m" && i + 1 < args.size()) {
            mailAddr = args[++i];
        } else if (args[i] == "-g" && i + 1 < args.size()) {
            privilege = stoi(args[++i]);
        }
    }

    // Check if this is the first user
    if (users.getSize() == 0) {
        // First user gets privilege 10
        User newUser(username, password, name, mailAddr, 10);
        if (users.insert(username, newUser)) {
            return 0;
        }
        return -1;
    }

    // Check if current user exists and is logged in
    User* cur = users.find(cur_user);
    if (!cur || !cur->loggedIn) {
        return -1;
    }

    // Check privilege requirement
    if (privilege >= cur->privilege) {
        return -1;
    }

    // Check if username already exists
    if (users.contains(username)) {
        return -1;
    }

    // Create new user
    User newUser(username, password, name, mailAddr, privilege);
    if (users.insert(username, newUser)) {
        return 0;
    }

    return -1;
}

int handle_login(const DynamicArray<string>& args) {
    string username, password;

    for (int i = 1; i < args.size(); i++) {
        if (args[i] == "-u" && i + 1 < args.size()) {
            username = args[++i];
        } else if (args[i] == "-p" && i + 1 < args.size()) {
            password = args[++i];
        }
    }

    User* user = users.find(username);
    if (!user || user->password != password || user->loggedIn) {
        return -1;
    }

    user->loggedIn = true;
    return 0;
}

int handle_logout(const DynamicArray<string>& args) {
    string username;

    for (int i = 1; i < args.size(); i++) {
        if (args[i] == "-u" && i + 1 < args.size()) {
            username = args[++i];
        }
    }

    User* user = users.find(username);
    if (!user || !user->loggedIn) {
        return -1;
    }

    user->loggedIn = false;
    return 0;
}

int handle_query_profile(const DynamicArray<string>& args) {
    string cur_user, username;

    for (int i = 1; i < args.size(); i++) {
        if (args[i] == "-c" && i + 1 < args.size()) {
            cur_user = args[++i];
        } else if (args[i] == "-u" && i + 1 < args.size()) {
            username = args[++i];
        }
    }

    User* cur = users.find(cur_user);
    User* target = users.find(username);

    if (!cur || !cur->loggedIn || !target) {
        return -1;
    }

    // Check permission: cur privilege > target privilege, or same user
    if (cur->privilege <= target->privilege && cur_user != username) {
        return -1;
    }

    cout << target->username << " " << target->name << " "
         << target->mailAddr << " " << target->privilege << endl;
    return 0;
}

int handle_add_train(const DynamicArray<string>& args) {
    string trainID;
    int stationNum = 0, seatNum = 0;

    for (int i = 1; i < args.size(); i++) {
        if (args[i] == "-i" && i + 1 < args.size()) {
            trainID = args[++i];
        } else if (args[i] == "-n" && i + 1 < args.size()) {
            stationNum = stoi(args[++i]);
        } else if (args[i] == "-m" && i + 1 < args.size()) {
            seatNum = stoi(args[++i]);
        } else if (args[i] == "-y" && i + 1 < args.size()) {
            // type parameter
            // For now, just skip
            i++;
        }
        // Skip other parameters for now
    }

    if (trainID.empty() || stationNum <= 1 || seatNum <= 0) {
        return -1;
    }

    // Check if trainID already exists
    if (trains.contains(trainID)) {
        return -1;
    }

    Train newTrain(trainID, stationNum, seatNum, "G"); // Default type
    if (trains.insert(trainID, newTrain)) {
        return 0;
    }

    return -1;
}

int handle_release_train(const DynamicArray<string>& args) {
    string trainID;

    for (int i = 1; i < args.size(); i++) {
        if (args[i] == "-i" && i + 1 < args.size()) {
            trainID = args[++i];
        }
    }

    Train* train = trains.find(trainID);
    if (!train || train->released) {
        return -1;
    }

    train->released = true;
    return 0;
}

int handle_delete_train(const DynamicArray<string>& args) {
    string trainID;

    for (int i = 1; i < args.size(); i++) {
        if (args[i] == "-i" && i + 1 < args.size()) {
            trainID = args[++i];
        }
    }

    Train* train = trains.find(trainID);
    if (!train || train->released) {
        return -1;
    }

    if (trains.remove(trainID)) {
        return 0;
    }

    return -1;
}

// Main command dispatcher
int process_command(const string& line) {
    DynamicArray<string> args = split(line, ' ');

    if (args.empty()) {
        return -1;
    }

    string command = args[0];

    if (command == "add_user") {
        return handle_add_user(args);
    } else if (command == "login") {
        return handle_login(args);
    } else if (command == "logout") {
        return handle_logout(args);
    } else if (command == "query_profile") {
        return handle_query_profile(args);
    } else if (command == "add_train") {
        return handle_add_train(args);
    } else if (command == "release_train") {
        return handle_release_train(args);
    } else if (command == "delete_train") {
        return handle_delete_train(args);
    } else if (command == "clean") {
        // Clear all data
        // For now, just return 0
        return 0;
    } else if (command == "exit") {
        cout << "bye" << endl;
        return -2; // Special return code for exit
    }

    return -1;
}

int main() {
    string line;

    while (getline(cin, line)) {
        trim(line);
        if (line.empty()) continue;

        int result = process_command(line);

        if (result == -2) {
            break; // Exit command
        } else if (result == 0 && line != "clean") {
            cout << result << endl;
        } else if (result != 0) {
            cout << result << endl;
        }
    }

    return 0;
}