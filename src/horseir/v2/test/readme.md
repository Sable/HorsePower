
Description

    Folder contains a list of test examples

    pass/*.hir   ## pass cases
    fail/*.hir   ## fail cases
    run.sh       ## running script

View code description (1st line)

    cat t/t0.hir | head -1

Generate docs from test cases

     (cd pass && ls *.hir | wc -l | xargs python ../gen-docs.py)

     or

     (cd fail && ls *.hir | wc -l | xargs python ../gen-docs.py)

     or

     ./run.sh view

Note

    cp -r ../v1/data/func tests


