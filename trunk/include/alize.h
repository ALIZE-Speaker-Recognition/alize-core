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

#if !defined(ALIZE_alize_h)
#define ALIZE_alize_h

#include "AutoDestructor.h"
#include "Exception.h"
#include "alizeString.h"
#include "RealVector.h"
#include "RefVector.h"
#include "Matrix.h"
#include "BoolMatrix.h"
#include "DoubleSquareMatrix.h"
#include "ULongVector.h"
#include "Config.h"
#include "Label.h"
#include "Histo.h"
#include "XLine.h"
#include "XList.h"
#include "LabelSet.h"
#include "CmdLine.h"
#include "Histo.h"

#include "Seg.h"
#include "SegCluster.h"
#include "SegServer.h"
#include "SegServerFileWriter.h"
#include "SegServerFileReaderRaw.h"

#include "DistribGD.h"
#include "DistribGF.h"
#include "MixtureGD.h"
#include "MixtureGF.h"
#include "FeatureFlags.h"
#include "Feature.h"

#include "LabelServer.h"
#include "MixtureServer.h"
#include "FeatureServer.h"
#include "MixtureStat.h"
#include "MixtureGDStat.h"
#include "MixtureGFStat.h"
#include "FrameAcc.h"
#include "FrameAccGD.h"
#include "FrameAccGF.h"
#include "StatServer.h"

#include "FeatureMultipleFileReader.h"
#include "FeatureFileReaderRaw.h"
#include "FeatureFileReaderSPro3.h"
#include "FeatureFileReaderSPro4.h"
#include "FeatureFileReaderHTK.h"
#include "FeatureFileReader.h"
#include "FeatureInputStreamModifier.h"
#include "MixtureFileReaderAmiral.h"
#include "MixtureFileReaderRaw.h"
#include "MixtureFileReaderXml.h"
#include "MixtureFileReader.h"
#include "MixtureFileWriter.h"
#include "MixtureServerFileWriter.h"
#include "MixtureServerFileReader.h"
#include "MixtureServerFileReaderXml.h"
#include "MixtureServerFileReaderRaw.h"
#include "FeatureFileWriter.h"
#include "ConfigFileReaderRaw.h"
#include "ConfigFileReaderXml.h"
#include "ConfigFileWriter.h"
#include "SegServerFileWriter.h"
#include "XListFileReader.h"
#include "LabelFileReader.h"
#include "ViterbiAccum.h"
#include "FeatureFileList.h"
#include "FileReader.h"
#include "AudioFrame.h"
#include "AudioFileReader.h"

#include "ConfigChecker.h"

#endif // !defined(ALIZE_alize_h)

