# This is my repository for CompTech FRKT 2nd course

## Short information about me

My name is: Molokanov Yaroslav, I am studying at B01-006(with B01-004 academic group)

It contains folders with programs which are divided on seminars (sem) :

## Title <a name="title"></a>
[Seminar 1](#sem1)

[Seminar 2](#sem2)

[Seminar 3](#sem3)

[Seminar 4](#sem4)

## Seminar 1 == "sem1" <a name="sem1"></a>

### Files, folders and short defenitions of them
#### myecho
myecho - that is my realisation of bash function echo with param -n;

### Make and launch
#### myecho
Make with ```gcc -Wall myecho.c -o myecho``` and launch with ```./myecho```

### Arguments
#### myecho
Pass arguments that you want to enable on your screen

Also, you can pass key -n, but only on the first position

[Go Back To Title](#title)

## Seminar 2 == "sem2" <a name="sem2"></a>

### Files, folders and short defenition of them
- pid.c
- myid.c
#### pid
pid - short function that output process id and parent id;
#### myid
myid - that is my realisation of bash function id;

### Make and launch
#### pid
Make pid with ```gcc -Wall pid.c -o pid``` and launch with ```./pid```
#### myid
Make myid with ```gcc -Wall myid.c -o myid``` and launch with ```./myid```

### Arguments
#### pid
There are no arguments to pass
#### myid
Pass name as an arguments if you want to get an information about process

You can choose not to pass arguments, at this case program will show you information about all processes

[Go Back To Title](#title)

## Seminar 3 == "sem3" <a name="sem3"></a>

### Files, folders and short defenition of them
- Folder mycat
  - Folder tests
    - test files
  - mycat.h
  - mycat.c
  - main.c
  - Launch
- Folder mycp
  - Folder files
    - test files
  - mycp.c

Functions mycat and mycp

#### mycat
mycat - that is my realisation of bash function cat without any params;

#### mycp
mycp - that is my realisation of bash function cp with params -i -v -f;

### Make and launch

Both programs can be made with ```make -f Launch``` but from their folders

#### mycat
Launch with ```./mycat```

#### mycp
Launch with ```./mycp```

### Arguments
#### mycat
Pass names of the files that you want to concatenate;

#### mycp
There are two variants to pass arguments:
~~~
1. file1, file2
~~~
It will copy file1(if it exists) to file2

~~~
2. file1, file2, ..., fileN, directory; where N = 1 to 2147483647;
~~~

It will copy files to directory with their local names

[Go Back To Title](#title)

## Seminar 4 == "sem4"<a name="sem4"></a>

### Files, folders and short defenition of them
- exec.c
- fork.c
- sleepsort.c

Functions exec, myfork, mysort
#### exec
exec - that is function which fork and call another function;
#### myfork
myfork - that is function which generate childs from their parents N times and print their id and parent id;

parent->child1->child2->...->childN
#### mysort
mysort - that is my realisation of sleepsort;

### Make and launch
#### exec
Make exec with ```gcc -Wall exec.c -o exec``` and launch with ```./exec```
#### myfork
Make myfork with ```gcc -Wall fork.c -o myfork``` and launch with ```./myfork```
#### mysort
Make mysort with ```gcc -Wall sleepsort.c -o mysort``` and launch with ```./mysort```


### Arguments
#### exec
Pass program name, which you want to execute with this program params
#### myfork
Pass number of processes that you want to generate
#### mysort
Pass array of numbers that you want to sort

[Go Back To Title](#title)