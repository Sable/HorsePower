
## Note

A data path can be a folder or an alias pointing to a folder.

## Data on sable-intel

For different scale factors, we use different folders

    SF1 : db1
    SF2 : db2
    SF4 : db2
    SF8 : db8

Create an alias to a real data folder (e.g. db2 for SF1)

    ln -s /mnt/local/tpch-tbl/db2 .

Note, the size of database usually is very large so that its data may be stored
in a different folder.


## Data on sable-tigger

Please follow the same configuration on sable-intel, but you should be aware of
the possible difference of real data folders and create new aliases.


