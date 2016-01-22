#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flip.h"
#include "img.h"


void pbm_image_free(PbmImage* img){
  free(img->data);
  free(img);
}

//Error: RET_EOF, RET_PBM_OK, RET_UNSUPPORTED_FILE_FORMAT, RET_INVALID_FORMAT
PbmImage* pbm_image_load_from_stream(FILE* stream, int* error){
  char typerow[500];
  char size[500];
  char bits[500];
  size[0] = '#';

  //gets type
  fgets(typerow, 500, stream);
  char type[3];
  strncpy(type, typerow, 2);
  type[2] = '\0';
  printf("Type: %s\n", type);

  //gets comment first and size
  while(size[0] == '#'){
    printf("Comment: %s\n", size);
    fgets(size, 500, stream);
  }

  printf("Size: %s\n", size);

  #ifdef DEBUG
    printf("Type: %s\n", type);
    printf("String Compare = %d\n", strcmp(type, PBM_TYPE_P5));
  #endif

  if(strcmp(type, PBM_TYPE_P5) == 0){
    printf("Done!\n");

  //  PbmImage* image = malloc (sizeof(char)*3 + sizeof(int)*2 + sizeof(char*));
    PbmImage* image = malloc (sizeof (PbmImage));
    strcpy(image->type, PBM_TYPE_P5);
    sscanf(size, "%d %d", &image->height, &image->width);

    //printf("Image: Type: %s, height: %d, width: %d\n", image->type, image->height, image->width);
    //get bits
    fgets(bits, 500, stream);
    //data allocation
    size_t binarysize = (image->width * image->height) + 1;
    //printf("%s\n", binarysize);
    //char* data;
    image->data = malloc(binarysize);
    fread(image->data, binarysize, 1, stream);
    //printf("%s\n", image->data);

    return image;
  }
  else{
    *error = RET_UNSUPPORTED_FILE_FORMAT;
    return NULL;
  }
}

int pbm_image_write_to_stream(PbmImage* img, FILE* targetStream){
  size_t binarysize = (img->width * img->height) + 1;
  char hw[sizeof(img->height) + sizeof(img->width) + sizeof(char)*2];
  printf("Size: %d\n", sizeof(img->height)+sizeof(img->width)+1);
  char type[4];
  char comments[24] = "#by Stefan Reip, ITM14!\n";
  char maxval[4] = "255\n";
  sprintf(type, "%s\n", img->type);
  sprintf(hw, "%d %d\n", img->height, img->width);

  printf("%s\n", type);
  fwrite(type, sizeof(char), sizeof(type)-1, targetStream);
  fwrite(comments, sizeof(char), sizeof(comments), targetStream);
  //printf(" Hallo >>%s<<\n", hw);
  fwrite(hw, sizeof(char), sizeof(hw)-2 , targetStream);
  fwrite(maxval, sizeof(char), sizeof(maxval)-1, targetStream);
  fwrite(img->data-1, 1, binarysize, targetStream);

  return 1;
}
