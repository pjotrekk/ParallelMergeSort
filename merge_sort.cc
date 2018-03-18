#include <iostream>
#include <algorithm>

using ul = unsigned long;
const int N = 100'000'000;

void print(const std::vector<int>& table) {
    for (const int &b : table) {
        std::cout << b << " ";
    }
    std::cout << '\n';
}

void fill_table(std::vector<int>& table) {
    for (int i = 0; i < N; ++i) {
        table.push_back((i * 3) % 13);
    }
}

std::vector<int> insertion_sort(std::vector<int>& table, long left, long right) {
    std::vector<int> A(table.begin() + left, table.begin() + right + 1);
    for (int i = 0; i < A.size(); ++i) {
        int j = i;
        while (j > 0 && A[j-1] > A[j]) {
            std::swap(A[j-1], A[j]);
            --j;
        }
    }
    return A;
}

bool isSorted(const std::vector<int>& table) {
    int i = 1;
    while (i < table.size()) {
        if (table[i-1] > table[i])
            return false;
        ++i;
    }
    return true;
}

std::vector<int> merge(std::vector<int>& T1, long p1, long r1, std::vector<int>& T2, long p2, long r2) {
    std::vector<int> A(T1.size() + T2.size());
    ul i = 0;
    while (p1 <= r1 && p2 <= r2) {
        if (T1[p1] <= T2[p2]) {
            A[i++] = T1[p1++];
        } else {
            A[i++] = T2[p2++];
        }
    }
    if (p1 > r1) {
        for (long k = p2; k <= r2; ++k) {
            A[i++] = T2[k];
        }
    }

    if (p2 > r2) {
        for (long k = p1; k <= r1; ++k) {
            A[i++] = T1[k];
        }
    }
    return A;
}

std::vector<int> merge_sort(std::vector<int>& table, long left, long right) {
    if (right - left <= 14) {
        std::vector<int> A = insertion_sort(table, left, right);
        return A;
    }
    long s = (left + right) / 2;
    std::vector<int> T1 = merge_sort(table, left, s);
    std::vector<int> T2 = merge_sort(table, s + 1, right);
    std::vector<int> A = merge(T1, 0, T1.size() - 1, T2, 0, T2.size() - 1);
    return A;
}


void sort(std::vector<int>& table) {
    std::vector<int> A = merge_sort(table, 0, table.size() - 1);
    table = std::move(A);
}


int main() {
    std::vector<int> table;
    fill_table(table);
    ul size1 = table.size();
    // std::vector<int> sorted_table = sort(table);
    sort(table);
    //print(sorted_table);
    ul size2 = table.size();
    if (size1 == size2)
        std::cout << "sizes ok" << '\n';
    if (isSorted(table)) {
        std::cout << "sorted" << '\n';
    }
    return 0;
}