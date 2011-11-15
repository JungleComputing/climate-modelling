#ifndef _UTIL_H_
#define _UTIL_H_

#define DELETE_ARG { for (cnt=i; cnt<(*argc)-1; cnt++){ \
                       (*argv)[cnt] = (*argv)[cnt+1]; \
                     }\
                     changed = 1;\
                     (*argc)--;\
                   }

#endif

