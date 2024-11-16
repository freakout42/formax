include ../cflags.make
OBJECTS = md5.o base64.o xdecrypt.o totp.o sha1.o hmacsha1.o

all: libmd5.a
small: libmd5.a

test: totp

libmd5.a: $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

totp: totp.c libmd5.a
	$(CC) $(CFLAGS) -DWITHMAIN -L. -o $@ totp.c -lmd5 -lm
	./$@ k >ttt
	echo ':9999999:' | diff ttt -
	rm ttt

clean:
	rm -f *.o *.a $(TARGET) totp

