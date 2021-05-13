void f(int x) {
    if (x > 15) {
        return; 
    } 
    f(x + 16); 
} 
void g(){
    f(5); 
} 
int main(){
    g(); 
    f(- 5); 
    g(); 
    int i = 0; 
    while (i < 5) {
    } 
    if (0) {
        i++; 
    } 
} 
