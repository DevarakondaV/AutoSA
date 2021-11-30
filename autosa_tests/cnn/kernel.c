#include "kernel.h"
#include "load.h"

int main(int argc, char **argv){
  data_t bias[kNum];

  data_t cin[I][R + K - 1][C + K - 1];
  data_t w[I][O][K][K];
  data_t cout[O][R][C];
  data_t cout_golden[O][R][C];
  LoadData(cin, w, bias);
  
#pragma scop
  for (int o = 0; o < O; o++)
    for (int r = 0; r < R; r++)
      for (int c = 0; c < C; c++) {
        //cout[r][c][o] = 0;
        for (int i = 0; i < I; i++)
          for (int p = 0; p < K; p++)
            for (int q = 0; q < K; q++) {
              cout[o][r][c] = cout[o][r][c] + cin[i][r + p][c + q] * w[i][o][p][q];
            }
      }
#pragma endscop  
 
  for (int o = 0; o < O; o++)
    for (int r = 0; r < R; r++)
      for (int c = 0; c < C; c++) {
        // cout_golden[r][c][o] = 0;
        cout_golden[o][r][c] = 0;
        for (int i = 0; i < I; i++)
          for (int p = 0; p < K; p++)
            for (int q = 0; q < K; q++) {
              cout_golden[o][r][c] = cout_golden[o][r][c] + cin[i][r + p][c + q] * w[i][o][p][q];
            }
      }

  int err = 0;
  float thres = 0.001;
  for (int o = 0; o < O; o++)
    for (int r = 0; r < R; r++)
      for (int c = 0; c < C; c++) {
        if (fabs((float)cout_golden[o][r][c] - (float)cout[o][r][c]) > thres) {
          err++;
        }
      }

  //if (err) {
  //  printf("Test failed with %d errors!\n", err);
  //  return -1;
  //} else {
  //  printf("Test passed!\n");
  //  return 0;
  //}
}
