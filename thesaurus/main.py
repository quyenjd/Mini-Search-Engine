import codecs
from bs4 import BeautifulSoup

# Download html file at http://www.gutenberg.org/files/28900/28900-h/28900-h.htm
# Huge thanks to SuzanaK and James Chaplin Fernald

keywords = BeautifulSoup(codecs.open('thesaurus.htm', 'r', 'iso-8859-1').read(), features='html.parser').select('h3')
out = codecs.open('thesaurus.csv', 'w', 'utf-8')

for k in keywords:
    keyword = k.text.lower().replace('.', '')
    if len(keyword.split(',')) > 1:
        keyword = keyword.split(',')[0]

    h4 = k.find_next_sibling('h4')
    syns = ''
    if 'Synonyms' in h4.text or 'Synonym' in h4.text:

        table = h4.find_next()
        if table and table.name == 'table':
            syns_tds = table.find_all('td')
            syns = [s.text.replace(',', '') for s in syns_tds]
            syns = [s.replace('.', '') for s in syns]
            syns = ",".join(syns)

    h4 = h4.find_next_sibling('h4')
    ants = ''
    if 'Antonyms' in h4.text or 'Antonym' in h4.text:

        table = h4.find_next()
        if table and table.name == 'table':
            ant_tds = table.find_all('td')
            ants = [s.text.replace(',', '') for s in ant_tds]
            ants = [s.replace('.', '') for s in ants]
            ants = ",".join(ants)

    # Each row has the form of 'WORD\tSYNONYMS\tANTONYMS' in which SYNONYMS and ANTONYMS lists are comma-separated
    out.write(keyword + '\t' + syns + '\t' + ants + '\n')
    
    if 'youthful' in keyword:
      break

out.close()
