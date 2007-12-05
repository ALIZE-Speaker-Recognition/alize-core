#!/bin/bash

clean()
{
	perl -e '
		system("make clean");
		open(LIST,"find *|") || die ("no find *");
		while(<LIST>)
		{
			chomp();
			(/Makefile.in$/ ||
			/Makefile$/ ) &&
			unlink($_) &&
			print "$_\n";
		}
		$list="aclocal.m4 config.cache config.log config.status configure \
		 INSTALL COPYING install-sh missing mkinstalldirs confdefs.h \
		 config.guess config.sub libtool ltmain.sh";
		foreach $file(split(/ /,$list))
		{
			unlink($file) &&
			print "$file\n";
		}'
}

build()
{
	aclocal \
	&& automake -a \
	&& autoconf \
	&& ./configure \
	&& make
}

case "$1" in
	rebuild)
	clean
	build
	;;
	clean)
	clean
	;;
	build|*)
	build
esac

cp src/libalize.a lib


