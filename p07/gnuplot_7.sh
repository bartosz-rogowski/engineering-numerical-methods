set term png size 1200, 600
set xrange [0:2]
set yrange [0:0.9]
set xlabel "x"
set ylabel "y"
set title "{/Symbol z}(x, y) dla Q = -1000"
set out "zeta_-1000.png"
set contour
#set cntrparam levels 40 # lub ponizsze:
set cbr [-200:350]
set cntrparam levels increment -200,10,350
unset surface
set view map
unset key
splot "zeta.dat" i 0 u 1:2:3:3 w l lt -1 palette t '' 

set contour
set out "psi_-1000.png"
set title "{/Symbol y}(x, y) dla Q = -1000"
set cbr [-55:-50]
set cntrparam levels increment -55,0.08,-50
unset surface
set view map
unset key
splot "psi.dat" i 0 u 1:2:3:3 w l lt -1 palette t '' 


set out "zeta_-4000.png"
set contour
#set cntrparam levels 40 # lub ponizsze:
set cbr [-900:1200]
set title "{/Symbol z}(x, y) dla Q = -4000"
set cntrparam levels increment -900,45,1200
unset surface
set view map
unset key
splot "zeta.dat" i 1 u 1:2:3:3 w l lt -1 palette t '' 

set contour
set out "psi_-4000.png"
set title "{/Symbol y}(x, y) dla Q = -4000"
set cbr [-218:-202]
set cntrparam levels increment -218,1,-202
unset surface
set view map
unset key
splot "psi.dat" i 1 u 1:2:3:3 w l lt -1 palette t '' 


set contour
set out "psi_4000.png"
set title "{/Symbol y}(x, y) dla Q = 4000"
set cbr [202:218]
set cntrparam levels increment 202,1,218
unset surface
set view map
unset key
splot "psi.dat" i 2 u 1:2:3:3 w l lt -1 palette t '' 
