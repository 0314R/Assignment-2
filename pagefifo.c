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

    // arr keeps track of the content of the frames
    int *arr = (int *)malloc(frames * sizeof(int));

    for (int i = 0; i < frames; i++)
    {
        arr[i] = 0;
    }

    // page reads the input, idx keeps track of which frame should be compared/updated
    // time keeps track of the number of updates over all frames, misses = #page faults
    int page, idx = 0, time = frames, misses = 0;
    // c makes sure the newline is read at the end
    char c;
    // flag makes sure the loop doesn't end immediately, since the first char read is a newline
    int flag = 0;

    while (((c = getchar()) != EOF && c != '\n') || (flag == 0))
    {
        flag = 1;
        // scan new page
        scanf("%d", &page);
        // if page fault: increase misses, replace page and update index
        if (find(arr, frames, page) == -1)
        {
            misses++;
            arr[idx] = page;
            idx = (idx + 1) % frames;
        }
    }

    printf("%d\n", misses);

    free(arr);

    return 0;
}
