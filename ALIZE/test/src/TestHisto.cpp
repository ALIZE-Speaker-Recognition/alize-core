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

#if !defined(ALIZE_TestHisto_cpp)
#define ALIZE_TestHisto_cpp

#include "TestHisto.h"
#include "Histo.h"
#include "Exception.h"
#include <iostream>
#include <cstdio>

using namespace std;

using namespace alize;

//-------------------------------------------------------------------------
TestHisto::TestHisto()
:Test() {}
//-------------------------------------------------------------------------
bool TestHisto::exec()
{
    Test::exec();
    {
        // test method : String getClassName() const
        {
            test(Histo(1).getClassName() == "Histo", __LINE__);
        }

        // test method : Histo(unsigned long nbBin = 10)
        {
            Histo h, h2(5);
            test(h._nbBin == 10, __LINE__);
            test(h2._nbBin == 5, __LINE__);
            test(h._bound.size() == 10+1, __LINE__);
            test(h._count.size() == 10, __LINE__);
            test(h._data.size() == 0, __LINE__);
        }

        // test method : void accumulateValue(Histo::TData value)
        {
            Histo h(5);
            h.accumulateValue(1.1);
            test(h._data.size()  == 1, __LINE__);
            test(h._data[0]  == 1.1, __LINE__);
            h.accumulateValue(2.2);
            test(h._data.size()  == 2, __LINE__);
            test(h._data[1]  == 2.2, __LINE__);
        }

        // test method : Histo(const Histo& h)
        {
            Histo h(5);
            h.accumulateValue(1);
            h.accumulateValue(2);
            h.accumulateValue(4);
            h.accumulateValue(5);
            h.accumulateValue(5);
            h.accumulateValue(10);
            h.accumulateValue(10);
            h.computeHisto();

            Histo h2(h);
            test(h2._nbBin == h._nbBin, __LINE__);
            test(h2._bound == h._bound, __LINE__);
            test(h2._count == h._count, __LINE__);
            //test(h2._data  == h._data, __LINE__);
        }
        
        // test method : const Histo& operator=(const Histo& h)
        {
            Histo h(5), h2;
            h.accumulateValue(1);
            h.accumulateValue(2);
            h.accumulateValue(4);
            h.accumulateValue(5);
            h.accumulateValue(5);
            h.accumulateValue(10);
            h.accumulateValue(10);
            h.computeHisto();

            h2 = h;
            test(h2._nbBin == h._nbBin, __LINE__);
            test(h2._bound == h._bound, __LINE__);
            test(h2._count == h._count, __LINE__);
            //test(h2._data  == h._data, __LINE__);
        }

        // test method : unsigned long size() const
        {
            Histo h(5);
            test(h.size()  == 5, __LINE__);
            h.accumulateValue(2.2);
            test(h.size()  == 5, __LINE__);
        }

        // test method : void computeHisto(int opt = 0)
        //  and method : TData operator() (TData score, int type = 0) const
        {
            {
                Histo h(3);
                h.accumulateValue(10);
                h.accumulateValue(15);
                h.accumulateValue(18);
                h.accumulateValue(19);
                h.accumulateValue(23);
                h.computeHisto();
                //cout << h.toString() << endl;
                /*  [10, 18] : 0.05
                    ]18, 19] : 0.2
                    ]19, 23] : 0.1    */

                test(h(0) == 0, __LINE__);
                test(h(5) == 0, __LINE__);
                test(h(10) == 0.05, __LINE__);
                test(h(11) == 0.05, __LINE__);
                test(h(14) == 0.05, __LINE__);
                test(h(15) == 0.05, __LINE__);
                test(h(16) == 0.05, __LINE__);
                test(h(17) == 0.05, __LINE__);
                test(h(18) == 0.05, __LINE__);
                test(h(18.001) == 0.2, __LINE__);
                test(h(19) == 0.2, __LINE__);
                test(h(23) == 0.1, __LINE__);
                test(h(24) == 0, __LINE__);

                test(h._data.size() == 0, __LINE__); // if parameter type = 0
            }
            {
                Histo h(3); // with no data
                try { h.computeHisto(); showError(__LINE__); }
                catch (Exception&) {}
                h.accumulateValue(10); // 1 data
                try { h.computeHisto(); showError(__LINE__); }
                catch (Exception&) {}
                h.accumulateValue(12); // more than 1 data
                try { h.computeHisto();}
                catch (Exception&) { showError(__LINE__); }
            }
            {
                Histo h(4);
                h.accumulateValue(9);
                h.accumulateValue(15);
                h.accumulateValue(15);
                h.accumulateValue(19);
                h.accumulateValue(24);
                h.computeHisto();
                //cout << h.toString() << endl;
                /*  [10, 18] : 0.05
                    ]18, 19] : 0.2
                    ]19, 23] : 0.1    */

                test(h(0) == 0, __LINE__);
                test(h(5) == 0, __LINE__);
                /* TODO : bug here !
                test(h(10) == 0.05, __LINE__);
                test(h(11) == 0.05, __LINE__);
                test(h(14) == 0.05, __LINE__);
                test(h(15) == 0.05, __LINE__);
                test(h(16) == 0.05, __LINE__);
                test(h(17) == 0.05, __LINE__);
                test(h(18) == 0.05, __LINE__);
                test(h(18.001) == 0.2, __LINE__);
                test(h(19) == 0.2, __LINE__);
                test(h(23) == 0.1, __LINE__);
                test(h(24) == 0, __LINE__);

                test(h._data.size() == 0, __LINE__); // if parameter type = 0
                */
            }
        }

        // test method : void save(const FileName&) const
        //  and method : void load(const FileName&)
        {
                Histo h(4), h1;
                h.accumulateValue(9);
                h.accumulateValue(15);
                h.accumulateValue(15);
                h.accumulateValue(19);
                h.accumulateValue(24);
                h.computeHisto();
                const String f("testHistoSave.txt.tmp");
                h.save(f);
                h1.load(f);
                test(h._bound == h1._bound, __LINE__);
                //test(h._count == h1._count, __LINE__); TODO : pb d'arrondis
                remove(f.c_str());

        }


    } // local objects are deleted here

    return endTest();
}
//-------------------------------------------------------------------------
String TestHisto::getClassName() const { return "TestHisto"; }
//-------------------------------------------------------------------------
TestHisto::~TestHisto() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestHisto_cpp)
