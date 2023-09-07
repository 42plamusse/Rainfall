Exploit: change the value of a function pointer stored in the heap to access a other function.

An unprotected buffer is pointing to an address in memory, right before the function pointer. As the input is not protected, we can overflow the buffer in order to write in the memory used by the function pointer.

```
payload = python -c 'print "A" * 0x48 + "\x54\x84\x04\x08"'
```
