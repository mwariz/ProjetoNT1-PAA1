#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node(int v) : data(v), next(nullptr) {}
};

class LinkedList {
    Node* head;
    int sz;

    // Reorganiza os nós em sublistas menores e maiores que o pivô,
    // depois reconecta recursivamente sem copiar valores
    Node* quickSort(Node* h) {
        if (!h || !h->next) return h;

        Node* pivot = h;
        Node* curr  = h->next;
        pivot->next = nullptr;

        Node* lessHead = nullptr, *lessTail = nullptr;
        Node* greaterHead = nullptr, *greaterTail = nullptr;

        while (curr) {
            Node* nxt = curr->next;
            curr->next = nullptr;

            if (curr->data <= pivot->data) {
                if (!lessHead) lessHead = lessTail = curr;
                else { lessTail->next = curr; lessTail = curr; }
            } else {
                if (!greaterHead) greaterHead = greaterTail = curr;
                else { greaterTail->next = curr; greaterTail = curr; }
            }
            curr = nxt;
        }

        lessHead    = quickSort(lessHead);
        greaterHead = quickSort(greaterHead);

        // encadeia: [menores] -> pivô -> [maiores]
        if (lessHead) {
            Node* tmp = lessHead;
            while (tmp->next) tmp = tmp->next;
            tmp->next = pivot;
        } else {
            lessHead = pivot;
        }
        pivot->next = greaterHead;

        return lessHead;
    }

public:
    LinkedList() : head(nullptr), sz(0) {}

    ~LinkedList() {
        while (head) {
            Node* tmp = head->next;
            delete head;
            head = tmp;
        }
    }

    // Insere no início da lista
    void insert(int val) {
        Node* node = new Node(val);
        node->next = head;
        head = node;
        sz++;
    }

    // Remove a primeira ocorrência de val; retorna false se não existir
    bool remove(int val) {
        Node* curr = head, *prev = nullptr;
        while (curr) {
            if (curr->data == val) {
                if (prev) prev->next = curr->next;
                else      head       = curr->next;
                delete curr;
                sz--;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    // Busca linear; retorna true se o valor está na lista
    bool search(int val) const {
        for (Node* curr = head; curr; curr = curr->next)
            if (curr->data == val) return true;
        return false;
    }

    void print() const {
        for (Node* curr = head; curr; curr = curr->next) {
            cout << curr->data;
            if (curr->next) cout << " -> ";
        }
        cout << "\n";
    }

    int size() const { return sz; }

    void sort() {
        if (head && head->next)
            head = quickSort(head);
    }
};

// -----------------------------------------------------------
// Auxiliar: executa as operações e ordena para cada cenário
// -----------------------------------------------------------
void runCase(const string& label, LinkedList& list, int removeVal, int searchVal) {
    cout << "=== " << label << " ===\n";
    cout << "Lista (" << list.size() << " nos): ";
    list.print();

    cout << "Removendo " << removeVal << ": "
         << (list.remove(removeVal) ? "removido" : "nao encontrado") << "\n";

    cout << "Buscando "  << searchVal << ": "
         << (list.search(searchVal)  ? "encontrado" : "nao encontrado") << "\n";

    cout << "Apos remocao (" << list.size() << " nos): ";
    list.print();

    list.sort();
    cout << "Apos Quick Sort: ";
    list.print();
    cout << "\n";
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    // --- Caso 1: valores aleatorios ---
    LinkedList randList;
    for (int i = 0; i < 10; i++)
        randList.insert(rand() % 99 + 1);
    runCase("Aleatorio", randList, 50, 30);

    // --- Caso 2: quase ordenado ---
    // Insere do maior pro menor → lista fica 1->2->...->10
    // Depois move dois elementos para o início, quebrando a ordem
    LinkedList almostList;
    for (int i = 10; i >= 1; i--)
        almostList.insert(i);
    almostList.remove(3);
    almostList.remove(7);
    almostList.insert(3);  // agora no início
    almostList.insert(7);  // agora no início
    runCase("Quase Ordenado", almostList, 5, 8);

    // --- Caso 3: ordem inversa ---
    // Inserção crescente → lista fica 10->9->...->1
    LinkedList revList;
    for (int i = 1; i <= 10; i++)
        revList.insert(i);
    runCase("Inverso", revList, 1, 6);

    return 0;
}
