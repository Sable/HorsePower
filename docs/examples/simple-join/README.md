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
    def find_valid_index(colVal:i64, indexBool:i64) : i64 {
        colSize   :? = @len(colVal);
        validBool :? = @lt(indexBool,colSize);
        indexSize :? = @len(indexBool);
        indexRange:? = @range(indexSize);
        validIndex:? = @compress(validBool, indexRange);
        return validIndex;
    }
    def find_valid_item(colVal:i64, indexBool:i64)  : i64 {
        colSize   :? = @len(colVal);
        validBool :? = @lt(indexBool,colSize);
        validItem :? = @compress(validBool, indexBool);
        return validItem;
    }
    def main() : table {
        a0:table = @load_table(`Employee);
        a1:table = @load_table(`Department);
        
        s0:sym = check_cast(@column_value(a0, `LastName)      , sym);
        s1:i64 = check_cast(@column_value(a0, `DepartmentID)  , i64);
        s2:i64 = check_cast(@column_value(a1, `DepartmentID)  , i64);
        s3:sym = check_cast(@column_value(a1, `DepartmentName), sym);        

        t0:i64 = @index_of       (s2,s1);
        t1:i64 = find_valid_index(s2,t0);
        t2:i64 = find_valid_item (s2,t0);

        r0:sym = @index(s0,t1);
        r1:i64 = @index(s1,t1);
        r2:sym = @index(s3,t2);

        k0:sym       = (`LastName,`DepartmentID,`DepartmentName);
        k1:list<sym> = @tolist(k0);
        k2:list<?>   = @list(r0,r1,r2);
        z:table      = @table(k1,k2);

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


