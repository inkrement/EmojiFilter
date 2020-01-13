CXXFLAGS  = -Wall  -O3
LDFLAGS = -lre2 -lboost_locale-mt -I./src/vendor/re2 -I./src/vendor/

emoji_filter: src/main.cc src/emojis.h
	$(CXX) -std=c++17 $(LDFLAGS)  $(CXXFLAGS) $(OBJ) src/main.cc -o emoji_filter 

test: emoji_filter
	./emoji_filter data/e_top1000.txt | ./pos_filter.py

clean:
	rm -f emoji_filter

install: emoji_filter
	cp emoji_filter /usr/local/bin
	cp pos_filter.py /usr/local/bin

.PHONY: test clean install