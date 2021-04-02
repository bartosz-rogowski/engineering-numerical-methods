set term png size 1200, 600
set xrange [0:2]
set yrange [0:0.9]
set xlabel "x"
set ylabel "y"
set out "u_-1000.png"
set title "u(x, y) dla Q = -1000"
set grid
set pm3d map
set cbrange [-2:16]
set palette color
splot "u.dat" i 0 u 1:2:3

set out "v_-1000.png"
set title "v(x, y) dla Q = -1000"
set grid
set pm3d map
set cbrange [-6:1]
set palette color
splot "v.dat" i 0 u 1:2:3


set out "u_-4000.png"
set title "u(x, y) dla Q = -4000"
set grid
set pm3d map
set cbrange [-10:70]
set palette color
splot "u.dat" i 1 u 1:2:3

set out "v_-4000.png"
set title "v(x, y) dla Q = -4000"
set grid
set pm3d map
set cbrange [-14:4]
set palette color
splot "v.dat" i 1 u 1:2:3