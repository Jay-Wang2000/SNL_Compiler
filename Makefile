main: main.cpp
	g++ main.cpp -o main 

clean:
	rm ./out/*.txt
	rm main
