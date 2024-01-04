#pragma once

struct Log {
    int left;
    int right;
    char act;
};

void initLog(int n);

void clearLog();

int getLogSize();

Log* getLogArr();

void insertionSort(int n, int arr[]);
