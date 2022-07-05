import jiagu
import jionlp
import synonyms
import textrank4zh


def get_keywords(text, count = 5):
	# return synonyms.keywords(text, count)
	return jiagu.keywords(text, count)


def get_summary(text, count = 5):
	return jiagu.summarize(text, count)


def get_sentiment(text):
	return jiagu.sentiment(text)