all:
	g++ ./src/calculator.cpp -o ./calc -lreadline

clean:
	rm -f ./calc