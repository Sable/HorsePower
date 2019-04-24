
## SQL Overview

SQL syntax consists of

- Data definition language (DDL)
- Data manipulation language (DML)
- Data I/O commands
- Transaction control commands
- Other auxiliary commands

HorseIR current only supports the code generated from a subset of DML.

| DML                 | MonetDB | HorseIR | Comment                     |
| :-----------------: | :-----: | :-----: | :-------------------------: |
| [SELECT][nk_select] |   Y     |    Y    |                             |
| SELECT INTO         |   Y     |    N    |                             |
| WITH SELECT         |   Y     |    N    |                             |
| INSERT VALUES       |   Y     |    N    |                             |
| INSERT SELECT       |   Y     |    N    |                             |
| UPDATE              |   Y     |    N    |                             |
| DELETE              |   Y     |    N    |                             |
| TRUNCATE            |   Y     |    N    |                             |
| PREPARE             |   Y     |    N    |                             |
| EXECUTE             |   Y     |    N    |                             |
| CALL                |   Y     |    N    |                             |
| SET                 |   Y     |    N    |                             |
| SET TIME ZONE       |   Y     |    N    |                             |
| SET CURRENT_TIMEZONE|   Y     |    N    |                             |

[nk_select]: #select

## SELECT

```
   SELECT <selection list>
    [ FROM <table_view_subselect_function list> [<alias>] ]
    [ WHERE <condition list> ]
    [ GROUP BY <grouping list> ]
    [ HAVING <group conditon list> ]
    [ { UNION | INTERSECT | EXCEPT } [ ALL | DISTINCT ] <select query> ]
    [ ORDER BY <ordering list [ASC | DESC] > ]
    [ LIMIT <pos nr> ]
    [ OFFSET <pos nr> ]
    [ SAMPLE <sample size> ]
```

**To-do: need to check SQL standard reference to get more details.**

## Reference

1. [MonetDB SQL Syntax Overview](https://www.monetdb.org/Documentation/Manuals/SQLreference/SQLSyntaxOverview)

