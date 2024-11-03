#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <functional> // needed for std:function

using namespace std;

// trieNode class definition representing each node in the trie structure
class TrieNode {
public:
    unordered_map<char, TrieNode*> children; // map of the child nodes (character to trienode)
    bool isEndOfWord; // marks if the node represents the end of a valid word

    TrieNode() : isEndOfWord(false) {} // constructor initializing isEndOfWord
};

// trie class definition containing methods for trie operations
class Trie {
private:
    TrieNode* root; // root node of the trie

    // helper function to recursively find all words with a given prefix
    void findWordsWithPrefixHelper(TrieNode* node, string currentWord, vector<string>& results) {
        if (node->isEndOfWord) {
            results.push_back(currentWord); // adds current word to result if it's a valid word
        }
        for (auto& child : node->children) { // recursive call for each child node
            findWordsWithPrefixHelper(child.second, currentWord + child.first, results);
        }
    }

public:
    Trie() {
        root = new TrieNode(); // initializes the root node
    }

    // insert word into the trie
    void insert(const string& word) {
        TrieNode* node = root;
        for (char ch : word) { 
            // creates a new node if character not found in current node's children
            if (!node->children.count(ch)) {
                node->children[ch] = new TrieNode();
            }
            node = node->children[ch]; // move to the child node
        }
        node->isEndOfWord = true; // marks the end of the word
        cout << "Inserted: " << word << endl;
    }

    // search for an exact word in the trie
    bool search(const string& word) {
        TrieNode* node = root;
        for (char ch : word) { 
            // if character not found, the word doesn't exist in the trie
            if (!node->children.count(ch)) {
                cout << "Word not found: " << word << endl;
                return false;
            }
            node = node->children[ch]; // move to the child node
        }
        if (node->isEndOfWord) {
            cout << "Word found: " << word << endl;
            return true; // word found
        } else {
            cout << "Prefix found but not a complete word: " << word << endl;
            return false; // only the prefix is found, not a complete word
        }
    }

    // check if there exists a word with the given prefix
    bool startsWith(const string& prefix) {
        TrieNode* node = root;
        for (char ch : prefix) {
            if (!node->children.count(ch)) { // if character not found, the prefix doesn't exist in the trie
                cout << "Prefix not found: " << prefix << endl;
                return false;
            }
            node = node->children[ch]; // moves to the child node
        }
        cout << "Prefix exists: " << prefix << endl;
        return true; // prefix exists
    }

    // find all words in the Trie that start with a given prefix
    vector<string> findWordsWithPrefix(const string& prefix) {
        TrieNode* node = root;
        vector<string> results;
        for (char ch : prefix) {
            // if prefix path doesn't exist, return empty results
            if (!node->children.count(ch)) {
                cout << "No words found with prefix: " << prefix << endl;
                return results;
            }
            node = node->children[ch]; // move to the child node
        }
        findWordsWithPrefixHelper(node, prefix, results); // calls helper function to find all words with the given prefix
        cout << "Words with prefix \"" << prefix << "\": ";
        for (const string& word : results) {
            cout << word << " ";
        }
        cout << endl;
        return results;
    }

    // destructor to free memory used by trie nodes
    ~Trie() {
        // recursive function to delete all nodes
        function<void(TrieNode*)> deleteNodes = [&](TrieNode* node) {
            for (auto& child : node->children) {
                deleteNodes(child.second); // recursively deletes all children
            }
            delete node; // delete the current node
        };
        deleteNodes(root); // start deletion from the root
    }
};

// function to read words from a file and insert them into the trie
void loadWordsFromFile(Trie& trie, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl; // error handling for opening file
        return;
    }

    string word;
    while (getline(file, word)) {
        // remove any whitespace around the word
        word.erase(remove_if(word.begin(), word.end(), ::isspace), word.end());
        if (!word.empty()) {
            trie.insert(word); // insert non-empty words into the trie
        }
    }
    file.close(); // close the fil after reading
    cout << "All words loaded from file: " << filename << endl;
}

// helper functions for specific word searches
void searchWords(Trie& trie, const vector<string>& words) {
    for (const string& word : words) {
        trie.search(word); // search each word and print result
    }
}

void findPrefixes(Trie& trie, const vector<string>& prefixes) {
    for (const string& prefix : prefixes) {
        trie.findWordsWithPrefix(prefix); // find all words with each prefix
    }
}

void performTasks(Trie& trie) {
    // task ii. search for specific words
    vector<string> searchWordsList = {"prefix", "data", "hello", "do", "workplace", "there", "pneumonia", "word2vec", "iPhone", "a-frame"};
    cout << "ii. Search for a word in the trie:\n";
    for (const string& word : searchWordsList) {
        cout << word << ": " << (trie.search(word) ? "Found" : "Not found") << endl;
    }

    // task iii. find words that start with a given prefix
    vector<string> prefixes = {"pre", "pro", "work", "i", "e"};
    cout << "\niii. Return words that start with a given prefix:\n";
    for (const string& prefix : prefixes) {
        vector<string> wordsWithPrefix = trie.findWordsWithPrefix(prefix);
        cout << prefix << "-: ";
        for (const string& word : wordsWithPrefix) {
            cout << word << " ";
        }
        cout << endl;
    }

    // task iv. return short words
    vector<string> shortWords = {"a", "an", "by"};
    cout << "\niv. Return short words:\n";
    for (const string& word : shortWords) {
        cout << word << ": " << (trie.search(word) ? "Found" : "Not found") << endl;
    }

    // task v. return hyphenated words
    vector<string> hyphenatedWords = {"e-mail", "t-shirt"};
    cout << "\nv. Return hyphenated words:\n";
    for (const string& word : hyphenatedWords) {
        cout << word << ": " << (trie.search(word) ? "Found" : "Not found") << endl;
    }

    // task vi. return words with numbers
    vector<string> numberWords = {"24hours", "2day"};
    cout << "\nvi. Return words with numbers:\n";
    for (const string& word : numberWords) {
        cout << word << ": " << (trie.search(word) ? "Found" : "Not found") << endl;
    }

    // task vii. return mixed case words
    vector<string> mixedCaseWords = {"JavaScript", "PowerPoint"};
    cout << "\nvii. Return mixed case words:\n";
    for (const string& word : mixedCaseWords) {
        cout << word << ": " << (trie.search(word) ? "Found" : "Not found") << endl;
    }

    // task viii. return special cases
    vector<string> specialCases = {"psychology", "pterodactyl", "xylophone", "yacht", "eBay", "iPad", "t-shirt", "x-ray"};
    cout << "\nviii. Return special cases:\n";
    for (const string& word : specialCases) {
        cout << word << ": " << (trie.search(word) ? "Found" : "Not found") << endl;
    }

    // task ix. handle exception and memory management (search for words that may not exist)
    vector<string> exceptionWords = {"abc", "xyz"};
    cout << "\nix. Handle exception and memory management:\n";
    for (const string& word : exceptionWords) {
        cout << word << ": " << (trie.search(word) ? "Found" : "Not found") << endl;
    }
}

// main function
int main() {
    Trie trie;

    // load words from the dataset file
    string filename = "Lab5_dictionary-dataset.txt"; // use the correct path
    loadWordsFromFile(trie, filename); // task i. insert words into a trie
    cout << "i. Insert words into a trie (all dataset)\n";
    cout << "Words loaded from file: " << filename << "\n\n";

    // perform remaining tasks
    performTasks(trie);

    return 0;
}
