austin_example.o: data_model.o chain.o master.o
	g++ data_model.o chain.o master.o main.cpp -o austin_example -std=c++11 -lgsl -lgslcblas -lpthread

master.o: Master.cpp Master.h
	g++ -c Master.cpp -o master.o -std=c++11 -lgsl -lgslcblas -lpthread

data_model.o: DataModel.cpp DataModel.h
	g++ -c DataModel.cpp -o data_model.o -std=c++11 -lgsl -lgslcblas -lpthread

chain.o : Chain.cpp Chain.h 
	g++ -c Chain.cpp -o chain.o -std=c++11 -lgsl -lgslcblas -lpthread

clean:
	rm chain.o data_model.o master.o
