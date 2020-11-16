import csv
import re
import sys
import pandas as pd
import numpy as np
from sklearn.feature_extraction.text import CountVectorizer
from nltk.corpus import stopwords
from nltk.stem import PorterStemmer
from nltk.stem import WordNetLemmatizer
from nltk.stem.lancaster import LancasterStemmer
from sklearn.naive_bayes import MultinomialNB, BernoulliNB

data_set_file = sys.argv[1]
test_set_file = sys.argv[2]

# read files
train = pd.read_csv(data_set_file, sep = '\t', header=None)
test = pd.read_csv(test_set_file, sep = '\t', header=None)

# get train data
train_sentence = np.array(train[1])
y_train = np.array(train[2])

# get test data
test_sentence = np.array(test[1])
test_id = np.array(test[0])
# print(test_id)
y_test = np.array(test[2])

def get_legal_sentence(raw_sentence):
    url_pattern = re.compile(r'(https|http)+:([a-zA-Z]|[$-_@.&+]|[!*\(\),]|(?:%[0-9a-fA-F]))')
    illegal_char_pattern = r'[^#@_$%\sa-zA-Z\d]'
    final_sentence = []
    for sentence in raw_sentence:
        remove_url = re.sub(url_pattern, ' ', sentence)
        remove_illegal_char = re.sub(illegal_char_pattern, '', remove_url)
        final_sentence.append(remove_illegal_char)
    return final_sentence


legal_train_sentence = np.array(get_legal_sentence(train_sentence))
legal_test_sentence = np.array(get_legal_sentence(test_sentence))

count = CountVectorizer(token_pattern='[#@_$%\w\d]{2,}')
X_train_bag_of_words = count.fit_transform(legal_train_sentence)

# use the bag of words and get training set
X_test_bag_of_words = count.transform(legal_test_sentence)

clf = BernoulliNB()
model = clf.fit(X_train_bag_of_words, y_train)

predicted_y = model.predict(X_test_bag_of_words)

for i in range(len(test_sentence)):
    print(test_id[i], predicted_y[i])