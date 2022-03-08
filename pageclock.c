#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int find(int *arr, int len, int page)
{
    for (int i = 0; i < len; i++)
    {
        if (arr[i] == page)
            return i;
    }
    return -1;
}

int main(int argc, char *argv[])
{
    int frames;
    scanf("%d", &frames);

    int *arr = (int *)malloc(frames * sizeof(int));
    int *rbit = (int *)malloc(frames * sizeof(int));

    // char x = getchar(); // gets spaces and newlines

    for (int i = 0; i < frames; i++)
    {
        //     int x;
        //     scanf("%d", &x);
        arr[i] = 0;
        rbit[i] = 0;
    }
    int misses = 0;

    int page, idx = 0;
    char c; // = getchar(); // ignore newline
    int flag = 0;

    while (((c = getchar()) != EOF && c != '\n') || (flag == 0))
    {
        flag = 1;
        scanf("%d", &page);
        int findIdx = find(arr, frames, page);

        if (findIdx == -1) // page fault
        {
            misses++;
            while (rbit[idx] == 1)
            {
                rbit[idx] = 0;
                idx = (idx + 1) % frames;
            }
            arr[idx] = page;
            idx = (idx + 1) % frames;
        }
        else
        {
            rbit[findIdx] = 1;
        }
    }

    printf("%d\n", misses);

    free(rbit);
    free(arr);

    return 0;
}
