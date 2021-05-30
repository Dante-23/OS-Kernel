#include <iostream>
#include <stdlib.h>
using namespace std;

char hex_string[128];

char* substr(char* str, unsigned int start_index, unsigned int end_index){
    unsigned int index = 0;
    while(start_index <= end_index) hex_string[index++] = str[start_index++];
    return hex_string;
}

unsigned char strncmp(char* str, char* param, unsigned int start_index, unsigned int end_index){
    while(start_index <= end_index){
        if(str[start_index] == param[start_index]) start_index++;
        else break;
    }
    return (start_index > end_index);
}

unsigned int mystoi(char* str){
    unsigned int number = 0;
    while(*str != '\0'){
        unsigned int digit = (unsigned int)(*str - 48);
        number = (number * 10) + digit;
        str++;
    }
    return number;
}

int main(){
    cout << mystoi((char*)"1253") << endl;
}







