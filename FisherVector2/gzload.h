#ifndef GZLOAD_H_INCLUDED
#define GZLOAD_H_INCLUDED
#include <zlib.h>
#include <iostream>
#include <string>
#include <ctime>
using namespace std;
#define GZ_BUF_SIZE 1048576

bool gzLoad(string gzfn, string &out)
{
    //open .gz file
    gzFile gzfp = gzopen(gzfn.c_str(),"rb");
    if(!gzfp)
    {
        return false;
    }

    //read and add it to out
    unsigned char buf[GZ_BUF_SIZE];
    int have;
    while( (have = gzread(gzfp,buf,GZ_BUF_SIZE)) > 0)
    {

        out.append((const char*)buf,have);
    }

    //close .gz file
    gzclose(gzfp);
    return true;
}


#endif // GZLOAD_H_INCLUDED
