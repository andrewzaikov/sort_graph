#include <stdio.h>
#include <stdlib.h>

int *left, *right;
char *act;
int cnt = 0;

void addLog(int l, int r, char a)
{
    left[cnt] = l;
    right[cnt] = r;
    act[cnt] = a;
    cnt++;
}

void insertionSort(int n, int arr[])
{
    int newElement, location;

    for (int i = 1; i < n; i++)
    {
        location = i - 1;
        newElement = arr[i];
        addLog(-1, i, 'c');
        while (location >= 0 && arr[location] > newElement)
        {
            arr[location + 1] = arr[location];
            addLog(location + 1, location, 'm');

            location = location - 1;
        }
        arr[location + 1] = newElement;
        addLog(location + 1, -1, 'i');
    }
}

int main()
{
    int arr[] = {5, 1, 3, 7, 2, 4, 10, 6, 9, 8};
    int n = sizeof(arr) / sizeof(int);

    left = new int[n * n];
    right = new int[n * n];
    act = new char[n * n];

    insertionSort(n, arr);

    for (int i = 0; i < cnt; i++)
    {
        printf("%d ... %d ... %c\n", left[i], right[i], act[i]);
    }
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }

    delete[] left;
    delete[] right;
    delete[] act;

    return 0;
}