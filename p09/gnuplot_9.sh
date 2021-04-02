set term png size 700, 600
set out "temp_it_100.png"
set xrange [0:40]
set yrange [0:40]
set xlabel "x"
set ylabel "y"
set title "Rozkład temperatury dla it = 100"
set grid
set pm3d map
set size ratio -1
splot "T.dat" i 0 u 1:2:3

set out "temp_it_200.png"
set title "Rozkład temperatury dla it = 200"
splot "T.dat" i 1 u 1:2:3


set out "temp_it_500.png"
set title "Rozkład temperatury dla it = 500"
splot "T.dat" i 2 u 1:2:3


set out "temp_it_1000.png"
set title "Rozkład temperatury dla it = 1000"
splot "T.dat" i 3 u 1:2:3


set out "temp_it_2000.png"
set title "Rozkład temperatury dla it = 2000"
splot "T.dat" i 4 u 1:2:3


set out "laplasjan_it_100.png"
set title "Laplasjan temperatury dla it = 100"
splot "laplasjan.dat" i 0 u 1:2:3 t ''

set out "laplasjan_it_200.png"
set title "Laplasjan temperatury dla it = 200"
splot "laplasjan.dat" i 1 u 1:2:3 t ''

set out "laplasjan_it_500.png"
set title "Laplasjan temperatury dla it = 500"
splot "laplasjan.dat" i 2 u 1:2:3 t ''

set out "laplasjan_it_1000.png"
set title "Laplasjan temperatury dla it = 1000"
splot "laplasjan.dat" i 3 u 1:2:3 t ''

set out "laplasjan_it_2000.png"
set title "Laplasjan temperatury dla it = 2000"
splot "laplasjan.dat" i 4 u 1:2:3 t ''