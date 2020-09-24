//
// Created by idz on 2019/4/7.
//

#ifndef MARSSIMULATOR_LOG_H
#define MARSSIMULATOR_LOG_H



#ifdef PRINT_LOG_YES

#define PRINT_LOG(where, type, action, state) \
   std::cout << where << "::" << type << " - " << action << " -> " << state << std::endl;

#else
#define PRINT_LOG(where, type, action, state) ;
#endif


#endif //MARSSIMULATOR_LOG_H
