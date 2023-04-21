
//

#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Huffman {
public:
    //scope of Huffman class-----------------------------------------------------------------------------------------------
    //nodes of the huffman tree
    struct Character {
        char symbol;
        int frequency;

        Character* left;
        Character* right;

        Character(char d, int f) {
            left = right = NULL;
            symbol = d;
            frequency = f;
        }//close constructor
    };

    //struct used to organize priority queue
    struct greater {
        bool operator() (Character* left, Character* right) {
            return (left->frequency > right->frequency);
        }
    };

    //Priority queue and related data structures for character data and their frequencies
    priority_queue<Character*, vector<Character*>, greater> PQ;
    int n;
    map<char, int> freq;    //store frequencies
    map<char, string> codes;        //store huffman codes

    //retreieve the characters used in the document and their frequencies
    Huffman(map<char, int>& frequency, int n) {

        this->freq = frequency;
        this->n = n;

        //start program once initilized
        HuffmanCodes();
    }//close constructor

    //create Huffman tree-----------------------------------------------------------------------------------------------------
    void HuffmanCodes() {
        struct Character* left{}, * right{}, * top{};
        createPriorityQueue();

        while (PQ.size() != 1)
        {
            left = PQ.top();
            PQ.pop();
            right = PQ.top();
            PQ.pop();
            top = new Character('%', left->frequency + right->frequency);
            top->left = left;
            top->right = right;
            insert(top);
        }

        storeCodes(PQ.top(), "");
        printCodes(PQ.top(), "");

    }//close HuffmanCodes

    //insert pointer into queue
    void insert(Character* p) {
        PQ.push(p);
    }//close insert

    //create Priority Queue
    void createPriorityQueue() {
        for (map<char, int>::iterator iter = freq.begin(); iter != freq.end(); iter++)
        {
            PQ.push(new Character(iter->first, iter->second));
        }
    }//close createPriorityQueue

    //print the huffman codes
    void printCodes(Character* root, string s) {
        if (!root)
            return;
        if (root->symbol != '%')
            cout << root->symbol << ": " << s << "\n";
        printCodes(root->left, s + "0");
        printCodes(root->right, s + "1");
    }//close printCodes

    void storeCodes(Character* root, string s) {
        if (!root)
            return;
        if (root->symbol != '%')
            codes[root->symbol] = s;
        cout << "FIXME: storeCodes - " << codes[root->symbol] << endl;
        storeCodes(root->left, s + "0");
        storeCodes(root->right, s + "1");
    }//close printCodes

    string decodeHuffman(Character* root, string s) {
        string decode = "";
        struct Character* current = root;
        for (int i = 0; i < s.size(); i++)
        {
            if (s[i] == '0') {
                current = current->left;
            }
            else {
                current = current->right;
            }

            if (current->left == NULL and current->right == NULL) {
                decode += current->symbol;
                current = root;
            }
        }

        return decode + '\0';
    }

};//close Huffman

int main()
{
    //Get input txt file and find total number of characters
    char filename[30], ch;
    ifstream input;
    cout << "Enter the name of the file: ";
    cin >> filename;
    input.open(filename, ifstream::in);
    if (!input)
    {
        cout << endl << "File doesn't exist or Access denied!";
        return 0;
    }

    //count the number of each character based on ASCII code
    int n = 0;
    //get data for huffman codes
    map<char, int> freq;

    while (input >> noskipws >> ch) 
    {
        n++;
        std::map<char, int>::iterator iter = freq.find(ch);

        if (iter != freq.end()) {
            iter->second++;
        }
        else {
            freq.insert(make_pair(ch, 1));
        }
    }

    //close access to file and print out the txt file summary
    input.close();
    cout << endl << "Total Characters = " << n;
    cout << endl;

    cout << "Summary: " << endl;
    for (auto i : freq)
    {
        cout << '{' << i.first << ": " << i.second << '}' << endl;
    }

    cout << endl;
    //Make huffman codes
    Huffman threeLittlePigs(freq, n);

    //the symbols of the txt and the characters' huffman codes are calculated. Now, encode the txt using the huffman codes
    cout << "FIXME: printing codes..." << endl;
    for (auto i : threeLittlePigs.codes)
    {
        cout << '{' << i.first << ": " << i.second << '}' << endl;
    }

    //store encoded story by taking one character at a time and storing it's huffman code in "encoded.txt"
    ofstream encodedStory("encoded.txt");
    input.clear();
    input.open(filename, ifstream::in);
    while (input >> noskipws >> ch)
    {   
        //find current character in huffman codes
        std::map<char, string>::iterator iter = threeLittlePigs.codes.find(ch);

        //put coded character in "encoded.txt"
        encodedStory << iter->second;
    }
    input.close();
    encodedStory.close();

    //decode huffman codes and store in txt file
    ofstream decodedStory("decoded.txt");
    ifstream encoded;
    encoded.open("encoded.txt", ifstream::in);
    string str;
    while (!encoded.eof())
    {
        getline(encoded, str);
        decodedStory << threeLittlePigs.decodeHuffman(threeLittlePigs.PQ.top(), str);
    }

    decodedStory.close();
    encodedStory.close();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
