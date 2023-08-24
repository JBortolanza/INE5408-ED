#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int ALPHABET_SIZE = 26;

struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    bool isWord;
    unsigned long position;
    unsigned long length;
    unsigned long wordCount;
};

TrieNode* createNode() {
    TrieNode* node = new TrieNode;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = nullptr;
    }
    node->isWord = false;
    node->position = 0;
    node->length = 0;
    node->wordCount = 0;
    return node;
}

void insertWord(TrieNode* root, const string& word, unsigned long position, unsigned long length) {
    TrieNode* current = root;
    for (char c : word) {
        int index = c - 'a';
        if (current->children[index] == nullptr) {
            current->children[index] = createNode();
        }
        current = current->children[index];
        current->wordCount++; // Incrementa o contador de palavras
    }
    current->isWord = true;
    current->position = position;
    current->length = length;
}

TrieNode* searchPrefix(TrieNode* root, const string& prefix) {
    TrieNode* current = root;
    for (char c : prefix) {
        int index = c - 'a';
        if (current->children[index] == nullptr) {
            return nullptr;
        }
        current = current->children[index];
    }
    return current;
}

void printPrefix(TrieNode* root, const string& prefix) {
    TrieNode* prefixNode = searchPrefix(root, prefix);
    if (prefixNode != nullptr) {
        cout << prefix << " is prefix of " << prefixNode->wordCount << " words" << endl;
        if (prefixNode->isWord) {
            cout << prefix << " is at (" << prefixNode->position << "," << prefixNode->length << ")" << endl;
        }
    } else {
        cout << prefix << " is not prefix" << endl;
    }
}

void parseDictionary(const string& filename, TrieNode* root) {
    ifstream file(filename);
    string line;
    unsigned long position = 0;
    while (getline(file, line)) {
        size_t openingBracket = line.find('[');
        size_t closingBracket = line.find(']');
        if (openingBracket != string::npos && closingBracket != string::npos && closingBracket > openingBracket + 1) {
            string word = line.substr(openingBracket + 1, closingBracket - openingBracket - 1);
            insertWord(root, word, position, line.length());
        }
        position += line.length() + 1; // Considerar também o caractere de quebra de linha '\n'
    }
    file.close();
}

int main() {
    string dictionaryFile;
    cin >> dictionaryFile;

    TrieNode* root = createNode();
    parseDictionary(dictionaryFile, root);

    string word;
    cin >> word;
    while (word != "0") {
        printPrefix(root, word);
        cin >> word;
    }

    return 0;
}