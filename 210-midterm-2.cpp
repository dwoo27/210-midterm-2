#include <iostream> //gives access to cout/endl
#include <string> //gives access to string data type
#include <array> //gives access to use arrays
#include <fstream> //for ifstream
#include <ctime> //for time random number generator
#include <cstdlib> //for random numbers
using namespace std; //able to exclude std::

const int MINUTES = 20; //used for minutes of sim
const int NAMES = 99; //used for # names in txt file

class DoublyLinkedList {
private:
    struct Node {
        string name;
        Node* prev;
        Node* next;
        Node(string nm, Node* p = nullptr, Node* n = nullptr) {
            name = nm;
            prev = p;
            next = n;
        }
    };

    Node* head;
    Node* tail;

public:
    DoublyLinkedList() { head = nullptr; tail = nullptr; }

    int size() { //returns number of customers in line
        int count = 0; //stores number of customers
        Node* current = head; //ptr to node to be counted, starting at head

        while (current) { //while current is not null
            count++; //update count
            current = current->next; //move current up list
        }

        return count; //return count
    }

    string getHead() { //return head name
        if (!head) { //check if line is empty
            return "Line is empty."; //return statement
        }

        return head->name; //return name of first person in line
    }

    string getTail(){ //return tail name
        if (!tail) { //check if line is empty
            return "Line is empty."; //return statement
        }

        return tail->name;
    }

    string getAtPos(int p) { //return name at pos
        if (p < 1 || p > size()) { //if pos is out of bounds
            return "Invalid position."; //return error statemnt
        }

        Node* current = head; //ptr to node to be found, starting at head

        for (int i = 1; i < p; i++) { //runs throough positions, starting at one until pos to be found
            current = current->next; //moves current up list
        }

        return current->name; //return name of customer at post
    }

    void insert_after(string name, int position) {
        if (position < 0) {
            cout << "Position must be >= 0." << endl;
            return;
        }

        Node* newNode = new Node(name);
        if (!head) {
            head = tail = newNode;
            return;
        }

        Node* temp = head;
        for (int i = 0; i < position && temp; ++i)
            temp = temp->next;

        if (!temp) {
            cout << "Position exceeds list size. Node not inserted.\n";
            delete newNode;
            return;
        }

        newNode->next = temp->next;
        newNode->prev = temp;
        if (temp->next)
            temp->next->prev = newNode;
        else
            tail = newNode;
        temp->next = newNode;
    }

    void delete_name(string n) {
        if (!head) return;

        Node* temp = head;

        while (temp && temp->name != n)
            temp = temp->next;

        if (!temp) return;

        if (temp->prev)
            temp->prev->next = temp->next;
        else
            head = temp->next;

        if (temp->next)
            temp->next->prev = temp->prev;
        else
            tail = temp->prev;

        delete temp;
    }

    void delete_pos(int pos) {
        if (!head) {
            cout << "List is empty." << endl;
            return;
        }

        if (pos == 1) {
            pop_front();
            return;
        }

        Node* temp = head;

        for (int i = 1; i < pos; i++) {
            if (!temp) {
                cout << "Position doesn't exist." << endl;
                return;
            }
            else
                temp = temp->next;
        }
        if (!temp) {
            cout << "Position doesn't exist." << endl;
            return;
        }

        if (!temp->next) {
            pop_back();
            return;
        }

        Node* tempPrev = temp->prev;
        tempPrev->next = temp->next;
        temp->next->prev = tempPrev;
        delete temp;
    }

    void push_back(string v) {
        Node* newNode = new Node(v);
        if (!tail)
            head = tail = newNode;
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void push_front(string v) {
        Node* newNode = new Node(v);
        if (!head)
            head = tail = newNode;
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }

    void pop_front() {

        if (!head) {
            cout << "List is empty." << endl;
            return;
        }

        Node* temp = head;

        if (head->next) {
            head = head->next;
            head->prev = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    void pop_back() {
        if (!tail) {
            cout << "List is empty." << endl;
            return;
        }
        Node* temp = tail;

        if (tail->prev) {
            tail = tail->prev;
            tail->next = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    ~DoublyLinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    void print() {
        Node* current = head;
        if (!current) {
            cout << "Line is empty." << endl;
            return;
        }
        while (current) {
            cout << "\t\t" << current->name << " ";
            current = current->next;
        }
        cout << endl;
    }

    void print_reverse() {
        Node* current = tail;
        if (!current) {
            cout << "Line is empty." << endl;
            return;
        }
        while (current) {
            cout << current->name << " ";
            current = current->prev;
        }
        cout << endl;
    }
};

void finNames(array<string, NAMES>&); //read names from txt file
void coutNames(array<string, NAMES>&); //cout names in array
string randName(array<string, NAMES>&); //chooses and retunrs randome name from array

int main() {
    srand(time(0)); //seeds random number generator

    array<string, NAMES> names; //array to store all names
    finNames(names); //call function to populate array

    DoublyLinkedList line; //empty customer line

    cout << "Store opens:" << endl; //print message

    for (int i = 0; i < 5; i++) { //adds 5 customers right away
        string customer = randName(names); //store random name from list
        line.push_back(customer); //creates new customer node and sends it to back of line
        cout << "\t" << customer << " joins the line" << endl; //print message
    }

    cout << "\t Resulitng line:" << endl; //print message
    line.print(); //print names in line
    cout << endl;

    
}

void finNames(array<string, NAMES>& names) {
    ifstream fin; //create fstream obj
    fin.open("names.txt"); //open "names.txt" file

    if (fin.good()) { //checks if file opened
        for (int i = 0; i < NAMES; i++) { //loops through each index in array
            getline(fin, names[i]); //reads line one at a time from file to names at index
        }
        fin.close(); //close file
    }
    else { //error message if file could not open
        cout << "File not found" << endl;
    }
}

void coutNames(array<string, NAMES>& names) {
    for (int i = 0; i < NAMES; i++) { //loop through each name in array
        cout << i + 1 << ". " << names[i] << endl; //cout name and +1 for pos number
    }
}

string randName(array<string, NAMES>& names) {
    int index = rand() % NAMES; //assigns random int 0 to NAMES - 1
    
    return names[index]; //return name at random pos
}