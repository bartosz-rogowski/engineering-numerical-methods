set term png size 800, 600
set out "rglob_S(it).png"
set xrange [1:100000]
set logscale x
set xlabel "it"
set ylabel "S"
set title "Relaksacja globalna - wykres zmian całki funkcjonalnej w zależności od liczby iteracji"
set grid
plot "rglob_S.dat" i 0 u 1:2 w l lw 2 t "{/Symbol w}_G = 0.6; it_{max} = 36886", \
	 '' i 1 u 1:2 w l lw 2 t "{/Symbol w}_G = 1.0; it_{max} = 23096"

set out "rlok_S(it).png"
set title "Relaksacja lokalna - wykres zmian całki funkcjonalnej w zależności od liczby iteracji"
plot "rlok_S.dat" i 0 u 1:2 w l lw 2 t "{/Symbol w}_L = 1.0; it_{max} = 12193", \
	 '' i 1 u 1:2 w l lw 2 t "{/Symbol w}_L = 1.4; it_{max} = 5547", \
	 '' i 2 u 1:2 w l lw 2 t "{/Symbol w}_L = 1.8; it_{max} = 1552", \
	 '' i 3 u 1:2 w l lw 2 t "{/Symbol w}_L = 1.9; it_{max} = 750"

unset logscale x

set pm3d map
set out "rglob_blad_wg_06.png"
set title "Relaksacja globalna - mapa błędu rozwiązania {/Symbol d} dla {/Symbol w}_G = 0.6"
set xlabel "x"
set ylabel "y"
set palette color
set xrange [0:15]
set yrange [0:10]
set border lw 3
plot "rglob_blad.dat" i 0 u 1:2:3 w p lw 4 palette t ""


set out "rglob_blad_wg_10.png"
set title "Relaksacja globalna - mapa błędu rozwiązania {/Symbol d} dla {/Symbol w}_G = 1.0"
set border lw 3
plot "rglob_blad.dat" i 1 u 1:2:3 w p lw 4 palette t ""

set out "rglob_V_wg_06.png"
set title "Relaksacja globalna - mapa zrelaksowanego potencjału V dla {/Symbol w}_G = 0.6"
set border lw 3
plot "rglob_V.dat" i 0 u 1:2:3 w p lw 4 palette t ""


set out "rglob_V_wg_10.png"
set title "Relaksacja globalna - mapa zrelaksowanego potencjału V dla {/Symbol w}_G = 1.0"
set border lw 3
plot "rglob_V.dat" i 1 u 1:2:3 w p lw 4 palette t ""