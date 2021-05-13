void func2(){
    int n = 10; 
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                // pass 
            } 
        } 
    } 
} 
void func1(){
    int n = 10; 
    for (int i = 0; i < n; ++i) {
        func2(); 
    } 
} 
int main(){
    int n = 10; 
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                // pass 
            } 
        } 
        func1(); 
    } 
} 
