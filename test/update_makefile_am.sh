echo "lib_LIBRARIES=libalize.a"> src/Makefile.am;
echo "libalize_a_SOURCES=" >> src/Makefile.am;
for i in src/*.cpp;do echo "`basename $i`\\";done >> src/Makefile.am
