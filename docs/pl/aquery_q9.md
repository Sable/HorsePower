## Query 9

```sql
WITH
    DateInfo(startYear, startDate) AS
    (SELECT
    getYear(max(TradeDate) - 365 * 3), max(TradeDate) - 365 * 3
    FROM price)

    // we introduce a dummy column to avoid having q
    // group on distinct
    splitdata(Dummy, Id) AS
    (SELECT 1, distinct(Id)
    FROM split
    WHERE Id IN Russell2000 AND
    getYear(SplitDate) >= first(DateInfo("startYear")))

    nosplit_avgpx AS
    (SELECT
    Id, year, avg(ClosePrice) as avg_px
    FROM price
    WHERE Id IN Russell2000 AND
    TradeDate >= firstDateInfo("startDate")
        ) AND
    Id NOT IN splitdate("Id")
    GROUP BY Id, getYear(TradeDate) as year)

    divdata AS
    (SELECT
    Id, year, sum(DivAmt) as total_divs
    FROM dividend
    WHERE Id IN Russell2000 AND
    getYear(AnnounceDate) >= first(DateInfo("startYear")) AND
    Id NOT IN splitdata("Id")
    GROUP BY Id, getYear(AnnounceDate) as year)

    SELECT
    Id, year, avg_px,
    total_divs, total_divs / avg_px as yield
    FROM nosplit_avgpx INNER JOIN divdata
    USING (Id, year)
```
