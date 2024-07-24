ARX=$(DESTDIR)/opt
DIST=dist/formax
all: $(DIST).tar.gz
	@echo 'you can now run "sudo make install"'
install: all
	test -d $(ARX) || mkdir $(ARX)
	gzip -cd <$(DIST).tar.gz | tar x -p -C $(ARX) -f -
$(DIST).tar.gz: generate/scotty.sq3
	mkdir dist
	chmod g-s dist
	mkdir $(DIST) $(DIST)/bin $(DIST)/lib $(DIST)/man $(DIST)/man/man1
	cp generate/makeform runform/runform $(DIST)/bin
	cp generate/scotty.* $(DIST)/lib
	strip $(DIST)/bin/* 2>/dev/null || true
	chmod -R u+rwX,g-w,g+rX,o-w,o+rX dist
	cd dist; tar cf - formax | gzip -c >formax.tar.gz
	if [ -d archive ]; then cp $(DIST).tar.gz archive/.tar.gz; fi
generate/scotty.sq3: runform/runform
	rm -f $@
	cd generate; make
runform/runform:
	cd runform; make
clean:
	./configure clean
