Procedure to turn an asm file into a binary buffer
--------------------------------------------------
1. Start DOSBox and make tasm accessible.
2. C> tasm /la file.asm
3. C> tdump file.obj > tdump.txt
4. Open tdump.txt and copy and paste the hex data.
5. Convert to C++ with for instance (take care of the \r one way or another):
   perl -pe 'if (s#^\s+(\w\w\w\w:)\s*(.*?)\s+\S+\r$#    $2\r#) { $a=$1; s#\b(\w\w)\b#0x$1,#g; s#\r$# // $a\r#; }' tdump.txt
