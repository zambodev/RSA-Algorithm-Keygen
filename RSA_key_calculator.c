#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

mpz_t number;
unsigned long setsNumber = 15;

void generateRandomNumber() {
    int isTrue = 0;
    gmp_randstate_t randomState;
    unsigned long seed;

    // Randomize system time
    sleep(3);
    seed = time(NULL);
    mpz_inits(number, NULL);
    
    // Generate random number
    gmp_randinit_mt(randomState);
    gmp_randseed_ui(randomState, seed);
    mpz_urandomb(number, randomState, 1024);
    
    // Check if the number is prime or probably prime
    while(isTrue == 0) {
        // Calculate the next primary number
        mpz_nextprime(number, number);
        // Prime check (the number of reps must be between 15 and 50)
        isTrue = mpz_probab_prime_p(number, setsNumber); 
    }

}

int main(int argc, char* argv[]) {

    // Declare output file
    FILE* outFile;
    int fileIsFound = 0;
    
    if(argc > 2) {
        
        for(int i=0; i < argc; i++) {
        
            if(strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) {
                
                // Create the output file
                outFile = fopen(argv[i+1], "w");
                fileIsFound = 1;
            } else if(strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--check-sets") == 0) {
                setsNumber = atoi(argv[i+1]);
            }
    
        }
    }


    /*
    *  Declare mpz intgers:
    *  "p" and "q" are big prime numbers
    *  "n" and "e" are the public keys
    *  "d" is the private key
    *  "m" message
    *  "c" ecrypted message
    *  "phi" Euler totient function
    */
    mpz_t uno, p, q, n, d, phi, e, m, c;

    // Initialize variables to NULL
    mpz_inits(uno, p, q, n, d, phi, e, m, c, number, NULL);

    // Assign unsigned int value (1) to mpz intger
    mpz_init_set_ui(uno, 1); 

    // Assing primary number value
    generateRandomNumber();
    mpz_set(p, number); 
    generateRandomNumber();
    mpz_set(q, number);

    mpz_clear(number);

    // Assign message value
    mpz_init_set_str(m, "5", 10);

    // n = p * q
    mpz_mul(n, p, q);

    // Print "n" value
    printf("\n\n%s", "n: ");
    mpz_out_str(stdout, 10, n);

    // Subtract 1 from p and q
    mpz_sub(p, p, uno);
    mpz_sub(q, q, uno);

    // phi = p * q
    mpz_mul(phi, p, q);

    mpz_clears(p, q, NULL);

    // Assign "e" value
    mpz_init_set_str(e, "65537", 10);

    // Print "e" val
    printf("\n\n%s", "e: ");
    mpz_out_str(stdout, 10, e);

    // Calculate "d" val
    mpz_invert(d, e, phi);

    mpz_clear(phi);

    // Print d val
    printf("\n\n%s", "d: ");
    mpz_out_str(stdout, 10, d);

    // Encrypt data -> c = m^e mod n
    mpz_powm(c, m, e, n);

    // Print encrypted message
    printf("\n\n%s", "c: ");
    mpz_out_str(stdout, 10, c);

    // Decrypt data -> m = c^d mod n
    mpz_powm(c, c, d, n);

    // Print decrypted message
    printf("\n\n%s", "c: ");
    mpz_out_str(stdout, 10, c);

    puts("\n");

    if(fileIsFound) {
        // Write "n" into file
        fprintf(outFile, "n: \n");
        mpz_out_str(outFile, 10, n);
        fprintf(outFile, "\n");
    
        // Write "e" into file
        fprintf(outFile, "e: \n");
        mpz_out_str(outFile, 10, e);
        fprintf(outFile, "\n");
    
        // Write data into file
        fprintf(outFile, "d: \n");
        mpz_out_str(outFile, 10, d);
        fprintf(outFile, "\n");
        
        // Close file
        fclose(outFile);
     
    }

    mpz_clears(uno, n, d, e, m, c, NULL);

    return 0;
}