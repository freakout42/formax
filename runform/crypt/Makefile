TARGET   = md5sum
OBJECTS  = md5.o base64.o xdecrypt.o
CFLAGS   = -O2 -Wall

all: $(TARGET)

$(TARGET): md5sum.o libmd5.a
	$(CC) -o $@ md5sum.o libmd5.a

libmd5.a: $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

clean:
	rm -f *.o *.a $(TARGET)

