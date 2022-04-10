import csv
import json

import jiagu
import jionlp
import synonyms
import textrank4zh

import globals


def run_test():
	# ADD TEST CODE
	
	# SIMPLE TEST
	
	# print(lexical.get_synonyms("美丽", 32))
	# print(lexical.get_synonyms("帅", 32))
	
	# END SIMPLE TEST
	
	# CSV TEST
	
	# synonyms
	print(globals.linesep + "================ Synonyms ================" + globals.linesep)
	with open("synonyms.csv") as synonyms_file:
		reader = csv.reader(synonyms_file)
		print(globals.linesep + "synonyms:")
		for row in reader:
			print('、'.join(synonyms.nearby(''.join(row), 20)[0]))
	print(globals.linesep + "================ End Synonyms ================" + globals.linesep)
	
	print(globals.linesep + "================ Lexical & Semantic ================" + globals.linesep)
	with open("paragraphs.csv") as paragraphs_file, open("entity-list.json", encoding = "utf-8") as entity_list_file:
		entity_list = json.load(entity_list_file)
		jio_entity_recognizer = jionlp.ner.LexiconNER(entity_list)
		tr4w = textrank4zh.TextRank4Keyword()
		tr4s = textrank4zh.TextRank4Sentence()
		reader = csv.reader(paragraphs_file)
		texts = []
		for row in reader:
			texts.append(''.join(row))
		for text in texts:
			print(globals.linesep + "text: " + globals.linesep + text)
		
		# word seg
		print(globals.linesep + "================ Word Seg ================" + globals.linesep)
		for text in texts:
			print(globals.linesep + "jiagu:")
			words_for_jiagu = jiagu.seg(text)
			print('/'.join(words_for_jiagu))
			print(globals.linesep + "synonyms:")
			words_for_synonyms = synonyms.seg(text)[0]
			print('/'.join(words_for_synonyms))
		
		# entity recognition
		print(globals.linesep + "================ Entity Recognition ================" + globals.linesep)
		for text in texts:
			print(globals.linesep + "jiagu:")
			print('/'.join(jiagu.ner(words_for_jiagu)))
			print(globals.linesep + "jionlp:")
			print(jio_entity_recognizer(text))
		
		# extract / generate keywords
		print(globals.linesep + "================ Keywords ================" + globals.linesep)
		keyword_count = 10
		for text in texts:
			print(globals.linesep + "jiagu:")
			print('；'.join(jiagu.keywords(text, keyword_count)))
			print(globals.linesep + "synonyms:")
			print('；'.join(synonyms.keywords(text, keyword_count)))
			print(globals.linesep + "textrank4zh:")
			tr4w.analyze(text = text)
			tr4w_keywords = tr4w.get_keywords(keyword_count)
			for word_item in tr4w_keywords:
				print(word_item.word, end = "；")
			print(globals.linesep)
		
		# extract / generate summary
		print(globals.linesep + "================ Summaries ================" + globals.linesep)
		for text in texts:
			print(globals.linesep + "jiagu:")
			print(globals.linesep.join(jiagu.summarize(text, 2)))
			print(globals.linesep + "jionlp:")
			print(jionlp.summary.extract_summary(text))
			print(globals.linesep + "textrank4zh:")
			tr4s.analyze(text)
			tr4s_summaries = tr4s.get_key_sentences(2)
			for sentence_item in tr4s_summaries:
				print(sentence_item.sentence)
		
		# sentimental analysis
		print(globals.linesep + "================ Sentimental Analysis ================" + globals.linesep)
		for text in texts:
			print(globals.linesep + "jiagu:")
			print(jiagu.sentiment(text))
			print(globals.linesep + "jionlp:")
			jio_sentiment_analyzer = jionlp.sentiment.LexiconSentiment()
			print(jio_sentiment_analyzer(text), sep = globals.linesep)
	
	print(globals.linesep + "================ End Lexical & Semantic ================" + globals.linesep)

# END CSV TEST

# END TEST CODE
