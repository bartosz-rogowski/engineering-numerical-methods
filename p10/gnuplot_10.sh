set term png size 800, 600
set out "E.png"
set xlabel "t"
set ylabel "E"
set key center right
set title "Wykres E"
plot "E.dat" i 0 u 1:2 w l lw 2 t '{/Symbol b} = 0.0', \
	'' i 1 u 1:2 w l lw 2 t '{/Symbol b} = 0.1', \
	'' i 2 u 1:2 w l lw 2 t '{/Symbol b} = 1.0'

set out "Ea.png"
set title "Wykres E dla {/Symbol a} = 1.0, {/Symbol b} = 1.0"
plot "Ea.dat" i 0 u 1:2 w l lw 2 t ''

set key
set out "u_0.0.png"
set xrange [0:50]
set yrange [0:15]
set xlabel "t"
set ylabel "x"
set title "Wykres u dla {/Symbol a} = 0.0, {/Symbol b} = 0.0"
set grid
set size ratio -1
set pm3d map
splot "u_0.0.dat" i 0 u 1:2:3

set out "u_0.1.png"
set title "Wykres u dla {/Symbol a} = 0.0, {/Symbol b} = 0.1"
set pm3d map
splot "u_0.1.dat" i 0 u 1:2:3

set out "u_1.0.png"
set title "Wykres u dla {/Symbol a} = 0.0, {/Symbol b} = 1.0"
set pm3d map
splot "u_1.0.dat" i 0 u 1:2:3

set out "u_a1.0.png"
set title "Wykres u dla {/Symbol a} = 1.0, {/Symbol b} = 1.0"
set pm3d map
splot "u_a1.0.dat" i 0 u 1:2:3