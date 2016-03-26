#ifndef MAT_H
#define MAT_H

#include <math.h>

#define nullptr ((T *) 0)

namespace core{

template <class T> class mat {
 public:
  mat();
  mat(const mat &m);
  mat(uint32_t nrows, uint32_t ncols);
  mat(uint32_t nrows, uint32_t ncols, T *data);
  mat(uint32_t nrows, uint32_t ncols, T **data);
  virtual ~mat();
  virtual void operator=(T d);
  virtual void operator=(const mat &m);

  // operation
  mat add(const mat &m) const;
  mat subtract(const mat &m) const;
  mat multiply(const mat &m) const;
  T sum() const;
  mat sum(uint32_t dim) const;
  mat t() const;
  mat appendrow(T v) const;
  mat insertrow(T v) const;
  mat appendcol(T v) const;
  mat insertcol(T v) const;
  void transpose();

  // operator
  mat operator+(const mat &m) const;
  mat operator-(const mat &m) const;
  mat operator*(const mat &m) const;
  mat operator%(const mat &m) const;

  mat operator-() const;
  mat operator+(T d) const;
  mat operator-(T d) const;
  mat operator*(T d) const;
  mat operator/(T d) const;

  T operator()(uint32_t i, uint32_t j) const;
  T operator[](uint32_t i) const;

  // static functions
  static mat zero(uint32_t nrows, uint32_t ncols);
  static mat ones(uint32_t nrows, uint32_t ncols);
  static mat eye(uint32_t dim);
  static mat rand(uint32_t nrows, uint32_t ncols);

  uint32_t nrows, ncols;
  T *data;
};

// implementation
template <class T> mat<T>::mat() {
  nrows = ncols = 0;
  data = nullptr;
}
template <class T> mat<T>::mat(const mat<T> &m) {
  nrows = m.nrows;
  ncols = m.ncols;
  data = new T[nrows * ncols];

  for (uint32_t i = 0 ; i < nrows ; i ++) {
    for (uint32_t j = 0 ; j < ncols ; j ++) {
      data[i * ncols + j] = m.data[i * ncols + j];
    }
  }
}
template <class T> mat<T>::mat(uint32_t nrows, uint32_t ncols) {
  this->nrows = nrows;
  this->ncols = ncols;
  this->data = new T[nrows * ncols];

  for (uint32_t i = 0 ; i < nrows ; i ++) {
    for (uint32_t j = 0 ; j < ncols ; j ++) {
      data[i * ncols + j] = 0;
    }
  }
}
template <class T> mat<T>::mat(uint32_t nrows, uint32_t ncols, T *data) {
  this->nrows = nrows;
  this->ncols = ncols;
  this->data = new T[nrows * ncols];

  for (uint32_t i = 0 ; i < nrows ; i ++) {
    for (uint32_t j = 0 ; j < ncols ; j ++) {
      this->data[i * ncols + j] = data[i * ncols + j];
    }
  }
}
template <class T> mat<T>::mat(uint32_t nrows, uint32_t ncols, T **data) {
  this->nrows = nrows;
  this->ncols = ncols;
  this->data = new T[nrows * ncols];

  for (uint32_t i = 0 ; i < nrows ; i ++) {
    for (uint32_t j = 0 ; j < ncols ; j ++) {
      this->data[i * ncols + j] = data[i][j];
    }
  }
}
template <class T> mat<T>::~mat() {
  if (data != nullptr)
    delete data;
}

template <class T> mat<T> mat<T>::add(const mat<T> &m) const {
  if (ncols != m.ncols || nrows != m.nrows) {
    return mat();
  } else {
    mat newMat(this->nrows, this->ncols);
    const uint32_t ncols = this->ncols, nrows = this->nrows;
    for (uint32_t i = 0 ; i < nrows ; ++i) {
      for (uint32_t j = 0 ; j < ncols ; ++j) {
        const uint32_t index = i * ncols + j;
        newMat.data[index] = this->data[index] + m.data[index];
      }
    }
    return newMat;
  }
}
template <class T> mat<T> mat<T>::subtract(const mat<T> &m) const {
  if (this->ncols != m.ncols || this->nrows != m.nrows) {
    return mat();
  } else {
    mat newMat(this->nrows, this->ncols);
    const uint32_t ncols = this->ncols, nrows = this->nrows;
    for (uint32_t i = 0 ; i < nrows ; i ++) {
      for (uint32_t j = 0 ; j < ncols ; j ++) {
        const uint32_t index = i * ncols + j;
        newMat.data[index] = this->data[index] - m.data[index];
      }
    }
    return newMat;
  }
}
template <class T> mat<T> mat<T>::multiply(const mat<T> &m) const {
  if (this->ncols != m.nrows) {
    return mat();
  } else {
    mat newMat(this->nrows, m.ncols);
    for (uint32_t i = 0 ; i < this->nrows ; ++i) {
      for (uint32_t j = 0 ; j < m.ncols ; ++j) {
        const uint32_t index = i * m.ncols + j;
        newMat.data[index] = 0;
        for (uint32_t k = 0 ; k < this->ncols ; ++k) {
          newMat.data[index] += this->data[i * this->ncols + k] *
              m.data[k * m.ncols + j];
        }
      }
    }
    return newMat;
  }
}
template <class T> T mat<T>::sum() const {
  mat sum = mat(1, 1);
  for (uint32_t i = 0 ; i < this->nrows ; ++i) {
    for (uint32_t j = 0 ; j < this->ncols ; ++j) {
      sum.data[0] += data[i * this->ncols + j];
    }
  }
  return sum;
}
template <class T> mat<T> mat<T>::sum(uint32_t dim) const {
  mat sum;
  switch (dim) {
    case 0:
      sum = mat(1, 1);
      for (uint32_t i = 0 ; i < this->nrows ; ++i) {
        for (uint32_t j = 0 ; j < this->ncols ; ++j) {
          sum.data[0] += data[i * this->ncols + j];
        }
      }
      return sum;
      break;
    case 1:
      sum = mat(1, this->ncols);
      for (uint32_t j = 0 ; j < this->ncols ; ++j) {
        for (uint32_t i = 0 ; i < this->nrows ; ++i) {
          sum.data[j] += data[i * this->ncols + j];
        }
      }
      return sum;
      break;
    case 2:
      sum = mat(this->nrows, 1);
      for (uint32_t i = 0 ; i < this->nrows ; ++i) {
        for (uint32_t j = 0 ; j < this->ncols ; ++j) {
          sum.data[i] += data[i * this->ncols + j];
        }
      }
      return sum;
      break;
  }
  return sum;
}
template <class T> mat<T> mat<T>::t() const {
  mat m(this->ncols, this->nrows);
  for (uint32_t i = 0 ; i < m.nrows ; ++i) {
    for (uint32_t j = 0 ; j < m.ncols ; ++j) {
      m.data[i * m.ncols + j] = this->data[j * this->ncols + i];
    }
  }
  return m;
}
template <class T> mat<T> mat<T>::insertrow(T v) const {
  mat<T> newmat(this->nrows + 1, this->ncols);
  for (uint32_t i = 0 ; i < newmat.nrows ; ++i) {
    for (uint32_t j = 0 ; j < newmat.ncols ; ++j) {
      if (i == 0) {
        newmat.data[i * newmat.ncols + j] = v;
      } else {
        newmat.data[i * newmat.ncols + j] = this->data[(i-1) * this->ncols + j];
      }
    }
  }
  return newmat;
}
template <class T> mat<T> mat<T>::appendrow(T v) const {
  mat<T> newmat(this->nrows + 1, this->ncols);
  for (uint32_t i = 0 ; i < newmat.nrows ; ++i) {
    for (uint32_t j = 0 ; j < newmat.ncols ; ++j) {
      if (i == newmat.nrows - 1) {
        newmat.data[i * newmat.ncols + j] = v;
      } else {
        newmat.data[i * newmat.ncols + j] = this->data[(i-1) * this->ncols + j];
      }
    }
  }
  return newmat;
}
template <class T> mat<T> mat<T>::insertcol(T v) const {
  mat<T> newmat(this->nrows, this->ncols + 1);
  for (uint32_t i = 0 ; i < newmat.nrows ; ++ i) {
    newmat.data[i * newmat.ncols] = v;
    for (uint32_t j = 1 ; j < newmat.ncols ; ++ j) {
      newmat.data[i * newmat.ncols + j] = this->data[i * this->ncols + (j-1)];
    }
  }
  return newmat;
}
template <class T> mat<T> mat<T>::appendcol(T v) const {
  mat<T> newmat(this->nrows, this->ncols + 1);
  for (uint32_t i = 0 ; i < newmat.nrows ; ++ i) {
    for (uint32_t j = 0 ; j < newmat.ncols - 1 ; ++ j) {
      newmat.data[i * newmat.ncols + j] = this->data[i * this->ncols + (j-1)];
    }
    newmat.data[i * newmat.ncols + newmat.ncols - 1] = v;
  }
  return newmat;
}
template <class T> void mat<T>::transpose() {
  float *newdata = new float[nrows * ncols];
  for (uint32_t i = 0 ; i < nrows ; ++i) {
    for (uint32_t j = 0 ; j < ncols ; ++j) {
      newdata[j * nrows + i] = data[i * ncols + j];
    }
  }
  if (data) delete data;
  data = newdata;
}

template <class T> void mat<T>::operator=(T d) {
  nrows = 1;
  ncols = 1;
  delete data;
  data = new T[1];
  data[0] = d;
}
template <class T> void mat<T>::operator=(const mat<T> &m) {
  nrows = m.nrows;
  ncols = m.ncols;
  delete data;
  data = new T[m.nrows * m.ncols];

  for (uint32_t i = 0 ; i < nrows ; ++i) {
    for (uint32_t j = 0 ; j < ncols ; ++j) {
      const uint32_t index = i * ncols + j;
      data[index] = m.data[index];
    }
  }
}
template <class T> mat<T> mat<T>::operator+(const mat<T> &m) const {
  return this->add(m);
}
template <class T> mat<T> mat<T>::operator-(const mat<T> &m) const {
  return this->subtract(m);
}
template <class T> mat<T> mat<T>::operator*(const mat<T> &m) const {
  return this->multiply(m);
}
template <class T> mat<T> mat<T>::operator%(const mat<T> &m) const {
  if (this->ncols != m.ncols || this->nrows != m.nrows) {
    return mat();
  } else {
    mat newMat(this->nrows, this->ncols);
    const uint32_t ncols = this->ncols, nrows = this->nrows;
    for (uint32_t i = 0 ; i < nrows ; ++i) {
      for (uint32_t j = 0 ; j < ncols ; ++j) {
        const uint32_t index = i * ncols + j;
        newMat.data[index] = this->data[index] * m.data[index];
      }
    }
    return newMat;
  }
}

template <class T> mat<T> mat<T>::operator-() const {
  mat m(this->nrows, this->ncols);
  for (uint32_t i = 0 ; i < m.nrows ; ++ i) {
    for (uint32_t j = 0 ; j < m.ncols ; ++ j) {
      m.data[i * m.ncols + j] = - m.data[i * m.ncols + j];
    }
  }
  return m;
}
template <class T> mat<T> mat<T>::operator+(T d) const {
  mat m(nrows, ncols);
  for (uint32_t i = 0 ; i < m.nrows ; ++ i) {
    for (uint32_t j = 0 ; j < m.ncols ; ++ j) {
      m.data[i * m.ncols + j] = this->data[i * this->ncols + j] + d;
    }
  }
  return m;
}
template <class T> mat<T> mat<T>::operator-(T d) const {
  mat m(this->nrows, this->ncols);
  for (uint32_t i = 0 ; i < m.nrows ; ++ i) {
    for (uint32_t j = 0 ; j < m.ncols ; ++ j) {
      m.data[i * m.ncols + j] = this->data[i * this->ncols + j] - d;
    }
  }
  return m;
}
template <class T> mat<T> mat<T>::operator*(T d) const {
  mat m(this->nrows, this->ncols);
  for (uint32_t i = 0 ; i < m.nrows ; ++ i) {
    for (uint32_t j = 0 ; j < m.ncols ; ++ j) {
      m.data[i * m.ncols + j] = this->data[i * this->ncols + j] * d;
    }
  }
  return m;
}
template <class T> mat<T> mat<T>::operator/(T d) const {
  mat m(nrows, ncols);
  for (uint32_t i = 0 ; i < m.nrows ; ++ i) {
    for (uint32_t j = 0 ; j < m.ncols ; ++ j) {
      m.data[i * m.ncols + j] = this->data[i * this->ncols + j] / d;
    }
  }
  return m;
}
template <class T> T mat<T>::operator()(uint32_t i, uint32_t j) const {
  return data[i * ncols + j];
}
template <class T> T mat<T>::operator[](uint32_t i) const {
  if (i >= 0 && i < ncols * nrows)
    return data[i];
  else return 0;
}

template <class T> mat<T> mat<T>::zero(uint32_t nrows, uint32_t ncols) {
  return mat(nrows, ncols);
}
template <class T> mat<T> mat<T>::ones(uint32_t nrows, uint32_t ncols) {
  mat m(nrows, ncols);
  for (uint32_t i = 0 ; i < nrows ; ++ i) {
    for (uint32_t j = 0 ; j < ncols ; ++ j) {
      m.data[i * ncols + j] = 1;
    }
  }
  return m;
}
template <class T> mat<T> mat<T>::eye(uint32_t dim) {
  mat m(dim, dim);
  for (uint32_t i = 0 ; i < dim ; i ++) {
    m.data[i * m.ncols + i] = 1;
  }
  return m;
}
template <class T> mat<T> mat<T>::rand(uint32_t nrows, uint32_t ncols) {
  mat m(nrows, ncols);
  for (uint32_t i = 0 ; i < nrows ; ++ i) {
    for (uint32_t j = 0 ; j < ncols ; ++ j) {
      m.data[i * ncols + j] = (rand() % 10000);
    }
  }
  return m;
}

} // end of core namesapce

#undef nullptr
#endif /* end of include guard: MAT_H */
