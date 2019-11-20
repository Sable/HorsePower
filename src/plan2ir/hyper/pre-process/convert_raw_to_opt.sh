#!/usr/bin/env bash

for x in {1..22}
do
    echo Processing query $x
    #python convert_raw_to_opt.py "raw/q${x}.json" > "opt/q${x}.json"       # not using OrderedDict
    python convert_raw_to_opt.py "../input-json/raw/q${x}.json" > "../output-hir/opt-order/q${x}.json"  # using OrderedDict
    #python convert_raw_to_opt.py "raw/q${x}.json" > "unopt/q${x}.json"
done


