#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Requirement: core web sm model (python -m spacy download en_core_web_sm)

import sys
import spacy
import ujson as json

def main():
    nlp = spacy.load('en_core_web_sm')

    for line in sys.stdin:
        dct = json.loads(line)

        text = dct['raw']

        doc = nlp(text)
        deps = [ i.dep_ for i in doc ]
        pos = [ i.pos_ for i in doc ]

        ## full sentences
        if not ("nsubj" in deps and ("iobj" in deps or 'dobj' in deps) and "VERB" in pos):
            continue

        ## spacy lang check
        if doc.lang_ != 'en':
            continue

        ## only one sentence
        if len(list(doc.sents)) > 1:
            continue
        
        print(
            json.dumps({
                'text' : dct['text'],
                'emoji' : dct['emojis']
            }, ensure_ascii=False
        ))

if __name__ == '__main__':
    main()