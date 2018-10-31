# HW1  
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
In this homework, we have to implement a Event-based medium filter systemC system model. 
I first change the sobel filter into medium filter in the provided source code. Then use it as the golden data for systemc version. The input I finally choose is lena\_noise.bmp, and the output is lena\_medium.bmp. TA may use "make check" in systemC folder to verify the correctness.

## Implementation Details
- Software: 
	- I use the source code & medium filter introduction website provided by TA & successfully change the sobel kernel into a medium filter


- SystemC:
	- I create a module & put read\_bmp, do\_medium, write\_bmp as method
	- I use three event: read_done, medium_done, write_free to ensure the execution order of the process. The event usage is listed as below:
		- read\_bmp ->(read_done) -> do\_medium
		- do\_ medium -> (medium_done) -> write\_bmp
		- write\_bmp -> (write_free) -> do\_medium
		- do\_medium -> (medium_free) -> read\_bmp
	- In addition, I use read\_bmp process to set the current x, y position. Therefore, the system have to wait for write bmp process to complete before move to the next x, y position.



## Experimental results
### Before the medium filter
 
![Image of Yaktocat](https://i.imgur.com/GhhChbU.png)

### After the medium filter

![Image of Yaktocat](https://i.imgur.com/nZgSpa5.png)

We can observe that the image is blurred and some defact is gone. Here we show the results of 5x5 filter.

## Discussions and conclusions
In this homework, I learned how to write finite state machine using systemC process & process. However, if we want more realistic we have to put ports on modules.

A very strange thing is that when I try to add more noise to the original bmp photo, I cannot write the result correctly. It will shift a little bit. I think it is because of the bmp file header doesn't meet the requirement.