reset
set term gif size 540,540 animate
set output "animate.gif"
n=1000   #n frames
set cbrange [0:5]
set xlabel "X Coordinate"
set ylabel "Y Coordinate"
i=0
load "animate.gnuplot"
set output

