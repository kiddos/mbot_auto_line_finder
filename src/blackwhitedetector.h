#include "mat.h"
#include <math.h>

#define NUM_DATA 100
#define MAX_VALUE 1024
#define MIN_VALUE 0

using core::mat;
typedef mat<float> Mat;

enum label {BLACK, WHITE};

class BlackWhiteDetector {
 public:
  BlackWhiteDetector();
  void forwardprop();
  void backwardprop();
  void feeddata(float x, bool output);
  double computecost(const Mat &theta1, const Mat &theta2);
  void computegrad();
  label predict(float x, float &a1, float &a2);
  float getlastx();

 private:
  Mat theta1, theta2;
  Mat grad1, grad2;
  Mat grad1check, grad2check;
  Mat z2, a2, z3, a3, h, x, kl;
  const float alpha, beta, p;
};

