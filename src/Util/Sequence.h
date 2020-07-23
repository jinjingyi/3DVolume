#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <iostream>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

void getFileNames(const std::string& dir, std::vector<std::string>& fileNames);

bool isSupportedPictureFormats(const std::vector<std::string>& fileNames);

bool isSupportedBinFormats(const std::vector<std::string>& vfileNames);

void string_replace( std::string &strBig, const std::string &strsrc, const std::string &strdst);

std::string getPathName(std::string strFullName);

#endif
