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


