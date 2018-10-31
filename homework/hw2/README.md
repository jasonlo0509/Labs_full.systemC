# HW2
### Yun-Chen Lo (107061548)

## How to run
```bash
In software/ 
$ make all // for compile c++ program
$ make run // for execute the program & apply medium filter to lena.bmp
$ make clean // for cleaning the executable binary
In systemC_partI/
$ make all // for compile systemC program
$ make check //for running the systemC program & compare with software version of the code
$ make clean // for cleaning the executable binary
In systemC_partII/
$ make all // for compile systemC program
$ make check //for running the systemC program & compare with software version of the code
$ make clean // for cleaning the executable binary
```

## General description
In this homework, we have to implement a Channel-based medium filter systemC system model. 
TA may use "make check" in systemC\_partI \& systemc\_partII folder to verify the correctness. SystemC\_partI stores the original implementation, while systemC\_partII stores the improved version.


## Implementation Details
- Software: 
	- I use the source code & medium filter introduction website provided by TA & successfully change the sobel kernel into a medium filter


- SystemC\_partI:
	- I create two modules \& three processes as required by the problem.
	- Input process will pass 9 pixels to the do\_medium filter, and do\_medium process will compute the corresponding output. Finally, the output process will write the result to the image pointer.

- SystemC\_partII:
	- In this optimized version of channel-based medium filter, input process will first send 9 pixels to fill the buffer in do\_medium process. Afterwards the input process will only send the right most 3 pixels to the do\_medium process. Outpu process is the same as the previous version.
	- In addition, we store an additional set of buffer in do\_medium process because select\_Kth will swap the pre-stored buffer data.

## Experimental results
### Before the medium filter
 
![Image of Yaktocat](https://i.imgur.com/GhhChbU.png)

### After the medium filter

![Image of Yaktocat](https://i.imgur.com/nZgSpa5.png)

We can observe that the image is blurred and some defact is gone. Here we show the results of 5x5 filter.


## Discussions and conclusions
The original version of channel-based medium filter requires 512\*512\*9=2,359,296 pixel transfer. But our optimized version use only (9 + 3\*511) \* 512 =789,504 transfer for a 512\*512 image.