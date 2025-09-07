#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <map>
#include <sstream>
#include <Windows.h>
using namespace std;

// ANSI ���� �ڵ�
const string RED = "\033[31m";
const string RESET = "\033[0m";

// -------------------- ���� �÷��� --------------------
struct CommandState {
    bool a = false;   // �빮�� ��ȯ
    bool b = false;   // �ܾ� ���� ���
    bool c = false;   // �빮�ڷ� �����ϴ� �ܾ� ����
    bool d = false;   // �� �� �Ųٷ�
    bool e = false;   // ���鿡 /
    bool f = false;   // �ܾ� ���� �Ųٷ�
    bool g = false;   // ���� ġȯ
    bool h = false;   // ���� �� ������
    int iState = 0;   // 0=����, 1=��������, 2=��������
    bool j = false;   // Ư�� �ܾ� ã�� ����

    char replaceFrom = '\0';
    char replaceTo = '\0';
    string searchWord = "";
};

// -------------------- ��ƿ --------------------
vector<string> readFile(const string& filename) {
    ifstream fin(filename);
    vector<string> lines;
    string line;
    while (getline(fin, line)) {
        lines.push_back(line);
    }
    return lines;
}

// -------------------- ���� �Լ� --------------------

// a: �빮�ڷ�
void applyUppercase(vector<string>& lines) {
    for (auto& line : lines) {
        for (auto& ch : line) ch = toupper(ch);
    }
}

// d: �� �� ��ü �Ųٷ�
void applyReverseLine(vector<string>& lines) {
    for (auto& line : lines) {
        reverse(line.begin(), line.end());
    }
}

// e: ���� �ڿ� /
void applyInsertSlash(vector<string>& lines) {
    for (auto& line : lines) {
        for (size_t i = 0; i < line.size(); i++) {
            if (line[i] == ' ') {
                line.insert(i + 1, "/");
                i++;
            }
        }
    }
}

// f: �ܾ� ���� �Ųٷ�
void applyReverseWords(vector<string>& lines) {
    for (auto& line : lines) {
        vector<string> words;
        stringstream ss(line);
        string word;
        while (ss >> word) words.push_back(word);
        reverse(words.begin(), words.end());
        line.clear();
        for (size_t i = 0; i < words.size(); i++) {
            if (i > 0) line += " ";
            line += words[i];
        }
    }
}

// g: ���� ġȯ
void applyReplaceChar(vector<string>& lines, char from, char to) {
    for (auto& line : lines) {
        for (auto& ch : line) {
            if (ch == from) ch = to;
        }
    }
}

// h: ���� �ڿ��� �� ������
void applyNumberSplit(vector<string>& lines) {
    vector<string> newLines;
    for (auto& line : lines) {
        string current;
        for (size_t i = 0; i < line.size(); i++) {
            current += line[i];
            if (isdigit(line[i]) && i + 1 < line.size()) {
                newLines.push_back(current);
                current.clear();
            }
        }
        if (!current.empty()) newLines.push_back(current);
    }
    lines = newLines;
}

// i: ���ĺ� ���� ����
int countAlphas(const string& line) {
    return count_if(line.begin(), line.end(), ::isalpha);
}
void applySort(vector<string>& lines, int mode) {
    if (mode == 1) {
        sort(lines.begin(), lines.end(), [](const string& a, const string& b) {
            return countAlphas(a) < countAlphas(b);
            });
    }
    else if (mode == 2) {
        sort(lines.begin(), lines.end(), [](const string& a, const string& b) {
            return countAlphas(a) > countAlphas(b);
            });
    }
}

// -------------------- ��� �Լ� --------------------
void printLines(const vector<string>& lines, const CommandState& state) {
    int capitalWordCount = 0;
    int searchWordCount = 0;

    for (auto& line : lines) {
        stringstream ss(line);
        string word;
        bool firstWord = true;
        while (ss >> word) {
            string outWord = word;

            // c: �빮�ڷ� �����ϴ� �ܾ� ����
            if (state.c && isupper(word[0])) {
                cout << RED << outWord << RESET;
                capitalWordCount++;
            }
            // j: Ư�� �ܾ� ���� (��ҹ��� ����)
            else if (state.j) {
                string lowerWord = word, lowerSearch = state.searchWord;
                transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::tolower);
                transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), ::tolower);
                if (lowerWord == lowerSearch) {
                    cout << RED << outWord << RESET;
                    searchWordCount++;
                }
                else {
                    cout << outWord;
                }
            }
            else {
                cout << outWord;
            }

            if (!ss.eof()) cout << " ";
        }

        if (state.b) {
            // �ܾ� ���� ���
            int count = 0;
            stringstream countSS(line);
            while (countSS >> word) count++;
            cout << " [" << count << "]";
        }
        cout << "\n";
    }

    if (state.c) cout << "�빮�ڷ� �����ϴ� �ܾ� ����: " << capitalWordCount << "\n";
    if (state.j) cout << "�˻� �ܾ� ����: " << searchWordCount << "\n";
}

// -------------------- ������ --------------------
void render(const vector<string>& original, const CommandState& state) {
    vector<string> lines = original;

    if (state.a) applyUppercase(lines);
    if (state.d) applyReverseLine(lines);
    if (state.e) applyInsertSlash(lines);
    if (state.f) applyReverseWords(lines);
    if (state.g) applyReplaceChar(lines, state.replaceFrom, state.replaceTo);
    if (state.h) applyNumberSplit(lines);
    if (state.iState != 0) applySort(lines, state.iState);

    printLines(lines, state);
}

// -------------------- ���� --------------------
int main() {
    string file;
    cout << "���� �̸�: ";
    cin >> file;
    vector<string> original = readFile(file);
    if (original.empty()) {
        cout << "������ ���� �� �����ϴ�.\n";
        return 1;
    }

    CommandState state;

    while (true) {
        system("cls");
        render(original, state);
        cout << "\n��� �Է� (a~j, q=����): ";
        char cmd;
        cin >> cmd;

        if (cmd == 'q') break;

        switch (cmd) {
        case 'a': state.a = !state.a; break;
        case 'b': state.b = !state.b; break;
        case 'c': state.c = !state.c; break;
        case 'd': state.d = !state.d; break;
        case 'e': state.e = !state.e; break;
        case 'f': state.f = !state.f; break;
        case 'g':
            state.g = !state.g;
            if (state.g) {
                cout << "�ٲ� ���ڿ� �� ���� �Է�: ";
                cin >> state.replaceFrom >> state.replaceTo;
            }
            break;
        case 'h': state.h = !state.h; break;
        case 'i': state.iState = (state.iState + 1) % 3; break;
        case 'j':
            state.j = !state.j;
            if (state.j) {
                cout << "ã�� �ܾ� �Է�: ";
                cin >> state.searchWord;
            }
            break;
        default: cout << "�߸��� ���!\n";
        }
    }

    return 0;
}
