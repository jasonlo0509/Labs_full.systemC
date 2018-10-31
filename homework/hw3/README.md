# HW3
### Yun-Chen Lo (107061548)

## How to run
```bash
In software/ 
$ make all // for compile c++ program
$ make run // for execute the program & apply medium filter to lena.bmp
$ make clean // for cleaning the executable binary
In systemC/
$ make all // for compile systemC program
$ make check //for running the systemC program & compare with software version of the code
$ make clean // for cleaning the executable binary
```

## General description
In this homework, we have to implement a tlm-based medium filter systemC system model. 
TA may use "make check" in systemC folder to verify the correctness. SystemC folder stores the optimized version.


## Implementation Details
- Software: 
	- The Code is used for generating golden data.


- SystemC(hw3 tlm-based implementation):
	- I change all the data transfer into TLM-based transfer, and use the initiator.cpp provided by TA. And after binding initiator \& target socket, I use regitered b_transport to send \& receive the data.
	- The overall architecture is the same as hw3, which has one Testbench \& one Medium circuit.
	- I change the process structure in the Testbench Module from one input & one output process to just one inout process. Because the original implementation won't make sure send & receive happens in a FIFO correct fashion. Therefore, the received picture is wrong. To ensure the correctness, I put send & receive in the same loop.
	

## Additional features of design and models

- Here I implement medium filter optimized version with less pixels actually sent to the do_medium process in Medium.cpp.

## Experimental results
### Before the medium filter
 
![Image of Yaktocat](https://i.imgur.com/GhhChbU.png)

### After the medium filter

![Image of Yaktocat](https://i.imgur.com/nZgSpa5.png)

We can observe that the image is blurred and some defact is gone. Here we show the results of 3x3 filter.


## Discussions and conclusions
I found a tiny bug but don't know why the original version of implementation could done. The original code get the remote pixel back to data.uc ptr, but the correct place should be in data.result. I have looked for the BUG for so long, If possible, could TA fix it?
