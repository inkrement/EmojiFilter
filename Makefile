CXXFLAGS = -Wall  -O3
LDFLAGS = -lre2 -pthread
UNAME := $(shell uname)

# boost locale is multithreaded on osx
ifeq ($(UNAME), Linux)
	LDFLAGS += -lboost_locale
endif
ifeq ($(UNAME), Darwin)
	LDFLAGS += -lboost_locale-mt
endif

emoji_filter: src/main.cc src/emojis.h
	$(CXX) -o emoji_filter src/main.cc -std=c++17 $(LDFLAGS) $(CXXFLAGS)

test: emoji_filter
	./emoji_filter data/e_top1000.txt | ./pos_filter.py

clean:
	rm -f emoji_filter

install: emoji_filter
	cp emoji_filter /usr/local/bin
	cp pos_filter.py /usr/local/bin

.PHONY: test clean install
