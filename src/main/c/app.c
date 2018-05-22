#include <time.h>
#include <time.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    time_t t;

    memset(&t, 0, sizeof(t));

    printf("%s\n", ctime(&t));

    return 0;
}
