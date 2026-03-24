#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <cstring>

const int HASH_TABLE_SIZE = 1000037; // A prime number for better distribution

template<typename T>
struct HashNode {
    std::string key;
    T value;
    HashNode* next;

    HashNode(const std::string& k, const T& v) : key(k), value(v), next(nullptr) {}
};

template<typename T>
class HashTable {
private:
    HashNode<T>** table;
    int capacity;
    int size;

    unsigned int hashFunction(const std::string& key) const {
        unsigned int hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash % capacity;
    }

public:
    HashTable(int cap = HASH_TABLE_SIZE) : capacity(cap), size(0) {
        table = new HashNode<T>*[capacity];
        memset(table, 0, sizeof(HashNode<T>*) * capacity);
    }

    ~HashTable() {
        for (int i = 0; i < capacity; i++) {
            HashNode<T>* current = table[i];
            while (current) {
                HashNode<T>* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }

    bool insert(const std::string& key, const T& value) {
        unsigned int index = hashFunction(key);

        // Check if key already exists
        HashNode<T>* current = table[index];
        while (current) {
            if (current->key == key) {
                return false; // Key already exists
            }
            current = current->next;
        }

        // Insert new node at beginning
        HashNode<T>* newNode = new HashNode<T>(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        size++;
        return true;
    }

    bool remove(const std::string& key) {
        unsigned int index = hashFunction(key);
        HashNode<T>* current = table[index];
        HashNode<T>* prev = nullptr;

        while (current) {
            if (current->key == key) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    table[index] = current->next;
                }
                delete current;
                size--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    T* find(const std::string& key) {
        unsigned int index = hashFunction(key);
        HashNode<T>* current = table[index];

        while (current) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }
        return nullptr;
    }

    const T* find(const std::string& key) const {
        unsigned int index = hashFunction(key);
        HashNode<T>* current = table[index];

        while (current) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }
        return nullptr;
    }

    bool contains(const std::string& key) const {
        return find(key) != nullptr;
    }

    int getSize() const {
        return size;
    }

    bool isEmpty() const {
        return size == 0;
    }
};

#endif // HASH_TABLE_H