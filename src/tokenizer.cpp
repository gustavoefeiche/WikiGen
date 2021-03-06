#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/tokenizer.hpp>
#include "pugixml.hpp"

#include "tokenizer.hpp"
#include "main.hpp"

std::vector<std::string> read_xml(std::string& fname) {
	pugi::xml_document document;

	if(!document.load_file(fname.c_str())) {
        std::cout << "Error while reading file. Exiting..." << std::endl;
        exit(EXIT_FAILURE); 
    }

	pugi::xml_node root = document.child("mediawiki");

	std::vector<std::string> mini_corpus;

	for(pugi::xml_node page: root.children("page")) {
		std::string content = page.child("revision").child_value("text");
		
		sanitize(content);
		std::vector<std::string> tokenized = tokenize(content);

		mini_corpus.insert(mini_corpus.end(), tokenized.begin(), tokenized.end());
	}

	if(mini_corpus.size() < N) {
		std::cout << "Need at least " << N << " words to create ngrams!" << std::endl;
		exit(EXIT_FAILURE);
	}

	return mini_corpus;
}

std::vector<std::string> tokenize(std::string& content) {
	tokenizer tok{content};

	std::vector<std::string> tokenized;

	for(tokenizer::iterator i = tok.begin(); i != tok.end(); ++i) {
		tokenized.push_back(*i);
	}

	return tokenized;
}

void sanitize(std::string& content) {
	std::replace_if(content.begin(), content.end(), is_unwanted_char, ' ');
	std::transform(content.begin(), content.end(), content.begin(), ::tolower);
}

bool is_unwanted_char(char c) {
	return !(c == '.' || c == ',' || c == '?') && !std::isalpha(c);
}