#include <iostream>
#include <stdlib.h>
using namespace std;

char int_string[512];

int strlen(char s[]){
    char *ptr = s;
    while(*ptr != '\0') ptr++;
    return ptr - s;
}

void reverse(char s[], int size){
    unsigned int i = 0, j = size - 1;
    while(i < j){
        char ch = s[i];
        s[i] = s[j];
        s[j] = ch;
        i++;
        j--;
    }
}

char* int_to_string(int number){
    int index = 0;
    if(number < 0){
        int_string[index++] = '-';
        number = -number;
    }
    while(number > 0){
        int value = number % 10;
        int_string[index++] = (char)(value + 48);
        number = number / 10;
    }
    int size = strlen(int_string);
    reverse(int_string, size);
    int_string[index++] = '\0';
    return int_string;
}

int main(){
    cout << int_to_string(223) << endl;
    // unsigned int j = 0xffffffff;
    // for(unsigned int i = 0; i < 32; i++){
    //     // j += (1 << i);
    // }
    // cout << j << endl;
}
