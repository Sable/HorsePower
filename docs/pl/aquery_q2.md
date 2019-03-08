## Query 2

```sql
WITH
    pxdata(Id, TradeDate, HighPrice, LowPrice) AS
    (SELECT
    Id, TradeDate,
    HighPrice, LowPrice
    FROM price
    WHERE Id IN stock1000 AND
    TradeDate BETWEEN startPeriod AND end Period)

    splitdata(Id, TradeDate, SplitFactor) AS
    (SELECT
    Id, SplitDate, SplitFactor
    FROM split
    WHERE Id IN stock1000 AND
    SplitDate BETWEEN startPeriod AND end Period)

    SELECT
    Id AS Id,
    TradeDate AS TradeDate,
    HighPrice - LowPrice as MaxDiff,
    FROM pxdata INNER JOIN splitdata
    USING (Id, TradeDate)
    ASSUMING ASC Id, ASC TradeDate
```
