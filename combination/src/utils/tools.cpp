//
// Created by vhdsih on 2019-06-15.
//

#include "tools.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>


int get_files(const std::string &dir, std::vector<std::string> &files) {
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        std::cout << "Error(" << errno << ") opening " << dir << std::endl;
        return errno;
    }
    while ((dirp = readdir(dp)) != NULL) {
        files.emplace_back(dirp->d_name);
    }
    closedir(dp);
    return 0;
}
