#include "flip.h"
#include "img.h"
/** @see flip.h */
int pbm_image_flip(PbmImage* image){
  int status = RET_PBM_ERROR;

  size_t count =  (image->height * image->width) + 1;

  //(c) Gerhard Seuchter, Folien
    for(int i = 1; i <= count/2; i++){
		char tmp = image->data[i];
		image->data[i] = image->data[count - 2 - i];
		image->data[count - 2 - i] = tmp;
	}

  status = RET_PBM_OK;
  return status;
}
