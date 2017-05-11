# Examples for database operations

## GROUP BY

[Source](https://www.w3schools.com/sql/sql_groupby.asp)

Table

```
CustomerID   : i32
CustomerName : str
ContactName  : str
Address      : str
City         : str
PostalCode   : str
Country      : sym
```

SQL

```sql
SELECT COUNT(CustomerID), Country
FROM Customers
GROUP BY Country
ORDER BY COUNT(CustomerID) DESC;
```

HorseIR

```
modeul _default{
    c0:dict<sym,i32> = column(Customers:table, `CustomerID:sym);
    c1:dict<sym,str> = column(Customers:table, `CustomerName:sym);
    c2:dict<sym,str> = column(Customers:table, `ContactName:sym);
    c3:dict<sym,str> = column(Customers:table, `Address:sym);
    c4:dict<sym,str> = column(Customers:table, `City:sym);
    c5:dict<sym,str> = column(Customers:table, `PostalCode:sym);
    c6:dict<sym,sym> = column(Customers:table, `Country:sym);

    t0:list<i32> = value(c6);
    t1:list<i32> = unique(t0);
    t2:list<list<i32>,list<i32>> = outer(`equal:fun, t1, t0);
    t3:list<i32> = reduct(`plus:fun, t2);
    t4:list<i32> = reduct(`plus:fun, t2);
    t5:list<i32> = desc(t4);

    c7:dict<sym,i32> = dict(`count_customerid:sym;, t3); //group by
    c8:dict<sym,i32> = c7[t5];
    c9:dict<sym,sym> = c6[t5];

    z0:list<?> = list(c8,c9);
    z:table = createTable(z0);
}
```

## ORDER BY

[Source](https://www.w3schools.com/sql/sql_orderby.asp)

*Note*: the same table `Customers` in [GROUP BY](#group_by).

SQL

```sql
SELECT * FROM Customers
ORDER BY Country ASC, CustomerName DESC;
```

HorseIR

```
modeul _default{
    c0:dict<sym,i32> = column(Customers:table, `CustomerID:sym);
    c1:dict<sym,str> = column(Customers:table, `CustomerName:sym);
    c2:dict<sym,str> = column(Customers:table, `ContactName:sym);
    c3:dict<sym,str> = column(Customers:table, `Address:sym);
    c4:dict<sym,str> = column(Customers:table, `City:sym);
    c5:dict<sym,str> = column(Customers:table, `PostalCode:sym);
    c6:dict<sym,sym> = column(Customers:table, `Country:sym);

    t0:list<i32> = value(c6);
    t1:list<str> = value(c1);
    t2:list<?>   = list(t0, t1);
    t3:list<bool>= list(1:bool, 0:bool); // ASC:true; DESC: false
    t4:list<i32> = order(t2, t3);

     c7:dict<sym,i32> = c0[t4];
     c8:dict<sym,i32> = c1[t4];
     c9:dict<sym,i32> = c2[t4];
    c10:dict<sym,i32> = c3[t4];
    c11:dict<sym,i32> = c4[t4];
    c12:dict<sym,i32> = c5[t4];
    c13:dict<sym,i32> = c6[t4];

    z0:list<?> = list(c7,c8,c9,c10,c11,c12,c13);
    z:table = createTable(z0);
}
```

ELI code for `order by`

```
    x <- 1 2 3 2 2
    y <- 5 3 7 1 2
    <x,[1.5]y
1 4 5 2 3
    x[1 4 5 2 3]
1 2 2 2 3
    y[1 4 5 2 3]
5 1 2 3 7
```

## MIN and MAX

[Source](https://www.w3schools.com/sql/sql_min_max.asp)

Table `Products`

```
ProductID   : i32
ProductName : str
SupplierID  : i32
CategoryID  : i32
Unit        : str
Price       : f64
```

SQL

```sql
SELECT MIN(Price) AS SmallestPrice
FROM Products;
```

HorseIR

```
modeul _default{
    c0:dict<sym,i32> = column(Products:table, `ProductID:i32);
    c1:dict<sym,str> = column(Products:table, `ProductName:str);
    c2:dict<sym,str> = column(Products:table, `SupplierID:i32);
    c3:dict<sym,str> = column(Products:table, `CategoryID:i32);
    c4:dict<sym,str> = column(Products:table, `Unit:str);
    c5:dict<sym,str> = column(Products:table, `Price:f64);

    t0:list<f64> = value(c5);
    t1:list<f64> = min(t0);
    c6:dict<sym,f64> = dict(`SmallestPrice:sym, t1);

    z0:list<dict<sym,f64>> = list(c6)
    z:table = createTable(z0); 
}
```

