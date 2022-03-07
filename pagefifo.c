#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int min(int *arr, int len)
{
    int low = arr[0];
    int idx = 0;
    for (int i = 1; i < len; i++)
    {
        if (low > arr[i])
        {
            low = arr[i];
            idx = i;
        }
    }
    return idx;
}

int main(int argc, char *argv[])
{
    int frames;
    scanf("%d", &frames);

    // arr keeps track of the content of the frames
    int *arr = (int *)malloc(frames * sizeof(int));
    // times keeps track of when each frame was last changed
    int *times = (int *)malloc(frames * sizeof(int));

    // initialize such that all frames have a page
    // #misses = #frames then
    for (int i = 0; i < frames; i++)
    {
        int x;
        scanf("%d", &x);
        arr[i] = x;
        times[i] = i;
    }
    int misses = frames;

    // page reads the input, idx keeps track of which frame should be compared/updated
    // time keeps track of the number of updates over all frames
    int page, idx = 0, time = frames;
    // c makes sure the newline is read at the end
    char c;
    while ((c = getchar()) != EOF && c != '\n')
    {
        scanf("%d", &page);
        if (arr[idx] != page)
        {
            misses++;
            arr[idx] = page;
            time++;
            times[idx] = time;
            idx = min(times, frames);
        }
    }

    printf("%d\n", misses);

    free(arr);
    free(times);

    return 0;
}
