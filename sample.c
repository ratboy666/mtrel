#include <stdio.h>

int main(int ac, char **av) {
    int *p;
    int n;
/* just a bit of magic -- the previous program that
 * exited leaves the exit code at 0x7e. hitech C
 * used 0x80, but that is overwritten by ccp. using
 * 0x7e actually makes this useful.
 */
    p = (void *)0x7e;
    n = *p; 
    printf("hello, world %d\n", n);
    return 5;
}
