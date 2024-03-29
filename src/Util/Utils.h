#ifndef UTILS_H
#define UTILS_H

#include <ctime>
#include <string>
#include <algorithm>
#include <limits>

/**
 * Split a string by a delimiter and get last substring
 * @param	s		    Input string
 * @param	delimiter	Delimiter
 * @return	Last substring
 */
std::string splitAndGetLast(std::string s, const std::string &delimiter);

std::string splitAndGetPrevious(std::string s, const std::string &delimiter);

/**
 * Get file extension
 * @param	s	Filename
 * @return	File extension
 */
std::string getFileExtension(const std::string &s);

std::string getFileNameNoExtension(const std::string &s);

/**
 * Return a string in uppercase
 * @param	s	Input string
 * @return	To upper string
 */
std::string toUpper(std::string s);

/**
 * Return the current date with the format YYYYMMDDHHMMSS
 * @return	String with formatted date
 */
std::string getCurrentDate();

#endif // UTILS_H
