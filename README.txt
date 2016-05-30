Student Name: Aakash hah
Student ID : 010767354

** FOR PREDEFINED SERVICES, LOGIN TO ROOT AND THEN COMPILE **

Compiling and running the Programs:


Question 1:Part A 
$ gcc 64_HW2_Q1A.c -o fork (Server Side)
$ ./fork 5000

$ gcc 64_HW2_TCPCLIENT.c -o tcpclient (Client Side)
$ ./tcpclient 127.0.0.1 5000

Question 1:Part B 
$ gcc 64_HW2_Q1B.c -o selectio (Server Side)
$ ./selectio

$ gcc 64_HW2_Q1B_CLIENT.c -o selectclient (Client Side)
$ ./selectclient 777

Question 1:Part C 
$ gcc 64_HW2_Q1C.c -o signal (Server Side)
$ ./signal 3000

$ gcc 64_HW2_Q1C_CLIENT.c -o signalclient (Client Side)
$ ./signalclient 127.0.0.1 3000

Question 1:Part D 
$ gcc 64_HW2_Q1D.c -pthread -o pthread (Server Side) 
$ ./pthread 4000

$ gcc 64_HW2_TCPCLIENT.c -o tcpclient (Client Side)
$ ./tcpclient 127.0.0.1 4000

Question 2: 
$ gcc 64_HW2_Q2_SERVER.c -o daytimeserver (Server Side) 
$ ./daytimeserver 37

$ gcc 64_HW2_TCPCLIENT.c -o tcpclient (Client Side)
$ ./tcpclient 127.0.0.1 37

