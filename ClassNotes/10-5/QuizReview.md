# Class Notes - 10/5

## Weekly Update
- No lab this week.
- Time to work on the project and continue with the next project.
- Test on Monday.

## Midterm
The midterm has two parts:
1. Paper quiz
2. Coding project completed at home

## Quiz Reflection
- Quizzes went reasonably well, but some students had trouble understanding pointers.
- Pointers are just addresses.

## Quiz Review

### Question 1
This question is asking you to take a pointer to an array/list and the number of elements, reverse the list, and then update the pointer so it points to the new reversed array.

In C, a function to do this could look like this:

Basic Solution
```c
void reverse_array(int *arr, int total) {
    for (int i = 0; i < total / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[total - 1 - i];
        arr[total - 1 - i] = temp;
    }
}
```

Best Solution 
```c
void reverse_array(int *start, int *end) {
    while (start < end) {
        int temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;
    }
}
```

