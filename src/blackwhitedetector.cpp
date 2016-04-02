//#include <Arduino.h>
#include "blackwhitedetector.h"

//void printmat(const Mat &m) {
  //for (uint32_t i = 0 ; i < m.nrows ; ++ i) {
    //for (uint32_t j = 0 ; j < m.ncols ; ++ j) {
      //Serial.print(m.data[i * m.ncols + j]);
      //Serial.print(" ");
    //}
    //Serial.println("");
  //}
//}
static void randmat(Mat &m) {
  for (uint32_t i = 0 ; i < m.nrows ; ++ i) {
    for (uint32_t j = 0 ; j < m.ncols ; ++ j)
      m.data[i * m.ncols + j] = 1.0f * random(1000) / 1000;
  }
}
Mat sigmoid(const Mat &m) {
  Mat newmat(m.nrows, m.ncols);
  for (uint32_t i = 0 ; i < m.nrows ; ++i) {
    for (uint32_t j = 0 ; j < m.ncols ; ++j) {
      float value = m.data[i * m.ncols + j];
      newmat.data[i * m.ncols + j] = 1.0f / (1.0f + exp(-value));
    }
  }
  return newmat;
}
Mat sigmoidgradient(const Mat &m) {
  Mat newmat(m.nrows, m.ncols);
  Mat one = Mat::ones(m.nrows, m.ncols);
  Mat s = sigmoid(m);
  newmat = s % (one - s);
  return newmat;
}
Mat logmat(const Mat &m) {
  Mat newmat(m.nrows, m.ncols);
  for (uint32_t i = 0 ; i < m.nrows ; ++i) {
    for (uint32_t j = 0 ; j < m.ncols ; ++j) {
      float value = m.data[i * m.ncols + j];
      newmat.data[i * m.ncols + j] = log(value);
    }
  }
  return newmat;
}
Mat addones(const Mat &m) {
  Mat newmat(m.nrows, m.ncols + 1);
  for (uint32_t i = 0 ; i < newmat.nrows ; ++ i) {
    newmat.data[i * newmat.ncols] = 1;
    for (uint32_t j = 0 ; j < m.ncols ; ++ j) {
      newmat.data[i * newmat.ncols + j + 1] = m.data[i * m.ncols + j];
    }
  }
  return newmat;
}
Mat removefirstcol(const Mat &m) {
  if (m.ncols > 1) {
    Mat newmat(m.nrows, m.ncols - 1);
    for (uint32_t i = 0 ; i < newmat.nrows ; ++i) {
      for (uint32_t j = 0; j < newmat.ncols; ++j) {
        newmat.data[i * newmat.ncols + j] = m.data[i * m.ncols + j + 1];
      }
    }
    return newmat;
  } else {
    return Mat();
  }
}
Mat KL(const Mat &a2, const float p) {
  Mat kl(a2.nrows, a2.ncols);
  for (uint32_t i = 0 ; i < kl.nrows ; ++i) {
    for (uint32_t j = 0 ; j < kl.ncols ; ++j) {
      const float pj = a2.data[i * a2.ncols + j];
      kl.data[i * kl.ncols + j] = -p / pj + (1-p) / (1-pj);
    }
  }
  return kl;
}
Mat KLcost(const Mat &a2, const float p) {
  Mat kl(a2.nrows, a2.ncols);
  for (uint32_t i = 0 ; i < kl.nrows ; ++i) {
    for (uint32_t j = 0 ; j < kl.ncols ; ++j) {
      const float pj = a2.data[i * a2.ncols + j];
      kl.data[i * kl.ncols + j] = p * log(p / pj) + (1-p) * log((1-p) / (1-pj));
    }
  }
  return kl;
}

BlackWhiteDetector::BlackWhiteDetector()
    : theta1(Mat(2, 2)), theta2(Mat(3, 1)),
      grad1(Mat(2, 2)), grad2(Mat(3, 1)),
      grad1check(Mat(2, 2)), grad2check(Mat(3, 1)),
      alpha(1e-3), beta(1e-1), p(5e-2) {
  x = Mat::zero(1, 2);

  // random initialize
  randmat(theta1);
  randmat(theta2);
}
void BlackWhiteDetector::feeddata(float x, bool output) {
  this->x.data[0] = 1;
  this->x.data[1] = x;

  forwardprop();
  backwardprop();

  theta1 = theta1 - grad1 * alpha;
  theta2 = theta2 - grad2 * alpha;

  //Serial.print("grad 1:");
  //printmat(grad1);
  //Serial.print("grad 2:");
  //printmat(grad2);

  //computecost(theta1, theta2);
  //computegrad();
  //Serial.print("grad 1 check:");
  //printmat(grad1check);

  //Serial.print("grad 2 check:");
  //printmat(grad2check);

  /*if (output) {*/
    /*Serial.print("\rx = ");*/
    /*Serial.print(x);*/
    /*Serial.print(" hypothesis: ");*/
    /*Serial.print(h.data[0]);*/
    /*Serial.print(" | cost: ");*/
    /*Serial.print(computecost(theta1, theta2));*/
  /*}*/
}
void BlackWhiteDetector::forwardprop() {
  z2 = x * theta1;
  a2 = sigmoid(z2);
  z3 = addones(a2) * theta2;
  a3 = sigmoid(z3);
  h = a3;

  kl = KL(a2, p);
}
void BlackWhiteDetector::backwardprop() {
  Mat delta3 = h - x.data[1];
  Mat klterm = addones(kl * beta);
  Mat delta2 = (delta3 * theta2.t() + klterm) %
      addones(sigmoidgradient(z2));
  Mat D2 = (addones(a2).t()) * delta3;
  Mat D1 = x.t() * removefirstcol(delta2);
  grad1 = D1;
  grad2 = D2;
}
double BlackWhiteDetector::computecost(const Mat &theta1, const Mat &theta2) {
  Mat Z2 = x * theta1;
  Mat A2 = sigmoid(Z2);
  Mat Z3 = addones(A2) * theta2;
  Mat A3 = sigmoid(Z3);
  Mat H = A3;

  Mat y = removefirstcol(x);
  Mat one = Mat::ones(y.nrows, y.ncols);
  Mat j = (Mat::zero(y.nrows, y.ncols) - y) % logmat(H) -
        (one - y) % logmat(one - H);

  // kullback-leibler
  Mat Kl = KLcost(A2, p);
  j = j + Kl.sum(0) * beta;
  return j.data[0];
}
void BlackWhiteDetector::computegrad() {
  const float eps = 0.05;
  grad1check = Mat::zero(theta1.nrows, theta1.ncols);
  grad2check = Mat::zero(theta2.nrows, theta2.ncols);

  Mat perturb = Mat::zero(theta1.nrows, theta1.ncols);
  for (uint32_t i = 0 ; i < perturb.nrows ; ++i) {
    for (uint32_t j = 0 ; j < perturb.ncols ; ++j) {
      perturb.data[i * perturb.ncols + j] = eps;
      double loss1 = computecost(theta1 - perturb, theta2);
      double loss2 = computecost(theta1 + perturb, theta2);
      grad1check.data[i * grad1check.ncols + j] = (loss2 - loss1) / (2 * eps);
      perturb.data[i * perturb.ncols + j] = 0;
    }
  }
  perturb = Mat::zero(theta2.nrows, theta2.ncols);
  for (uint32_t i = 0 ; i < perturb.nrows ; ++i) {
    for (uint32_t j = 0 ; j < perturb.ncols ; ++j) {
      perturb.data[i * perturb.ncols + j] = eps;
      double loss1 = computecost(theta1, theta2 - perturb);
      double loss2 = computecost(theta1, theta2 + perturb);
      grad2check.data[i * grad2check.ncols + j] = (loss2 - loss1) / (2 * eps);
      perturb.data[i * perturb.ncols + j] = 0;
    }
  }
}
label BlackWhiteDetector::predict(float x, float &a1, float &a2) {
  Mat blackX(1, 2);
  blackX.data[0] = 1;
  blackX.data[1] = 0;
  Mat blackZ2 = blackX * theta1;
  Mat blackA2 = sigmoid(blackZ2);
  int index = 0;
  if (blackA2.data[1] >= blackA2.data[0]) index = 1;

  Mat X(1, 2);
  X.data[0] = 1;
  X.data[1] = x;
  Mat Z2 = X * theta1;
  Mat A2 = sigmoid(Z2);

  a1 = A2.data[0];
  a2 = A2.data[1];

  /*Serial.print("x: ");*/
  /*Serial.print(x);*/
  /*Serial.print(" | cost: ");*/
  /*Serial.print(computecost(theta1, theta2));*/
  /*Serial.print(" | hypothesis: ");*/
  /*Serial.println(sigmoid(addones(A2) * theta2).data[0]);*/

  if (A2.data[index] >= A2.data[1-index]) {
    return BLACK;
  } else {
    return WHITE;
  }
}
float BlackWhiteDetector::getlastx() {
  return x.data[1];
}

