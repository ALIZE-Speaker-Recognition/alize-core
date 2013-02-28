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

#if !defined(ALIZE_BoolMatrix_h)
#define ALIZE_BoolMatrix_h

#if defined(_WIN32)
#if defined(ALIZE_EXPORTS)
#define ALIZE_API __declspec(dllexport)
#else
#define ALIZE_API __declspec(dllimport)
#endif
#else
#define ALIZE_API
#endif

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


//#include "RealVector.h"
//#include "DoubleSquareMatrix.h"
#include "alizeString.h"
#include "Exception.h"
#include "Config.h"
//#include "Feature.h"

//#define TINY 1.0e-20 

// Définition du Rand pour Windows
//#if defined(_WIN32)
//	#define drand48()((double)rand()/RAND_MAX)
//	#define srand48(n)srand((n));
//#endif

using namespace std;

namespace alize
{
  /// This class implements a matrix of type boolean values.<br>
  /// Inside the object, the matrix is stored as a single-dimension
  /// array.<br>
  /// WARNING : contrary to class DoubleSquareMatrix, row index is FIRST
  /// argument and column index is SECOND argument<br>
  /// This will probably change for class DoubleSquareMatrix in next release
  /// of Alize
  ///
  /// @author Anthony Larcher
  /// @version 2.0
  /// @date 2013

  class ALIZE_API BoolMatrix : public Object
  //template <class T> class ALIZE_API BoolMatrix : public Object
  {
  // friend class TestMatrix;

  public:
  
    /// Creates a matrix of type bool
    /// @param c cols of the matrix
    /// @param r rows of the matrix
    ///
    BoolMatrix(unsigned long rows = 0, unsigned long cols = 0)
      :Object(), _cols(cols), _rows(rows), _array(rows*cols, rows*cols) {}

    /// Creates a BoolMatrix and loads its content from a file
    /// @param f file name
    ///
    explicit BoolMatrix(const FileName& f)
      :Object() { load(f, Config()); }

    /// Creates a BoolMatrix and loads its content from a file
    /// @param f file name
    /// @param c configuration
    ///
    explicit BoolMatrix(const FileName& f, const Config& c)
      :Object() { load(f, c); }
             
    /// Copy operator. Copy a BoolMatrix into this BoolMatrix
    /// @param m the BoolMatrix 
    ///
    BoolMatrix& operator=(const BoolMatrix& m)
    {
      _array = m._array;
      _cols = m._cols;
      _rows = m._rows;
      return *this;
    }
    
    bool operator==(const BoolMatrix& m) const
    {
      return (_cols == m._cols && _rows == m._rows && _array == m._array);
    }

    bool operator!=(const BoolMatrix& m) const { return !(*this == m); }

    BoolMatrix(const BoolMatrix& m)
    :Object(), _cols(m._cols), _rows(m._rows), _array(m._array) {}

    virtual ~BoolMatrix() {}

    /// Returns the number of columns of this BoolMatrix
    ///
    unsigned long cols() const { return _cols; }

    /// Returns the number of rows of this BoolMatrix
    ///
    unsigned long rows() const { return _rows; }

    /// Sets the new dimensions of the BoolMatrix
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
    void setAllValues(bool v) { _array.setAllValues(v); }

    /// Overloaded operator() to access an element in this BoolMatrix.
    /// @param row row of the element to access
    /// @param col column of the element to access
    /// @return a REFERENCE to the element
    /// @exception IndexOutOfBoundsException
    ///
    bool& operator()(unsigned long row, unsigned long col)
    {
      assertIsInBounds(__FILE__, __LINE__, col, _cols);
      assertIsInBounds(__FILE__, __LINE__, row, _rows);
      return _array[row*_cols+col];
    }

    /// Overloaded operator() to access an element in this CONSTANT BoolMatrix.
    /// @param row row of the element to access
    /// @param col column of the element to access
    /// @return a COPY of the element
    /// @exception IndexOutOfBoundsException
    ///
    bool operator()(unsigned long row, unsigned long col) const
    {
      assertIsInBounds(__FILE__, __LINE__, col, _cols);
      assertIsInBounds(__FILE__, __LINE__, row, _rows);
      return _array[row*_cols+col];
    }

    /// Transposes this BoolMatrix
    /// @return this BoolMatrix
    ///
    BoolMatrix& transpose()
    {
      unsigned long c, r, cc, rr;
      RealVector<bool> tmp = _array;
      bool* t = tmp.getArray();
      bool* p = _array.getArray();
      for (r=0, rr=0; r<_rows; r++, rr+=_cols)
        for (c=0, cc=0; c<_cols; c++, cc+=_rows)
          p[r+cc] = t[c+rr];
      r = _rows;
      _rows = _cols;
      _cols = r;
      return *this;
    }

    /// Transposes this constant BoolMatrix into a new BoolMatrix
    /// @return the new BoolMatrix
    ///
    BoolMatrix transpose() const
    {
      BoolMatrix tmp = *this;
      return tmp.transpose();
    }

    /// Saves a BoolMatrix depending on the saveMatrixFormat type
    /// @param f file name
    /// @return this BoolMatrix
    ///
    void save(const FileName& f) { save(f, Config()); }
    
    /// Saves a BoolMatrix depending on the saveMatrixFormat type
    /// @param f file name
    /// @param c configuration
    /// @return this BoolMatrix
    ///
    void save(const FileName& f, const Config& c)
    {
     if (c.getParam("saveMatrixFormat")=="DT") saveDT(f,c);
     else if (c.getParam("saveMatrixFormat")=="DB") saveDB(f,c); 
      else throw Exception("saveMatrixFormat unknown! DT (Dense Text) or DB (Dense Binary)",__FILE__,__LINE__);
    }
    
    /// Saves this BoolMatrix in a file (Dense Text BoolMatrix File Format)<br/>
    /// http://tedlab.mit.edu/~dr/SVDLIBC/SVD_F_DT.html
    /// @param f file name
    /// @param c configuration
    /// @return this BoolMatrix
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
    
    /// Save a BoolMatrix in a file (Dense Binary BoolMatrix format)<br/>
    /// @param f file name
    /// @param c configuration
    /// @return this BoolMatrix
    ///
    void saveDB(const FileName&f,const Config& c)
    {
      try {
            ofstream outputMat(f.c_str(),ios::out|ios::binary);
            if(!outputMat)
                throw IOException("Cannot open file", __FILE__, __LINE__,f);
            unsigned long rows=this->rows();
            unsigned long cols=this->cols();        
            bool* array=_array.getArray();


			if (sizeof(unsigned int) == 4){
				outputMat.write((char*)&rows,sizeof(unsigned int));
				outputMat.write((char*)&cols,sizeof(unsigned int));
			}
			else if (sizeof(unsigned long) == 4){
				outputMat.write((char*)&rows,sizeof(unsigned long));
				outputMat.write((char*)&cols,sizeof(unsigned long));
			}

            outputMat.write((char*)array,rows*cols*sizeof(bool));
            outputMat.close();
          }
          catch (Exception& e) {cout << e.toString().c_str() << endl;}
    }
    
    /// Loads a BoolMatrix depending on loadMatrixFormat
    /// @param f file name
    /// @return this BoolMatrix
    ///
    void load(const FileName& f) { load(f, Config()); }
    
    /// Loads a BoolMatrix depending on loadMatrixFormat
    /// @param f file name    
    /// @param c configuration
    /// @return this BoolMatrix
    ///
    void load(const FileName& f, const Config& c)
    {
      if (c.getParam("loadMatrixFormat")=="DT") loadDT(f,c);
      else if (c.getParam("loadMatrixFormat")=="DB") loadDB(f,c); 
      else throw Exception("loadMatrixFormat unknown! DT (Dense Text) or DB (Dense Binary)",__FILE__,__LINE__);
    }
    /// Loads a BoolMatrix from a file (Dense Text BoolMatrix File Format)<br/>
    /// http://tedlab.mit.edu/~dr/SVDLIBC/SVD_F_DT.html
    /// @param f file name
    /// @param c configuration
    /// @return this BoolMatrix
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
          (*this)(j, i) = s->toBool();
          i++;
        }
        j++;
      }
    }
    
    /// Loads a BoolMatrix from a file (Dense Binary BoolMatrix format)<br/>
    /// @param f file name
    /// @param c configuration
    /// @return this BoolMatrix
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

//            inputMat.read((char*)&cols,sizeof(cols)); 
            setDimensions(rows,cols);
            inputMat.read((char*)_array.getArray(),rows*cols*sizeof(bool));
            inputMat.close();
          }
          catch (Exception& e) {cout << e.toString().c_str() << endl;}
    }

    /// Use this method to access directly to the internal vector
    /// @return a pointer on the first element
    /// @warning Fast but dangerous ! Use preferably operator()(row, col).
    ///
    bool* getArray() const { return _array.getArray(); }

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

    virtual String getClassName() const { return "BoolMatrix"; }

    /// Extract a sub BoolMatrix from  the current one
    /// @return a submatrix
    /// @param line line number of the first selected element
    /// @param col column number of the first selected element
    /// @param n_rows number of line of the sub BoolMatrix
    /// @param n_cols number of columns of the sub BoolMatrix
    ///
    BoolMatrix crop(unsigned long line, unsigned long col, unsigned long n_rows, unsigned long n_cols)
    {
	if( (line + n_rows > _rows) || (col + n_cols > _cols) ){
		throw Exception("Cannot crop a such big BoolMatrix",__FILE__,__LINE__);
	}
	
	BoolMatrix subM(n_rows, n_cols);
	bool* subm=subM.getArray();
	bool* array = _array.getArray();
	
	for(unsigned long i=0; i<n_rows; i++){
		for(unsigned long j=0; j<n_cols; j++){
			subm[i*n_cols+j] =  array[(line+i)*_cols+col+j];
		}
	}
	return subM;
    } 
    
    /// Concatenate two matrices in column within the current one
    ///
    /// @param M1 the first BoolMatrix to concatenate
    /// @param M2 the second BoolMatrix to concatenate
    ///
    void concatCols(const BoolMatrix& M1, const BoolMatrix& M2){
	
	//Verify the number of columns of the two matrices
	if( M1.cols() != M2.cols() ){
		throw Exception("Cannot concatenate the given matrices, wrong dimensionality",__FILE__,__LINE__);
	}
	
	unsigned long nrows = M1.rows()+M2.rows();
	this->setDimensions(nrows,M1.cols());
	
	bool* m1=M1.getArray();
	bool* m2 = M2.getArray();

	for(unsigned long i=0; i<M1.cols()*M1.rows(); i++){
		_array[i] =  m1[i];
	}
	for(unsigned long i=0; i<M2.cols()*M2.rows(); i++){
		_array[i+M1.cols()*M1.rows()] =  m2[i];
	}
    }

    /// Concatenate two matrices in line within the current one
    ///
    /// @param M1 the first BoolMatrix to concatenate
    /// @param M2 the second BoolMatrix to concatenate
    ///
    void concatRows(const BoolMatrix& M1, const BoolMatrix& M2){
	
	//Verify the number of columns of the two matrices
	if( M1.rows() != M2.rows() ){
		throw Exception("Cannot concatenate the given matrices, wrong dimensionality",__FILE__,__LINE__);
	}
	
	unsigned long ncols = M1.cols()+M2.cols();
	this->setDimensions(M1.rows(),ncols);
	
	bool* m1=M1.getArray();
	bool* m2 = M2.getArray();
	
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
    RealVector<bool> _array;

  };

} // end namespace alize

#endif  // ALIZE_BoolMatrix_h

