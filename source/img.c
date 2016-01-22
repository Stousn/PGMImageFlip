#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flip.h"
#include "img.h"

/** @see img.h  */
void pbm_image_free(PbmImage* img){
  free(img->data);
  free(img);
}

/** @see img.h  */
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

  #ifdef DEBUG
    printf("Type: %s\n", type);
  #endif

  //Error if type is wrong
  if(strcmp(type, PBM_TYPE_P5) != 0){
    error* = RET_UNSUPPORTED_FILE_FORMAT;
    return NULL;
  }

  //gets comment first (ignored) and size
  while(size[0] == PBM_COMMENT_CHAR){
    #ifdef DEBUG
      printf("Comment: %s\n", size);
    #endif
    fgets(size, 500, stream);
  }

  #ifdef DEBUG
    printf("Size: %s\n", size);
    printf("String Compare = %d\n", strcmp(type, PBM_TYPE_P5));
  #endif

    error = RET_OUT_OF_MEMORY;
    //allocation of PbmImage struct
    PbmImage* image = malloc (sizeof (PbmImage));
    strcpy(image->type, PBM_TYPE_P5); //set type
    sscanf(size, "%d %d", &image->height, &image->width); //set size of data

    //get bits
    fgets(bits, 500, stream);

    //data allocation
    size_t binarysize = (image->width * image->height) + 1;
    image->data = malloc(binarysize);
    //fgets(bits, 1, stream);
    fread(image->data, binarysize, 1, stream);

    #ifdef DEBUG
      printf("Struct filled with:\n", );
      printf("Type: '%s'\n", image->type);
      printf("Width: '%d' Height: '%d'\n", image->width, image->height);
      printf("Data: '%s'\n", image->data);
    #endif

    error = RET_PBM_OK;
    return image;
}

/** @see img.h  */
int pbm_image_write_to_stream(PbmImage* img, FILE* targetStream){
  int status = RET_PBM_ERROR;
  //sets vars for header
  size_t binarysize = (img->width * img->height) + 1;
  char hw[10];//sizeof(img->height) + sizeof(img->width) + sizeof(char)*2];
  char type[4];
  char comments[24] = "#by Stefan Reip, ITM14!\n";
  char maxval[4] = "255\n";
  //Adds missing \n
  sprintf(type, "%s\n", img->type);
  sprintf(hw, "%d %d\n", img->height, img->width);

  //Writes to output file
  fwrite(type, 1, sizeof(type)-1, targetStream);
  fwrite(comments, 1, sizeof(comments), targetStream);
  //printf(" Hallo >>%s<<\n", hw);
  fwrite(hw, 1, sizeof(hw)-2 , targetStream);
  fwrite(maxval, 1, sizeof(maxval)-1, targetStream);
  fwrite(img->data, 1, binarysize, targetStream);

  /*
  fwrite(type,1,sizeof(type)-1,targetStream);
  	fwrite(comment,1,sizeof(comment),targetStream);
  	fwrite(wh,1,sizeof(wh)-1,targetStream);
  	fwrite(dp,1,sizeof(int)-1,targetStream);
  	fwrite(img->data,1,size,targetStream);

  */
  status = RET_PBM_OK;
  return status;
}
