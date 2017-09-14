## Experimental settings

### List of machines

| Name          | CPU                  | Clock    | Threads     | RAM      | GPU                          | Cores   |
| :-------------| :--------------------| :------: | :-----------| :------- | :----------------------------| :-----: |
| sable-lynx    | Intel Core i7-3820   | 3.60GHz  | 8  (1x8)    | 8   GB   | [GeForce GTX 560 Ti][gtx560] | 384     |
| sable-deer    | Intel Core i7-3820   | 3.60GHz  | 8  (1x8)    | 16  GB   | [GeForce GTX 560 Ti][gtx560] | 384     |
| sable-cheetah | Intel Core i7-3930K  | 3.20GHz  | 12 (1x12)   | 16  GB   | [GeForce GT 720][gt720]      | 192     |
| sable-intel   | Intel Xeon E7-4850   | 2.00GHz  | 80 (4x20)   | 128 GB   | N/A                          | N/A     |
| chf-pc        | Intel Core i7-3610QM | 2.30GHz  | 8  (1x8)    | 8   GB   | [GeForce GT 630M][gt630]     | 96      |

[gt630]: https://www.geforce.com/hardware/notebook-gpus/geforce-gt-630m/specifications
[gt720]: https://www.geforce.com/hardware/desktop-gpus/geforce-gt-720/specifications
[gtx560]: https://www.geforce.com/hardware/desktop-gpus/geforce-gtx-560ti/specifications

### MonetDB

Configurations on sable-intel 

```no-highlight
- Data: /mnt/local/datafarm
- Script: /mnt/local/script
  + ./runsql 6  // run tpch query 6
- TPC-H: /mnt/local/tpch_2_17_0
```

Query timing

```no-highlight
call sys.querylog_enable();
\<../tpch_2_17_0/chf/myqueries/6.sql
call sys.querylog_disable();
select id, query, avg(run) as run_time from sys.querylog_history group by id,query;
```

### OpenMP

OpenMP affinity ([link](http://pages.tacc.utexas.edu/~eijkhout/pcse/html/omp-affinity.html))

```no-highlight
OMP_PLACES=cores
OMP_PROC_BIND=spread
// or
GOMP_CPU_AFFINITY="0-37:4 1-38:4 2-39:4 3-40:4"
```

