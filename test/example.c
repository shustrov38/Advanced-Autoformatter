int badFuncName(int z) {
    if (z > 50) {
        return 1; 
    } 
    return 2; 
    // return badFuncName(z + 10); 
} 
void GoodFuncName(){
    int x = 0; 
    for (int i = 0; i < 100; ) {
        ++i; 
        if (i % 3) {
            x = badFuncName(i); 
        } 
    } 
} 
void a(){
    c(); 
} 
void b(){
    a(); 
} 
void c(){
    b(); 
} 
void uselessFunc(){
    return; 
} 
int main(){
    int BadVar = 6; 
    int unusedVar = 6; 
    int NotInitedVar; 
    for (int i = 0; i < BadVar; ++i) {
        GoodFuncName(); 
    } 
    NotInitedVar++; 
    a(); 
    while (sqrt (4) == 2) {
        int z = 0; 
        z += 50; 
        // break; 
    } 
} 
