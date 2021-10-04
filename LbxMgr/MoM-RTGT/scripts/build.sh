#!/bin/sh -e

# CONFIG
PLATFORM=Linux
COMPILER=gcc
VERSION=
[ -z "$QTDIR" ] && QTDIR="/usr/local/Trolltech/Qt-4.8.0"
GIT_REPOS="ssh://ilikeserena@git.code.sf.net/p/momrtgt/code"
CHECKOUTDIR="$PWD/checkout"
RELEASEDIR="$PWD/releases"
MAKE_ALL="make all"

# FUNCTIONS
BUILD=
CHECKOUT=
FETCH=
GENERATE=
TEST=
RELEASE=

usage() {
    echo "USAGE:"
    echo ""
    echo "\$ $0 [-bcfghrt] [-p <platform>] [-q <qtdir>] [-R <URL>] <version>"
    echo ""
    echo "-a           Do everything for a regular release"
    echo "             Equivalent to -bcfgrt"
    echo "-b           Compile all sources"
    echo "-c           Checkout to <version>"
    echo "-f           Fetch <version> from remote repository"
    echo "             Implies -c that the sources must be checked out to <version>"
    echo "-g           Generate source. Necessary for a new build"
    echo "-p <platform> Build for the platform 'Linux' or 'Windows'"
    echo "             Current value is '$PLATFORM'"
    echo "-q <qtdir>   Specify the location of Qt root directory"
    echo "             Current value is '$QTDIR'"
    echo "-R <URL>     Specify the location of the GIT repository"
    echo "             Current value is '$GIT_REPOS'"
    echo "             Implies -f that the sources must be fetched"
    echo "-r           Copy to release directory and compress"
    echo "-t           Run tests to verify the build"
    echo "<version>    Specify the version to build"
    echo "             This should be the name of a branch in the repository"
    echo ""
    echo "EXAMPLE:"
    echo ""
    echo "\$ $0 -a 0.2.1"
    echo ""
    echo "    This will check out branch 0.2.1 from the GIT repository to '$CHECKOUTDIR',"
    echo "    build everything and copy it to '$RELEASEDIR'"
    echo "    where the applications will be compressed as well, ready for upload."
    echo ""
    exit 2
}

# options may be followed by one colon to indicate they have a required argument
options=$(getopt -o abcfghp:q:R:rt -l all,build,checkout,fetch,generate,help,platform:,qtdir:,repository:,release,test -- "$@")

#set -- $options

while [ $# -gt 0 ]
do
    # for options with required arguments, an additional shift is required
    case $1 in
    (--) shift; break;;
    -a|--all)        BUILD=1 ; FETCH=1 ; CHECKOUT=1 ; GENERATE=1 ; RELEASE=1 ; TEST=1 ;;
    -b|--build)      BUILD=1 ;;
    -c|--checkout)   CHECKOUT=1 ;;
    -f|--fetch)      FETCH=1 ; CHECKOUT=1 ;;
    -g|--generate)   GENERATE=1 ;;
    -h|--help)       usage ; exit 2 ;;
    -p|--platform)   PLATFORM="$2" ; shift ;;
    -q|--qtdir)      QTDIR="$2" ; shift ;;
    -R|--repository) GIT_REPOS="$2" ; shift ; FETCH=1 ; CHECKOUT=1 ;;
    -r|--release)    RELEASE=1 ;;
    -t|--test)       TEST=1 ;;
    (-*)             echo "$0: error - unrecognized option $1" 1>&2 ; exit 2 ;;
    (*)              break ;;
    esac
    shift
done

# MAKE PLATFORM SPECIFIC CHOICES
case $PLATFORM in
    Linux)   COMPILER="gcc" ; MAKE_ALL="make -j4" ;;
    Windows) COMPILER="mingw" ; MAKE_ALL="make -j4" ;;
#    Windows) COMPILER="msvc2008" ; MAKE_ALL="nmake -f Makefile.Release" ;;
    *)       echo "$0: error - platform must be either 'Linux' or 'Windows' and not $PLATFORM" 1>&2 ; exit 2 ;;
esac

VERSION="$1"

echo "CHECKOUTDIR=$CHECKOUTDIR"
echo "COMPILER=$GCC"
echo "GIT_REPOS=$GIT_REPOS"
echo "MAKE_ALL=$MAKE_ALL"
echo "PLATFORM=$PLATFORM"
echo "QTDIR=$QTDIR"
echo "RELEASEDIR=$RELEASEDIR"
echo "VERSION=$VERSION"
echo ""

if [ -z "$VERSION" ]; then
    echo "$0: error - version expected" 1>&2
    exit 2
fi
shift

if [ $# -gt 0 ]; then
    echo "$0: error - no more arguments were expected: $@" 1>&2
    exit 2
fi


# FETCH from repository
if [ -n "$FETCH" ]; then
    echo "-------------------------"
    echo "CHECKOUT:"
    echo "Initialize git repository in '$CHECKOUTDIR'"
    mkdir -p "$CHECKOUTDIR"
    cd "$CHECKOUTDIR"
    git init

    echo "Fetch branch '$VERSION' to '$CHECKOUTDIR' from '$GIT_REPOS'"
    cd "$CHECKOUTDIR"
    git fetch "$GIT_REPOS" $VERSION
fi

# CHECK OUT to <version>
if [ -n "$CHECKOUT" ]; then
    echo "Check out '$VERSION' in '$CHECKOUTDIR'"
    cd "$CHECKOUTDIR"
    git checkout $VERSION
fi


# GENERATE
if [ -n "$GENERATE" ]; then
    echo "-------------------------"
    echo "GENERATE:"
    cd "$CHECKOUTDIR/MoMEditorTemplate"
    perl generate_code.pl
fi

# BUILD ALL
if [ -n "$BUILD" ]; then
    echo "-------------------------"
    echo "BUILD: MoMCustomizer"
    cd "$CHECKOUTDIR/MoMCustomizer"
    "$QTDIR/bin/qmake"
    $MAKE_ALL 

    echo "-------------------------"
    echo "BUILD: MoMCustomizer"
    cd "$CHECKOUTDIR/MoMTweaker"
    "$QTDIR/bin/qmake"
    $MAKE_ALL 
fi

# TEST
if [ -n "$TEST" ]; then
    echo "-------------------------"
    echo "EXECUTING TESTS:"
    cd "$CHECKOUTDIR/MoMTest"
    "$QTDIR/bin/qmake"
    $MAKE_ALL
    bin/MoMTest
fi

# MAKE RELEASE FOR UPLOAD
if [ -n "$RELEASE" ]; then
    # COPY TO RELEASE DIRECTORIES
    echo "-------------------------"
    echo "COPY TO RELEASE DIRECTORIES:"
    echo "Remove old release directories if any"
    rm -rf "$RELEASEDIR/MoMCustomizer-$VERSION-$PLATFORM/" || true
    rm -rf "$RELEASEDIR/MoMTweaker-$VERSION-$PLATFORM/" || true

    echo "Copy MoMCustomizer to '$RELEASEDIR/MoMCustomizer-$VERSION-$PLATFORM/'"
    mkdir "$RELEASEDIR/MoMCustomizer-$VERSION-$PLATFORM/"
    cp "$CHECKOUTDIR/MoMCustomizer/$COMPILER/bin/MoMCustomizer" "$RELEASEDIR/MoMCustomizer-$VERSION-$PLATFORM/MoMCustomizer-$VERSION"
    cp "$QTDIR/lib"/libQtCore.so.4.8.0 "$RELEASEDIR/MoMCustomizer-$VERSION-$PLATFORM/"
    cp "$QTDIR/lib"/libQtGui.so.4.8.0 "$RELEASEDIR/MoMCustomizer-$VERSION-$PLATFORM/"
# TODO: WINDOWS
#    cp "$QTDIR/bin"/QtCore4.dll "$RELEASEDIR/MoMCustomizer-$VERSION-$PLATFORM/"
#    cp "$QTDIR/bin"/QtGui4.dll "$RELEASEDIR/MoMCustomizer-$VERSION-$PLATFORM/"
# TODO: MSVC-redist?
    cp "$CHECKOUTDIR/scripts/MoMCustomizer.sh" "$RELEASEDIR/MoMCustomizer-$VERSION-$PLATFORM/"
    cp "$CHECKOUTDIR/MoMCustomizer"/*.txt "$RELEASEDIR/MoMCustomizer-$VERSION-$PLATFORM/"

    echo "Copy MoMCustomizer to '$RELEASEDIR/MoMTweaker-$VERSION-$PLATFORM/'"
    mkdir "$RELEASEDIR/MoMTweaker-$VERSION-$PLATFORM/"
    cp "$RELEASEDIR/MoMTweaker-TEMPLATE-$PLATFORM"/* "$RELEASEDIR/MoMTweaker-$VERSION-$PLATFORM/"
    cp "$CHECKOUTDIR/MoMTweaker/$COMPILER/bin/MoMTweaker" "$RELEASEDIR/MoMTweaker-$VERSION-$PLATFORM/MoMTweaker-$VERSION"
    cp "$QTDIR/lib"/libQtCore.so.4.8.0 "$RELEASEDIR/MoMTweaker-$VERSION-$PLATFORM/"
    cp "$QTDIR/lib"/libQtGui.so.4.8.0 "$RELEASEDIR/MoMTweaker-$VERSION-$PLATFORM/"
# TODO: WINDOWS
#    cp "$QTDIR/bin"/QtCore4.dll "$RELEASEDIR/MoMCustomizer-$VERSION-$PLATFORM/"
#    cp "$QTDIR/bin"/QtGui4.dll "$RELEASEDIR/MoMCustomizer-$VERSION-$PLATFORM/"
# TODO: MSVC-redist?
    cp "$CHECKOUTDIR/scripts/MoMTweaker.sh" "$RELEASEDIR/MoMTweaker-$VERSION-$PLATFORM/"
    cp "$CHECKOUTDIR/MoMTweaker"/*.txt "$RELEASEDIR/MoMTweaker-$VERSION-$PLATFORM/"

    # COMPRESS
    echo "-------------------------"
    echo "COMPRESS:"
    cd "$RELEASEDIR"
    echo "Remove old compressed files if any"
# TODO: WINDOWS
    rm "MoMCustomizer-$VERSION-$PLATFORM.tar.gz" || true
    rm "MoMTweaker-$VERSION-$PLATFORM.tar.gz" || true
    echo "Compress files"
    tar czf "MoMCustomizer-$VERSION-$PLATFORM.tar.gz" "MoMCustomizer-$VERSION-$PLATFORM/"
    tar czf "MoMTweaker-$VERSION-$PLATFORM.tar.gz" "MoMTweaker-$VERSION-$PLATFORM/"

    echo ""
    echo "<<< Did your remember to update the CHANGES.txt files? >>>"
    echo ""
fi

echo "Done!"

