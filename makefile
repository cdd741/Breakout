
all:
	@echo "Compiling..."
	g++ -std=c++14 -o breakout main.cpp ball.cpp layout.cpp -L/opt/X11/lib -lX11
	./a1

clean:
	-rm *o
