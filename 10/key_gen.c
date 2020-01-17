/*
Author: walid.daboubi@gmail.com
About: Written to solve SANS Holiday Hack 2019 challenge 10
Date: Jan 2020
*/

#include <stdio.h>
#include <windows.h>
#include <strsafe.h>

long multiply(long long x, long long y) {
    return  (long)x * y;
}

int main()
{
    long timestamp = 1578901591;
    int KEY_LENGTH = 8;
    long seed = timestamp;
    long state = 0;
    char* hex_chunk = "";
    char key[16] = "";
    int len;

        for (int i = 0; i < KEY_LENGTH; i++)
        {
            hex_chunk = "";
            state = multiply(seed, 0x343FD);
            state = state + 0x269EC3;
            seed = state;
            state = state >> 0x10;
            state = state & 0x7FFF;
            sprintf(hex_chunk, "%x", state);
            len = strlen(hex_chunk);
            const char* last_two = &hex_chunk[len - 2];
            strcat(key, last_two);
        }
        printf("The seed value is %i\n", timestamp);
        printf("The key value is %s\n", key);
}
