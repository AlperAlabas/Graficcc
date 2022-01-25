#include "FileLoader.hpp"
#include<fstream>
#include<string>
#include<iostream>
#include<ios>
#include<sstream>
#include<limits>
#include<algorithm>
std::string FileLoader::getRawData(const std::string& fileName)
{
    std::ifstream dosya(fileName);
    
    

    if(!dosya.is_open())
    {
        std::cout<<fileName<<" - Dosya Acilamadi"<<std::endl;
        return  "";
    }

    std::string strData;
    char siradaki;
    while((siradaki=dosya.get())!=EOF)
    {
        strData +=siradaki;
    }
    
    return strData;
    
}
 