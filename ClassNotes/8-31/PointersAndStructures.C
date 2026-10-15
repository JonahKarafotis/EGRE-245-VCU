#include <stdio.h>
struct date { int day, month, year; };
void f1(struct date *d1, struct date *d2) {
(*d1).day = 20; (*d1).month = 6; (*d1).year = 1974;
(*d2).day = 20; (*d2).month = 6; (*d2).year = 1974;
}
struct date f2(int d, int m, int y) {
struct date temp;
temp.day = d; temp.month = m; temp.year = y;
return temp;
}
int main() {
struct date day1, day2, day3;
day1.day = 5; day1.month = 9; day1.year = 2012;
day2.day = 5; day2.month = 9; day2.year = 2012;
f1(&day1,&day2); 
// When using a function that takes a pointer to a structure as an argument, you need to use the & operator to pass the address of the
// structure variable to the function. This allows the function to modify the original structure variable directly, rather than working with a copy of it. 
// In this case, f1 modifies the values of day1 and day2 directly through their pointers.
printf("%d-%d-%d\n",day1.month,day1.day,day1.year);
printf("%d-%d-%d\n",day2.month,day2.day,day2.year);
day3 = f2(12,4,2018);
printf("%d-%d-%d\n",day3.month,day3.day,day3.year);
}
