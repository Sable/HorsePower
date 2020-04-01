#!/bin/bash
for id in {1..22}
do
    echo "Downloading from HyPer: q${id}"
    wget http://hyper-db.de/queries/${id}.sql -O q${id}.sql
done
echo "Done."
