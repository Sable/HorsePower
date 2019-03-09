## Query 5

```sql
WITH
    pxdata(Id, TradeDate, ClosePrice) AS
    (SELECT
    Id, TradeDate, ClosePrice
    FROM price
    WHERE Id IN stock1000 AND
    TradeDate >= start6Mo AND
    TradeDate <  start6Mo + 31 * 6)

    splitdata(Id, SplitDate, SplitFactor) AS
    (SELECT
    Id, SplitDate, SplitFactor
    FROM split
    WHERE Id IN stock1000 AND
    SplitDate >= start6Mo)

    splitadj(Id, TradeDate, ClosePrice) AS
    (SELECT
    Id, TradeDate,
    first(ClosePrice * prd(SplitFactor))
    FROM pxdata INNER JOIN splitdata USING Id
    WHERE TradeDate < SplitDate
    GROUP BY Id, TradeDate)

    avgInfo(Id, TradeDate, ClosePrice, m21, m5) AS
    (SELECT
    Id,
    TradeDate,
    fill(pxdata.ClosePrice, splitadj.ClosePrice),
    avgs(21, fill(pxdata.ClosePrice, splitadj.ClosePrice))
    FROM pxdata FULL OUTER JOIN splitadj
    USING (Id, TradeDate)
    ASSUMING ASC Id, ASC TradeDate
    GROUP BY Id)

    SELECT *
    FROM FLATTEN(avgInfo)
    ASSUMING ASC Id, ASC TradeDate
```

