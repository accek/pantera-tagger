# Pantera Tagger User's Guide #



## Installation ##

### Prerequisites ###

  * C++ Boost Libraries, namely:
    * Boost.System
    * Boost.Filesystem
    * Boost.Regex (ICU support not required)
    * Boost.Program\_Options
    * Boost.Serialization
    * Boost.IOStreams
    * Boost.DateTime
    * Boost.MPI
  * Some [MPI](http://en.wikipedia.org/wiki/Message_Passing_Interface) implementation, eg. [OpenMPI](http://www.open-mpi.org/)
  * [ICU4C libary](http://site.icu-project.org/)
  * [Morfeusz](http://nlp.ipipan.waw.pl/~wolinski/morfeusz/) (`libmorfeusz.so.0.6`, morphological analyzer for Polish)
  * Java (JDK)
  * libtcmalloc from [google-perftools](http://code.google.com/p/google-perftools/) v. 1.3 or later (optional, but noticeably increases efficiency). Please note that some distributions, namely Ubuntu, have older versions in their repositories.
  * automake, autoconf, libtool
  * [CMake](http://www.cmake.org/)

Two major third-party components: [TaKIPI](http://nlp.pwr.wroc.pl/takipi/) (another tagger of Polish; we use its morphological guesser) and [segment](http://sourceforge.net/projects/segment/) (sentencer) are distributed with Pantera.

### Installing prerequisites ###

If you do not have root access or just do not want to install the needed
prerequisites in a global location, feel free to use some folder in your
home directory. Ths guide will follow this way. So:

```
PREFIX=$HOME/pantera
mkdir -p $PREFIX/src
cd $PREFIX/src
echo "export PATH=\"$PREFIX/bin:\$PATH\"" >> ~/.profile  # or .bashrc or whatever
echo "export LD_LIBRARY_PATH=\"$PREFIX/lib:\$LD_LIBRARY_PATH\"" >> ~/.profile  # or .bashrc or whatever
```

At this point, better restart the shell.

The first potentially absent component will be the MPI library. For example the
newest Ubuntu does not distribute MPI any more. So:

```
wget "http://www.open-mpi.org/software/ompi/v1.4/downloads/openmpi-1.4.2.tar.bz2"
tar jxf openmpi-1.4.2.tar.bz2
cd openmpi-1.4.2
./configure --prefix=$PREFIX --without-memory-manager
make
make install
cd ..
```
The `--without-memory-manager` option is needed on some platforms to workaround an unidentified bug in OpenMPI's optional memory manager.

The other problem may be the missing Boost.MPI. In this case it is wise to compile
all the needed Boost components from source. To compile Boost, [bjam](http://sourceforge.net/projects/boost/files/boost-jam/3.1.18/boost-jam-3.1.18.tgz/download) binary is needed. Most distros have it.
Now let's move to installing Boost:

```
wget "http://sourceforge.net/projects/boost/files/boost/1.43.0/boost_1_43_0.tar.bz2/download"
[ -f download ] && mv download boost_1_43_0.tar.bz2  # some versions of wget save as 'download'
tar jxf boost_1_43_0.tar.bz2
cd boost_1_43_0
bjam --with-system --with-filesystem --with-mpi \
  --with-regex --with-program_options \
  --with-iostreams --prefix=$PREFIX \
  install
cd ..
```

If for some reason, bjam incorrectly detects the location of `mpicxx` binary,
help using
```
echo "using mpi : path_to_mpicxx ;" > ~/user-config.jam
```
and retry.

Next -- missing ICU4C.

```
wget "http://download.icu-project.org/files/icu4c/4.4.1/icu4c-4_4_1-src.tgz"
tar zxf icu4c-4_4_1-src.tgz
cd icu/source
./configure --prefix=$PREFIX
make
make install
cd ../..
```

Also make sure that the `pl_PL.utf8` locale is installed. You may test
this by compiling and running the following program (C++):
```
#include <iostream>
#include <locale>
main() { std::cout << std::locale("pl_PL.utf8").name() << std::endl; }
```
If the program is aborted with an exception, you need to install this
locale.

### Installing Pantera ###

```
svn checkout http://pantera-tagger.googlecode.com/svn/trunk/ pantera-tagger
cd pantera-tagger
tar -C third_party/morfeusz -zxf path/to/morfeusz.tgz
autoreconf -i
./configure --prefix=$PREFIX
make
```

If you like, you may run `make install`, but generally it is not needed.
Feel free to use the `pantera` binary inside `src` folder.

Also, if you installed some of the prerequisites manually, you may need to pass
some additional options to `./configure`. I personally use the following command:
```
./configure \
  MPICXX=$PREFIX/bin/mpicxx \
  --without-openmp \
  --with-boost=$PREFIX \
  --with-boost-libdir="$PREFIX/lib" \
  --with-boost-system=boost_system \
  --with-boost-filesystem=boost_filesystem \
  --with-boost-regex=boost_regex \
  --with-boost-program-options=boost_program_options \
  --with-boost-iostreams=boost_iostreams \
  --with-boost-serialization=boost_serialization \
  --with-boost-mpi=boost_mpi \
  --prefix="$PREFIX"
```

### Final notes ###

  * If you use different version of Morfeusz, you may need to adjust some paths in `third_party/morfeusz/Makefile.am`. If the version of Morfeusz used emits the new tagset, adjust `MORFEUSZ_TAGSET` in `src/nlpcommon/morfeusz.h` to `nkjp`.


## Usage ##

Well, don't forget about
```
./pantera --help
```

### Training ###

I assume there exists some folder called `training_data` which contains one or
more subfolders with handwritten golden tags, saved in XCES format `morph.xml`
(and there are no other files ending `.xml` looking inside like XCES).
```
./pantera \
  --tagset [ipipan|nkjp]
  --create-engine my_engine.btengine \
  --training-data traning_data/ \
  --threshold 6
```

### Tagging ###

```
./pantera \
  --tagset [ipipan|nkjp]
  --engine my_engine.btengine
  file_or_folder_xces_or_nkjp_one_or_more
```

Please note that
  * if the input file contains morphosyntactic annotation, you probably want to disable morphosyntactic annotation in Pantera, passing `--no-morph` option
  * if the input file is split into sentences or if you do not need sentencing, pass `--no-sentencer`

### Using older Morfeusz morphosyntactic analyzer ###

It is recommended to ask Marcin Woliński for a newer version of Morfeusz,
which supports newer Polish tagset -- NKJP. If using an older version, change the definition
of `MORFEUSZ_TAGSET` to `ipipan` in `src/nlpcommon/morfeusz.h`.

### Using system-wide installed Morfeusz ###

If you want to not use the downloaded Morfeusz, but you already have `libmorfeusz` installed, pass `--without-bundled-morfeusz` to `./configure`. You do not need to have any files in `third_party/morfeusz` in this case.

### Custom morphosyntactic dictionary ###

Sometimes morfeusz or guesser return strange interpretations for some forms.
You may create a custom morphosyntactic dictionary, which will be used instead
of morfeusz for some forms.

First you need to create it. The format is like below:
```
form
 [base]
 tag
 tag
 [base]
 tag
```
for example:
```
tak
 [tak]
 conj
 qub
 [taka]
 subst:pl:gen:f
```
Then use command line option `-d` and pass the name of the file as its argument.

Lookups to the custom dictionary are always case-insensitive.

### Running on multiple processors / multiple machines ###

To run Pantera using more than one process, just prefix the command
with `mpirun -np number_of_processes`, eg.
```
mpirun -np 4 ./pantera --engine my_engine.btengine --tagset nkjp some_folder
```
This also applies to training. It is even more important there especially
if the training corpus is large. If the tagger is compiled in 32-bit mode,
there may be not enough address space in one process to store all data.
Runing on multiple processors may alleviate this problem.

To run Pantera on more than one machine, please refer to MPI documentation.

### Output control ###

Pantera supports gzip-compressed output files for NKJP output format.
Use `-z` option to enable this option.

You may also use `--skip-done` option to skip processing files for which all output files exist.
Nowadays Pantera creates output files atomically, so it is not possible that a Pantera process stopped in the middle of writing an output file causes a subsequent run with `--skip-done` miss this file. This is implemented by writing output to temporary files first and then renaming them to final names.

## Problems and known issues ##

http://code.google.com/p/pantera-tagger/issues/list