
#include "person.h"
#include "misc.h"
#include <fstream>
#include <sstream>
#include <algorithm>

static string trim(string value) {
    string::size_type start = value.find_first_not_of(" \t\r\n");
    string::size_type end = value.find_last_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }
    return value.substr(start, end - start + 1);
}

static void parseContactLine(string line, string& type, string& data) {
    string::size_type open = line.find('(');
    string::size_type close = line.find(')');
    type = trim(line.substr(open + 1, close - open - 1));
    data = trim(line.substr(close + 1));
    if (!data.empty() && data[0] == ':') {
        data = trim(data.substr(1));
    }
}

Person::Person() {
    set_person();
}

Person::~Person() {
    delete birthdate;
    delete email;
    delete phone;
}

Person::Person(string f_name, string l_name, string b_date, string email_str, string phone_str) {
    this->f_name = f_name;
    this->l_name = l_name;
    this->birthdate = new Date(b_date);
    this->next = NULL;
    this->prev = NULL;

    string eType, eAddr;
    parseContactLine(email_str, eType, eAddr);
    this->email = new Email(eType, eAddr);

    string pType, pNum;
    parseContactLine(phone_str, pType, pNum);
    this->phone = new Phone(pType, pNum);
}

Person::Person(string filename) {
    set_person(filename);
}

void Person::set_person() {
    string temp;
    string eType, eAddr, pType, pNum;

    cout << "First Name: ";
    getline(cin, f_name);

    cout << "Last Name: ";
    getline(cin, l_name);

    cout << "Birthdate (M/D/YYYY): ";
    getline(cin, temp);
    birthdate = new Date(temp);

    cout << "Type of email address: ";
    getline(cin, eType);
    cout << "Email address: ";
    getline(cin, eAddr);
    email = new Email(eType, eAddr);

    cout << "Type of phone number: ";
    getline(cin, pType);
    cout << "Phone number: ";
    getline(cin, pNum);
    phone = new Phone(pType, pNum);

    next = NULL;
    prev = NULL;
}

void Person::set_person(string filename) {
    ifstream file(filename.c_str());
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    getline(file, f_name);
    getline(file, l_name);

    string dateStr;
    getline(file, dateStr);
    birthdate = new Date(dateStr);

    string line4, line5;
    getline(file, line4);
    getline(file, line5);

    string emailLine, phoneLine;
    if (line4.find('@') != string::npos) {
        emailLine = line4;
        phoneLine = line5;
    } else {
        phoneLine = line4;
        emailLine = line5;
    }

    string pType, pNum;
    parseContactLine(phoneLine, pType, pNum);
    phone = new Phone(pType, pNum);

    string eType, eAddr;
    parseContactLine(emailLine, eType, eAddr);
    email = new Email(eType, eAddr);

    file.close();
    next = NULL;
    prev = NULL;
}

bool Person::operator==(const Person& rhs) {
    return (f_name == rhs.f_name && l_name == rhs.l_name && *birthdate == *rhs.birthdate);
}

bool Person::operator!=(const Person& rhs) {
    return !(*this == rhs);
}

void Person::print_person() {
    cout << l_name << ", " << f_name << endl;
    birthdate->print_date("Month D, YYYY");
    phone->print();
    email->print();
    for (vector<Person*>::size_type i = 0; i < myfriends.size(); i++) {
        string code = codeName(myfriends[i]->f_name, myfriends[i]->l_name);
        cout << code << " (" << myfriends[i]->f_name << " " << myfriends[i]->l_name << ")" << endl;
    }
}

void Person::makeFriend(Person* newFriend) {
    for (vector<Person*>::size_type i = 0; i < myfriends.size(); i++) {
        if (myfriends[i] == newFriend) {
            return;
        }
    }
    myfriends.push_back(newFriend);
}

void Person::print_friends() {
    cout << l_name << ", " << f_name << endl;
    cout << "--------------------------------" << endl;

    vector<string> codes;
    vector<int> indices;
    for (vector<Person*>::size_type i = 0; i < myfriends.size(); i++) {
        codes.push_back(codeName(myfriends[i]->f_name, myfriends[i]->l_name));
        indices.push_back(i);
    }

    for (vector<int>::size_type i = 0; i < indices.size(); i++) {
        for (vector<int>::size_type j = i + 1; j < indices.size(); j++) {
            bool shouldSwap = false;
            if (codes[indices[i]][0] > codes[indices[j]][0]) {
                shouldSwap = true;
            } else if (codes[indices[i]][0] == codes[indices[j]][0]) {
                if (codes[indices[i]].length() > 1 && codes[indices[j]].length() > 1) {
                    if (codes[indices[i]][1] > codes[indices[j]][1]) {
                        shouldSwap = true;
                    }
                }
            }
            if (shouldSwap) {
                int temp = indices[i];
                indices[i] = indices[j];
                indices[j] = temp;
            }
        }
    }

    for (vector<int>::size_type i = 0; i < indices.size(); i++) {
        Person* f = myfriends[indices[i]];
        cout << f->f_name << ", " << f->l_name << endl;
    }
}

string Person::getCode() {
    return codeName(f_name, l_name);
}

string Person::getFName() {
    return f_name;
}

string Person::getLName() {
    return l_name;
}

string Person::getPhoneStr() {
    return phone->get_contact("raw");
}

string Person::getEmailStr() {
    return email->get_contact("raw");
}

string Person::getDateStr() {
    return birthdate->get_date_str();
}
