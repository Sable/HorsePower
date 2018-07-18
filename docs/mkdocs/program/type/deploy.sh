#!/bin/bash
from=genpdf/*.png
dest=../../docs/horseir/types/

#list=(lt eq plus minus mul mod logic append compress indexof order member vector
list=(vector)

for func in ${list[@]}
do
    echo generating $func.png
    ./run.sh $func
    echo copying genpdf/$func.png to $dest
    cp genpdf/$func.png $dest
done

