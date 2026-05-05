EE355 Phase 2

Phase 2 adds friends to the TrojanBook project from Phase 1.

Compile:
g++ -std=c++98 test_network.cpp network.cpp person.cpp date.cpp contact.cpp misc.cpp -o test_network
g++ -std=c++98 test_person_eq.cpp person.cpp date.cpp contact.cpp misc.cpp -o test_person_eq

Main parts:
- Person now has a vector of friends called myfriends.
- Connect makes two people friends with each other.
- codeName makes a simple ID from first name and last name.
- saveDB and loadDB also save and load friend codes.
- Wise Search can search by phone, email, date, name, or code name.

Files:
- contact.h / contact.cpp
- date.h / date.cpp
- person.h / person.cpp
- network.h / network.cpp
- misc.h / misc.cpp
- networkDB.txt
- person_template.txt
- test_network.cpp
- test_person_eq.cpp

Test results:
- test_network.cpp compiled successfully.
- test_person_eq.cpp compiled successfully.
- networkDB.txt loaded correctly.
- Saving and loading again kept the same people.
- A test database with friend codes loaded and printed both sides of the friendship.
