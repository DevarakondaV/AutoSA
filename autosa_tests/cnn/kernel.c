#include "kernel.h"
#include "load.h"

int main(int argc, char **argv){
  // Allocate memory on heap to avoid stack overflow.
  static data_t cin[kNum][kInImSize][kInImSize];
  static data_t w[kNum][kNum][kKernel][kKernel];
  static data_t bias[kNum];
  static data_t cout[kNum][kOutImSize][kOutImSize];

  if (argc > 2) {
    clog << "Usage: " << argv[0] << " [data dir]\n";
    return EXIT_FAILURE;
  }

  const string data_dir = argc == 2 ? string(argv[1]) + "/" : "";
  LoadData(data_dir, cin, w, bias);

  // data_t cin[R + K - 1][C + K - 1][I];
  // data_t w[O][K][K][I];
  // data_t cout[R][C][O];
  // data_t cout_golden[R][C][O];
  data_t cout_golden[O][R][C];

  // // data initialization
  // for (int i = 0 ; i < I; i++)
  //   for (int r = 0; r < R + K - 1; r++)
  //     for (int c = 0; c < C + K - 1; c++) {
  //       cin[r][c][i] = i;
  //     }

  // for (int o = 0; o < O; o++)
  //   for (int i = 0; i < I; i++) 
  //     for (int p = 0; p < K; p++)
  //       for (int q = 0; q < K; q++) {
  //         w[o][p][q][i] = o;
  //       }
 
#pragma scop
  for (int o = 0; o < O; o++)
    for (int r = 0; r < R; r++)
      for (int c = 0; c < C; c++) {
        //cout[r][c][o] = 0;
        for (int i = 0; i < I; i++)
          for (int p = 0; p < K; p++)
            for (int q = 0; q < K; q++) {
              // cout[r][c][o] = cout[r][c][o] + cin[r + p][c + q][i] * w[o][p][q][i];
              cout[o][r][c] = cout[o][r][c] + cin[i][r + p][c + q] * w[i][o][p][q];
            }
      }
#pragma endscop  
 
  for (int o = 0; o < O; o++)
    for (int r = 0; r < R; r++)
      for (int c = 0; c < C; c++) {
        cout_golden[r][c][o] = 0;
        for (int i = 0; i < I; i++)
          for (int p = 0; p < K; p++)
            for (int q = 0; q < K; q++) {
              // cout_golden[r][c][o] = cout_golden[r][c][o] + cin[r + p][c + q][i] * w[o][p][q][i];
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
