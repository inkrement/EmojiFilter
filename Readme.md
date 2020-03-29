# Extract Emoji-Tweets
This program can be used to extract and preprocess tweets. It expects a tsv file as input (such as a MySQL/MariaDB dump) and converts it to line-wise json (e.g., jsonl, ndjson). Selected tweets contain at least one emoji of the unicode group "smileys & emotions". This group contains 150 different emojis which are likely used to express emotions on social media.


## Dependencies

The C++ part requires following packages:

 - [re2](https://github.com/google/re2/wiki/Syntax)
 - [boost locale](https://www.boost.org/doc/libs/1_72_0/libs/locale/doc/html/index.html)
 
Those can be installed on ubuntu with a single-line:

```sh
apt install libre2-dev libboost-all-dev
```

Python requirements (spacy & ujson) can be installed as follows:
```sh
pip install -r requirements.txt
python -m spacy download en_core_web_sm
```

## Usage
An example can be found in the `main.cc`, which reads an MySQL dump, extracts relevant lines and prints the preprocessed data to stdout. 

```sh
# build
make
sudo make install

## example
emoji_filter EmojiTweets.txt > filtered_tweets.jsonl

## .. or with additional pos filter
emoji_filter EmojiTweets.txt | pos_filter.py > filtered_tweets.jsonl
```
