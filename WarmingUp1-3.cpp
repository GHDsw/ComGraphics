#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <Windows.h>
using namespace std;

struct Point {
    int x, y, z;
    bool valid = false;
};

Point points[10];           // ���� ����Ʈ
Point backupPoints[10];     // f ������ �� ������(�ε���/valid ����)
bool sortedMode = false;    // f ��� ����
bool hasBackup = false;    // ������ ���� ����

// --------- ��ƿ ----------
double dist2(const Point& p) { // �������� �Ÿ��� ����(���Ŀ�: sqrt ���ʿ�)
    return 1.0 * p.x * p.x + 1.0 * p.y * p.y + 1.0 * p.z * p.z;
}

void printList() {
    for (int i = 9; i >= 0; --i) {
        if (points[i].valid) {
            cout << i << " " << points[i].x << " " << points[i].y << " " << points[i].z << "\n";
        }
        else {
            cout << i << "\n";
        }
    }
}

int countPoints() {
    int c = 0;
    for (int i = 0; i < 10; ++i) if (points[i].valid) ++c;
    return c;
}

void invalidateSort() {
    sortedMode = false;
    hasBackup = false;
}

// --------- ��� ���� ----------
void addTop(int x, int y, int z) {
    invalidateSort();
    for (int i = 0; i < 10; ++i) {
        if (!points[i].valid) {
            points[i] = { x, y, z, true };
            return;
        }
    }
    // �� á���� �ƹ� �͵� �� �� (�ʿ� �� ��å ���� ����)
    // cout << "����Ʈ�� ���� á���ϴ�!\n";
}

void removeTop() {
    invalidateSort();
    for (int i = 9; i >= 0; --i) {
        if (points[i].valid) {
            points[i].valid = false;
            return;
        }
    }
}

void addBottom(int x, int y, int z) {
    invalidateSort();
    for (int i = 9; i > 0; --i) points[i] = points[i - 1];
    points[0] = { x, y, z, true };
}

void removeBottom() {
    invalidateSort();
    points[0].valid = false;
}

void shiftDown() { // 0��9, 1��0, �� 9��8
    invalidateSort();
    Point t = points[0];
    for (int i = 0; i < 9; ++i) points[i] = points[i + 1];
    points[9] = t;
}

void clearAll() {
    for (int i = 0; i < 10; ++i) points[i].valid = false;
    invalidateSort();
}

void sortByDistanceToggle() {
    if (!sortedMode) {
        // ������ ����(�ε���/valid ����)
        for (int i = 0; i < 10; ++i) backupPoints[i] = points[i];
        hasBackup = true;
        sortedMode = true;

        // ��ȿ�� ���� ��Ƽ� �Ÿ� �������� ����
        vector<Point> v;
        v.reserve(10);
        for (int i = 0; i < 10; ++i) if (points[i].valid) v.push_back(points[i]);
        sort(v.begin(), v.end(), [](const Point& a, const Point& b) {
            return dist2(a) < dist2(b);
            });

        // �ε��� 0���� �� ĭ ���� ���ġ (�������� ����)
        for (int i = 0; i < 10; ++i) points[i].valid = false;
        for (int i = 0; i < (int)v.size(); ++i) points[i] = v[i];
    }
    else {
        // ������ ����: ���� �ε���/��ĭ �״��
        if (hasBackup) {
            for (int i = 0; i < 10; ++i) points[i] = backupPoints[i];
        }
        sortedMode = false;
        hasBackup = false;
    }
}

// --------- ���� ���� ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    char cmd;
    while (1) {
        cout << "�Է� ����: cmd 0 0 0" << endl << "�Է�: ";
        cin >> cmd;
        system("cls");
        if (cmd == 'q') break;

        if (cmd == '+') {
            int x, y, z; cin >> x >> y >> z;
            addTop(x, y, z);
        }
        else if (cmd == '-') {
            removeTop();
        }
        else if (cmd == 'e') {
            int x, y, z; cin >> x >> y >> z;
            addBottom(x, y, z);
        }
        else if (cmd == 'd') {
            removeBottom();
        }
        else if (cmd == 'a') {
            cout << countPoints() << "\n";
        }
        else if (cmd == 'b') {
            shiftDown();
        }
        else if (cmd == 'c') {
            clearAll();
        }
        else if (cmd == 'f') {
            sortByDistanceToggle();
        }

        printList();
    }
    return 0;
}
