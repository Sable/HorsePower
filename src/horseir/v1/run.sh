#!/bin/bash
## ./run.sh <mode> ./horse -c cpu -q 5 -o f
## mode: test_

exe=./horse

usage(){
    printf '%s\n' \
      "1) fusion experiment"                                  \
      "   $1 fusion <id>  ## ${exe} -c cpu -q <id> -o fs" ""  \
      "2) save a dot file and generate a pdf"                 \
      "   $1 dot <id>     ## ${exe} -dot -q <id>" ""          \
      "3) load a complete database and dump to a bin folder"  \
      "   $1 dump         ## ${exe} -x 1" ""                  \
      "4) try experiment to print the result of type sizes"   \
      "   $1 type         ## ${exe} -x 3" ""                  \
      "5) test front-end with hand-craft examples"            \
      "   $1 front <file> ## ${exe} -f -n data/func/<file>" ""
}

helper(){
    usage $0 && exit 1
}

dot2pdf(){
    dot -Tpdf $1.dot -o $1.pdf
}

if [ "$#" -eq 1 ]; then
    mode=$1
    if [ ${mode} = "dump" ]; then
        (set -x && ${exe} -x 1)
        exit 1
    elif [ ${mode} = "type" ]; then
        (set -x && ${exe} -x 3)
    else
        helper $0
    fi
elif [ "$#" -eq 2 ]; then
    mode=$1
    qid=$2
    if [ ${mode} = "fusion" ]; then
        (set -x && ${exe} -c cpu -q ${qid} -o fs)
    elif [ ${mode} = "dot" ]; then
        tempFile="/tmp/dot-q${qid}"
        (set -x && ${exe} -dot -q ${qid} > ${tempFile}.dot) \
          && dot2pdf ${tempFile} \
          && echo "Saved! (dot) ${tempFile}.dot ; (pdf) ${tempFile}.pdf"
    elif [ ${mode} = "front" ]; then
        (set -x && ${exe} -f -n data/func/$2)
    else
        helper $0
    fi
else
    helper $0
fi

## generate all dot files
## for id in {1..22}; do ./run.sh dot $id; done

