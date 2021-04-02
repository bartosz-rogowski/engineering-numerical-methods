set term png size 800, 600
set out "trapezy_x(t).png"
set xrange [0:40]
set xlabel "t"
set ylabel "x(t)"
set title "Metoda trapezów"
set grid
plot "dane.dat" i 0 u 1:3 w l lw 2 t "TOL=10^{-2}", \
	 '' i 1 u 1:3 w l lw 2 t "TOL=10^{-5}"

set out "trapezy_v(t).png"
set ylabel "v(t)"
plot "dane.dat" i 0 u 1:4 w l lw 2 t "TOL=10^{-2}", \
	 '' i 1 u 1:4 w l lw 2 t "TOL=10^{-5}"

set out "trapezy_dt(t).png"
set ylabel "{/Symbol D}t(t)"
plot "dane.dat" i 0 u 1:2 w lp lw 2 t "TOL=10^{-2}", \
	 '' i 1 u 1:2 w lp lw 2 t "TOL=10^{-5}"



set title "Metoda RK2"
set out "RK2_x(t).png"
set ylabel "x(t)"
plot "dane.dat" i 2 u 1:3 w l lw 2 t "TOL=10^{-2}", \
	 '' i 3 u 1:3 w l lw 2 t "TOL=10^{-5}"

set out "RK2_v(t).png"
set ylabel "v(t)"
plot "dane.dat" i 2 u 1:4 w l lw 2 t "TOL=10^{-2}", \
	 '' i 3 u 1:4 w l lw 2 t "TOL=10^{-5}"

set out "RK2_dt(t).png"
set ylabel "{/Symbol D}t(t)"
plot "dane.dat" i 2 u 1:2 w lp lw 2 t "TOL=10^{-2}", \
	 '' i 3 u 1:2 w lp lw 2 t "TOL=10^{-5}"

set xrange [-2.5:2.5]
set yrange [-8:8]
set out "RK2_v(x).png"
set xlabel "x"
set ylabel "v(x)"
plot "dane.dat" i 2 u 3:4 w l lw 2 t "TOL=10^{-2}", \
	 '' i 3 u 3:4 w l lw 2 t "TOL=10^{-5}"

set title "Metoda trapezów"
set out "trapezy_v(x).png"
plot "dane.dat" i 0 u 3:4 w l lw 2 t "TOL=10^{-2}", \
	 '' i 1 u 3:4 w l lw 2 t "TOL=10^{-5}"
