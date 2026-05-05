
#include "network.h"
#include <limits>
#include "misc.h"
#include <fstream>
#include <dirent.h>
#include <cstring>

static string trim(string value) {
    string::size_type start = value.find_first_not_of(" \t\r\n");
    string::size_type end = value.find_last_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }
    return value.substr(start, end - start + 1);
}

Network::Network() {
    head = NULL;
    tail = NULL;
    count = 0;
}

Network::Network(string fileName) {
    head = NULL;
    tail = NULL;
    count = 0;
    loadDB(fileName);
}

Network::~Network() {
    Person* current = head;
    while (current != NULL) {
        Person* temp = current;
        current = current->next;
        delete temp;
    }
}

Person* Network::search(Person* searchEntry) {
    Person* ptr = head;
    while (ptr != NULL) {
        if (*ptr == *searchEntry) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

Person* Network::search(string fname, string lname) {
    Person* ptr = head;
    while (ptr != NULL) {
        if (ptr->f_name == fname && ptr->l_name == lname) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

Person* Network::wiseSearch(string query) {
    Person* ptr = head;
    while (ptr != NULL) {
        if (ptr->getPhoneStr() == query) return ptr;
        if (ptr->getEmailStr() == query) return ptr;
        if (ptr->getDateStr() == query) return ptr;
        if (ptr->getFName() == query) return ptr;
        if (ptr->getLName() == query) return ptr;
        if (ptr->getCode() == query) return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

void Network::loadDB(string filename) {
    ifstream file(filename.c_str());
    if (!file) {
        cout << "File " << filename << " does not exist!" << endl;
        return;
    }

    Person* current = head;
    while (current != NULL) {
        Person* temp = current;
        current = current->next;
        delete temp;
    }
    head = NULL;
    tail = NULL;
    count = 0;

    string firstLine, fname, lname, dateStr, line4, line5;
    vector<vector<string> > allFriendCodes;

    while (getline(file, firstLine)) {
        firstLine = trim(firstLine);
        if (firstLine.empty()) continue;
        if (firstLine[0] == '-') continue;

        string::size_type comma = firstLine.find(',');
        if (comma != string::npos) {
            lname = trim(firstLine.substr(0, comma));
            fname = trim(firstLine.substr(comma + 1));
            getline(file, dateStr);
            getline(file, line4);
            getline(file, line5);
        } else {
            fname = firstLine;
            getline(file, lname);
            getline(file, dateStr);
            getline(file, line4);
            getline(file, line5);
        }

        lname = trim(lname);
        dateStr = trim(dateStr);
        line4 = trim(line4);
        line5 = trim(line5);

        string emailLine, phoneLine;
        if (line4.find('@') != string::npos) {
            emailLine = line4;
            phoneLine = line5;
        } else {
            phoneLine = line4;
            emailLine = line5;
        }

        Person* p = new Person(fname, lname, dateStr, emailLine, phoneLine);
        push_back(p);

        vector<string> friendCodes;
        string line;
        while (getline(file, line)) {
            line = trim(line);
            if (line.empty()) continue;
            if (line[0] == '-') break;

            string friendCode = "";
            for (string::size_type i = 0; i < line.length(); i++) {
                if (line[i] == ' ') break;
                friendCode += line[i];
            }
            friendCodes.push_back(friendCode);
        }
        allFriendCodes.push_back(friendCodes);
    }

    file.close();

    Person* ptr = head;
    vector<vector<string> >::size_type idx = 0;
    while (ptr != NULL && idx < allFriendCodes.size()) {
        for (vector<string>::size_type i = 0; i < allFriendCodes[idx].size(); i++) {
            Person* friendPtr = head;
            while (friendPtr != NULL) {
                if (friendPtr->getCode() == allFriendCodes[idx][i]) {
                    ptr->makeFriend(friendPtr);
                    friendPtr->makeFriend(ptr);
                    break;
                }
                friendPtr = friendPtr->next;
            }
        }
        ptr = ptr->next;
        idx++;
    }
}

void Network::saveDB(string filename) {
    ofstream file(filename.c_str());
    if (!file) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    Person* ptr = head;
    while (ptr != NULL) {
        file << ptr->l_name << ", " << ptr->f_name << endl;
        file << ptr->birthdate->get_date_str("Month D, YYYY") << endl;
        file << "Phone " << ptr->phone->get_contact() << endl;
        file << "Email " << ptr->email->get_contact() << endl;
        for (vector<Person*>::size_type i = 0; i < ptr->myfriends.size(); i++) {
            string code = codeName(ptr->myfriends[i]->f_name, ptr->myfriends[i]->l_name);
            file << code << " (" << ptr->myfriends[i]->f_name << " " << ptr->myfriends[i]->l_name << ")" << endl;
        }
        file << "--------------------" << endl;
        ptr = ptr->next;
    }

    file.close();
}

void Network::printDB() {
    cout << "Number of people: " << count << endl;
    cout << "------------------------------" << endl;
    Person* ptr = head;
    while (ptr != NULL) {
        ptr->print_person();
        cout << "------------------------------" << endl;
        ptr = ptr->next;
    }
}

void Network::push_front(Person* newEntry) {
    newEntry->prev = NULL;
    newEntry->next = head;

    if (head != NULL)
        head->prev = newEntry;
    else
        tail = newEntry;

    head = newEntry;
    count++;
}

void Network::push_back(Person* newEntry) {
    newEntry->next = NULL;
    newEntry->prev = tail;

    if (tail != NULL)
        tail->next = newEntry;
    else
        head = newEntry;

    tail = newEntry;
    count++;
}

bool Network::remove(string fname, string lname) {
    Person* target = search(fname, lname);
    if (target == NULL) return false;

    Person* ptr = head;
    while (ptr != NULL) {
        for (vector<Person*>::size_type i = 0; i < ptr->myfriends.size(); i++) {
            if (ptr->myfriends[i] == target) {
                ptr->myfriends.erase(ptr->myfriends.begin() + i);
                break;
            }
        }
        ptr = ptr->next;
    }

    if (target->prev != NULL)
        target->prev->next = target->next;
    else
        head = target->next;

    if (target->next != NULL)
        target->next->prev = target->prev;
    else
        tail = target->prev;

    delete target;
    count--;
    return true;
}

void Network::showMenu() {
    int opt;
    while (1) {
        cout << "\033[2J\033[1;1H";
        printMe("banner");

        cout << "Select from below: \n";
        cout << "1. Save network database \n";
        cout << "2. Load network database \n";
        cout << "3. Add a new person \n";
        cout << "4. Remove a person \n";
        cout << "5. Print people with last name \n";
        cout << "6. Connect (make friends) \n";
        cout << "7. Wise Search \n";
        cout << "\nSelect an option ... ";

        if (cin >> opt) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Wrong option! " << endl;
            return;
        }

        string fname, lname, fileName, bdate;
        cout << "\033[2J\033[1;1H";

        if (opt == 1) {
            cout << "Saving network database \n";
            cout << "Enter the name of the save file: ";
            getline(cin, fileName);
            saveDB(fileName);
            cout << "Network saved in " << fileName << endl;
        }
        else if (opt == 2) {
            cout << "Loading network database \n";

            DIR* dir = opendir(".");
            if (dir != NULL) {
                struct dirent* entry;
                while ((entry = readdir(dir)) != NULL) {
                    string name = entry->d_name;
                    if (name.length() > 4 && name.substr(name.length() - 4) == ".txt") {
                        cout << name << endl;
                    }
                }
                closedir(dir);
            }

            cout << "Enter the name of the load file: ";
            getline(cin, fileName);

            ifstream testFile(fileName.c_str());
            if (!testFile) {
                cout << "File " << fileName << " does not exist!" << endl;
            } else {
                testFile.close();
                loadDB(fileName);
                cout << "Network loaded from " << fileName << " with " << count << " people \n";
            }
        }
        else if (opt == 3) {
            cout << "Adding a new person \n";
            Person* newPerson = new Person();

            if (search(newPerson->f_name, newPerson->l_name) != NULL) {
                cout << "Person already exists! \n";
                delete newPerson;
            } else {
                push_front(newPerson);
            }
        }
        else if (opt == 4) {
            cout << "Removing a person \n";
            cout << "First name: ";
            getline(cin, fname);
            cout << "Last name: ";
            getline(cin, lname);

            if (remove(fname, lname)) {
                cout << "Remove Successful! \n";
            } else {
                cout << "Person not found! \n";
            }
        }
        else if (opt == 5) {
            cout << "Print people with last name \n";
            cout << "Last name: ";
            getline(cin, lname);

            bool found = false;
            Person* ptr = head;
            while (ptr != NULL) {
                if (ptr->l_name == lname) {
                    ptr->print_person();
                    cout << "------------------------------" << endl;
                    found = true;
                }
                ptr = ptr->next;
            }
            if (!found) {
                cout << "Person not found! \n";
            }
        }
        else if (opt == 6) {
            cout << "Make friends: \n";

            cout << "Person 1" << endl;
            cout << "First Name: ";
            getline(cin, fname);
            cout << "Last Name: ";
            getline(cin, lname);

            Person* p1 = search(fname, lname);
            if (p1 == NULL) {
                cout << "Person not found! \n";
            } else {
                cout << "Person 2" << endl;
                cout << "First Name: ";
                getline(cin, fname);
                cout << "Last Name: ";
                getline(cin, lname);

                Person* p2 = search(fname, lname);
                if (p2 == NULL) {
                    cout << "Person not found! \n";
                } else {
                    p1->makeFriend(p2);
                    p2->makeFriend(p1);

                    cout << endl;
                    p1->print_person();
                    cout << endl;
                    p2->print_person();
                }
            }
        }
        else if (opt == 7) {
            cout << "Wise Search: \n";
            cout << "Search By: \n";
            string query;
            getline(cin, query);

            Person* found = wiseSearch(query);
            if (found != NULL) {
                cout << endl;
                found->print_person();
            } else {
                cout << "Person not found! \n";
            }
        }
        else
            cout << "Nothing matched!\n";

        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "\n\nPress Enter key to go back to main menu ... ";
        string temp;
        getline(cin, temp);
        cout << "\033[2J\033[1;1H";
    }
}
