For both algorithms we chose to have an array representing the frames.
Then a while loop continues until a newline or EOF is read.
In each iteration the next number is read, after that we check whether
that number is currently in one of the frames. After that each
program deals with that according to the corresponding algorithm.

To check whether the page is already in one of the frames, the function
int find(int *arr, int len, int page) is used. This function checks whether
a page is already in one of the frames and returns the index of the array
where it is. If it is not in the array, it returns -1.

For this part of the lab, it was not necessary to use more complicated 
data structures than just an array.