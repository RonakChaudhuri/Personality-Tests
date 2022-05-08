//
// Project 2 - Personality Tests
// Name: Ronak Chaudhuri
// Project Overview: In this program, the task is to create a Personality Test
// using the five factor model. Given a file of questions, and a file of
// specific themes containing people and their five factor scores, the program
// ask the user random questions taken from the files of questions and based on
// the answers given, matches the user to a person/object from the file of
// specific themes using cosine similarity scores to determine the highest
// similarity.
// Creative Component: The theme used for the creative component of the project
// was Dragon Ball Z.

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "driver.h"

using namespace std;

/* This function takes in a set of Question object type paramater, and then
   opens the question.txt file and then updates a Question object's name and
   map of factors members, then adds each question object to the set of
   questions, updating it in main. */
void loadQuestions(set<Question> &setqs) {
    ifstream filestream;
    filestream.open("questions.txt");
    Question q;
    map<char, int> f;
    string line, question;
    string num;
    int firstSpace = 0;
    while (!filestream.eof()) {
        getline(filestream, line);
        stringstream s(line);
        getline(s, question, '.');
        question.push_back('.');
        q.questionText = question;
        if (q.questionText == ".") {
            break;
        }
        while (getline(s, question, ':')) {
            getline(s, num, ' ');
            if (firstSpace == 0) {
                f[question.at(1)] = stoi(num);
                firstSpace++;
            } else {
                f[question.at(0)] = stoi(num);
            }
        }
        firstSpace = 0;
        q.factors = f;
        setqs.emplace(q);
        f.clear();
    }
}
/* This function takes in a filename and a set of Person objects, then opens the
   file and fills a person object's name and map of scores from the file, and
   adds it to the set of Person objects updating it in main. */
void loadPeople(string filename, set<Person> &people) {
    string line, n;
    ifstream filestream;
    filestream.open(filename);
    Person p;
    map<char, int> scr;
    int firstSpace = 0;
    string num;
    while (!filestream.fail()) {
        getline(filestream, line);
        stringstream s(line);
        getline(s, n, '.');
        p.name = n;
        if (p.name == "") {
            break;
        }
        while (getline(s, n, ':')) {
            getline(s, num, ' ');
            if (firstSpace == 0) {
                scr[n.at(1)] = stoi(num);
                firstSpace++;
            } else {
                scr[n.at(0)] = stoi(num);
            }
        }
        firstSpace = 0;
        p.scores = scr;
        people.emplace(p);
        s.clear();
    }
}
/* This function updates the filename based on user input paramater, and then
   calls the loadPeople function with updated filename. */
void setFile(int input, set<Person> &people) {
    string filename;
    if (input == 1) {
        filename = "BabyAnimals.people";
    } else if (input == 2) {
        filename = "Brooklyn99.people";
    } else if (input == 3) {
        filename = "Disney.people";
    } else if (input == 4) {
        filename = "Hogwarts.people";
    } else if (input == 5) {
        filename = "MyersBriggs.people";
    } else if (input == 6) {
        filename = "SesameStreet.people";
    } else if (input == 7) {
        filename = "StarWars.people";
    } else if (input == 8) {
        filename = "Vegetables.people";
    } else if (input == 9) {
        filename = "mine.people";
    }
    loadPeople(filename, people);
}
/* Function to print user answer choices. */
void printAnswerChoices() {
    cout << "1. Strongly disagree" << endl;
    cout << "2. Disagree" << endl;
    cout << "3. Neutral" << endl;
    cout << "4. Agree" << endl;
    cout << "5. Strongly agree" << endl << endl;
    cout << "Enter your answer here (1-5): ";
}
/* Function to print all shows. */
void printShows() {
    cout << "1. BabyAnimals" << endl;
    cout << "2. Brooklyn99" << endl;
    cout << "3. Disney" << endl;
    cout << "4. Hogwarts" << endl;
    cout << "5. MyersBriggs" << endl;
    cout << "6. SesameStreet" << endl;
    cout << "7. StarWars" << endl;
    cout << "8. Vegetables" << endl;
    cout << "9. mine" << endl;
    cout << "0. To end program." << endl << endl;
    cout << "Choose test number (1-9, or 0 to end): ";
}
int main() {
    int numq, input;
    int n = 0;
    int answerInput = 0;
    set<Question> setqs;
    set<Person> people;
    map<Question, int> answers;
    map<char, int> scores;
    Question q;
    Person similar;
    cout << "Welcome to the Personality Quiz!" << endl << endl;
    cout << "Choose number of questions:";
    cin >> numq;
    cout << endl;
    loadQuestions(setqs);
    while (n < numq) {
        cout << "How much do you agree with this statement?" << endl;
        q = randomQuestionFrom(setqs);
        cout << "\"" << q.questionText << "\"" << endl << endl;
        printAnswerChoices();
        cin >> answerInput;
        answers[q] = answerInput;
        cout << endl;
        n++;
    }
    scores = scoresFrom(answers);
    while (true) {
        printShows();
        cin >> input;
        if (input == 0) {
            cout << "Goodbye!" << endl;
            return 0;
        }
        setFile(input, people);
        similar = mostSimilarTo(scores, people);
        cout << "You got " << similar.name << "!" << endl << endl;
        people.clear();
    }
}
