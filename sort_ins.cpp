#include <stdio.h>
#include <stdlib.h>
#include "sort_ins.h"


Log *logs = NULL;
int cnt = 0;

void initLog(int n)
{
    if (NULL != logs) {
        clearLog();
    }
    logs = new Log[n * n];
}

void clearLog()
{
    delete[] logs;
    logs = NULL;
    cnt = 0;
}

Log* getLogArr() 
{
    return logs;
}

int getLogSize()
{
    return cnt;
}

void addLog(int l, int r, char a)
{
    Log rec;

    rec.left = l;
    rec.right = r;
    rec.act = a;

    logs[cnt++] = rec;;
}

void insertionSort(int n, int arr[])
{
    int newElement, location;

    for (int i = 1; i < n; i++)
    {
        location = i - 1;
        newElement = arr[i];
        addLog(-1, i, 'c');     // Choose [i] element
        while (location >= 0 && arr[location] > newElement)
        {
            arr[location + 1] = arr[location];
            addLog(location + 1, location, 'm');    // Move [location] to [location+1]

            location = location - 1;
        }
        arr[location + 1] = newElement;
        addLog(location + 1, -1, 'i');  // Insert chosen into [location+1]
    }
}


/* 
int main()
{
    int arr[] = {5, 1, 3, 7, 2, 4, 10, 6, 9, 8};
    int n = sizeof(arr) / sizeof(int);

    initLog(n);

    insertionSort(n, arr);

    printf("l ... r ... a\n");
    for (int i = 0; i < cnt; i++)
    {
        printf("%d ... %d ... %c\n", logs[i].left, logs[i].right, logs[i].act);
    }
    printf("arr = {");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("}");

    clearLog();

    return 0;
} */
