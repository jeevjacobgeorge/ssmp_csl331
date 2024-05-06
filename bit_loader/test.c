#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int bits[12];
char bit_hex[] = "E00";
void conv_to_bin(int b)
{
    for (int i = 11; i >= 0; i--)
    {
        bits[i] = b % 2;
        b = b / 2;
    }
}
int main()
{
    int b;

    b = strtol(bit_hex,NULL,16);
    conv_to_bin(b);
    for(int i = 0; i < 12; i++)
        printf("%d",bits[i]);
    return 0;

}