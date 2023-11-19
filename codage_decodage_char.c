#include <stdio.h>
#include <stdlib.h>

//fonction qui affiche le code binnaire d'un caractère
char* char_en_code_binaire(char c)
{
    int i;
    char *code = malloc(9 * sizeof(char));
    for (i = 7; i >= 0; i--)
    {
        if (c & (1 << i))
        {
            code[7-i]='1';
        }
        else
        {
            code[7-i]='0';
        }
    }
    code[8]='\0';
    return code;
}
//fonction qui transforme un code binaire en caractère
char code_binaire_en_char(char code[])
{
    int i;
    char c = 0;
    for (i = 0; i < 8; i++)
    {
        if (code[i] == '1')
            c |= (1 << (7 - i));
    }
    return c;
}
