#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int frames;
    scanf("%d", &frames);

    int *arr = (int *)malloc(frames * sizeof(int));
    for (int i = 0; i < frames; i++)
    {
        int x;
        scanf("%d", &x);
        arr[i] = x;
    }
    int misses = frames;

    int page, idx = 0;
    char c;
    while ((c = getchar()) != EOF && c != '\n')
    {
        scanf("%d", &page);
        if (arr[idx] != page)
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
