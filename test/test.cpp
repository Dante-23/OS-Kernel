#include <iostream>
#include <stdlib.h>
using namespace std;

char buffer[32];

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
    while(number > 0){
        int value = number % 10;
        buffer[index++] = (char)(value + 48);
        number = number / 10;
    }
    int size = strlen(buffer);
    buffer[index++] = '\0';
    return buffer;
}

int main(){
    int number = 257;
    char* ptr = int_to_string(number);
    while(*ptr != 0){
        cout << *ptr;
        ptr++;
    }
    cout << endl;
}

