## Experimental settings

### List of machines

| Name          | CPU (GFlops)                | Inst.(Up to)| Threads     | RAM      | GPU                                   | Cores   |
| :-------------| :-------------------------: | :---------: | :-----------| :------- | :------------------------------------:| :-----: |
| sable-tigger  | i7-8700K  @ 3.70GHz (320GF) | AVX2        | 12  (1x12)  | 32  GB   | [GeForce GTX 1080 Ti (c6.1)][gtx1080] | 3584    |
| sable-beaver  | i7-4770   @ 3.40GHz (177GF) | AVX2        | 4  (1x4)    | 16  GB   | (See below)                           | ...     |
| sable-lynx    | i7-3820   @ 3.60GHz (105GF) | AVX         | 8  (1x8)    | 8   GB   | [GeForce GTX 560 Ti (c2.1)][gtx560]   | 384     |
| sable-deer    | i7-3820   @ 3.60GHz (105GF) | AVX         | 8  (1x8)    | 16  GB   | [GeForce GTX 560 Ti (c2.1)][gtx560]   | 384     |
| sable-cheetah | i7-3930K  @ 3.20GHz (135GF) | AVX         | 12 (1x12)   | 16  GB   | [GeForce GT 720 (c3.5)][gt720]        | 192     |
| sable-intel   | E7-4850   @ 2.00GHz (240GF) | SSE4.2      | 80 (4x20)   | 128 GB   | N/A                                   | N/A     |
| chf-pc        | i7-3610QM @ 2.30GHz (87 GF) | AVX         | 8  (1x8)    | 8   GB   | [GeForce GT 630M (c2.1)][gt630]       | 96      |
| chf-mac       | i5-5257U  @ 2.70GHz (70 GF) | AVX2        | 4  (1x4)    | 8   GB   | [Intel Iris Graphics 6100][iris]      | N/A     |

New GPUs We Already Purchased

- [GeForce GTX 1070Ti (c6.1)][gtx1070]: 2432 cores      (sable-beaver,card 1)
- [Radeon Vega 64][radeon64]          : 4096 processors (sable-beaver,card 2)
- [GeForce GTX 1080Ti (c6.1)][gtx1080]: 3584 cores      (sable-tigger)

[gt630]: https://www.geforce.com/hardware/notebook-gpus/geforce-gt-630m/specifications
[gt720]: https://www.geforce.com/hardware/desktop-gpus/geforce-gt-720/specifications
[gtx560]: https://www.geforce.com/hardware/desktop-gpus/geforce-gtx-560ti/specifications
[iris]: https://ark.intel.com/products/84985/Intel-Core-i5-5257U-Processor-3M-Cache-up-to-3_10-GHz
[gtx1070]: https://www.nvidia.com/en-us/geforce/products/10series/geforce-gtx-1070-ti
[radeon64]: https://gaming.radeon.com/en/product/vega/radeon-rx-vega-64/
[gtx1080]: https://www.nvidia.com/en-us/geforce/products/10series/geforce-gtx-1080-ti/?ncid=pa-pai-mdtgmdt-41599

*Note: From [Apple support][apple support], we know that chf-mac can work with OpenGL 4.1 and OpenCL 1.2.*

[apple support]:https://support.apple.com/en-ca/HT202823

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

