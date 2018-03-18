#include <iostream>
#include <vector>
#include <algorithm>
#include <future>

using ul = unsigned long;
const int N = 300'000;

void print(const std::vector<int>& table) {
    for (const int &b : table) {
        std::cout << b << " ";
    }
    std::cout << '\n';
}

void fill_table(std::vector<int>& table) {
    for (int i = 0; i < N; ++i) {
        table.push_back((i * 3) % 199);
    }
}

std::vector<int> insertion_sort(std::vector<int>& table, ul left, ul right) {
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

long search(const std::vector<int>& T, int x, long left, long right) {
    while (left < right) {
        long s = (left + right) / 2;
        if (T[s] >= x)
            right = s;
        else
            left = s + 1;
    }

    if (T[left] < x)
        return left + 1;
    return left;
}

void normal_merge(std::vector<int>& T1, long p1, long r1, std::vector<int>& T2,
                  long p2, long r2, std::vector<int>& A, long p3) {
    while (p1 <= r1 && p2 <= r2) {
        if (T1[p1] <= T2[p2]) {
            A[p3++] = T1[p1++];
        } else {
            A[p3++] = T2[p2++];
        }
    }
    if (p1  >  r1) {
        for (long k = p2; k <= r2; ++k) {
            A[p3++] = T2[k];
        }
    } else {
        for (long k = p1; k <= r1; ++k) {
            A[p3++] = T1[k];
        }
    }
}

int copyTtoA(std::vector<int> &T2, long p2, long r2, std::vector<int> &A,
             long p3, long r3) {
    if (r3 - p3 + 1 < 1000) {
        for (long i = p2; i <= r2; ++i) {
            A[p3++] = T2[i];
        }
        return 0;
    }
    long s = (p2 + r2) / 2;

    std::future<int> handle = std::async(copyTtoA, std::ref(T2), p2, s, std::ref(A), p3, p3 + s);
    copyTtoA(T2, s + 1, r2, A, p3 + s + 1, r3);
    handle.get();
    return 0;
}


int merge(std::vector<int>& T1, long p1, long r1, std::vector<int>& T2,
          long p2, long r2, std::vector<int>& A, long p3, long r3) {
    if (r1 - p1 < r2 - p2) {
        merge(T2, p2, r2, T1, p1, r1, A, p3, r3);
        return 0;
    }
    if (r3 - p3 + 1 <= 1000) {
        normal_merge(T1, p1, r1, T2, p2, r2, A, p3);
        return 0;
    }

    long q1, q2, q3;
    q1 = (p1 + r1) / 2;
    int x = T1[q1];
    q2 = search(T2, x, p2, r2);
    q3 = p3 + (q1 - p1) + (q2 - p2);

    A[q3] = x;

    if (q2 > r2) {
        copyTtoA(T1, q1 + 1, r1, A, q3 + 1, r3);
        merge(T1, p1, q1 - 1, T2, p2, r2, A, p3, q3 - 1);
        return 0;
    }

    if (q2 == 0) {
        copyTtoA(T1, p1, q1 - 1, A, p1, q3 - 1);
        merge(T1, q1 + 1, r1, T2, p2, r2, A, q3 + 1, r3);
        return 0;
    }

    std::future<int> handle = std::async(merge, std::ref(T1), p1, q1 - 1,
                                         std::ref(T2), p2, q2 - 1, std::ref(A), p3, q3 -1);
    merge(T1, q1+1, r1, T2, q2, r2, A, q3+1, r3);
    handle.get();

    return 0;
}

std::vector<int> merge_sort(std::vector<int>& table, ul left, ul right) {
    if (right - left + 1 <= 14) {
        std::vector<int> A = insertion_sort(table, left, right);
        return A;
    }

    ul s = (left + right) / 2;
    std::future<std::vector<int>> TT = std::async(merge_sort, std::ref(table), left, s);
    std::vector<int> T2 = merge_sort(table, s + 1, right);
    std::vector<int> T1 = TT.get();
    std::vector<int> A(T1.size() + T2.size());
    merge(T1, 0, T1.size() - 1, T2, 0, T2.size() - 1, A, 0, A.size() - 1);

    return A;
}

void sort(std::vector<int>& table) {
    std::vector<int> A = merge_sort(table, 0, table.size() - 1);
    table = std::move(A);
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


int main() {
    std::vector<int> table;
    fill_table(table);
  //  print(table);
    ul size1 = table.size();
   // std::vector<int> sorted_table = sort(table);
    sort(table);
    //print(sorted_table);
    ul size2 = table.size();
   // print(table);
    if (size1 == size2)
        std::cout << "sizes ok" << '\n';
    if (isSorted(table)) {
        std::cout << "sorted" << '\n';
    }
    return 0;
}