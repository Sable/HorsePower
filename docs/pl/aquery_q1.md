## Query 1

```sql
WITH
    pxdata(Id, TradeDate, HighPrice, LowPrice, ClosePrice, OpenPrice, Volume) AS
    (SELECT
    Id, TradeDate,
    HighPrice, LowPrice,
    ClosePrice, OpenPrice,
    Volume
    FROM price
    WHERE Id IN stock1000 AND
    TradeDate >= start300Days AND
    TradeDate <= start300Days + 300)

    splitdata(Id, SplitDate, SplitFactor) AS
    (SELECT
    Id, SplitDate, SplitFactor
    FROM split
    WHERE Id IN stock1000 AND
    SplitDate >= start300Days)

    adjdata(Id, TradeDate, AdjFactor) AS
    (SELECT
    Id, TradeDate, prd(SplitFactor)
    FROM
    Pxdata INNER JOIN splitdata USING Id
    WHERE TradeDate < SplitDate
    GROUP BY Id, TradeDate)

    SELECT Id, TradeDate,
    HighPrice * fill(1, AdjFactor) as HIghPrice,
    LowPrice * fill(1, AdjFactor) as LowPrice,
    ClosePrice * fill(1, AdjFactor) as ClosePrice,
    OpenPrice * fill(1, AdjFactor) as OpenPrice,
    Volume / fill(1, AdjFactor) as Volume
    FROM
    pxdata FULL OUTER JOIN adjdata
    USING (Id, TradeDate)
    ASSUMING ASC Id, ASC TradeDate
```
