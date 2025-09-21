#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
/*
 * ========================================================
 * ReLU on large vector (bit hack).
 * ReLU = max(0,x)
 * Sets all negative nums to 0 and leaves positives values.  
 * ========================================================
 */
const int INT_BITS = sizeof(int)*8 - 1; // num of bits in a int 

typedef struct Vector {
    float* data;
    int size;
} vec_t;

// function pointer type, arguments match func arguments
typedef void (*func_ptr)(const vec_t*, vec_t*); 

void relu(const vec_t* vec_in, vec_t* vec_out)
{
    /*
     * Take the addr of vec[i] turn it into a int addr, then dereference,
     * tmp is now a int with the float representation and sign if negative.
     * Apply the mask: tmp = tmp & ~(0x100000000) --> if negative then tmp = 2^32 else no changes
     * Now we do the reverse of the first operation, if negative 2^32 as int will give a 0.0 
     * represented as a float, if positive the original float value is returned.
     */

    // using memcpy here x2 proc time
    for(int i = 0; i < vec_in->size; i++){
        //int tmp;
        // memcpy(&tmp, &vec_in->data[i], sizeof(int));
        int tmp = (*((int *) &vec_in->data[i]));
        tmp &= ~(tmp >> INT_BITS);
        // float f;
        // memcpy(&f, &tmp, sizeof(float));
        vec_out->data[i] = *((float*) &tmp);
        // vec_out->data[i] = f;
    }
}

int main(void) 
{
    vec_t vector_input;
    vec_t vector_output;
    func_ptr f = relu;
    float tab[] = {1.1f,-2.2f,3.3f,4.4f,-5.5f,-6.6f,7.7f,87.1f};
    vector_input.data = tab;
    vector_input.size = 8;
    vector_output.size = 8;

    clock_t begin = clock();

    vector_output.data = malloc(vector_input.size * sizeof(float));
    f(&vector_input, &vector_output);

    clock_t end = clock();
    float time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
    printf("%f \n", time_spent);
    printf("Vector: \n");

    for(int i = 0; i < vector_output.size; i++){
        printf("%f \n", vector_output.data[i]);
    }

    free(vector_output.data);
}    


