#include < stdio . h > 
#include < math . h > 
int main(){
   int n , k; 
   scanf(" %d " , & n); 
   int m[n]; 
   for (int i = 0; i < n; ++i) {
      scanf(" %d " , & m[i); 
   } 
   k = 0; 
   for (int i = 1; i < n - 1; ++i) {
      if (m[i - 1]> m[i]&& m[i + 1]> m[i) {
         k += 1; 
      } 
   } 
   if (m[0]< m[1) {
      k += 1; 
   } 
   if (m[n - 1]< m[n - 2) {
      k += 1; 
   } 
   printf(" %d " , k); 
   return 0; 
