# mtrel
Microsoft REL for HiTech C (Z80)

AS2MAC converts AS file to MAC file -- this was used to port libc and libf to REL format,
but is otherwise obsolete

OBJTOMAC converts OBJ files to MAC files:

c -v -c hello.c

objtomac hello.obj >hello.mac

m80 =hello

l80 zcrtcpm,hello,hello/n/e

