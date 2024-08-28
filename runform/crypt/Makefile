OBJECTS = md5.o base64.o xdecrypt.o totp.o sha1.o hmacsha1.o
CFLAGS = -O2 -Wall

all: libmd5.a

libmd5.a: $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

totp: totp.c sha1.o hmacsha1.o
	$(CC) -DWITHMAIN -o $@ totp.c sha1.o hmacsha1.o -lm

clean:
	rm -f *.o *.a $(TARGET) totp

