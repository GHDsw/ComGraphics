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

// ANSI 색상 코드
const string RED = "\033[31m";
const string RESET = "\033[0m";

// -------------------- 상태 플래그 --------------------
struct CommandState {
    bool a = false;   // 대문자 변환
    bool b = false;   // 단어 개수 출력
    bool c = false;   // 대문자로 시작하는 단어 강조
    bool d = false;   // 한 줄 거꾸로
    bool e = false;   // 공백에 /
    bool f = false;   // 단어 순서 거꾸로
    bool g = false;   // 문자 치환
    bool h = false;   // 숫자 뒤 나누기
    int iState = 0;   // 0=원래, 1=오름차순, 2=내림차순
    bool j = false;   // 특정 단어 찾기 강조

    char replaceFrom = '\0';
    char replaceTo = '\0';
    string searchWord = "";
};

// -------------------- 유틸 --------------------
vector<string> readFile(const string& filename) {
    ifstream fin(filename);
    vector<string> lines;
    string line;
    while (getline(fin, line)) {
        lines.push_back(line);
    }
    return lines;
}

// -------------------- 가공 함수 --------------------

// a: 대문자로
void applyUppercase(vector<string>& lines) {
    for (auto& line : lines) {
        for (auto& ch : line) ch = toupper(ch);
    }
}

// d: 한 줄 전체 거꾸로
void applyReverseLine(vector<string>& lines) {
    for (auto& line : lines) {
        reverse(line.begin(), line.end());
    }
}

// e: 공백 뒤에 /
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

// f: 단어 순서 거꾸로
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

// g: 문자 치환
void applyReplaceChar(vector<string>& lines, char from, char to) {
    for (auto& line : lines) {
        for (auto& ch : line) {
            if (ch == from) ch = to;
        }
    }
}

// h: 숫자 뒤에서 줄 나누기
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

// i: 알파벳 개수 정렬
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

// -------------------- 출력 함수 --------------------
void printLines(const vector<string>& lines, const CommandState& state) {
    int capitalWordCount = 0;
    int searchWordCount = 0;

    for (auto& line : lines) {
        stringstream ss(line);
        string word;
        bool firstWord = true;
        while (ss >> word) {
            string outWord = word;

            // c: 대문자로 시작하는 단어 강조
            if (state.c && isupper(word[0])) {
                cout << RED << outWord << RESET;
                capitalWordCount++;
            }
            // j: 특정 단어 강조 (대소문자 무시)
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
            // 단어 개수 출력
            int count = 0;
            stringstream countSS(line);
            while (countSS >> word) count++;
            cout << " [" << count << "]";
        }
        cout << "\n";
    }

    if (state.c) cout << "대문자로 시작하는 단어 개수: " << capitalWordCount << "\n";
    if (state.j) cout << "검색 단어 개수: " << searchWordCount << "\n";
}

// -------------------- 렌더링 --------------------
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

// -------------------- 메인 --------------------
int main() {
    string file;
    cout << "파일 이름: ";
    cin >> file;
    vector<string> original = readFile(file);
    if (original.empty()) {
        cout << "파일을 읽을 수 없습니다.\n";
        return 1;
    }

    CommandState state;

    while (true) {
        system("cls");
        render(original, state);
        cout << "\n명령 입력 (a~j, q=종료): ";
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
                cout << "바꿀 문자와 새 문자 입력: ";
                cin >> state.replaceFrom >> state.replaceTo;
            }
            break;
        case 'h': state.h = !state.h; break;
        case 'i': state.iState = (state.iState + 1) % 3; break;
        case 'j':
            state.j = !state.j;
            if (state.j) {
                cout << "찾을 단어 입력: ";
                cin >> state.searchWord;
            }
            break;
        default: cout << "잘못된 명령!\n";
        }
    }

    return 0;
}
