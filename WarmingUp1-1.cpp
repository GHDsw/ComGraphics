#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

const int SIZE = 4;

struct Matrix {
    int data[SIZE][SIZE];
    int backup[SIZE][SIZE];  // e, f, ���� ��ɾ�� ������� �������
    bool modified = false;   // ���� ���� �÷���
};

// ���� �ʱ�ȭ
void randomMatrix(Matrix& m) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            m.data[i][j] = rand() % 9 + 1; // 1~9
            m.backup[i][j] = m.data[i][j];
        }
    }
    m.modified = false;
}

// ���
void printMatrix(const Matrix& m) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            cout << setw(3) <<' ' << m.data[i][j];
        cout << "\n";
    }
    cout << endl;
}

// ����
Matrix addMatrix(const Matrix& a, const Matrix& b) {
    Matrix r;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            r.data[i][j] = a.data[i][j] + b.data[i][j];
    return r;
}

// ����
Matrix subMatrix(const Matrix& a, const Matrix& b) {
    Matrix r;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            r.data[i][j] = a.data[i][j] - b.data[i][j];
    return r;
}

// ����
Matrix mulMatrix(const Matrix& a, const Matrix& b) {
    Matrix r{};
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            r.data[i][j] = 0;
            for (int k = 0; k < SIZE; k++)
                r.data[i][j] += a.data[i][k] * b.data[k][j];
        }
    return r;
}

// ��Ľ� (��� ���)
int determinant(int mat[SIZE][SIZE], int n) {
    if (n == 1) return mat[0][0];
    if (n == 2) return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
    int det = 0;
    int temp[SIZE][SIZE];
    int sign = 1;
    for (int f = 0; f < n; f++) {
        int subi = 0;
        for (int i = 1; i < n; i++) {
            int subj = 0;
            for (int j = 0; j < n; j++) {
                if (j == f) continue;
                temp[subi][subj] = mat[i][j];
                subj++;
            }
            subi++;
        }
        det += sign * mat[0][f] * determinant(temp, n - 1);
        sign = -sign;
    }
    return det;
}

// ��ġ���
Matrix transpose(const Matrix& m) {
    Matrix r;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            r.data[i][j] = m.data[j][i];
    return r;
}

// �ּҰ� ����/�ǵ�����
void applyMin(Matrix& m) {
    if (!m.modified) {
        int minVal = m.data[0][0];
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                if (m.data[i][j] < minVal) minVal = m.data[i][j];
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                m.data[i][j] -= minVal;
        m.modified = true;
    }
    else { // �ǵ�����
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                m.data[i][j] = m.backup[i][j];
        m.modified = false;
    }
}

// �ִ밪 ���ϱ�/�ǵ�����
void applyMax(Matrix& m) {
    if (!m.modified) {
        int maxVal = m.data[0][0];
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                if (m.data[i][j] > maxVal) maxVal = m.data[i][j];
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                m.data[i][j] += maxVal;
        m.modified = true;
    }
    else {
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                m.data[i][j] = m.backup[i][j];
        m.modified = false;
    }
}

// ��� ����/�ǵ�����
void applyMultiple(Matrix& m, int n) {
    if (!m.modified) {
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++) {
                if (m.data[i][j] % n != 0) m.data[i][j] = 0;
            }
        m.modified = true;
    }
    else {
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                m.data[i][j] = m.backup[i][j];
        m.modified = false;
    }
}

int main() {
    srand((unsigned)time(NULL));
    Matrix A, B;
    randomMatrix(A);
    randomMatrix(B);

    cout << "�ʱ� ��� A:\n"; printMatrix(A);
    cout << "�ʱ� ��� B:\n"; printMatrix(B);

    char cmd;
    while (true) {
        cout << "��ɾ� �Է� (m,a,d,r,t,e,f,1~9,s,q): ";
        cin >> cmd;
        system("cls");

        cout << "�ʱ� ��� A:\n"; printMatrix(A);
        cout << "�ʱ� ��� B:\n"; printMatrix(B);

        if (cmd == 'q') break;
        else if (cmd == 'a') { cout << "A+B:\n"; printMatrix(addMatrix(A, B)); }
        else if (cmd == 'd') { cout << "A-B:\n"; printMatrix(subMatrix(A, B)); }
        else if (cmd == 'm') { cout << "A*B:\n"; printMatrix(mulMatrix(A, B)); }
        else if (cmd == 'r') {
            cout << "det(A) = " << determinant(A.data, SIZE) << "\n";
            cout << "det(B) = " << determinant(B.data, SIZE) << "\n";
        }
        else if (cmd == 't') {
            cout << "A^T:\n"; printMatrix(transpose(A));
            cout << "det(A^T) = " << determinant(transpose(A).data, SIZE) << "\n";
            cout << "B^T:\n"; printMatrix(transpose(B));
            cout << "det(B^T) = " << determinant(transpose(B).data, SIZE) << "\n";
        }
        else if (cmd == 'e') {
            applyMin(A); applyMin(B);
            cout << "����� A:\n"; printMatrix(A);
            cout << "����� B:\n"; printMatrix(B);
        }
        else if (cmd == 'f') {
            applyMax(A); applyMax(B);
            cout << "����� A:\n"; printMatrix(A);
            cout << "����� B:\n"; printMatrix(B);
        }
        else if (cmd >= '1' && cmd <= '9') {
            int n = cmd - '0';
            applyMultiple(A, n); applyMultiple(B, n);
            cout << n << "�� ����� ���� A:\n"; printMatrix(A);
            cout << n << "�� ����� ���� B:\n"; printMatrix(B);
        }
        else if (cmd == 's') {
            randomMatrix(A); randomMatrix(B);
            cout << "���ο� A:\n"; printMatrix(A);
            cout << "���ο� B:\n"; printMatrix(B);
        }
    }
    return 0;
}
