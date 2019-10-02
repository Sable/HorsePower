
Description

    Folder contains a list of test examples

    t/*.hir   ## pass cases
    f/*.hir   ## fail cases
    run.sh    ## running script

View code description (1st line)

    cat t/t0.hir | head -1

Generate docs from test cases

     (cd t && ls *.hir | wc -l | xargs python ../gen-docs.py)

     or

     (cd f && ls *.hir | wc -l | xargs python ../gen-docs.py)

     or

     ./run.sh view


