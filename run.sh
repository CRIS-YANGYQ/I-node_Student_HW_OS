g++ -c authenticator.cpp -o authenticator.o
g++ -c definedTime.cpp -o definedTime.o
g++ -c file.cpp -o file.o
g++ -c user_function.cpp -o user_function.o

g++ -c user.cpp -o user.o
g++ -c course.cpp -o course.o
g++ -c homework.cpp -o homework.o

g++ -c modeApplication.cpp -o modeApplication.o
g++ -c main.cpp -o main.o
g++ authenticator.o definedTime.o file.o user.o course.o user_function.o homework.o modeApplication.o main.o -o myprogram 
