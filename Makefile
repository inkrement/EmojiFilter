CXXFLAGS  = -Wall  -O3
OBJ = 
LDFLAGS = -lre2 -lboost_locale-mt -I./src/vendor/re2 -I./src/vendor/

emoji_filter: src/main.cc src/emojis.h
	$(CXX) -std=c++17 $(LDFLAGS)  $(CXXFLAGS) $(OBJ) src/main.cc -o emoji_filter 

filter: emoji_filter
	./emoji_filter data/e_top1000.txt 

clean:
	rm -f emoji_filter

.PHONY: filter clean