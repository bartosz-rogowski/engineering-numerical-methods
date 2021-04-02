set term png size 800, 600
set out "Euler.png"
set xrange [0:5]
set xlabel "t"
set title "Rozwiązania numeryczne metodą jawną Eulera"
set yrange [0:1]
set grid
plot "data.dat" i 0 u 1:2 w lp lw 2 t "{/Symbol D}t = 0.01", \
	 ''			i 1 u 1:2 w lp lw 2 t "{/Symbol D}t = 0.1", \
	 '' 		i 2 u 1:2 w lp lw 2 t "{/Symbol D}t = 1.0", \
	 ''			i 9 u 1:2 w lp lw 2 t "e^{(-x)}"

set out "RK2.png"
set title "Rozwiązania numeryczne metodą RK2"
plot "data.dat" i 3 u 1:2 w lp lw 2 t "{/Symbol D}t = 0.01", \
	 ''			i 4 u 1:2 w lp lw 2 t "{/Symbol D}t = 0.1", \
	 '' 		i 5 u 1:2 w lp lw 2 t "{/Symbol D}t = 1.0", \
	 ''			i 9 u 1:2 w lp lw 2 t "e^{(-x)}"

set out "RK4.png"
set title "Rozwiązania numeryczne metodą RK4"
plot "data.dat" i 6 u 1:2 w lp lw 2 t "{/Symbol D}t = 0.01", \
	 ''			i 7 u 1:2 w lp lw 2 t "{/Symbol D}t = 0.1", \
	 '' 		i 8 u 1:2 w lp lw 2 t "{/Symbol D}t = 1.0", \
	 ''			i 9 u 1:2 w lp lw 1 t "e^{(-x)}"

#-------------------------------------------------------- bledy

set out "bledy_Euler.png"
set logscale y
set yrange[10**(-5) : 0.5]
set title "Błędy rozwiązań numerycznych metodą jawną Eulera"
set grid
plot "data.dat" i 0 u 1:3 w l lw 2 t "{/Symbol D}t = 0.01", \
	 ''			i 1 u 1:3 w l lw 2 t "{/Symbol D}t = 0.1", \
	 '' 		i 2 u 1:3 w l lw 2 t "{/Symbol D}t = 1.0"

set out "bledy_RK2.png"
set yrange[10**(-7) : 0.5]
set title "Błędy rozwiązań numerycznych metodą RK2"
plot "data.dat" i 3 u 1:3 w l lw 2 t "{/Symbol D}t = 0.01", \
	 ''			i 4 u 1:3 w l lw 2 t "{/Symbol D}t = 0.1", \
	 '' 		i 5 u 1:3 w l lw 2 t "{/Symbol D}t = 1.0"

set out "bledy_RK4.png"
set yrange[10**(-13) : 0.01]
set title "Błędy rozwiązań numerycznych metodą RK4"
plot "data.dat" i 6 u 1:3 w l lw 2 t "{/Symbol D}t = 0.01", \
	 ''			i 7 u 1:3 w l lw 2 t "{/Symbol D}t = 0.1", \
	 '' 		i 8 u 1:3 w l lw 2 t "{/Symbol D}t = 1.0"

#------------------------------------------------------------ obwod 
set key right top
unset logscale y
set xrange [0:0.25]
set yrange [-0.0014:0.003]
array I[4]
I[1] = "0.5*{/Symbol w}_0"
I[2] = "0.8*{/Symbol w}_0"
I[3] = "1.0*{/Symbol w}_0"
I[4] = "1.2*{/Symbol w}_0"
set out "Q.png"
set title "Rozwiązania numeryczne obwodu RLC - wykresy Q(t)"
plot "obwod.dat" i 0 u 1:2 w l lw 2 t "{/Symbol w}_V = ".I[1], \
	'' 	i 1 u 1:2 w l lw 2 t "{/Symbol w}_V = ".I[2], \
	'' 	i 2 u 1:2 w l lw 2 t "{/Symbol w}_V = ".I[3], \
	'' 	i 3 u 1:2 w l lw 2 t "{/Symbol w}_V = ".I[4]

set yrange[-0.15:0.15]
set out "I.png"
set title "Rozwiązania numeryczne obwodu RLC - wykresy I(t)"
plot "obwod.dat" i 0 u 1:3 w l lw 2 t "{/Symbol w}_V = ".I[1], \
	'' 	i 1 u 1:3 w l lw 2 t "{/Symbol w}_V = ".I[2], \
	'' 	i 2 u 1:3 w l lw 2 t "{/Symbol w}_V = ".I[3], \
	'' 	i 3 u 1:3 w l lw 2 t "{/Symbol w}_V = ".I[4]

# do for [NUMER=3:6] {
# 	k = NUMER - 2
# 	set title "Rozwiązania numeryczne obwodu RLC dla {/Symbol w}_V = ".I[k]
# 	plot "obwod.dat" i (k-1) u 1:3 w lp t "Q", \
# 	 '' 		 	 i (k-1) u 1:3 w lp t "I"
#}

#lp linia ciagla