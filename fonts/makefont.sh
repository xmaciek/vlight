#!/bin/bash
for i in {32..127} 
do 



declare -i x=$i
c=$(printf \\$(printf '%03o' $x))
# s=$(printf "\x`echo $i`")
convert -background none -fill white -font DejaVuSansMono-Bold.ttf -pointsize 14 label:"`echo $c`" $i.tga

done;
