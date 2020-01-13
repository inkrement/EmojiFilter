CXXFLAGS  = -Wall  -O3
LDFLAGS = -lre2 -lboost_locale-mt -I./src/vendor/re2 -I./src/vendor/

emoji_filter: src/main.cc src/emojis.h
	$(CXX) -std=c++17 $(LDFLAGS)  $(CXXFLAGS) $(OBJ) src/main.cc -o emoji_filter 

test: emoji_filter
	./emoji_filter data/e_top1000.txt | ./pos_filter.py

clean:
	rm -f emoji_filter

.PHONY: test clean