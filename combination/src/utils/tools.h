//
// Created by vhdsih on 2019-06-15.
//

#ifndef PICTURECOLLAGEMAKER_TOOLS_H
#define PICTURECOLLAGEMAKER_TOOLS_H

#ifdef __APPLE__
#include <sys/uio.h>
#else
#include <sys/io.h>
#endif

#include <string>
#include <vector>


int get_files(const std::string &dir, std::vector<std::string> &files);

#endif //PICTURECOLLAGEMAKER_TOOLS_H
