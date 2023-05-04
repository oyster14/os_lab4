# module load gcc-9.2

iosched: iosched.cpp
		g++ -g iosched.cpp -o iosched
clean:
		rm -f ioscheds *~#