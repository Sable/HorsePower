
## Overview

Description: folder contains a list of test examples

    pass/*.hir   ## pass cases
    fail/*.hir   ## fail cases
    run.sh       ## main script

    tpch         ## tpch queries
    udf          ## udf functions
    aquery       ## aquery examples



## Unit Test Section

View code description (1st line)

    cat pass/t0.hir | head -1


Generate docs from test cases

     (cd pass && ls *.hir | wc -l | xargs python ../gen-docs.py)

     or

     (cd fail && ls *.hir | wc -l | xargs python ../gen-docs.py)

     or

     ./run.sh view

## Legacy

Moving from v1 to v2

    cp -r src/horseir/v1/data/func src/horseir/v2/tests

Moving from v2 to v3

    mv src/horseir/v2/tests tests


