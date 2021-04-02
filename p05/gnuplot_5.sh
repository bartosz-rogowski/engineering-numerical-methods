set term png size 800, 600
set out "S(it).png"
set xrange [0:600]
set xlabel "it"
set ylabel "S"
set title "Wykres zmian całki funkcjonalnej w zależności od liczby iteracji"
set grid
plot "S.dat" i 0 u 1:2 w l lw 3 t "k = 16", \
	 '' i 1 u 1:2 w l lw 3 t "k = 8", \
	 '' i 2 u 1:2 w l lw 3 t "k = 4", \
	 '' i 3 u 1:2 w l lw 3 t "k = 2", \
	 '' i 4 u 1:2 w l lw 3 t "k = 1"

unset grid
set xlabel "x"
set ylabel "y"
set xrange [0:25.6]
set yrange [0:25.6]
set pm3d map
set palette color
set size ratio -1
do for [NUMER=1:5] {
	temp=NUMER-1
	k=2**(5-NUMER)
	set out "V_".k.".png"
	set title "Mapa potencjału V dla k = ".k.""
	splot "V.dat" i temp u 1:2:3
}
