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

    // arrays for the frames and the R bit
    int *arr = (int *)malloc(frames * sizeof(int));
    int *rbit = (int *)malloc(frames * sizeof(int));

    // set both to zero
    for (int i = 0; i < frames; i++)
    {
        arr[i] = 0;
        rbit[i] = 0;
    }

    int page, idx = 0, misses = 0;
    char c;
    // flag makes sure the loop doesn't end immediately, since the first char read is a newline
    int flag = 0;

    while (((c = getchar()) != EOF && c != '\n') || (flag == 0))
    {
        flag = 1;
        // scan new page and check if it is already in the array
        scanf("%d", &page);
        int findIdx = find(arr, frames, page);

        // if papge fault: increase misses, replace the first page that has R bit = 0
        // and update the idx accordingly
        if (findIdx == -1)
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
        // if no page fault, just set R bit to 1
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
