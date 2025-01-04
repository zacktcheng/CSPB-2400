#include <stdio.h>
#include "cs1300bmp.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Filter.h"
#include <omp.h>

using namespace std;

#include "rdtsc.h"

//
// Forward declare the functions
//
Filter * readFilter(string filename);
double applyFilter(Filter *filter, cs1300bmp *input, cs1300bmp *output);

int main(int argc, char **argv) {
  
  if ( argc < 2) {
    fprintf(stderr,"Usage: %s filter inputfile1 inputfile2 .... \n", argv[0]);
  }

  //
  // Convert to C++ strings to simplify manipulation
  //
  string filtername = argv[1];

  //
  // remove any ".filter" in the filtername
  //
  string filterOutputName = filtername;
  string::size_type loc = filterOutputName.find(".filter");
  if (loc != string::npos) {
    //
    // Remove the ".filter" name, which should occur on all the provided filters
    //
    filterOutputName = filtername.substr(0, loc);
  }

  Filter *filter = readFilter(filtername);

  double sum = 0.0;
  int samples = 0;

  for (int inNum = 2; inNum < argc; inNum++) {
    string inputFilename = argv[inNum];
    string outputFilename = "filtered-" + filterOutputName + "-" + inputFilename;
    struct cs1300bmp *input = new struct cs1300bmp;
    struct cs1300bmp *output = new struct cs1300bmp;
    int ok = cs1300bmp_readfile( (char *) inputFilename.c_str(), input);

    if ( ok ) {
      double sample = applyFilter(filter, input, output);
      sum += sample;
      samples++;
      cs1300bmp_writefile((char *) outputFilename.c_str(), output);
    }
    delete input;
    delete output;
  }
  fprintf(stdout, "Average cycles per sample is %f\n", sum / samples);

}

class Filter * readFilter(string filename) {
  ifstream input(filename.c_str());

  if ( ! input.bad() ) {
    int size = 0;
    input >> size;
    Filter *filter = new Filter(size);
    int div;
    input >> div;
    filter -> setDivisor(div);
    for (int i=0; i < size; i++) {
      for (int j=0; j < size; j++) {
	int value;
	input >> value;
	filter -> set(i,j,value);
      }
    }
    return filter;
  } else {
    cerr << "Bad input in readFilter:" << filename << endl;
    exit(-1);
  }
}


double applyFilter(class Filter *filter, cs1300bmp *input, cs1300bmp *output) {
  
  long long cycStart, cycStop;

  cycStart = rdtscll();
  
  // Eliminate Loop Inefficiencies
  // Compute the input dimensions only once and use this value in our test condition
  int width = input->width;
  int height = input->height;
  int filterSize = filter->getSize();
  int *filterArr = filter->get(0, 0); // Use direct memory access to eliminate the function call in the inner loop
  
  // Reduce Procedure Calls
  // Minimize the function calls in the inner loop
  int filterDivisor = filter->getDivisor();
  int heightMinusOne = height - 1;
  int widthMinusOne = width - 1;
  output->width = width;
  output->height = height;

  // Improve spatial locality
  // Row-major order
  // Loop unrolling
  // Apply OpenMP sample "Loop construct: for" notation on https://bisqwit.iki.fi/story/howto/openmp/ 
  #pragma omp parallel num_threads(9)
  {
    #pragma omp for
    for (int row = 1; row < heightMinusOne; row++) {
      for (int col = 1; col < widthMinusOne; col++) {

        // Loop unrolling
        int r = 0;
        int g = 0;
        int b = 0;
        
        // Reduction in strength
        int filterSizeAccu = 0;
        
        for (int i = 0; i < filterSize; i++) {
          int ci = row + i - 1;
          for (int j = 0; j < filterSize; j++) {
            int cj = col + j - 1;
            unsigned char * inputColors = input -> color[ci][cj];
            int filterValue = filterArr[filterSizeAccu + j];
            r += inputColors[0] * filterValue;
            g += inputColors[1] * filterValue;
            b += inputColors[2] * filterValue;
          }
          filterSizeAccu += filterSize;
        }

        r /= filterDivisor;
        g /= filterDivisor;
        b /= filterDivisor;

        if (r < 0) { r = 0; }
        if (r > 255) { r = 255; }
        if (g < 0) { g = 0; }
        if (g > 255) { g = 255; }
        if (b < 0) { b = 0; }
        if (b > 255) { b = 255; }

        unsigned char * outputColors = output -> color[row][col];
        outputColors[0] = r;
        outputColors[1] = g;
        outputColors[2] = b;
      }
    }
  }

  cycStop = rdtscll();
  double diff = cycStop - cycStart;
  double diffPerPixel = diff / (output -> width * output -> height);
  fprintf(stderr, "Took %f cycles to process, or %f cycles per pixel\n", diff, diff / (output -> width * output -> height));
  return diffPerPixel;
}