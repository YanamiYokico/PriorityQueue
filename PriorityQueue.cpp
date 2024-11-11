#include <iostream>
#include <string>
#include <initializer_list>
using namespace std;

template<typename T>
class Node {
public:
    explicit Node(T value, Node* next = nullptr) {
        this->next = next;
        this->value = value;
    };
    void setValue(T value) { this->value = value; }
    void setNext(Node* next) { this->next = next; }
    T getValue() const { return value; }
    Node* getNext() const { return next; }
    template<typename U>
    friend ostream& operator<<(ostream& out, const Node<U>& node);
private:
    T value;
    Node* next;
};

template<typename U>
ostream& operator<<(ostream& out, const Node<U>& node) {
    out << node.value << '\t';
    return out;
}

template<typename T, typename Compare>
class Queue {
public:
    Queue() : head(nullptr), tail(nullptr), counter(0) {}
    ~Queue() {
        while (!empty()) {
            dequeue();
        }
    }

    void enqueue(const T& value);
    void priorityPush(const T& value);
    void dequeue();
    void print() const;
    bool empty() const;
    bool full() const;
    void pullHighestPriority();
    const T& peek();

private:
    Node<T>* head;
    Node<T>* tail;
    size_t counter;
    Compare priority;
};

template<typename T, typename Compare>
void Queue<T, Compare>::enqueue(const T& value) {
    Node<T>* temp = new Node<T>(value);

    if (!head) {
        head = temp;
    }
    else {
        tail->setNext(temp);
    }
    tail = temp;
    counter++;
}

template<typename T, typename Compare>
void Queue<T, Compare>::priorityPush(const T& value) {
    Node<T>* temp = new Node<T>(value);
    if (!head || priority(value, head->getValue())) {
        temp->setNext(head);
        head = temp;
    }
    else {
        Node<T>* cur = head;
        Node<T>* prev = nullptr;
        while (cur && !priority(value, cur->getValue())) {
            prev = cur;
            cur = cur->getNext();
        }
        prev->setNext(temp);
        temp->setNext(cur);
    }
    counter++;
}

template<typename T, typename Compare>
void Queue<T, Compare>::dequeue() {
    if (!head) return;

    Node<T>* cur = head;
    head = head->getNext();
    delete cur;
    counter--;
}

template<typename T, typename Compare>
void Queue<T, Compare>::print() const {
    Node<T>* cur = head;

    while (cur) {
        cout << cur->getValue();
        cur = cur->getNext();
    }
}

template<typename T, typename Compare>
bool Queue<T, Compare>::empty() const {
    return counter == 0;
}

template<typename T, typename Compare>
bool Queue<T, Compare>::full() const {
    return !empty();
}

template<typename T, typename Compare>
void Queue<T, Compare>::pullHighestPriority()
{
    if (empty()) throw logic_error("Queue is empty.");

    Node<T>* cur = head;
    head = head->getNext();
    delete cur;
    --counter;
}

template<typename T, typename Compare>
const T& Queue<T, Compare>::peek()
{
    if (empty()) throw logic_error("Queue is empty.");

    return head->getValue();
}

struct Greater {
    bool operator()(int left, int right) const {
        return left < right;
    }
};



struct Human {
    string name;
    int rate;
    static int currentIndex;

    Human() : name(""), rate(0) {}
    Human(string n, int r) : name(n), rate(r) { id = currentIndex++; }

    int id;

    friend ostream& operator<<(ostream& out, const Human& other)
    {
        out << other.id << '.' << other.name << " rate: " << other.rate << endl;
        return out;
    }
};
int Human::currentIndex = 1;
struct CompareHumanByRate
{
    bool operator()(const Human& left, const Human& right)
    {
        return left.rate < right.rate;
    }
};

int main() {
    Queue<Human, CompareHumanByRate> humans;

    cout << humans.empty() << endl;

    for (int i = 0; i < 5; ++i) {
        string name = "Person" + to_string(i + 1);
        int rate = rand() % 100;
        Human human(name, rate);
        humans.priorityPush(human);
    }
    cout << humans.full() << endl;

    humans.print();
    cout << endl;
}
