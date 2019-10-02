#!/bin/bash
targ_lib=libhorse.a
targ_folder=compile-static

make ${targ_lib} && make obj_clean && mkdir -p ${targ_folder} && mv ${targ_lib} ${targ_folder}
if [ $? = 0 ] ; then
    echo "Success: <${targ_lib}> has been moved to <${targ_folder}>"
else
    echo "Fails."
fi

