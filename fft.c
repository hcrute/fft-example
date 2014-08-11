//author Henry Crute
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <fftw3.h>

#define SWAP(a,b)tempr=(a);(a)=(b);(b)=tempr
//tempr is a variable from our FFT function

#define SAMPLE_SIZE 32


//prints array of numbers with specified length
print_array(double *array, int length) {
   int i;
   for (i = 0; i < length; i++) {
      fprintf(stdout, "%f\n", array[i]);
   }
}

print_fftw_complex(fftw_complex *array, int length) {
   int i;
   for (i = 0; i < length; i++) {
      fprintf(stdout, "%f\n", array[i][0]);
      fprintf(stdout, "%f\n", array[i][1]);
   }
}

//retreives data from file and inserts into buffer array
//returns size of array
//arguments, buffer array, and opened input file
int get_array(double *buffer, FILE *in) {
   uint8_t inputString[32];
   int i = 0;
	while(fgets(inputString, sizeof(inputString), in)) {
      //replaces new line with null
      int length = strlen(inputString);
      if (length < 2) {
         continue;
      }
		inputString[length - 2] = '\0';
      //printf("doesn't fail here %s\n", inputString);
      buffer[i] = strtol(inputString, NULL, 10);
		//fprintf(stdout, "%i\n", number);
      i++;
	}
   return i;
}

int main(int argc, char *argv[]) {
   FILE *input;
   double *in = fftw_alloc_real(SAMPLE_SIZE);
   fftw_complex *out = fftw_alloc_complex(SAMPLE_SIZE / 2 + 1);
   if (argc > 2) {
		printf("Usage: fft input\r\n");
	} else {
	  input = fopen(argv[1], "r");
	  if (input == NULL) {
	    printf("Couldn't open %s for reading.\r\n", argv[1]);
		return(1);
	  }
	}
   //absurd number of data points for buffer array
   double buffer[262144 * 2];
   int size = get_array(buffer, input);
   //print_array(buffer, size);
   

   double sample[SAMPLE_SIZE];
   int i;
   for (i = 0; i < SAMPLE_SIZE; i++) {
      sample[i] = buffer[i];
   }
   //print_array(in, SAMPLE_SIZE);
   print_array(sample, SAMPLE_SIZE);
   fprintf(stdout, "%i numbers\n", size);
   //real to complex plan
   fftw_plan plan = fftw_plan_dft_r2c_1d(SAMPLE_SIZE, in, out,
                           FFTW_MEASURE | FFTW_PRESERVE_INPUT);
   for (i = 0; i < SAMPLE_SIZE; i++) {
      in[i] = buffer[i];
   }
   fftw_execute(plan);
   print_array(in, SAMPLE_SIZE);
   printf("\n");
   print_fftw_complex(out, SAMPLE_SIZE / 2 + 1);
   //destructors
   fftw_destroy_plan(plan);
   fftw_free(in);
   fftw_free(out);
   fftw_cleanup();
   fclose(input);
   return(0);
}
