import jiagu
import jionlp
import synonyms
import textrank4zh


def get_synonyms(word, count = 16):
	return synonyms.nearby(word, count)[0]


def tokenize(text):
	# return synonyms.seg(text)[0]
	return jiagu.seg(text)
