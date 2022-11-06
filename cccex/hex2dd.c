#include <stdio.h>
#include <math.h>
#include <stdlib.h> // for strtol

int main(int argc, char **argv)
{

    if (argc == 0)
    {
        return -1;
    }

    int hex_nr = strtol(argv[1], NULL, 10);
    int index_power = 0;
    int multiplied = 16;
    int to_multiply = 0;
    int show_number = 0;

    while (hex_nr > 0)
    {
        to_multiply = pow(16, index_power);
        show_number += (to_multiply * (hex_nr % 10));
        hex_nr = hex_nr / 10;
        index_power++;
    }
    printf("%d", show_number);
}
