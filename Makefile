target:	fork selectio signal pthread daytimeserver tcpclient selectclient signalclient

fork: 64_HW2_Q1A.c
	gcc 64_HW2_Q1A.c -o fork

selectio: 64_HW2_Q1B.c
	gcc 64_HW2_Q1B.c -o selectio

signal: 64_HW2_Q1C.c
	gcc 64_HW2_Q1C.c -o signal

pthread: 64_HW2_Q1D.c
	gcc 64_HW2_Q1D.c -pthread -o pthread 

daytimeserver: 64_HW2_Q2_SERVER.c
	gcc 64_HW2_Q2_SERVER.c -o daytimeserver

tcpclient: 64_HW2_TCPCLIENT.c
	gcc 64_HW2_TCPCLIENT.c -o tcpclient

selectclient: 64_HW2_Q1B_CLIENT.c
	gcc 64_HW2_Q1B_CLIENT.c -o selectclient

signalclient: 64_HW2_Q1C_CLIENT.c
	gcc 64_HW2_Q1C_CLIENT.c -o signalclient


clean:
	rm -f fork selectio signal pthread daytimeserver tcpclient selectclient signalclient *~
