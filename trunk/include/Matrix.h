/*
	This file is part of ALIZE which is an open-source tool for 
	speaker recognition.

    ALIZE is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as 
    published by the Free Software Foundation, either version 3 of 
    the License, or any later version.

    ALIZE is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public 
    License along with ALIZE.
    If not, see <http://www.gnu.org/licenses/>.
        
	ALIZE is a development project initiated by the ELISA consortium
	[alize.univ-avignon.fr/] and funded by the French Research 
	Ministry in the framework of the TECHNOLANGUE program 
	[www.technolangue.net]

	The ALIZE project team wants to highlight the limits of voice
	authentication in a forensic context.
	The "Person  Authentification by Voice: A Need of Caution" paper 
	proposes a good overview of this point (cf. "Person  
	Authentification by Voice: A Need of Caution", Bonastre J.F., 
	Bimbot F., Boe L.J., Campbell J.P., Douglas D.A., Magrin-
	chagnolleau I., Eurospeech 2003, Genova].
	The conclusion of the paper of the paper is proposed bellow:
	[Currently, it is not possible to completely determine whether the 
	similarity between two recordings is due to the speaker or to other 
	factors, especially when: (a) the speaker does not cooperate, (b) there 
	is no control over recording equipment, (c) recording conditions are not 
	known, (d) one does not know whether the voice was disguised and, to a 
	lesser extent, (e) the linguistic content of the message is not 
	controlled. Caution and judgment must be exercised when applying speaker 
	recognition techniques, whether human or automatic, to account for these 
	uncontrolled factors. Under more constrained or calibrated situations, 
	or as an aid for investigative purposes, judicious application of these 
	techniques may be suitable, provided they are not considered as infallible.
	At the present time, there is no scientific process that enables one to 
	uniquely characterize a person=92s voice or to identify with absolute 
	certainty an individual from his or her voice.]
	Contact Jean-Francois Bonastre for more information about the licence or
	the use of ALIZE

	Copyright (C) 2003-2010
	Laboratoire d'informatique d'Avignon [lia.univ-avignon.fr]
	ALIZE admin [alize@univ-avignon.fr]
	Jean-Francois Bonastre [jean-francois.bonastre@univ-avignon.fr]
*/

#if !defined(ALIZE_Matrix_h)
#define ALIZE_Matrix_h

#if defined(_WIN32)
#if defined(ALIZE_EXPORTS)
#define ALIZE_API __declspec(dllexport)
#else
#define ALIZE_API __declspec(dllimport)
#endif
#else
#define ALIZE_API
#endif

#pragma warning(disable: 4244) // possible loss of data
#pragma warning(disable: 4146) // unary minus operator applied to unsigned type

#include <new>
#include <math.h>

#include <cstdio>
#include <iostream>
#include <fstream>
#include <memory.h>
#include <cstdlib>
#if defined (_WIN32)
#define uint32_t unsigned __int32
#else
#include <stdint.h>
#endif


#include "RealVector.h"
#include "DoubleSquareMatrix.h"
#include "alizeString.h"
#include "Exception.h"
#include "Config.h"
#include "Feature.h"

#define TINY 1.0e-20 

// Définition du Rand pour Windows
#if defined(_WIN32)
	#define drand48()((double)rand()/RAND_MAX)
	#define srand48(n)srand((n));
#endif

using namespace std;

namespace alize
{
  /// This template class implements a matrix of type-T values.<br>
  /// Inside the object, the matrix is stored as a single-dimension
  /// array.<br>
  /// WARNING : contrary to class DoubleSquareMatrix, row index is FIRST
  /// argument and column index is SECOND argument<br>
  /// This will probably change for class DoubleSquareMatrix in next release
  /// of Alize
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2006

  template <class T> class ALIZE_API Matrix : public Object
  {
    friend class TestMatrix;

  public:
  
    /// Creates a matrix of type T
    /// @param c cols of the matrix
    /// @param r rows of the matrix
    ///
    Matrix(unsigned long rows = 0, unsigned long cols = 0)
      :Object(), _cols(cols), _rows(rows), _array(rows*cols, rows*cols) {}

    /// Creates a matrix of type T and loads its content from a file
    /// @param f file name
    ///
    explicit Matrix(const FileName& f)
      :Object() { load(f, Config()); }

    /// Creates a matrix of type T and loads its content from a file
    /// @param f file name
    /// @param c configuration
    ///
    explicit Matrix(const FileName& f, const Config& c)
      :Object() { load(f, c); }
      
    /// Creates a matrix of type T with 1 row and v.size() rows<br>
    /// Copy content of v into this matrix
    /// @param v the vector
    ///
    template <class R> explicit Matrix(const RealVector<R>& v)
    {
      _cols = v.size();
      _rows = 1;
      _array = v;
    }
      
    /// Creates a matrix of type T with 1 row and f.getVectSize() rows<br>
    /// Copy content of the feature into this matrix
    /// @param f the feature
    ///
    explicit Matrix(const Feature &f)
    {
      _array.setSize(f.getVectSize());
      _cols = f.getVectSize();
      _rows = 1;      
      for (unsigned long i=0; i<f.getVectSize(); i++)
        _array[i]=(T)f[i];
    }
    
    /// Creates a matrix of type T with from a DoubleSquareMatrix (idxs are inverted)
    /// @param f the feature
    ///
    explicit Matrix(const DoubleSquareMatrix &M)
    {
      _cols = M.size();
      _rows =M.size(); 
	  _array.setSize(_cols*_rows);
      for (unsigned long i=0; i<_rows; i++){
              for (unsigned long j=0; j<_cols; j++){
                    _array[i*_rows+j]=(T)M(i,j);
			  }
	  }
    }
      
    /// Copy operator. Copy a matrix into this matrix
    /// @param m the matrix 
    ///
    Matrix<T>& operator=(const Matrix<T>& m)
    {
      _array = m._array;
      _cols = m._cols;
      _rows = m._rows;
      return *this;
    }
    
    bool operator==(const Matrix<T>& m) const
    {
      return (_cols == m._cols && _rows == m._rows && _array == m._array);
    }

    bool operator!=(const Matrix<T>& m) const { return !(*this == m); }

    Matrix(const Matrix<T>& m)
    :Object(), _cols(m._cols), _rows(m._rows), _array(m._array) {}

    virtual ~Matrix() {}

    /// Returns the number of columns of this matrix
    ///
    unsigned long cols() const { return _cols; }

    /// Returns the number of rows of this matrix
    ///
    unsigned long rows() const { return _rows; }

    /// Sets the new dimensions of the matrix
    /// @param cols the number of columns
    /// @param rows the number of rows
    ///
    void setDimensions(const unsigned long rows, const unsigned long cols)
    {
      _cols = cols;
      _rows = rows;
      _array.setSize(cols*rows);
    }

    /// Sets all the values to a a particular value
    /// @param v the value to set
    ///
    template <class R> void setAllValues(R v) { _array.setAllValues(v); }

    /// Overloaded operator() to access an element in this matrix.
    /// @param row row of the element to access
    /// @param col column of the element to access
    /// @return a REFERENCE to the element
    /// @exception IndexOutOfBoundsException
    ///
    T& operator()(unsigned long row, unsigned long col)
    {
      assertIsInBounds(__FILE__, __LINE__, col, _cols);
      assertIsInBounds(__FILE__, __LINE__, row, _rows);
      return _array[row*_cols+col];
    }

    /// Overloaded operator() to access an element in this CONSTANT matrix.
    /// @param row row of the element to access
    /// @param col column of the element to access
    /// @return a COPY of the element
    /// @exception IndexOutOfBoundsException
    ///
    T operator()(unsigned long row, unsigned long col) const
    {
      assertIsInBounds(__FILE__, __LINE__, col, _cols);
      assertIsInBounds(__FILE__, __LINE__, row, _rows);
      return _array[row*_cols+col];
    }

    /// Transposes this matrix
    /// @return this matrix
    ///
    Matrix<T>& transpose()
    {
      unsigned long c, r, cc, rr;
      RealVector<T> tmp = _array;
      T* t = tmp.getArray();
      T* p = _array.getArray();
      for (r=0, rr=0; r<_rows; r++, rr+=_cols)
        for (c=0, cc=0; c<_cols; c++, cc+=_rows)
          p[r+cc] = t[c+rr];
      r = _rows;
      _rows = _cols;
      _cols = r;
      return *this;
    }

    /// Transposes this constant matrix into a new matrix
    /// @return the new matrix
    ///
    Matrix<T> transpose() const
    {
      Matrix<T> tmp = *this;
      return tmp.transpose();
    }

    /// Inverts this matrix
    /// @return this matrix
    ///
    Matrix<T>& invert()
    {
      if(_cols!=_rows)
        throw Exception("Cannot invert matrix, non square matrix", __FILE__, __LINE__);

      int n = _cols;
      int i,j;
      T** a=(T**)malloc((n+1)*sizeof(T*));
      for (i=0;i<=n;i++)
        a[i]=(T*)malloc((n+1)*sizeof(T));
      for(j=1;j<=n;j++)
        for(i=1;i<=n;i++)
          a[i][j] = (*this)(i-1,j-1);

      int*indx = (int*)malloc((n+1)*sizeof(int));
      T** y = (T**)malloc((n+1)*sizeof(T*));     
      for (i=0;i<=n;i++)
        y[i]=(T*)malloc((n+1)*sizeof(T));  
      
      T* d = (T*)malloc((n+1)*sizeof(T)); 
      ludcmp(a,n,indx,d);
      free(d);

      T* col = (T*)malloc((n+1)*sizeof(T));            
      for(j=1;j<=n;j++)
      {
        for(i=1;i<=n;i++)
          col[i]=(T)0.0;
        col[j]=(T)1.0;
        lubksb(a,n,indx,col);
        for(i=1;i<=n;i++)
          y[i][j]=col[i];
      }

      for(j=1;j<=n;j++)
        for(i=1;i<=n;i++)
          (*this)(i-1,j-1) = y[i][j];

        free(col);
      free(indx);
      for (i=0;i<=n;i++)
      {
        free(a[i]);
        free(y[i]);
        a[i] = NULL;
        y[i] = NULL;
      }
      free(a);
      free(y);
      return *this;
    }

    /// Inverts this constant matrix into a new matrix
    /// @return the new matrix
    ///
    Matrix<T> invert() const
    {
      Matrix<T> tmp = *this;
      return tmp.invert();
    }

    /// Multiplies this matrix by an other matrix and returns
    /// the result in a new matrix (new matrix = this * m);
    /// @param m the matrix 
    /// @return a new matrix
    ///
    Matrix<T> operator*(const Matrix<T>& m) const
    {
      if (_cols != m._rows)
        throw Exception("Cannot multiply matrices", __FILE__, __LINE__);
      const unsigned long cols = m._cols;
      Matrix<T> tmp(_rows, cols);
      tmp.setAllValues(0.0);
      T* pTmp = tmp._array.getArray();
      T* pM = m._array.getArray();
      T* p = _array.getArray();
      unsigned long i, j, k, i_cols, itmp_cols, kcols;
      for (i=0, i_cols=0, itmp_cols=0; i<_rows; i++, i_cols+=_cols, itmp_cols += cols)
        for (j=0; j<cols; j++)
          for (k=0, kcols=0; k<_cols; k++, kcols+=cols)
            pTmp[itmp_cols+j] += p[i_cols+k] * pM[kcols+j];
      return tmp;
    }

    /// Multiplies this matrix by an other matrix (this *= m)
    /// @param m a matrix
    /// @return this matrix
    ///
    Matrix<T>& operator*=(const Matrix<T>& m)
    {
      (*this) = (*this)*m;
      return *this;
    }

    /// Multiplies this matrix by a scalar value
    /// @param v the scalar value
    /// @return this matrix
    ///
    Matrix<T>& operator*=(double v)
    {
      _array *= v;
      return *this;
    }

    /// Multiplies this matrix by a scalar value and returns
    /// the result in another matrix
    /// @param v the scalar value
    /// @return a matrix
    ///
    Matrix<T> operator*(double v) const
    {
      Matrix<T> tmp = *this;
      tmp._array *= v;
      return tmp;
    }

    /// Adds this matrix and an other matrix and returns
    /// the result in a new matrix (new matrix = this + m);
    /// @param m the matrix 
    /// @return a new matrix
    ///
    Matrix<T> operator+(const Matrix<T>& m) const
    {
      Matrix<T> tmp(*this);
      tmp._array += m._array;
      return tmp;
    }

    /// Adds this matrix and an other matrix (this += m)
    /// @param m a matrix
    /// @return this matrix
    ///
    Matrix<T>& operator+=(const Matrix<T>& m)
    {
      if (_cols != m._cols || _rows != m._rows)
        throw Exception("Dimensions of matrices do not match", __FILE__, __LINE__);
      _array += m._array;
      return *this;
    }

    /// Substracts a matrix from  this matrix and returns
    /// the result in a new matrix (new matrix = this - m);
    /// @param m the matrix 
    /// @return a new matrix
    ///
    Matrix<T> operator-(const Matrix<T>& m) const
    {
      Matrix<T> tmp(*this);
      tmp._array -= m._array;
      return tmp;
    }

    /// Substracts a matrix from this matrix (this -= m)
    /// @param m a matrix
    /// @return this matrix
    ///
    Matrix<T>& operator-=(const Matrix<T>& m)
    {
      if (_cols != m._cols || _rows != m._rows)
        throw Exception("Dimensions of matrices do not match", __FILE__, __LINE__);
      _array -= m._array;
      return *this;
    }    

    /// Saves a matrix depending on the saveMatrixFormat type
    /// @param f file name
    /// @return this matrix
    ///
    void save(const FileName& f) { save(f, Config()); }
    
    /// Saves a matrix depending on the saveMatrixFormat type
    /// @param f file name
    /// @param c configuration
    /// @return this matrix
    ///
    void save(const FileName& f, const Config& c)
    {
     if (c.getParam("saveMatrixFormat")=="DT") saveDT(f,c);
     else if (c.getParam("saveMatrixFormat")=="DB") saveDB(f,c); 
      else throw Exception("saveMatrixFormat unknown! DT (Dense Text) or DB (Dense Binary)",__FILE__,__LINE__);
    }
    
    /// Saves this matrix in a file (Dense Text Matrix File Format)<br/>
    /// http://tedlab.mit.edu/~dr/SVDLIBC/SVD_F_DT.html
    /// @param f file name
    /// @param c configuration
    /// @return this matrix
    ///
    void saveDT(const FileName& f, const Config& c)
    {
      XList l;
      l.addLine().addElement(String::valueOf(_rows))
                 .addElement(String::valueOf(_cols));
      for (unsigned long j=0; j<_rows; j++)
      {
        XLine& li = l.addLine();
        for (unsigned long i=0; i<_cols; i++)
          li.addElement(String::valueOf((*this)(j,i)));
      }
      l.save(f, c);
    }
    
    /// Save a matrix in a file (Dense Binary Matrix format)<br/>
    /// @param f file name
    /// @param c configuration
    /// @return this matrix
    ///
    void saveDB(const FileName&f,const Config& c)
    {
      try {
            ofstream outputMat(f.c_str(),ios::out|ios::binary);
            if(!outputMat)
                throw IOException("Cannot open file", __FILE__, __LINE__,f);
            unsigned long rows=this->rows();
            unsigned long cols=this->cols();        
            T * array=_array.getArray();


			if (sizeof(unsigned int) == 4){
				outputMat.write((char*)&rows,sizeof(unsigned int));
				outputMat.write((char*)&cols,sizeof(unsigned int));
			}
			else if (sizeof(unsigned long) == 4){
				outputMat.write((char*)&rows,sizeof(unsigned long));
				outputMat.write((char*)&cols,sizeof(unsigned long));
			}

            outputMat.write((char*)array,rows*cols*sizeof(T));
            outputMat.close();
          }
          catch (Exception& e) {cout << e.toString().c_str() << endl;}
    }
    
    /// Loads a matrix depending on loadMatrixFormat
    /// @param f file name
    /// @return this matrix
    ///
    void load(const FileName& f) { load(f, Config()); }
    
    /// Loads a matrix depending on loadMatrixFormat
    /// @param f file name    
    /// @param c configuration
    /// @return this matrix
    ///
    void load(const FileName& f, const Config& c)
    {
      if (c.getParam("loadMatrixFormat")=="DT") loadDT(f,c);
      else if (c.getParam("loadMatrixFormat")=="DB") loadDB(f,c); 
      else throw Exception("loadMatrixFormat unknown! DT (Dense Text) or DB (Dense Binary)",__FILE__,__LINE__);
    }
    /// Loads a matrix from a file (Dense Text Matrix File Format)<br/>
    /// http://tedlab.mit.edu/~dr/SVDLIBC/SVD_F_DT.html
    /// @param f file name
    /// @param c configuration
    /// @return this matrix
    ///
    void loadDT(const FileName& f, const Config& c)
    {
      XList l(f, c);
      unsigned long rows = l.getLine(0).getElement(0).toLong();
      unsigned long cols = l.getLine(0).getElement(1).toLong();
      setDimensions(rows,cols);
      l.rewind();
      l.getLine();
      XLine* p;
      String* s;
      unsigned long j = 0;
      while ((p = l.getLine()))
      {
        unsigned long i = 0;
        while ((s = p->getElement()))
        {
          (*this)(j, i) = (T) s->toDouble();
          i++;
        }
        j++;
      }
    }
    
    /// Loads a matrix from a file (Dense Binary Matrix format)<br/>
    /// @param f file name
    /// @param c configuration
    /// @return this matrix
    ///
    void loadDB(const FileName&f,const Config& c)
    {
      try {
            ifstream inputMat(f.c_str(),ios::in|ios::binary);
            if(!inputMat){
              throw IOException("Cannot open file", __FILE__, __LINE__,f);
          }

			unsigned long rows, cols;

			if (sizeof(unsigned long) == 4){
				inputMat.read((char*)&rows,sizeof(unsigned long));
				inputMat.read((char*)&cols,sizeof(unsigned long));
			}
			else if (sizeof(unsigned int) == 4){

				unsigned int r,c;
				inputMat.read((char*)&r,sizeof(unsigned int)); 
				inputMat.read((char*)&c,sizeof(unsigned int)); 
				rows = (unsigned long)r;
				cols = (unsigned long)c;

			}

            setDimensions(rows,cols);
            inputMat.read((char*)_array.getArray(),rows*cols*sizeof(T));
            inputMat.close();
          }
          catch (Exception& e) {cout << e.toString().c_str() << endl;}
    }

    /// Random init of a matrix <br/>
    /// @return this matrix
    ///
    void randomInit()
    {
      try {
	for ( unsigned long r=0; r<_rows; r++ )     
		for ( unsigned long c=0; c<_cols; c++ )         
			(*this)(r,c)= (T) drand48();
          }
          catch (Exception& e) {cout << e.toString().c_str() << endl;}
    }
    
    /// Use this method to access directly to the internal vector
    /// @return a pointer on the first element
    /// @warning Fast but dangerous ! Use preferably operator()(row, col).
    ///
    T* getArray() const { return _array.getArray(); }

    virtual String toString() const
    {
      String s = Object::toString()
        + "\n  dimensions  = " + String::valueOf(_rows)+"x"+String::valueOf(_cols);
      for (unsigned long j=0; j<_rows; j++)
      {
         for (unsigned long i=0; i<_cols; i++)
         s += "\n  [" + String::valueOf(j)
            + "," + String::valueOf(i)
            + "] = " + String::valueOf((*this)(j,i));
        s += "\n";
      }
      return s;
    }

    virtual String getClassName() const { return "Matrix"; }

    /// Extract a sub matrix from  the current one
    /// @return a submatrix
    /// @param line line number of the first selected element
    /// @param col column number of the first selected element
    /// @param n_rows number of line of the sub matrix
    /// @param n_cols number of columns of the sub matrix
    ///
    Matrix<T> crop(unsigned long line, unsigned long col, unsigned long n_rows, unsigned long n_cols)
    {
	if( (line + n_rows > _rows) || (col + n_cols > _cols) ){
		throw Exception("Cannot crop a such big Matrix",__FILE__,__LINE__);
	}
	
	Matrix<T> subM(n_rows, n_cols);
	T* subm=subM.getArray();
	T* array = _array.getArray();
	
	for(unsigned long i=0; i<n_rows; i++){
		for(unsigned long j=0; j<n_cols; j++){
			subm[i*n_cols+j] =  array[(line+i)*_cols+col+j];
		}
	}
	return subM;
    } 
    
    /// Concatenate two matrices in column within the current one
    ///
    /// @param M1 the first matrix to concatenate
    /// @param M2 the second matrix to concatenate
    ///
    void concatCols(const Matrix<T>& M1, const Matrix<T>& M2){
	
	//Verify the number of columns of the two matrices
	if( M1.cols() != M2.cols() ){
		throw Exception("Cannot concatenate the given matrices, wrong dimensionality",__FILE__,__LINE__);
	}
	
	unsigned long nrows = M1.rows()+M2.rows();
	this->setDimensions(nrows,M1.cols());
	
	T* m1=M1.getArray();
	T* m2 = M2.getArray();

	for(unsigned long i=0; i<M1.cols()*M1.rows(); i++){
		_array[i] =  m1[i];
	}
	for(unsigned long i=0; i<M2.cols()*M2.rows(); i++){
		_array[i+M1.cols()*M1.rows()] =  m2[i];
	}
    }

    /// Concatenate two matrices in line within the current one
    ///
    /// @param M1 the first matrix to concatenate
    /// @param M2 the second matrix to concatenate
    ///
    void concatRows(const Matrix<T>& M1, const Matrix<T>& M2){
	
	//Verify the number of columns of the two matrices
	if( M1.rows() != M2.rows() ){
		throw Exception("Cannot concatenate the given matrices, wrong dimensionality",__FILE__,__LINE__);
	}
	
	unsigned long ncols = M1.cols()+M2.cols();
	this->setDimensions(M1.rows(),ncols);
	
	T* m1=M1.getArray();
	T* m2 = M2.getArray();
	
	for(unsigned long r=0; r<M1.rows(); r++){
		for(unsigned long c1=0; c1<M1.cols(); c1++){
			_array[r*ncols + c1] =  m1[r*M1.cols()+c1];
		}
		
		for(unsigned long c2=0; c2<M2.cols(); c2++){
			_array[r*ncols + M1.cols() + c2] =  m2[r*M2.cols()+c2];
		}
	}
    }



  private:

    uint32_t _cols;
    uint32_t _rows;
    RealVector<T> _array;

	unsigned long fabs(unsigned long x) { return x; };
    void ludcmp(T **a, int n, int *indx, T *d)
    {
      int i,imax,j,k;
      T big,dum,sum,temp;
      T *vv;
      vv=(T*)malloc((n+1)*sizeof(T));
      for(i=0;i<=n;i++) vv[i]=(T)0.0;
      *d=(T)1.0; 
      //fprintf(stderr,"%d ",*d);      
      for (i=1;i<=n;i++) { 
          big=(T)0.0; 
          //fprintf(stderr,"%d ",i);           
          for (j=1;j<=n;j++)
              if ((temp=(T)fabs(a[i][j])) > big) big=(T)temp;
          if (big == 0.0) big=big;//fprintf(stderr,"Ciao");
          vv[i]=(T)(1.0/big);
      }
      //fprintf(stderr,"step 1\n");      
      for (j=1;j<=n;j++) { 
          for (i=1;i<j;i++) { 
              sum=a[i][j];
              for (k=1;k<i;k++) sum -= a[i][k]*a[k][j];
              a[i][j]=sum;
          }
          //    fprintf(stderr,"step 2\n");   
          big=(T)0.0;
          for (i=j;i<=n;i++) {
              sum=a[i][j];
              for (k=1;k<j;k++) sum -= a[i][k]*a[k][j];
              a[i][j]=sum;
              if ( (dum=vv[i]*(T)fabs(sum)) >= big) {
                  big=dum;
                  imax=i;
              }
          }
          if (j != imax) {
              for (k=1;k<=n;k++) {
                  dum=a[imax][k];
                  a[imax][k]=a[j][k];
                  a[j][k]=dum;
              }
              *d = -(*d); 
              vv[imax]=vv[j];
          }
          indx[j]=imax;
          if (a[j][j] == 0.0) a[j][j]=(T)TINY;
          if (j != n) {
              dum=(T)(1.0/(a[j][j]));
              for (i=j+1;i<=n;i++) a[i][j] *= dum;
          }
      } 
      free(vv);
    }

    void lubksb(T **a, int n, int *indx, T b[])
    {
      int i,ii=0,ip,j;
      T sum;
      for (i=1;i<=n;i++) { 
          ip=indx[i];
          sum=b[ip];
          b[ip]=b[i];
          if (ii)
          for (j=ii;j<=i-1;j++) sum -= a[i][j]*b[j];
          else if (sum) ii=i;
          b[i]=sum; 
      }
      for (i=n;i>=1;i--) { 
          sum=b[i];
          for (j=i+1;j<=n;j++) sum -= a[i][j]*b[j];
          b[i]=sum/a[i][i]; 
      } 
    }

  };

  typedef Matrix<double> DoubleMatrix;
#if defined(_WIN32)
  template class Matrix<double>;
  template class Matrix<unsigned long>;
  template class Matrix<float>;
#endif
  
} // end namespace alize

#endif  // ALIZE_Matrix_h

