set term png size 1200, 400
set out "v_x.png"
set xrange [0:4]
set yrange [0:0.9]
set xlabel "x"
set ylabel "y"
set title "v_x"
set grid
set pm3d map
splot "v.txt" i 0 u 1:2:3

set out "v_y.png"
set title "v_y"
splot "v.txt" i 0 u 1:2:4

set out "x_sr.png"
set title "x_{sr}"
set xrange [0:0.6]
set yrange [0.5:4]
set xlabel "t_n"
set ylabel "x_{sr}"
set grid
plot "cx.txt" i 0 u 1:3 w lp lw 2 t 'D = 0', \
	'' i 1 u 1:3 w lp lw 2 t 'D = 0.1'

set out "c.png"
set title "c"
set xrange [0:0.6]
set yrange [0.5:1.1]
set xlabel "t_n"
set ylabel "c"
set grid
plot "cx.txt" i 0 u 1:2 w l lw 1 t 'D = 0', \
	'' i 1 u 1:2 w l lw 1 t 'D = 0.1'

reset
set term gif size 1000,400 animate delay 100
set output "u_D0.gif"
n=4    #liczba klatek
set view map # widok z gory
set size ratio -1
set cbr [0:]
set xrange [0:4]
set yrange [0:0.9]
set xlabel "x"
set ylabel "y"
set title "Animacja (5 klatek) rozkładu u dla D = 0"
do for [k=0:n] {
  splot "u_D0.txt" index k u 1:2:3 w pm3d title sprintf("t=%i",k)
} 

reset
set term gif size 1000,400 animate delay 100
set output "u_D1.gif"
n=4    #liczba klatek
set view map # widok z gory
set size ratio -1
set cbr [0:]
set title "Animacja (5 klatek) rozkładu u dla D = 0.1"
do for [k=0:n] {
  splot "u_D1.txt" index k u 1:2:3 w pm3d title sprintf("t=%i",k)
} 