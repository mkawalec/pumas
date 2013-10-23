reset
set term gif size 540,540 animate
set output "animate.gif"
n=100   #n frames
set xlabel "X Coordinate"
set ylabel "Y Coordinate"
i=0
load "animate.gnuplot"
set output

