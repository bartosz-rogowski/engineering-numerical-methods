set term png size 600, 600
set out "mapa1.png"
set xrange [0:5]
set yrange [0:5]
set xlabel "x"
set ylabel "y"
set title "Mapa potencjału dla n_x = n_y = 50, ({/Symbol e}_1 ={/Symbol e}_2 = 1)"
set grid
set pm3d map
set palette defined (-10 "blue", 0 "white", 10 "red" )
set size ratio -1
splot "mapy.dat" i 0 u 1:2:3

set out "mapa2.png"
set xrange [0:10]
set yrange [0:10]
set title "Mapa potencjału dla n_x = n_y = 100, ({/Symbol e}_1 ={/Symbol e}_2 = 1)"
splot "mapy.dat" i 1 u 1:2:3

set out "mapa3.png"
set xrange [0:20]
set yrange [0:20]
set title "Mapa potencjału dla n_x = n_y = 200, ({/Symbol e}_1 ={/Symbol e}_2 = 1)"
splot "mapy.dat" i 2 u 1:2:3

set palette defined (-0.8 "blue", 0 "white", 0.8 "red" )

set out "mapa4.png"
set xrange [0:10]
set yrange [0:10]
set cbrange [-0.8:0.8]
set title "Mapa potencjału dla n_x = n_y = 100, ({/Symbol e}_1 ={/Symbol e}_2 = 1), niezerowa gęstość"
splot "mapy2.dat" i 0 u 1:2:3

set out "mapa5.png"
set title "Mapa potencjału dla n_x = n_y = 100, ({/Symbol e}_1 = 1, {/Symbol e}_2 = 2), niezerowa gęstość"
splot "mapy2.dat" i 1 u 1:2:3

set out "mapa6.png"
set title "Mapa potencjału dla n_x = n_y = 100, ({/Symbol e}_1 = 1, {/Symbol e}_2 = 10), niezerowa gęstość"
splot "mapy2.dat" i 2 u 1:2:3