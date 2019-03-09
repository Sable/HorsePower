## Query 7

```sql
-- encode execution of our trading strategy
FUNCTION
execStrategy(alloc, mavgday, mavgmonth, px) {
    buySignal := mavgday > mavgmonth;
    alloc * prd(
        CASE maxs(buySignal)
            WHEN TRUE THEN
                CASE buySignal
                    WHEN TRUE THEN 1 / px
                    ELSE px
                END
            ELSE 1
        END)
}

WITH
    pxdata(Id, TradeDate, ClosePrice) AS
    (SELECT
    Id, TradeDate, ClosePrice
    FROM price
    WHERE Id IN stock10 AND
    TradeDate >= max(TradeDate) - 365)

    splitdata(Id, SplitDate, SplitFactor) AS
    (SELECT Id, SplitDate, SplitFactor
    FROM split
    WHERE Id IN stock10 AND
    SplitDate >= max(SplitDate) - 365)

    splitadj(Id, TradeDate, ClosePrice) AS
    (SELECT
    Id, TradeDate,
    first(ClosePrice * prd(SplitFactor))
    FROM
    pxdata INNER JOIN splitdata USINGId
    WHERE TradeDate < SplitDate
    GROUP BY Id, TradeDate)

    adjpxdata(Id, TradeDate, ClosePrice) AS
    (SELECT
    Id, TradeDate,
    fill(pxdata.ClosePrice, splitadj.ClosePrice) AS ClosePrice
    FROM pxdata FULL OUTER JOIN splitadj
    USING (Id, TradeDate))

    movingAvgs(Id, TradeDate, ClosePrice, m21day, m5month) AS
    (SELECT
    Id, TradeDate, ClosePrice,
    avgs( 21, ClosePrice),
    avgs(160, ClosePrice)
    FROM adjpxdata
    ASSUMING ASC Id, ASC TradeDate
    GROUP BY Id)

    simulated AS (
    SELECT Id,
    execStrategu(10000, m21day, m5month, ClosePrice) AS result,
    last(m21day) > last(m5month) AS stillInvested
    FROM FLATTEN(movingAvgs)
    WHERE FLATTEN(movingAvgs)
    WHERE Id = prev(Id) AND
    (prev(m5month) <= prev(m21day) & m5month > m21day)
    OR
    (prev(m5month) >= prev(m21day) & m5month < m21day)
    GROUP BY Id)

    latestPxs AS (
    SELECT *
    FROM adjpxdata
    WHERE TradeDate = max(TradeDate))

    SELECT
    sum (
        fill(10000, result *
            CASE WHEN stillInvested
                THEN ClosePrice
            ELSE
                1
            END)
    ) AS stock_value
    FROM latestPxs FULL OUTER JOIN simulated
    USING Id
```
