/*
Alize is a free, open tool for speaker recognition

Alize is a development project initiated by the ELISA consortium
  [www.lia.univ-avignon.fr/heberges/ALIZE/ELISA] and funded by the
  French Research Ministry in the framework of the
  TECHNOLANGUE program [www.technolangue.net]
  [www.technolangue.net]

The Alize project team wants to highlight the limits of voice 
  authentication in a forensic context.
  The following paper proposes a good overview of this point:
  [Bonastre J.F., Bimbot F., Boe L.J., Campbell J.P., Douglas D.A., 
  Magrin-chagnolleau I., Person  Authentification by Voice: A Need of 
  Caution, Eurospeech 2003, Genova]
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
  the use of Alize

Copyright (C) 2003-2005
  Laboratoire d'informatique d'Avignon [www.lia.univ-avignon.fr]
  Frederic Wils [frederic.wils@lia.univ-avignon.fr]
  Jean-Francois Bonastre [jean-francois.bonastre@lia.univ-avignon.fr]
      
This file is part of Alize.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#if !defined(ALIZE_TestMatrix_cpp)
#define ALIZE_TestMatrix_cpp

#include "TestMatrix.h"
#include "Matrix.h"
#include "alizeString.h"
#include "Exception.h"
#include <cmath>
#include <iostream>

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestMatrix::TestMatrix()
:Test() {}
//-------------------------------------------------------------------------
bool TestMatrix::exec()
{
  Test::exec();
  {
    typedef DoubleMatrix M;

    // test method : String getClassName() const

    test(M(1).getClassName() == "Matrix", __LINE__);

    // test method : explicit Matrix(unsigned long rows, unsigned long cols);
    {
      M m;
      test(m._rows == 0, __LINE__);
      test(m._cols == 0, __LINE__);
      test(M(0)._rows == 0, __LINE__);
      test(M(0)._cols == 0, __LINE__);
      test(M(1)._rows == 1, __LINE__);
      test(M(1)._cols == 0, __LINE__);
    }

    // test method : unsigned long cols() const;
    {
      test(M(5,0).cols() == 0, __LINE__);
      test(M(5,1).cols() == 1, __LINE__);
      test(M(5,99).cols() == 99, __LINE__);
    }

    // test method : unsigned long rows() const;
    {
      test(M(0, 5).rows() == 0, __LINE__);
      test(M(1, 5).rows() == 1, __LINE__);
      test(M(99, 5).rows() == 99, __LINE__);
    }

    // test method : double operator()(unsigned long i,
    //                 unsigned long j) const;
    {
      M m1(2,2);
      m1(0,0) = 10;
      m1(0,1) = 11;
      m1(1,0) = 12;
      m1(1,1) = 13;
      const M m(m1);
      try { m(3,0); showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}
      try { m(0,3); showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}

      test(m(0,0) == m1(0,0), __LINE__);
      test(m(0,1) == m1(0,1), __LINE__);
      test(m(1,0) == m1(1,0), __LINE__);
      test(m(1,1) == m1(1,1), __LINE__);
    }

    // test method : double& operator()(unsigned long i, unsigned long j)
    {
      M m(2,2);
      try { m(3,0); showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}
      try { m(0,3);  showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}

      m._array[0+0*m.cols()]= 1.2;
      m._array[0+1*m.cols()]= 3.4;
      m._array[1+0*m.cols()]= 5.6;
      m._array[1+1*m.cols()]= 7.8;
      test(m(0,0) == 1.2, __LINE__);
      test(m(1,0) == 3.4, __LINE__);
      test(m(0,1) == 5.6, __LINE__);
      test(m(1,1) == 7.8, __LINE__);

      m(0,0) = 10;
      m(1,0) = 11;
      m(0,1) = 12;
      m(1,1) = 13;
      test(m(0,0) == 10, __LINE__);
      test(m(1,0) == 11, __LINE__);
      test(m(0,1) == 12, __LINE__);
      test(m(1,1) == 13, __LINE__);
    }

    // test method : Matrix(const Matrix<T>&);
    {
      M m1(2,2);
      m1(0,0) = 10;
      m1(0,1) = 11;
      m1(1,0) = 12;
      m1(1,1) = 13;
      const M m(m1);
      test(m.cols() == m1.cols(), __LINE__);
      test(m.rows() == m1.rows(), __LINE__);
      test(m(0,0) == m1(0,0), __LINE__);
      test(m(0,1) == m1(0,1), __LINE__);
      test(m(1,0) == m1(1,0), __LINE__);
      test(m(1,1) == m1(1,1), __LINE__);
    }

    // test method : template class<R> explicit Matrix(const Realvector<R>& v);
    {
      RealVector<double> v;
      v.addValue(1);
      v.addValue(2);
      v.addValue(3);
      v.addValue(4);
      const M m(v);
      test(m.cols() == v.size(), __LINE__);
      test(m.rows() == 1, __LINE__);
      test(m(0,0) == v[0], __LINE__);
      test(m(0,1) == v[1], __LINE__);
      test(m(0,2) == v[2], __LINE__);
      test(m(0,3) == v[3], __LINE__);
    }

    // test method : explicit Matrix(const Feature& v);
    {
      Feature v(4);
      v[0] = 1;
      v[1] = 2;
      v[2] = 3;
      v[3] = 4;
      const M m(v);
      test(m.cols() == v.getVectSize(), __LINE__);
      test(m.rows() == 1, __LINE__);
      test(m(0,0) == v[0], __LINE__);
      test(m(0,1) == v[1], __LINE__);
      test(m(0,2) == v[2], __LINE__);
      test(m(0,3) == v[3], __LINE__);
    }

    // test method : void setAllValue(T v)
    {
      double v = 1.2;
      M m(2,2);
      m.setAllValues(v);
      test(m(0,0) == v, __LINE__);
      test(m(0,1) == v, __LINE__);
      test(m(1,0) == v, __LINE__);
      test(m(1,1) == v, __LINE__);
    }

    // test method : double* getArray() const;
    {
      M m(2,3);
      test(m._array.getArray() == m.getArray(), __LINE__);
    }

    // test method : bool operator==(const Matrix<T>&) const
    //  and method : bool operator!=(const Matrix<T>&) const
    {
      M m(2,2);
      m(0,0) = 10;
      m(0,1) = 11;
      m(1,0) = 12;
      m(1,1) = 13;
      
      M m1(m);
      test(m == m1, __LINE__);
      test(!(m != m1), __LINE__);

      m(0,0) = 0;
      test(!(m == m1), __LINE__);
      test(m != m1, __LINE__);
      m(0,0) = 10;

      m(1,0) = 0;
      test(!(m == m1), __LINE__);
      test(m != m1, __LINE__);
    }

    // test method : const Matrix<T>& operator=(const Matrix<T>& v)
    {
      M m(2,2);
      m(0,0) = 10;
      m(0,1) = 11;
      m(1,0) = 12;
      m(1,1) = 13;
      try
      {
        M m1(2);
        M m2(m);
        m1 = m;
        test(m1 == m, __LINE__);
        m2 = m2; // auto affectation
        test(m2 == m, __LINE__);
        M m3(3);
        m3 = m;
        test(m3 == m, __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }      
    }

    // test method : void setDimensions(unsigned long cols, unsigned long rows)
    {
      M m(2,3);
      m.setDimensions(10,20);
      test(m._cols == 20, __LINE__);
      test(m._rows == 10, __LINE__);
      test(m._array.size() == 10*20, __LINE__);
    }

    // test load(const FileName&)
    {
      const String fileName = "testMatrix0.txt";
      XList liste;
      liste.addLine().addElement("4").addElement("3");
      liste.addLine().addElement("8").addElement("0").addElement("42");
      liste.addLine().addElement("0").addElement("56").addElement("22");
      liste.addLine().addElement("2").addElement("0").addElement("5");
      liste.addLine().addElement("0").addElement("0").addElement("0");
      liste.save(fileName);

      M m(fileName); // load matrix
      remove(fileName.c_str());
      test(m.rows() == 4, __LINE__);
      test(m.cols() == 3, __LINE__);
      for (int rows=0; rows<m.rows(); rows++)
        for (int cols=0; cols<m.cols(); cols++)
          test(String::valueOf(m(rows, cols)) == liste.getLine(rows+1).getElement(cols), __LINE__);
    }
    
    // test save(const FileName&)
    {
      const String fileName = "testMatrix0.txt";

      M m;

      m.save(fileName);
      M m1(fileName);
      remove(fileName.c_str());
      test(m == m1, __LINE__);
    }

    // test Matrix<T>& operator*=(double v)
    {
      M m(2,2);
      m(0,0) = 1;
      m(0,1) = 2;
      m(1,0) = 3;
      m(1,1) = 4;
      m *= 2.0;
      test(m(0,0) == 2, __LINE__);
      test(m(0,1) == 4, __LINE__);
      test(m(1,0) == 6, __LINE__);
      test(m(1,1) == 8, __LINE__);
    }
    // test Matrix<T>& operator*=(const Matrix<T>&)
    {
      M m1(2,2);
      m1(0,0) = 1;
      m1(0,1) = 0;
      m1(1,0) = -1;
      m1(1,1) = 3;

      M m2(2,2);
      m2(0,0) = 3;
      m2(0,1) = 1;
      m2(1,0) = 2;
      m2(1,1) = 1;

      m1 *= m2;

      test(m1(0,0) == 3, __LINE__);
      test(m1(0,1) == 1, __LINE__);
      test(m1(1,0) == 3, __LINE__);
      test(m1(1,1) == 2, __LINE__);

      M m3(4,3);
      m3(0,0) = 1;
      m3(0,1) = 2;
      m3(0,2) = 3;
      m3(1,0) = 4;
      m3(1,1) = 5;
      m3(1,2) = 6;
      m3(2,0) = 7;
      m3(2,1) = 8;
      m3(2,2) = 9;
      m3(3,0) = 10;
      m3(3,1) = 11;
      m3(3,2) = 12;

      M m4(3,4);
      m4(0,0) = 10;
      m4(0,1) = 11;
      m4(0,2) = 12;
      m4(0,3) = 13;
      m4(1,0) = 14;
      m4(1,1) = 15;
      m4(1,2) = 16;
      m4(1,3) = 17;
      m4(2,0) = 18;
      m4(2,1) = 19;
      m4(2,2) = 20;
      m4(2,3) = 21;

      m3 *= m4;

      test(m3(0,0) == 92, __LINE__);
      test(m3(0,1) == 98, __LINE__);
      test(m3(0,2) == 104, __LINE__);
      test(m3(0,3) == 110, __LINE__);
      test(m3(1,0) == 218, __LINE__);
      test(m3(1,1) == 233, __LINE__);
      test(m3(1,2) == 248, __LINE__);
      test(m3(1,3) == 263, __LINE__);
      test(m3(2,0) == 344, __LINE__);
      test(m3(2,1) == 368, __LINE__);
      test(m3(2,2) == 392, __LINE__);
      test(m3(2,3) == 416, __LINE__);
      test(m3(3,0) == 470, __LINE__);
      test(m3(3,1) == 503, __LINE__);
      test(m3(3,2) == 536, __LINE__);
      test(m3(3,3) == 569, __LINE__);
    }

    // test Matrix<T> operator*(const Matrix<T>&) const
    {
      M m1(2,2);
      m1(0,0) = 1;
      m1(0,1) = 0;
      m1(1,0) = -1;
      m1(1,1) = 3;

      M m4(2,2);
      m4(0,0) = 3;
      m4(0,1) = 1;
      m4(1,0) = 2;
      m4(1,1) = 1;

      M m2 = m1 * m4;

      test(m2(0,0) == 3, __LINE__);
      test(m2(0,1) == 1, __LINE__);
      test(m2(1,0) == 3, __LINE__);
      test(m2(1,1) == 2, __LINE__);
    }
    // test Matrix<T> operator+(const Matrix<T>&) const
    {
      M m1(2,2);
      m1(0,0) = 1;
      m1(0,1) = 0;
      m1(1,0) = -1;
      m1(1,1) = 3;

      M m4(2,2);
      m4(0,0) = 3;
      m4(0,1) = 1;
      m4(1,0) = 2;
      m4(1,1) = 1;

      M m2 = m1 + m4;

      test(m2(0,0) == m1(0,0)+m4(0,0), __LINE__);
      test(m2(0,1) == m1(0,1)+m4(0,1), __LINE__);
      test(m2(1,0) == m1(1,0)+m4(1,0), __LINE__);
      test(m2(1,1) == m1(1,1)+m4(1,1), __LINE__);
    }
    // test Matrix<T>& operator+=(const Matrix<T>&)
    {
      M m1(2,2);
      m1(0,0) = 1;
      m1(0,1) = 0;
      m1(1,0) = -1;
      m1(1,1) = 3;

      M m4(2,2);
      m4(0,0) = 3;
      m4(0,1) = 1;
      m4(1,0) = 2;
      m4(1,1) = 1;

      M m2 = m1;

      m2 += m4;

      test(m2(0,0) == m1(0,0)+m4(0,0), __LINE__);
      test(m2(0,1) == m1(0,1)+m4(0,1), __LINE__);
      test(m2(1,0) == m1(1,0)+m4(1,0), __LINE__);
      test(m2(1,1) == m1(1,1)+m4(1,1), __LINE__);
    }
    // test Matrix<T> operator-(const Matrix<T>&) const
    {
      M m1(2,2);
      m1(0,0) = 1;
      m1(0,1) = 0;
      m1(1,0) = -1;
      m1(1,1) = 3;

      M m4(2,2);
      m4(0,0) = 3;
      m4(0,1) = 1;
      m4(1,0) = 2;
      m4(1,1) = 1;

      M m2 = m1 - m4;

      test(m2(0,0) == m1(0,0)-m4(0,0), __LINE__);
      test(m2(0,1) == m1(0,1)-m4(0,1), __LINE__);
      test(m2(1,0) == m1(1,0)-m4(1,0), __LINE__);
      test(m2(1,1) == m1(1,1)-m4(1,1), __LINE__);
    }
    // test Matrix<T>& operator-=(const Matrix<T>&)
    {
      M m1(2,2);
      m1(0,0) = 1;
      m1(0,1) = 0;
      m1(1,0) = -1;
      m1(1,1) = 3;

      M m4(2,2);
      m4(0,0) = 3;
      m4(0,1) = 1;
      m4(1,0) = 2;
      m4(1,1) = 1;

      M m2 = m1;

      m2 -= m4;

      test(m2(0,0) == m1(0,0)-m4(0,0), __LINE__);
      test(m2(0,1) == m1(0,1)-m4(0,1), __LINE__);
      test(m2(1,0) == m1(1,0)-m4(1,0), __LINE__);
      test(m2(1,1) == m1(1,1)-m4(1,1), __LINE__);
    }

    // test void Matrix<T>& transpose()
    {
      M m3(3,4);
      m3(0,0) = 10;
      m3(0,1) = 11;
      m3(0,2) = 12;
      m3(0,3) = 13;
      m3(1,0) = 14;
      m3(1,1) = 15;
      m3(1,2) = 16;
      m3(1,3) = 17;
      m3(2,0) = 18;
      m3(2,1) = 19;
      m3(2,2) = 20;
      m3(2,3) = 21;

      M m4 = m3.transpose();
      test(m4 == m3, __LINE__);

      test(m3.cols() == 3, __LINE__);
      test(m3.rows() == 4, __LINE__);

      test(m3(0,0) == 10, __LINE__);
      test(m3(0,1) == 14, __LINE__);
      test(m3(0,2) == 18, __LINE__);
      test(m3(1,0) == 11, __LINE__);
      test(m3(1,1) == 15, __LINE__);
      test(m3(1,2) == 19, __LINE__);
      test(m3(2,0) == 12, __LINE__);
      test(m3(2,1) == 16, __LINE__);
      test(m3(2,2) == 20, __LINE__);
      test(m3(3,0) == 13, __LINE__);
      test(m3(3,1) == 17, __LINE__);
      test(m3(3,2) == 21, __LINE__);
    }
    // test void Matrix<T> transpose() const
    {
      M m(3,4);
      m(0,0) = 10;
      m(0,1) = 11;
      m(0,2) = 12;
      m(0,3) = 13;
      m(1,0) = 14;
      m(1,1) = 15;
      m(1,2) = 16;
      m(1,3) = 17;
      m(2,0) = 18;
      m(2,1) = 19;
      m(2,2) = 20;
      m(2,3) = 21;

      const M m1 = m;
      M m3 = m1.transpose();
      m3.transpose();
      test(m1 == m3, __LINE__);
    }

    // test void Matrix<T>& invert()
    {
      /*M m(4,4);
      int i, j;
      int n = m.cols();
      for (i=0;i<n;i++)
        for (j=0;j<n;j++)
            if (i==j)
              m(i, j)=i*j;
            else
              m(i,j)=1;
      M m1 = m;
      cout << m.toString() << endl;
      m.invert();
      cout << m.toString() << endl;
      m.invert();
      test(m == m1, __LINE__);*/
      unsigned long ii,jj;
      M m2(2,2);
      m2(0,0) = 30;
      m2(0,1) = 20;
      m2(1,0) = 20;
      m2(1,1) = 36;
      M mi2 = m2;
      //cout << m2.toString() << endl;
      m2.invert();
      //cout << m2.toString() << endl;
      m2.invert();
      //cout << m2.toString() << endl;
      for (ii=0; ii<2; ii++)
        for (jj=0; jj<2; jj++)
          test (fabs(mi2(ii,jj)-m2(ii,jj)) < 1e-13, __LINE__);

      M m3(3,3);
      double a = m3(0,0) = 10;
      double b = m3(0,1) = 2;
      double c = m3(0,2) = 3;
      double d = m3(1,0) = 2;
      double e = m3(1,1) = 5;
      double f = m3(1,2) = 1;
      double g = m3(2,0) = 3;
      double h = m3(2,1) = 1;
      double i = m3(2,2) = 7;
      M mi3 = m3;
      m3.invert();
      m3.invert();
      for (ii=0; ii<3; ii++)
        for (jj=0; jj<3; jj++)
          test (fabs(mi3(ii,jj)-m3(ii,jj)) < 1e-13, __LINE__);

      m3(0, 0) = 3;
      m3(1, 0) = 2;
      m3(2, 0) = 1;
      m3(0, 1) = 2;
      m3(1, 1) = 2;
      m3(2, 1) = 1;
      m3(0, 2) = 1;
      m3(1, 2) = 1;
      m3(2, 2) = 1;
      mi3 = m3;
      m3.invert();
      m3.invert();
      for (ii=0; ii<3; ii++)
        for (jj=0; jj<3; jj++)
          test (fabs(mi3(ii,jj)-m3(ii,jj)) < 1e-13, __LINE__);

      // dimension = 4 // valeurs récupérées sur http://www.cs.ut.ee/~toomas_l/linalg/lin2/node24.html
      M m4(4,4);
      m4(0,0) = 1;
      m4(1,0) = 2;
      m4(2,0) = -1;
      m4(3,0) = 2;
      m4(0,1) = 2;
      m4(1,1) = 8;
      m4(2,1) = 6;
      m4(3,1) = 0;
      m4(0,2) = -1;
      m4(1,2) = 6;
      m4(2,2) = 21;
      m4(3,2) = -2;
      m4(0,3) = 2;
      m4(1,3) = 0;
      m4(2,3) = -2;
      m4(3,3) = 25;
      M mi4 = m4;
      m4.invert(); // inverse
      m4.invert(); // inverse de l'inverse
      for (ii=0; ii<4; ii++)
        for (jj=0; jj<4; jj++)
          test (fabs(mi4(ii,jj)-m4(ii,jj)) < 1e-12, __LINE__);

   }

    // test void Matrix<T> invert() const
    {
      M m(3,3);
      m(0,0) = 10;
      m(0,1) = 11;
      m(0,2) = 12;
      m(1,0) = 14;
      m(1,1) = 15;
      m(1,2) = 16;
      m(2,0) = 18;
      m(2,1) = 19;
      m(2,2) = 20;

      const M m1 = m;
      M m3 = m1.invert();
      m3.invert();
      for (unsigned long ii=0; ii<3; ii++)
        for (unsigned long jj=0; jj<3; jj++)
          test (fabs(m1(ii,jj)-m(ii,jj)) < 1e-13, __LINE__);
    }



  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestMatrix::getClassName() const
{ return "TestMatrix"; }
//-------------------------------------------------------------------------
TestMatrix::~TestMatrix() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestMatrix_cpp)
