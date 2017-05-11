
## GROUP BY and ORDER BY

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

