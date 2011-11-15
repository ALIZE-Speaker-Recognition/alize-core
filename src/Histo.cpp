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

#if !defined(ALIZE_Histo_cpp)
#define ALIZE_Histo_cpp

#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <fstream>
#include <cstdio>
#include "Object.h"
#include "Histo.h"
#include "alizeString.h"
#include "RealVector.h"
#include "Exception.h"

using namespace std;
using namespace alize;

const Histo::TData Histo::EPS = 0.0000000000000000001;

//-------------------------------------------------------------------------
Histo::Histo(unsigned long nbBin)
:Object(), _nbBin(nbBin), _bound(nbBin+1, nbBin+1), _count(nbBin, nbBin) {} 
//-------------------------------------------------------------------------
Histo::Histo(const Histo& h)
:Object(), _nbBin(h._nbBin), _bound(h._bound), _count(h._count),
 _data(h._data) {}
//-------------------------------------------------------------------------
const Histo& Histo::operator=(const Histo& h)
{
  if (this != &h)
  {
    _nbBin = h._nbBin;
    _bound = h._bound;
    _count = h._count;
    _data  = h._data;
  }
  return *this;   
}
//-------------------------------------------------------------------------


// ******************************************************
// Class Histo distribution estimation, tools and writing
// ******************************************************

//-------------------------------------------------------------------------
Histo::TData Histo::lowerBound(unsigned long i) const
{
  if ((_nbBin == 0) || (i>=_nbBin))
    throw Exception("Index out of bounds", __FILE__, __LINE__);
  return _bound[i];
}
//-------------------------------------------------------------------------
Histo::TData Histo::higherBound(unsigned long i) const
{
  if ((_nbBin == 0) || (i>=_nbBin))
    throw Exception("Index out of bounds", __FILE__, __LINE__);
  return _bound[i+1];
}
//-------------------------------------------------------------------------
Histo::TData Histo::count(unsigned long i) const
{
  if ((_nbBin == 0) || (i>=_nbBin))
    throw Exception("Index out of bounds", __FILE__, __LINE__);
  return _count[i];
}
//-------------------------------------------------------------------------
Histo::TData Histo::operator()(TData score, int type) const
{
  if (type==0)
  {
    if (_nbBin == 0 || score < _bound[0] || score > _bound[_nbBin])
      return 0;
    unsigned long i;
    for (i=0; score>_bound[i+1]; i++)
      ;
    return _count[i];
  }
  if (type == 1)
  {
    if (_nbBin == 0 || score <= _bound[0])
      return 0;
    if (score >= _bound[_nbBin])
      return _nbBin-1;
    unsigned long i;
    for (i=0; score>_bound[i+1]; i++)
      ;
    return i;
  }
  throw Exception("Invalid type", __FILE__, __LINE__);
  return 0; // never called
}
//-------------------------------------------------------------------------
void Histo::accumulateValue(TData value) { _data.addValue(value); }
//-------------------------------------------------------------------------
void Histo::computeHisto(int opt)
{
  if (_data.size() < 2)
    throw Exception("Too few data to build the histo", __FILE__, __LINE__);
  if (_nbBin != 0)
  {
    unsigned long b = 0;// bin index
    const unsigned long nbData = _data.size();
  
    _data.ascendingSort(); // sort
    // build the histo
    unsigned long i=0; // Data index
    TData bound = (TData)nbData / (TData)_nbBin;
    _bound[0] = _data[0];
    TData nbValBin = 1; // Nb data for the current bin

    while (i<nbData)
    {   
      if (i < bound-1) // All data without the last
      {
        i++;
        nbValBin++;
      }
      else
      {   // We are on the last VALUE of the bin 
        i++;
        while ((i<nbData) && (_data[i]==_data[i-1]))
        { // In case of equal values
          nbValBin++;
          i++;
        }
        if (i == nbData)
          break;
        // Set the end of the bin and the begin of the next one...
        _bound[b+1] = _data[i];
      
        // we deal with the last data/bin after the loop
        if ((_bound[b+1]-_bound[b])>EPS) 
          _count[b] = nbValBin / (nbData*(_bound[b+1]-_bound[b]));
        else 
          _count[b] = EPS;
	
        // Initialize next bin
        b++;
        bound = i-1 + (TData)(nbData-i+1) / (_nbBin-b);
        nbValBin=1;
      }
    }

    // end of the current Bin
    _bound[b+1] = _data[nbData-1];
  
    if (_bound[b+1]-_bound[b] > EPS)
      _count[b] = nbValBin / (nbData*(_bound[b+1]-_bound[b]));
    else
      _count[b] = EPS;

    // Not used bins at the end
    for (b++; b<_nbBin; b++)
    {
      _bound[b+1] = _bound[b];
      _count[b] = 0;
    }
  }
 
  if (opt == 0)
  {
    _data.clear();
  }
  else
    throw Exception("Invalid option", __FILE__, __LINE__);


/*
  if (_nbBin != 0)
  {
    _data.ascendingSort(); // ascending sort
    unsigned long nbData = _data.size();
    unsigned long b = 0, i=0;

    TData width = _data[nbData-1] - _data[0];
    _bound[_nbBin] = _data[nbData-1];
    _bound[0] = _data[0];
    TData bound = _data[0] + width/_nbBin;
    _count[0] = 0.0;

    while (true)
    {
      if (_data[i] < bound || _data[i] == _bound[_nbBin])
      {
        _count[b]++;
        i++;
        if (i == nbData) // if no data 
          break; // leave loop
      }
      else
      {
        // end of the current bin
        _bound[b+1] = _data[i];
        if (_bound[b+1]-_bound[b] > EPS)
          _count[b] /= nbData*(_bound[b+1]-_bound[b]);
        else
          _count[b] = EPS;
        // go to next bin
        b++;
        bound = _data[0] + (width*(b+1))/_nbBin;
        _count[b] = 0.0;
      }
    }
    // end of the current bin
    if (_bound[b+1]-_bound[b] > EPS)
      _count[b] /= nbData*(_bound[b+1]-_bound[b]);
    else
      _count[b] = EPS;

    // Not used bins at the end
    for (b++; b<_nbBin; b++)
    {
      _bound[b] = _bound[b-1];
      _count[b] = 0.0;
    }
  }*/
 
  if (opt == 0)
  {
    _data.clear();
  }
  else
    throw Exception("Invalid option", __FILE__, __LINE__);

}

// Other, general IO, Public Functions for Histo 
//-------------------------------------------------------------------------
void Histo::load(const FileName& f)
{
  const char *fileName = f.c_str();
  char S[200];
  int nbBin;
  long i;
  float tmp1, tmp2;
  
  ifstream txtFile(fileName, ios::in);
  if (!txtFile)
    throw IOException("Cannot open file", __FILE__, __LINE__, f);
  txtFile >> nbBin;    
  Histo hTmp(nbBin);
  i = 0;
  txtFile.getline(S,200);
  while ((i<nbBin) && txtFile)
  {
    if ((S[0]!='#') && (S[0]!=0) && (S[0]!=13)&& (S[0]!=10))
    {
      sscanf(S, "%f  %f", &tmp1, &tmp2);
      hTmp._bound[i] = tmp1;
      hTmp._count[i] = tmp2;
      i++;   
    }
    txtFile.getline(S, 200);
  }
  if (i == nbBin)
  {
    sscanf(S, "%f", &tmp1);
    hTmp._bound[nbBin] = tmp1;
  }
  else
    throw IOException("Not enough data in file", __FILE__, __LINE__, f);
     
   (*this) = hTmp;   
}
//-------------------------------------------------------------------------
void Histo::save(const FileName& f) const
{
  ofstream txtFile(f.c_str(), ios::out);
  if (!txtFile)
    throw IOException("Cannot open file", __FILE__, __LINE__, f);
  txtFile << _nbBin << endl;
  for (unsigned long i=0; i<_nbBin; i++)
    txtFile << _bound[i] << "	" << _count[i] << endl;
  txtFile << _bound[_nbBin] << endl;
}   
//-------------------------------------------------------------------------
void Histo::saveGnuplot(const FileName& f) const
{
  ofstream txtFile(f.c_str(), ios::out);
  if (!txtFile)
    throw IOException("Cannot open file", __FILE__, __LINE__, f);
  txtFile << _bound[0] << " " << 0<< endl; 

  for (unsigned long i=0; i<_nbBin; i++)
  {
    txtFile << _bound[i] << " " << _count[i]<< endl; 
    txtFile << _bound[i+1] << " " << _count[i]<< endl; 
  }
  txtFile << _bound[_nbBin] << " " << 0<< endl; 
}    
//-------------------------------------------------------------------------
// Divise each bin by factor
//-------------------------------------------------------------------------
void Histo::div(real_t factor)
{
  if (factor == 0.0) 
    throw Exception("factor cannot be 0.0", __FILE__, __LINE__);
  for (unsigned long i=0; i<_nbBin ;i++)
  { _count[i] /= factor; }
}
//-------------------------------------------------------------------------
unsigned long Histo::size() const { return _nbBin; }
//-------------------------------------------------------------------------
String Histo::getClassName() const { return "Histo"; }
//-------------------------------------------------------------------------
String Histo::toString() const
{
  unsigned long i;
  real_t sum = 0.0;
  String s = Object::toString() + " Nb bin = " + String::valueOf(_nbBin);
  for (i=0; i<_nbBin; i++)
  {
    s += "\n  " + String::valueOf(_bound[i]) + " : "
    + String::valueOf(_count[i]);
    sum += (_bound[i+1] - _bound[i]) * _count[i];
  }
  if (_nbBin != 0)
    s += "\n  " + String::valueOf(_bound[i]);
  return s;
}
//-------------------------------------------------------------------------
Histo::~Histo() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_Histo_cpp)
