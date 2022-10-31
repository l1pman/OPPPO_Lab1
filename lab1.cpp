#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <utility>
using namespace std;

enum Month {
    January,
    February,
    March,
    April,
    May,
    June,
    July,
    August,
    September,
    October,
    November,
    December
};

string returnStringMonth(Month month) {
    switch (month) {
        case January:
            return "January";
        case February:
            return "February";
        case March:
            return "March";
        case April:
            return "April";
        case May:
            return "May";
        case June:
            return "June";
        case July:
            return "July";
        case August:
            return "August";
        case September:
            return "September";
        case October:
            return "October";
        case November:
            return "November";
        case December:
            return "December";
    }
}

Month returnEnumMonth(string month) {
    if (month == "January") {
        return January;
    }
    else if (month == "February") {
        return February;
    }
    else if (month == "March") {
        return March;
    }
    else if (month == "April") {
        return April;
    }
    else if (month == "May") {
        return May;
    }
    else if (month == "June") {
        return June;
    }
    else if (month == "July") {
        return July;
    }
    else if (month == "August") {
        return August;
    }
    else if (month == "September") {
        return September;
    }
    else if (month == "October") {
        return October;
    }
    else if (month == "November") {
        return November;
    }
    else if (month == "December") {
        return December;
    }
    else {
        throw std::invalid_argument("invalid argument!");
    }
}

struct Plant {
    char plant_name[30];
};

struct Tree {
    Plant plant;
    int age;
};

struct Bush {
    Plant plant;
    Month flowering_month;
};

struct Cactus {
    Plant plant;
    double thorn_length;
};

enum PlantType {
    Tree_type,
    Bush_type,
    Cactus_type
};

struct Union {
    PlantType type;
    union {
        Tree data_tree;
        Bush data_bush;
        Cactus data_cactus;
    } data;
};

struct Node {
    Node* next;
    Union anUnion;
};

struct List {
    Node* first = nullptr;
};

const regex new_tree(R"(tree \{[[:upper:]][a-zA-Z]+,\s*\d+\})");
const regex new_bush(R"(bush \{[[:upper:]][a-zA-Z]+,\s*\w+\})");
const regex new_cactus(R"(cactus \{[[:upper:]][a-zA-Z]+,\s*[+-]?([0-9]*[.])?[0-9]+\})");

const regex print(R"(print\(\d+\))");
const regex print_all(R"(printAll\(\))");

const regex delete_all(R"(deleteAll\(\))");

const regex delete_plant(R"(delete\(\d+\))");
const regex delete_all_trees(R"(deleteAllTrees\(\))");
const regex delete_all_bushes(R"(deleteAllBushes\(\))");
const regex delete_all_cactuses(R"(deleteAllCactuses\(\))");
const regex sortByName(R"(sort_by_name\(\))");

const regex print_all_bushes(R"(printAllBushes\(\))");


void setNextNode(List& list, Node* nextNode) {
    if (list.first == nullptr) {
        list.first = nextNode;
        nextNode->next = nextNode;
    }
    else {
        Node* lastNode = list.first;
        while (lastNode->next != list.first) {
            lastNode = lastNode->next;
        }
        lastNode->next = nextNode;
        nextNode->next = list.first;
    }
}

void createTree(List& list, const char* name, long age) {
    Node* tree_node = new Node;
    tree_node->anUnion.type = Tree_type;
    strcpy_s(tree_node->anUnion.data.data_tree.plant.plant_name, name);
    tree_node->anUnion.data.data_tree.age = age;

    setNextNode(list, tree_node);
}

void createBush(List& list, const char* name, Month month) {
    Node* bush_node = new Node;
    bush_node->anUnion.type = Bush_type;
    strcpy_s(bush_node->anUnion.data.data_bush.plant.plant_name, name);
    bush_node->anUnion.data.data_bush.flowering_month = month;

    setNextNode(list, bush_node);
}

void createCactus(List& list, const char* name, double thorn_length) {
    Node* cactus_node = new Node;
    cactus_node->anUnion.type = Cactus_type;
    strcpy_s(cactus_node->anUnion.data.data_cactus.plant.plant_name, name);
    cactus_node->anUnion.data.data_cactus.thorn_length = thorn_length;

    setNextNode(list, cactus_node);
}

vector<string> splitLine(string& line) {
    vector<string> resultLine;

    size_t position;
    string argument;
    string delimiter = ", ";

    while ((position = line.find(delimiter)) != std::string::npos) {
        argument = line.substr(0, position);
        resultLine.push_back(argument);
        line = line.substr(position + delimiter.length());
    }
    resultLine.push_back(line);

    return resultLine;
}

void createPlant(List& list, string line, int type) {
    vector<string> resultLine = splitLine(line);
    char name[30];
    strcpy_s(name, resultLine.at(0).c_str());

    switch (type) {
    case 0: {
        int age = stoi(resultLine.at(1));
        createTree(list, name, age);
        break;
    }
    case 1: {
        Month month;
        try {
            month = returnEnumMonth(resultLine.at(1));
        }
        catch (const invalid_argument& ex) {
            cout << "\t\tWrong Month" << endl;
            return;
        }
        createBush(list, name, month);
        break;
    }
    case 2: {
        double thorn_length = stod(resultLine.at(1));
        createCactus(list, name, thorn_length);
        break;
    }
    }
}



string getPlantName(Node* node) {
    switch (node->anUnion.type) {
    case Tree_type:
        return node->anUnion.data.data_tree.plant.plant_name;
    case Bush_type:
        return node->anUnion.data.data_bush.plant.plant_name;
    case Cactus_type:
        return node->anUnion.data.data_cactus.plant.plant_name;
    }
}

bool isSortedByName(List& list) {
    Node* currentNode = list.first;
    while (currentNode->next != list.first) {
        if (getPlantName(currentNode) > getPlantName(currentNode->next)) {
            return false;
        }
        currentNode = currentNode->next;
    }
    return true;
}

void sort_by_name(List &list) {
    
    bool  isNotSorted = true;
    
    Node *nextNode;
    Node *currentNode = list.first;

    Node *previousNode = list.first;
    while (previousNode->next != list.first) {
        previousNode = previousNode->next;
    }

    while (isNotSorted) {
        nextNode = currentNode->next;
        while (currentNode->next != list.first) {
            if (getPlantName(currentNode) > getPlantName(nextNode)) {

                if (previousNode->next == list.first) {
                    list.first = nextNode;
                }

                previousNode->next = currentNode->next;
                currentNode->next = nextNode->next;
                nextNode->next = currentNode;

                previousNode = nextNode;
                nextNode = currentNode->next;
            }
            else {
                previousNode = currentNode;
                currentNode = nextNode;
                nextNode = currentNode->next;
            }
        }

        previousNode = currentNode;
        currentNode = currentNode->next;

        isNotSorted = !isSortedByName(list);

    }
}



void printItem(Node* node) {
    switch (node->anUnion.type) {
    case Tree_type:
        cout << "|Tree|: " << node->anUnion.data.data_tree.plant.plant_name
            << ", |Age|: " << node->anUnion.data.data_tree.age << "" << endl;
        break;
    case Bush_type:
        cout << "|Bush|: " << node->anUnion.data.data_bush.plant.plant_name
            << ", |Flowering month|: " << returnStringMonth(node->anUnion.data.data_bush.flowering_month) << "" << endl;
        break;
    case Cactus_type:
        cout << "|Cactus|: " << node->anUnion.data.data_cactus.plant.plant_name
            << ", |Thorn Length|: " << node->anUnion.data.data_cactus.thorn_length << "" << endl;
        break;
    }
}

bool isListEmpty(List& list) {
    if (list.first == nullptr) {
        cout << "----START----" << endl;
        cout << "List is empty" << endl;
        cout << "----END----" << endl;
        return true;
    }
    return false;
}

void printItems(List& list, int number) {
    if (isListEmpty(list)) {
        return;
    }

    cout << "----START----" << endl;
    Node* currentNode = list.first;
    for (int i = 0; i < number; i++) {
        cout << i << " - ";
        printItem(currentNode);
        currentNode = currentNode->next;
    }
    cout << "----END----" << endl;
}



void printAllItems(List& list) {
    if (isListEmpty(list)) {
        return;
    }

    int counter = 0;
    cout << "----START----" << endl;
    Node* currentNode = list.first;
    while (currentNode->next != list.first) {
        cout << counter << " - ";
        counter++;
        printItem(currentNode);
        currentNode = currentNode->next;
    }
    cout << counter << " - ";
    printItem(currentNode);
    cout << "----END----" << endl;

}
void printBushes(List& list) {
    if (isListEmpty(list)) {
        return;
    }

    int counter = 0;
    cout << "----START----" << endl;
    Node* currentNode = list.first;
    while (currentNode->next != list.first) {
        counter++;
        if (currentNode->anUnion.type == Bush_type) {
            cout << counter << " - ";
            printItem(currentNode);
        }
        currentNode = currentNode->next;
    }
    if (currentNode->anUnion.type == Bush_type) {
        cout << counter << " - ";
        printItem(currentNode);
    }
    cout << "----END----" << endl;
    
}


void deleteItem(List& list, int number) {
    if (isListEmpty(list)) {
        return;
    }
    int len_list = 0;
    Node* currentNode = list.first;
    while (currentNode->next != list.first) {
        len_list++;
        currentNode = currentNode->next;
    }

    if (number > len_list) {
        cout << "WRONG POSITON" << endl;
        return;
    }
    currentNode = list.first;

    
    if (currentNode->next == list.first) {
        delete currentNode;
        list.first = nullptr;
        return;
    }

    Node* previousNode = nullptr;
    for (int i = 0; i < number; i++) {
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    if (previousNode == nullptr) {
        while (currentNode->next != list.first) {
            currentNode = currentNode->next;
        }

        currentNode->next = currentNode->next->next;
        delete list.first;
        list.first = currentNode->next;
    }
    else {
        previousNode->next = currentNode->next;
        delete currentNode;
    }
}

void deleteAllItems(List& list) {
    if (isListEmpty(list)) {
        return;
    }

    Node* previousNode;
    Node* currentNode = list.first;
    while (currentNode->next != list.first) {
        previousNode = currentNode;
        currentNode = currentNode->next;
        delete previousNode;
    }

    delete currentNode;
    list.first = nullptr;
}

void deleteByIF(List& list, int mode) {
    if (isListEmpty(list)) {
        return;
    }

    Node* currentNode = list.first;
    bool firstCondition, secondCondition, thirdCondition;
    if (currentNode->next == list.first) {
        firstCondition = currentNode->anUnion.type == Tree_type && mode == 0;
        secondCondition = currentNode->anUnion.type == Bush_type && mode == 1;
        thirdCondition = currentNode->anUnion.type == Cactus_type && mode == 2;
        if (firstCondition || secondCondition || thirdCondition) {
            delete currentNode;
            list.first = nullptr;
            return;
        }
        else {
            return;
        }
    }

    Node* previousNode;
    while (currentNode->next != list.first) {
        currentNode = currentNode->next;
    }

    previousNode = currentNode;
    currentNode = currentNode->next;

    while (currentNode->next != list.first) {
        firstCondition = currentNode->anUnion.type == Tree_type && mode == 0;
        secondCondition = currentNode->anUnion.type == Bush_type && mode == 1;
        thirdCondition = currentNode->anUnion.type == Cactus_type && mode == 2;

        if (firstCondition || secondCondition || thirdCondition) {
            if (currentNode == list.first) {
                list.first = currentNode->next;
            }
            previousNode->next = currentNode->next;
            delete currentNode;
            currentNode = previousNode->next;
        }
        else {
            previousNode = currentNode;
            currentNode = previousNode->next;
        }
    }

    firstCondition = currentNode->anUnion.type == Tree_type && mode == 0;
    secondCondition = currentNode->anUnion.type == Bush_type && mode == 1;
    thirdCondition = currentNode->anUnion.type == Cactus_type && mode == 2;

    if (firstCondition || secondCondition || thirdCondition) {
        if (currentNode == list.first) {
            list.first = nullptr;
            delete currentNode;
        }
        else {
            previousNode->next = currentNode->next;
            delete currentNode;
        }
    }
}

void  deleteAllTreeItems(List &list) {
    deleteByIF(list, 0);
}
void  deleteAllBushItems(List& list) {
    deleteByIF(list, 1);
}
void  deleteAllCactusItems(List& list) {
    deleteByIF(list, 2);
}

int main() {
    List list{};
    ifstream file("E:/C++/OPPPO/lab1/input2.txt");

    if (!file) {
        cout << "Something went wrong while opening the file!" << endl;
        return -1;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            line = line.substr(0, line.find(" //"));

            if (regex_match(line, new_tree)) {
                cout << "\tCREATE TREE COMMAND" << endl;
                line = line.substr(line.find('{') + 1, line.length() - 7);
                createPlant(list, line, 0);
            }
            else if (regex_match(line, new_bush)) {
                cout << "\tCREATE BUSH COMMAND" << endl;
                line = line.substr(line.find('{') + 1, line.length() - 7);
                createPlant(list, line, 1);
            }
            else if (regex_match(line, new_cactus)) {
                cout << "\tCREATE CACTUS COMMAND" << endl;
                line = line.substr(line.find('{') + 1, line.length() - 8);
                createPlant(list, line, 2);
            }
            else if (regex_match(line, print)) {
                int number = stoi(line.substr(line.find('(') + 1, line.length() - 7));
                cout << "PRINT " << number << " ITEMS COMMAND" << endl;
                printItems(list, number);
            }
            else if (regex_match(line, sortByName)) {
                cout << "\nSortByName command" << endl;
                sort_by_name(list);
            }
            else if (regex_match(line, print_all)) {
                cout << "PRINT ALL ITEMS COMMAND" << endl;
                printAllItems(list);
            }
            else if (regex_match(line, print_all_bushes)) {
                cout << "PRINT ALL BUSHES COMMAND" << endl;
                printBushes(list);
            }
            else if (regex_match(line, delete_plant)) {
                int number = stoi(line.substr(line.find('(') + 1, line.length() - 8));
                cout << "\tDELETE " << number << " ITEM COMMAND" << endl;
                deleteItem(list, number);
            }
            else if (regex_match(line, delete_all)) {
                cout << "\tDELETE ALL ITEMS COMMAND" << endl;
                deleteAllItems(list);
            }
            else if (regex_match(line, delete_all_trees)) {
                cout << "\tDELETE ALL TREES COMMAND" << endl;
                deleteAllTreeItems(list);
            }
            else if (regex_match(line, delete_all_bushes)) {
                cout << "\tDELETE ALL BUSHES COMMAND" << endl;
                deleteAllBushItems(list);
            }
            else if (regex_match(line, delete_all_cactuses)) {
                cout << "\tDELETE ALL CACTUSES COMMAND" << endl;
                deleteAllCactusItems(list);
            }
            else {
                cout << "\t\tWRONG COMMAND" << endl;
            }
        }
        else {
            cout << endl;
        }
    }
    file.close();

    return 0;
}
