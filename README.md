<img src="http://alize.univ-avignon.fr/images/alize-core.png" alt="The ALIZÉ Core Library logo" height="198" >

# ALIZÉ Core Library

*This package is part of the ALIZÉ project: <http://alize.univ-avignon.fr>*



Welcome to ALIZÉ!
-----------------

The ALIZÉ Core library forms the foundation of the the [ALIZÉ platform](http://alize.univ-avignon.fr), an opensource platform for Automatic Speaker Recognition. Its focus is on low-level functions, mainly all the functions required to use Gaussian mixtures, as well as I/O functions for various file formats.
It is a base upon which various algorithms for speaker recognition can be implemented, such as was done with the `LIA_RAL` framework (the higher-level part of the ALIZÉ platform).

The ALIZÉ Core Library, like the rest of the ALIZÉ platform, is an open project. Feel free to contribute.


FAQ
---

#### What is needed to build and install the ALIZÉ Core library?

`aclocal`, `autoconf`, `automake` and `libtool` are required.


#### How to compile the ALIZÉ Core library under Linux, Mac OS and Cygwin

In a shell, run successively:

	aclocal
	automake
	autoconf
	./configure
	make

**Note:** At step 2, you may need to use `automake --add-missing` if the file `compile` cannot be found.


#### How to compile the ALIZÉ Core library under Windows with Visual C++

Use the `ALIZÉ.sln` solution file. If your version of Visual Studio is newer than 2010, you just have to convert it using Visual studio tools.


#### I have problems compiling ALIZÉ

Please read this technote:
<http://alize.univ-avignon.fr/mediawiki/index.php/Main_Page>

If you cannot find an answer to your problem, please feel free to contact the ALIZÉ community through [the dev-alize mailing list](mailto:dev-alize@listes.univ-avignon.fr) or [LinkedIn](https://www.linkedin.com/groups/2323703).


#### How to generate documentation

In a shell, in the main folder of ALIZÉ:

	doxygen doxygen.cfg

Of course, you need to have [`doxygen`](http://www.doxygen.org) installed.


#### ALIZÉ allows speaker verification/authentication but needs features as input. Where can I obtain them?

In in order to extract from the speech signal the kind of features needed by ALIZÉ, the ALIZÉ team recommends using [the SPro toolkit](http://spro.gforge.inria.fr), which is available under the MIT licence (starting with version 5.0).
 

------------------------------------------------------------

In case you encounter difficulties not listed here, please refer to:
<http://alize.univ-avignon.fr/mediawiki/index.php/Main_Page>
or reach out to the ALIZÉ community, through LinkedIn or the mailing list.

