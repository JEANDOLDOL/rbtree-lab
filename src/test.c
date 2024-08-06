#include <stdio.h>

int main()
{
    char * str[2];
    str[0] = "hello!";
    str[1] = "jungler";

    printf("1. %s\n",str[0]+1); // %s는 문자열을 출력. e부터의 문자열을 출력.
    printf("2. %s\n",(str + 1)[0]+2); // 마찬가지로 n부터의 문자열을 출력.
}