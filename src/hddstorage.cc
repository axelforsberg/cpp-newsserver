/* hddstorage.cc */

#include "hddstorage.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <dirent.h>
#include <sstream>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

namespace client_server {

	/*
	 * Constructor for HDDStorage
	 * Creates the "filesystem" directory if it not exists
	 */	
	HDDStorage::HDDStorage(){
		DIR *dir = opendir (ROOT_DIR.c_str());
		if(dir == 0){
			mkdir(ROOT_DIR.c_str(),S_IRWXU);
		}
	}

	/*
	 * Keep tracks of all uniqe numbers.
	 * Read and writes to a specified file existing in every directory
	 */
	size_t HDDStorage::getCount(string filepath){
		ifstream input(filepath.c_str());
		size_t t = 0;
		if (input == 0) {
			ofstream output(filepath.c_str());
			output << ++t;
		} else {
			input >> t;
			ofstream output(filepath.c_str());
			output << ++t;
		}
		return t;
			
	}
	
	/*
	 * Help method to assist deleteArticle().
	 * Deletes a specified file in a specifed newsgroup folder
	 */
	string HDDStorage::delFile(string ngName, size_t nbr){
		stringstream s1;
		s1 << nbr;
		stringstream s2;
		s2 << ROOT_DIR;
		s2 << ngName;
		s2 << "/";
		DIR *dir = opendir(s2.str().c_str());
		dirent *entry = readdir(dir);
		while(entry) {
			string temp;
			stringstream s3(entry->d_name);
			getline(s3,temp,'_');
			if(temp == s1.str()){
				s2 << entry->d_name;
				remove(s2.str().c_str());
				return "ANS_ACK";
			}
			entry = readdir(dir);
		}
		closedir(dir);
		return "ERR_ART_DOES_NOT_EXIST";
	}
	
	/*
	 * Private help method to assist deleteNewsgroup().
	 * Deletes all files in a specifed newsgroup folder
	 */
	void HDDStorage::delAllFiles(string ngName){
		stringstream s1;
		vector<string> list;
		s1 << ROOT_DIR;
		s1 << ngName;
		s1 << "/";
		DIR *dir = opendir(s1.str().c_str());
		dirent *entry = readdir(dir);
		while(entry) {
			stringstream s1(entry->d_name);
			if(s1.str() != ".."  && s1.str() !="."){
				list.push_back(entry->d_name);
			}
			entry = readdir(dir);
		}
		closedir(dir);

		for(string s: list){
			stringstream s3;
			s3 << ROOT_DIR;
			s3 << ngName;
			s3 << "/";
			s3 << s;
			remove(s3.str().c_str());
		}
	}
	
	/*
	 * Private help method to assist createArticle().
	 * Creates a file containing author, title and content
	 * Saves it in a specified folder
	 */
	void HDDStorage::createFile(string ngName, const string& title, const string& author, const string& content){
		stringstream ss; 
		ss << ROOT_DIR;
		ss << ngName;
		ss << "/";
		string temp = ss.str() + "counterlog";
		ss << getCount(temp);
		ss << "_";
		ss << title;
		ofstream output(ss.str());
		output << title << endl;
		output << author << endl;
		output << content << endl;
	}
	
	/*
	 * Private help method to assist listArticles().
	 * Saves the names and numbers on all files in a specified newsgroup folder
	 */
	vector<string> HDDStorage::listAllFiles(string ngName){
		vector<string> list;
		vector<string> result;
		stringstream s1;
		s1 << ROOT_DIR;
		s1 << ngName;
		s1 << "/";
		DIR *dir = opendir(s1.str().c_str());
		dirent *entry = readdir(dir);
		while(entry) {
			stringstream s2(entry->d_name);
			if(s2.str() != ".."  && s2.str() !="." && s2.str() != "counterlog"){
				list.push_back(entry->d_name);
			}
			entry = readdir(dir);
		}
		closedir(dir);
		
		if(list.size()==0){
			result.push_back("ERR_NO_ARTS");
			return result;
		}
		
		sort(list.begin(),list.end());
		
		for(string s: list){
			string temp;
			stringstream ss(s);
			getline(ss,temp,'_');
			result.push_back(temp);
			getline(ss,temp);
			result.push_back(temp);
		}
		return result;
	}
	
	/*
	 * Private help method to getArticle()
	 * Get a specified article in a specified newsgroup
	 * Saves the content to a vector and return it;
	 */
	vector<string> HDDStorage::getFile(string ngName, const size_t& nbr){
		vector<string> result;
		stringstream s1;
		stringstream s2;
		
		s1 << nbr;
		
		s2 << ROOT_DIR;
		s2 << ngName;
		s2 << "/";
		DIR *dir = opendir(s2.str().c_str());
		dirent *entry = readdir(dir);
		while(entry) {
			string temp;
			stringstream s3(entry->d_name);
			getline(s3,temp,'_');
			if(temp == s1.str()) {
				string filename = s2.str()+s3.str();
				ifstream input(filename.c_str());
				
				getline(input, temp);
				result.push_back(temp);
				
				getline(input, temp);
				result.push_back(temp);
				
				stringstream s4;
				s4 << input.rdbuf();
				string content = s4.str();
				content.pop_back();
				result.push_back(content);
		
				return result;
			}
			entry = readdir(dir);
		}
		
		result.push_back("ERR_ART_DOES_NOT_EXIST");
		return result;
	}
	
	/*
	 * List articles in a specified newsgroup
	 * Saves the names and numbers on all files in a vector
	 */
	vector<string> HDDStorage::listArticles(const size_t& nbr){
		vector<string> result;
		
		stringstream s1;
		s1 << nbr;
		DIR *dir = opendir(ROOT_DIR.c_str());
		dirent *entry = readdir(dir);
		while(entry) {
			string temp;
			stringstream s2(entry->d_name);
			getline(s2,temp,'_');
			if(temp == s1.str()) {
				return listAllFiles(entry->d_name);
			}
			entry = readdir(dir);
		}
		result.push_back("ERR_NG_DOES_NOT_EXIST");
		
		return result;
	}
	
	/*
	 * List all existing newsgroups
	 * Saves the names and numbers on all folders in a vector
	 */
	vector<string> HDDStorage::listNewsGroups(){
		vector<string> list;
		
		DIR *dir = opendir(ROOT_DIR.c_str());
		dirent *entry = readdir(dir);
		while (entry){
				stringstream se(entry->d_name);
				if(se.str() != ".."  && se.str() !="." && se.str() != "counterlog"){
					list.push_back(entry->d_name);
				}
				entry = readdir(dir);
			}
		closedir(dir);
		
		sort(list.begin(),list.end());
		
		vector<string> result;
		for(string s: list){
			string temp;
			stringstream ss(s);
			getline(ss,temp,'_');
			result.push_back(temp);
			getline(ss,temp);
			result.push_back(temp);
		}
		return result;
	}

	/*
	 * Creates a newsgroup
	 * Returns true if no newsgroup with that name already existed
	 */
	bool HDDStorage::createNewsGroup(const std::string& name){
		string temp;
		DIR *dir = opendir(ROOT_DIR.c_str());
		dirent *entry = readdir(dir);
		while(entry) {
			stringstream ss(entry->d_name);
			getline(ss,temp,'_');
			getline(ss,temp);
			if(temp == name) {
				return false;
			}
			entry = readdir(dir);
		}
		closedir(dir);
		stringstream se;
		se << ROOT_DIR;
		se << getCount(NG_COUNTER_LOG);
		se << "_";
		se << name;
		mkdir(se.str().c_str(),S_IRWXU);
		return true;
	}

	/*
	 * Deletes a newsgroup
	 * Returns true if a newsgroup with that number existed
	 */
	bool HDDStorage::deleteNewsGroup(const size_t& nbr){
		stringstream s1;
		s1 << nbr;
		DIR *dir = opendir(ROOT_DIR.c_str());
		dirent *entry = readdir(dir);
		while(entry) {
			string temp;
			stringstream s2(entry->d_name);
			getline(s2,temp,'_');
			if(temp == s1.str()) {
				stringstream s3;
				s3 <<ROOT_DIR;
				s3 <<entry->d_name;
				s3 << "/";
				if(remove(s3.str().c_str())!=0){
					delAllFiles(entry->d_name);
					remove(s3.str().c_str());
				}
				closedir(dir);
				return true;
			}
			entry = readdir(dir);
		}
		closedir(dir);
		return false;
	}

	/*
	 * Creates a article in a specified newsgroup
	 * Returns true if a newsgroup with that numbers exists.
	 */
	bool HDDStorage::createArticle(const size_t& nbr, const string& title, const string& author, const string& content){
		stringstream s1;
		s1 << nbr;
		DIR *dir = opendir(ROOT_DIR.c_str());
		dirent *entry = readdir(dir);
		while(entry) {
			string temp;
			stringstream s2(entry->d_name);
			getline(s2,temp,'_');
			if(temp == s1.str()) {
				stringstream s3(entry->d_name);
				createFile(s3.str(), title, author, content);
				return true;
			}
			entry = readdir(dir);
		}
		
		return false;	
	}

	/*
	 * Deletes a specified article in a specified newsgroup
	 * Returns a string with a succes / failure code
	 */
	string HDDStorage::deleteArticle(const size_t& n_nbr, const size_t& a_nbr){
		stringstream s1;
		s1 << n_nbr;
		DIR *dir = opendir(ROOT_DIR.c_str());
		dirent *entry = readdir(dir);
		while(entry) {
			string temp;
			stringstream s2(entry->d_name);
			getline(s2,temp,'_');
			if(temp == s1.str()) {
				return delFile(entry->d_name, a_nbr);
			}
			entry = readdir(dir);
		}
		closedir(dir);
		return "ERR_NG_DOES_NOT_EXIST";
	}

	/*
	 * Retrieve an articles in a specified newsgroup
	 * Returns a vector<string> with the title, author and content of the article
	 */
	vector<string> HDDStorage::getArticle(const size_t& n_nbr, const size_t& a_nbr){
		stringstream s1;
		s1 << n_nbr;
		DIR *dir = opendir(ROOT_DIR.c_str());
		dirent *entry = readdir(dir);
		while(entry) {
			string temp;
			stringstream s2(entry->d_name);
			getline(s2,temp,'_');
			if(temp == s1.str()) {
				stringstream s3(entry->d_name);
				return getFile(s3.str().c_str(),a_nbr);
			}
			entry = readdir(dir);
		}
		vector<string> result;
		result.push_back("ERR_NG_DOES_NOT_EXIST");
		return result;
	}
}






