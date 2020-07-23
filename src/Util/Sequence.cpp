#include "Sequence.h"
#include "Utils.h"

void getFileNames(const std::string& dir, std::vector<std::string>& vfileNames){
	fs::path path(dir);
	if (!fs::exists(path)){
		std::cerr << dir << " not exists" << std::endl;
	}
 
	fs::directory_iterator end_iter;
	for (fs::directory_iterator iter(path); iter!=end_iter; ++iter){
		if (fs::is_regular_file(iter->status())){
            vfileNames.push_back(iter->path().string());
		}
	}
    std::sort(vfileNames.begin(), vfileNames.end());
}

bool isSupportedPictureFormats(const std::vector<std::string>& vfileNames){
    if (vfileNames.size() == 0){
        return false;
    }
    std::string ext;
    for (int i = 0; i < vfileNames.size(); i++){
        ext = toUpper(getFileExtension(vfileNames[i]));
        if (ext != "BMP" &&
            ext != "PNG" &&
            ext != "JPG" &&
            ext != "TIF"){
            return false;
        }
    }
    return true;
}

bool isSupportedBinFormats(const std::vector<std::string>& vfileNames){
    if (vfileNames.size() == 0){
        return false;
    }
    std::string ext;
    for (int i = 0; i < vfileNames.size(); i++){
        ext = toUpper(getFileExtension(vfileNames[i]));
        if (ext != "BIN"){
            return false;
        }
    }
    return true;
}

void string_replace( std::string &strBig, const std::string &strsrc, const std::string &strdst){
    std::string::size_type pos = 0;
    std::string::size_type srclen = strsrc.size();
    std::string::size_type dstlen = strdst.size();

    while( (pos=strBig.find(strsrc, pos)) != std::string::npos )
    {
        strBig.replace( pos, srclen, strdst );
        pos += dstlen;
    }
}

std::string getPathName(std::string strFullName){
    if (strFullName.empty())
    {
        return "";
    }

    string_replace(strFullName, "\\", "/");

    std::string::size_type iPos = strFullName.find_last_of('/') + 1;

    return strFullName.substr(iPos, strFullName.length() - iPos);
}
