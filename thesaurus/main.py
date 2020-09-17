# Huge thanks to Šarūnas Navickas (zaibacu) for the synonym collection.

import json

wdict = {}
with open('thesaurus.jsonl') as f:
    with open('thesaurus.csv', 'w') as g:
        for line in f:
            arr = json.loads(line)
            if arr['word'] in wdict.keys():
                wdict[arr['word']] += list(set(arr['synonyms']) - set(wdict[arr['word']]))
            else:
                wdict[arr['word']] = arr['synonyms']
        for pair in wdict.items():
            syn_list = [x for x in pair[1] if x.lower() != pair[0].lower()]
            if len(syn_list) > 0:
                g.write(pair[0] + '\t' + ','.join(syn_list) + '\n')
