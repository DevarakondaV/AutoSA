#ifndef LOAD_H_
#define LOAD_H_
#include "kernel.h"

const int kNum = I;
const int kKernel = K;
const int kInImSize = R;

void LoadData(float input[kNum][kInImSize][kInImSize],
              float weight[kNum][kNum][kKernel][kKernel], float bias[kNum]);

#endif
