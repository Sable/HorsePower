#!/bin/bash

folder="online-query"
for id in {1..22}
do
    echo "Downloading from HyPer: q$id"
    mkdir -p ${folder} && wget http://hyper-db.de/queries/$id.sql -O ${folder}/q$id.sql
done
echo "Done."
