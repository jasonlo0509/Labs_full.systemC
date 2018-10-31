# HW4
### Yun-Chen Lo (107061548)

## How to run
```bash
In systemC/
$ make all // for compile systemC program
$ make check //for running the systemC program & compare with software version of the code
$ make clean // for cleaning the executable binary
```

## General description
In this homework, we have to implement a tlm-bus based medium filter systemC system model. 
TA may use "make check" in systemC folder to verify the correctness. SystemC folder stores the optimized version.


## Implementation Details
- SystemC:
	- I reuse the architecture provided by lab4. The only change is the inout process in Testbench module, and do\_medium process in SobelFilter module.
	- The inout process will send the data to the bus, and bus will decode the address \& put the data to circuit. After the data is sent back to the bus, testbench will get the data and write out.
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
The lab4 code is very deep in hierarchy, so further review of the code is necessary!
