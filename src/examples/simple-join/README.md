# A simple join - Employee and Department

Two tables, `Employee` and `Department`, join on a unique `ID`.

## SQL query

Table: Employee

```sql
CREATE TABLE Employee (LastName varchar(99), DepartmentID int);
```

Table: Department

```sql
CREATE TABLE Department (DepartmentID int, DepartmentName varchar(99));
```

Query

```sql
select * from Employee,Department
where Employee.DepartmentID = Department.DepartmentID;
```

## HorseIR

```
/*
 * varchar(99) -> sym
 * int         -> i64
 */
module default {
    import Builtin.*;
    def find_valid_index(colVal:list<i64>, indexBool:list<i64>) : list<i64> {
        colSize   :? = @len(colVal);
        validBool :? = @lt(indexBool,colSize);
        indexSize :? = @len(indexBool);
        indexRange:? = @range(indexSize);
        validIndex:? = @reduce(validBool, indexRange);
        return validIndex;
    }
    def find_valid_item(colVal:list<i64>, indexBool:list<i64>)  : list<i64> {
        colSize   :? = @len(colVal);
        validBool :? = @lt(indexBool,colSize);
        validItem :? = @reduce(validBool, indexBool);
        return validItem;
    }
    def main() : table {
        s0:list<sym> = check_cast(@column_value(`Employee,   `LastName)      , list<sym>);
        s1:list<i64> = check_cast(@column_value(`Employee,   `DepartmentID)  , list<i64>);
        s2:list<i64> = check_cast(@column_value(`Department, `DepartmentID)  , list<i64>);
        s3:list<sym> = check_cast(@column_value(`Department, `DepartmentName), list<sym>);

        t0:list<i64> = @index_of       (s2,s1);
        t1:list<i64> = find_valid_index(s2,t0);
        t2:list<i64> = find_valid_item (s2,t0);

        r0:list<sym> = @index(s0,t1);
        r1:list<i64> = @index(s1,t1);
        r2:list<sym> = @index(s3,t2);

        d0:dict<sym,sym> = @dict(`LastName      , r0);
        d1:dict<sym,i64> = @dict(`DepartmentID  , r1);
        d2:dict<sym,sym> = @dict(`DepartmentName, r2);

        z0:?    = @list(d0,d1,d2);
        z:table = @table(z0);
        return z;
    }
    
}
```

## Data sample

Employee

```
Rafferty       | 31 |
Jones          | 33 |
Heisenberg     | 33 |
Robinson       | 34 |
Smith          | 34 |
Williams       | 35 |
```

Department

```
31 | Sales       |
33 | Engineering |
34 | Clerical    |
35 | Marketing   |
```

**Expected output**

```
LastName       | ID | DepartmentName |
--------------------------------------
Rafferty       | 31 | Sables         |
Jones          | 33 | Engineering    |
Heisenberg     | 33 | Engineering    |
Robinson       | 34 | Clerical       |
Smith          | 34 | Clerical       |
Williams       | 35 | Marketing      |
```


