# Extract Emoji-Tweets
This program can be used to extract and preprocess tweets. Selected tweets contain at least one emoji of the unicode group "smileys & emotions". This group contains 150 different emojis which are likely used to express emotions on social media.

An example can be found in the `main.cc`, which reads an MySQL dump, extracts relevant lines and prints the preprocessed data to stdout. 

```sh
# build
make 

## example
./emoji_filter EmojiTweets.txt > filtered_tweets.jsonl
```

## Dependencies
 - [re2](https://github.com/google/re2/wiki/Syntax)
 - [boost locale](https://www.boost.org/doc/libs/1_72_0/libs/locale/doc/html/index.html)