#include <windows.h>
#include <stdio.h>

int main() {
    printf("Test 1: Riproducendo success.wav...\n");
    PlaySound("assets/sounds/succes.wav", NULL, SND_FILENAME | SND_SYNC);
    printf("Finito. Premi Enter.\n");
    getchar();
    
    printf("Test 2: Riproducendo error.wav...\n");
    PlaySound("assets/sounds/error.wav", NULL, SND_FILENAME | SND_SYNC);
    printf("Finito. Premi Enter.\n");
    getchar();
    
    printf("Test 3: Success di nuovo...\n");
    PlaySound("assets/sounds/succes.wav", NULL, SND_FILENAME | SND_SYNC);
    printf("Finito. Premi Enter.\n");
    getchar();
    
    printf("Test 4: Error di nuovo...\n");
    PlaySound("assets/sounds/error.wav", NULL, SND_FILENAME | SND_SYNC);
    printf("Finito!\n");
    
    return 0;
}