#!/bin/sh -xe

# Function
APPLYPATCH=0
CREATEPATCH=0

# Parameters
BASELINEDIR=
WORKDIR=
PATCHFILE=
FILES=

usage() {
    printf "USAGE:\n"
    printf "\n"
    printf "To create a patch:\n"
    printf "   %s -c -f <from-dir> -t <to-dir> -p <patchfile>\n" "$0"
    printf "To apply a patch to the <to-dir>:\n"
    printf "   %s -a -p <patchfile> -t <to-dir>\n" "$0"
    printf "\n"
    printf "Examples:\n"
    printf "\n"
    printf "1) Create a patch from insecticide 1.40h to 1.40i:\n"
    printf "$ %s -c -f insecticide-1.40h -t insecticide-1.40i -p insecticide-1.40h-to-1.40i.patch\n" "$0"
    printf "\n"
    printf "2) Apply the patch to your MAGIC-work which is based on insecticide-1.40h:\n"
    printf "$ %s -a -p insecticide-1.40h-to-1.40i.patch -t MAGIC-work\n" "$0"
    printf "\n"
    exit 2
}

create_patch() {

    echo "Create patch file '$PATCHFILE' from '$BASELINEDIR' and '$WORKDIR'"
    echo "" > $PATCHFILE
    for FILE in $FILES ; do
        echo "Process files '$BASELINEDIR/$FILE' and '$WORKDIR/$FILE'"
        xxd -g1 $BASELINEDIR/$FILE $BASELINEDIR/$FILE.xxd
        xxd -g1 $WORKDIR/$FILE $WORKDIR/$FILE.xxd
        diff -Naur $BASELINEDIR/$FILE.xxd $WORKDIR/$FILE.xxd >> $PATCHFILE || true
        rm $BASELINEDIR/$FILE.xxd
        rm $WORKDIR/$FILE.xxd
    done
    echo "Patch file '$PATCHFILE' created"
    
}

apply_patch() {

    cp $PATCHFILE $WORKDIR
    cd "$WORKDIR"

    for FILE in $FILES ; do
        echo "Prepare file '$FILE'"
        xxd -g1 $FILE $FILE.xxd
    done
    
    echo "Apply patch file '$PATCHFILE'"
    patch -Np1 < $PATCHFILE
    
    for FILE in $FILES ; do
        echo "Overwrite '$FILE'"
        xxd -r -g1 $FILE.xxd $FILE
    done

    echo "Remove temporary files"
    for FILE in $FILES ; do
        rm $FILE.xxd
    done
    
    echo "Patch '$PATCHFILE' applied"
}


while getopts acf:t:p: name
do
    case $name in
    a)  APPLYPATCH=1 ;;
    c)  CREATEPATCH=1 ;;
    f)  BASELINEDIR="$OPTARG" ;;
    t)  WORKDIR="$OPTARG" ;;
    p)  PATCHFILE="$OPTARG" ;;
    ?)  usage ; exit 2 ;;
    esac
done
shift $(($OPTIND - 1))

echo "APPLYPATCH=$APPLYPATCH"
echo "CREATEPATCH=$CREATEPATCH"
echo "BASELINEDIR=$BASELINEDIR"
echo "WORKDIR=$WORKDIR"
echo "PATCHFILE=$PATCHFILE"

if [ "$CREATEPATCH" = "1" ]; then
    [ -z "$BASELINEDIR" ] && echo "Please specify a -f <from-dir>" && exit 1
    [ -z "$WORKDIR" ] && echo "Please specify a -t <to-dir>" && exit 1
    [ -z "$PATCHFILE" ] && echo "Please specify a -p <patchfile>" && exit 1
    FILES=`ls $WORKDIR`
    echo "FILES=$FILES"
    create_patch
fi

if [ "$APPLYPATCH" = "1" ]; then
    [ -n "$BASELINEDIR" ] && echo "Please do NOT specify a -f <from-dir>" && exit 1
    [ -z "$WORKDIR" ] && echo "Please do specify a -t <to-dir>" && exit 1
    [ -z "$PATCHFILE" ] && echo "Please specify a -p <patchfile>" && exit 1
    FILES=`grep -o '[^/]*\.xxd' $PATCHFILE | grep -o '^[^.]*\.[^.]*' | sort -u`
    echo "FILES=$FILES"
    apply_patch
fi
