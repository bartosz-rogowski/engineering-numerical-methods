set term png size 800, 600
set xrange [0:100]
set yrange [0:500]
set out "Metoda_Picarda.png"
set xlabel "t"
set title "Metoda Picarda"
set grid
plot "dane.dat" i 0 u 1:2 w lp lw 2 t "u(t)", \
	 '' i 0 u 1:3 w lp lw 2 t "z(t) = N-u(t)"

set out "Metoda_Newtona.png"
set title "Metoda Newtona"
plot "dane.dat" i 1 u 1:2 w lp lw 2 t "u(t)", \
	 '' i 1 u 1:3 w lp lw 2 t "z(t) = N-u(t)"

set out "nRK2.png"
set title "Niejawna metoda RK2"
plot "dane.dat" i 2 u 1:2 w lp lw 2 t "u(t)", \
	 '' i 2 u 1:3 w lp lw 2 t "z(t) = N-u(t)"
