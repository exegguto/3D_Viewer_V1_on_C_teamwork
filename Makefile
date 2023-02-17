CC=gcc
FLAGS=-Wall -Werror -Wextra
ROAD=.
FILENAME=test.out
PROJECTNAME=viewer
TESTNAME=test.c
FULL_PACK=viewer.c test.c
GCOV=--coverage
LIBS=`pkg-config --cflags --libs check` -lm
OS=$(shell uname)
ifeq ($(OS), Linux)
  OPEN=xdg-open
  REMOVELINE=sed -i '$$ d'
  REMOVELINES=head -n -6 ./123.txt
else
  OPEN=open
  REMOVELINE=sed -i '' -e '$$ d'
  REMOVELINES=tail -r 123.txt | tail -n +6 | tail -r
endif


all: $(TESTNAME) $(PROJECTNAME).a install gcov_report dvi

install:
	mkdir build
	cd build && qmake ../src.pro && make
	
uninstall:
	rm -rf build

dist: clean uninstall
	tar -czf 3D_Viewer_v1_0.tar.gz *

dvi:
	rm -f dvi.aux dvi.log dvi.pdf
	pdflatex dvi.tex
	$(OPEN) dvi.pdf

tests: $(PROJECTNAME).a
	$(CC) $(FLAGS) -g $(TESTNAME) -L . $(PROJECTNAME).a $(LIBS) -o $(FILENAME) -std=c11

gcov_report: $(FULL_PACK) $(PROJECTNAME).h
	gcc $(CFLAGS) $(GCOV) -g $(FULL_PACK) -o $(FILENAME) -std=c11 $(LIBS) -dumpbase ''
	./test.out
	lcov -t "$(PROJECTNAME)" -o $(PROJECTNAME).info -c -d .
	genhtml -o report $(PROJECTNAME).info
	gcov $(PROJECTNAME).info -f > ./123.txt
	$(REMOVELINE) ./report/index.html
	$(REMOVELINE) ./report/index.html
	$(REMOVELINE) ./report/index-sort-f.html
	$(REMOVELINE) ./report/index-sort-f.html
	$(REMOVELINE) ./report/index-sort-l.html
	$(REMOVELINE) ./report/index-sort-l.html
	$(REMOVELINES) > ./1234.txt && mv ./1234.txt ./123.txt
	sed 's/:/:<b>/g' ./123.txt > ./1234.txt && mv ./1234.txt ./123.txt
	sed 's/%/%<\/b>/g' ./123.txt > ./1234.txt && mv ./1234.txt ./123.txt
	sed -e ':a' -e 'N' -e '$$!ba' -e 's/\n/<br>/g' ./123.txt > ./1234.txt && mv ./1234.txt ./123.txt
	cat ./123.txt | tee -a ./report/index.html ./report/index-sort-f.html ./report/index-sort-l.html > /dev/null
	echo '\t</body>\n</html>' | tee -a ./report/index.html ./report/index-sort-f.html ./report/index-sort-l.html > /dev/null
	rm 123.txt
	$(OPEN) ./report/index.html

$(PROJECTNAME).a: $(PROJECTNAME).o
	ar -rcs $(PROJECTNAME).a $(PROJECTNAME).o

$(PROJECTNAME).o: $(PROJECTNAME).c $(PROJECTNAME).h
	$(CC) $(FLAGS) -c -g $(PROJECTNAME).c -o $(PROJECTNAME).o

clean:
	rm -rf *.dSYM *.out *.gcda *.gcno *.info *.gcov *.a *.o *FILE *.txt dvi.log dvi.pdf dvi.aux report

style:
	cp ../materials/linters/.clang-format .
	clang-format -n *.c *.h *.cpp
	rm .clang-format

leaks:
	leaks -atExit -- $(ROAD)/$(FILENAME)

gr:
	valgrind --log-file="valgrind.txt" --tool=memcheck --track-origins=yes --leak-check=full -v $(ROAD)/$(FILENAME)
