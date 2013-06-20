
#include <io.h>

#include<string>
#include<iostream>

using namespace std;

int dfsFolder(const string &sfoldPath,vector<string> &vecFilename)
{
    _finddata_t Fileinfo;
    string strfind = sfoldPath + "\\*";
    long Handle = _findfirst(strfind.c_str(),&Fileinfo);
    if(-1 == Handle)
    {
        cout<<"can not match the folder path!\n"<<endl;
        exit(-1);
    do
    {
        if(Fileinfo.attrib & _A_SUBDIR)
        {
            if((strcmp(Fileinfo.name,".") != 0) && (strcmp(Fileinfo.name,"..") != 0))
            {
                string newPath = sfoldPath + "\\" + Fileinfo.name;
                dfsFolder(newPath,vecFilename);
            }
        }
        else {
            string sFilename = sfoldPath +"\\" + Fileinfo.name;
            vecFilename.push_back(sFilename);
        }
    }while(_findnext(Handle,&Fileinfo) == 0 );
    _findclose(Handle);
    return (int)vecFilename.size();
}
