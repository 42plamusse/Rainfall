set disassembly i
define all
   x/9i $eip
   echo \n
   i r
   echo \n
   x/64wx $esp
end

define nall
   set $instr=$eip
   ni
   x/i $instr
   all
end

OR for 64 bits

set disassembly i
define all
   x/9i $rip
   echo \n
   i r
   echo \n
   x/64wx $rsp
end

define nall
   set $instr=$rip
   ni
   x/i $instr
   all
end

