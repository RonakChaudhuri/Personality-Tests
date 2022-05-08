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

#include <math.h>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "myrandom.h"

using namespace std;

constexpr double lowest_double = std::numeric_limits<double>::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
    string questionText;     // Text of the question
    map<char, int> factors;  // Map from factors to +1 or -1
    friend bool operator<(const Question& lhs, const Question& rhs) {
        return lhs.questionText < rhs.questionText;
    }
    friend bool operator==(const Question& lhs, const Question& rhs) {
        return lhs.questionText == rhs.questionText;
    }
    friend bool operator!=(const Question& lhs, const Question& rhs) {
        return lhs.questionText != rhs.questionText;
    }
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
    string name;            // Name of the person
    map<char, int> scores;  // Map from factors to +1 or -1
    friend bool operator<(const Person& lhs, const Person& rhs) {
        return lhs.name < rhs.name;
    }
    friend bool operator==(const Person& lhs, const Person& rhs) {
        return lhs.name == rhs.name;
    }
    friend bool operator!=(const Person& lhs, const Person& rhs) {
        return lhs.name != rhs.name;
    }
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
 */
Question randomElement(set<Question>& questions) {
    int ind = randomInteger(0, (int)questions.size() - 1);
    int i = 0;
    for (auto e : questions) {
        if (i == ind) {
            return e;
        }
        i++;
    }
    return {};
}

/* This  function take in a set of Question objects, and randoms selects a
   question from the set, and returns that questiom, returns out_of_range
   exception if empty. */
Question randomQuestionFrom(set<Question>& questions) {
    if (questions.size() == 0) {
        throw out_of_range("Questions: Empty");
    }
    Question q;
    q = randomElement(questions);
    questions.erase(q);
    return q;
}
/* This function takes in a map of questions asked and users answers, then
   adds every five factor character found into a set, then fills a map
   with each five factor character key that was found and their calculated
   total score based on input for each factor values, and then returns that
   map. */
map<char, int> scoresFrom(map<Question, int>& answers) {
    set<char> c;
    map<char, int> scores;
    for (auto& i : answers) {
        for(auto& j : i.first.factors) {
            c.emplace(j.first);
        }
    }
    for (auto &i : answers) {
        for(auto &j : c) {
            if(i.first.factors.count(j) == 1) {
                scores[j] += (i.second - 3) * (i.first.factors.at(j));
            }
        }
    }
    return scores;
}

/* This function takes in a map of the five factor characters and their score
   for each one, and places all the five factor characters found into a set.
   Then it calculates the normalized value of each one by using the formula of
   taking the square root of the sum and dividing each score by that value, and
   then puts it into a different map with the same five factor characters as
   the key but the values being the normalized values. Also throws an error if
   you divide by zero. */
map<char, double> normalize(map<char, int>& scores) {
    double normal = 0.0;
    double sum = 0.0;
    set<char> c;
    map<char, double> normalized;
    for (auto& i : scores) {
        c.emplace(i.first);
    }
    for (auto& i : scores) {
        sum += pow(i.second, 2);
    }
    normal = sqrt(sum);
    if (normal == 0.0) {
        throw invalid_argument("Dividing by zero error");
    }
    for (auto& i : c) {
        normalized[i] += scores.at(i) / normal;
    }
    return normalized;
}

/* This function takes in two maps of five factor character keys and normalized
   double values, and then it adds all the similar characters into a set. Then
   it loops through the set and adds to the total cosineSimilarity score by
   multyiplying the normalize value of the first map and second map for each
   factor character. */
double cosineSimilarityOf(const map<char, double>& lhs,
                          const map<char, double>& rhs) {
    set<char> c;
    double cosineSimilarity = 0.0;
    for (auto& i : lhs) {
        if(rhs.count(i.first) == 1) {
            c.emplace(i.first);
        }
    }
    for (auto& i : c) {
        cosineSimilarity += lhs.at(i) * rhs.at(i);
    }
    return cosineSimilarity;
}

/* This function takes in the a map of the users char five factor keys and score
   for each, as well as a set of Person objects that contains each person's name
   and their <char, int> map like the user, then loops through the entire set
   and sets a max value if a higher cosine similarity value is found between the
   normalized scores of the user and person from the set, and returns the Person
   with the highest cosine similarity score. Also throws an error if the set of
   people is empty. */
Person mostSimilarTo(map<char, int>& scores, set<Person>& people) {
    double max = lowest_double;
    Person similar;
    if (people.empty()) {
        throw invalid_argument("No people");
    }
    for (auto i : people) {
        if (cosineSimilarityOf(normalize(scores), normalize(i.scores)) > max) {
            max = cosineSimilarityOf(normalize(scores), normalize(i.scores));
            similar = i;
        }
    }
    return similar;
}
