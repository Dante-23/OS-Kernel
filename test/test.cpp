#include <iostream>
using namespace std;
// address must be 16 bits
#define get_low_8(address) (unsigned short)((address) & 0xff)
#define get_high_8(address) (unsigned short)(((address) >> 8) & 0xff)

#define get_low_16(address) (unsigned short)((address) & 0xFFFF)
#define get_high_16(address) (unsigned short)(((address) >> 16) & 0xFFFF)

void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';
}

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

void unsignedhex_to_string(int n, char str[]){

}

int main(){
    int n = 0x12345;
    int div = 0x10;
    cout << n / div << endl;
}

