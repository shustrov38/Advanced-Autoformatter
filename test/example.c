#include <stdio.h>
#define max 100 
void g(){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            f(); 
        } 
    } 
} 
void f(){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
        } 
    } 
} 
int main(){
    int size; 
    scanf("%d", &size); 
    char m[size], sc; 
    scanf("%c", &m[0]); 
    for (int i = 0; size > 0; i++, --size) {
        scanf("%c", &m[i]); 
        if (m[i - 1] == '(' && m[i] == ')') {
            m[i] = 0; 
            m[i - 1] = 0; 
            i -= 2; 
            --a; 
            b = {0, 0, 0}; 
        } 
        for (int i = 0; i < n; ++i) {
            ++i; 
        } 
        f(); 
    } 
    printf("sfsfsf    gdsgdsg    sfs          sf%d", c); 
    if (m[0] == 0) {
        printf(Right); 
    } 
    else {
        printf(Wrong); 
    } 
    c = a && b; 
    return 0; 
} 
