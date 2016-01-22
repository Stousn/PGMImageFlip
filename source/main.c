#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flip.h"
#include "img.h"
#include "main.h"

/**
*  @brief Command line parsing, File handles
*  @param Usage: flip <Path of input file> <Path of output file>
*/
int main(int argc, char const *argv[]) {
  //Variables
  PbmImage* img;
  int error = RET_FLIP_OK;

  //Checks if the amount of given arguments is correct
  if (argc == 3){
    int* errorp = &error;

    //Trys to open input file
    FILE* source = fopen(argv[1], "rb");
    if(!source) {
      perror("ERROR: Opening input file failed");
      return ARGC_NO_SUCH_FILE;
    }

    //Loads PBM image from source file.
    img = pbm_image_load_from_stream(source, errorp);

    //After reading, the source file is closed
    fclose(source);

    //Checks if loading went wrong.
    if(img == NULL){
      perror("ERROR: Loading PBM image failed. Check Error code for further information\n");
      return error;
    }

    //Fliping of image
    error = pbm_image_flip(img);
    if(error != 0){
      perror("ERROR: Flipping PBM image failed. Check Error code for further information\n");
      return error;
    }

    //Writing flipped image into output file
    FILE* output = fopen(argv[2], "w+b");
    error = pbm_image_write_to_stream(img, output);
    if(error != 0){
      perror("ERROR: Writing PBM image failed. Check Error code for further information\n");
      return error;
    }

    //Closing the output file
    fclose(output);
  }
  else{
    //Wrong arguments
    printf("Usage: flip <Path of input file> <Path of output file>\n");
    return ARGC_INVALID_ARGUMENT;
  }
  //Frees all allocated memory
  pbm_image_free(img);

  //Returns error code or 0 if everything went fine
  return error;
}
