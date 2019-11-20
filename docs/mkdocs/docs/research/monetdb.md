### Overview

The collection of MonetDB related knowledge and practice.

- [MonetDB on GitHub](https://github.com/MonetDB/MonetDB)
- Clauses: [Plan][Plan], [Explain][Explain]

[Explain]: https://www.monetdb.org/Documentation/Manuals/SQLreference/Explain
[Plan]: https://www.monetdb.org/Documentation/Manuals/SQLreference/PlanSQL

### mclient

+ Control ouput format (`mclient -f <format>`)
    - `tab`: default
    - `raw`: no delimiters
    - `xml`: ugly

### Specific Commands

Output raw plans (sableintel)

```
(cd /mnt/local/script-tpch && ./getplan_raw)
```

Standard input

```
echo 'some query here.' | monetdb -d <database>
```

### Appendix

Source code links

- [mclient.c](https://github.com/MonetDB/MonetDB/blob/59e09736d74e880ebc8f06651b0478cf801ceb7c/clients/mapiclient/mclient.c#L87)

Others

- [MonetDB keys](http://wukefe.github.io/bookreview/keys/monetdb/)

