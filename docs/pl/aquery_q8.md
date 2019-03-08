## Query 8

```sql
FUNCTION covariance(x, y) {
    xmean := avg(x);
    ymean := avg(y);
    avg((x - xmean) * (y - ymean))
}

FUNCTION sd(x) {
    sqrt(covariance(x, x))
}

FUNCTION pairCorr(x, y) {
    covariance(x, y) / (sd(x) * sd(y))
}
<q> pairCorrEach:pairCorr'</q>

WITH
    stockGrouped(Id, ClosePrice) AS
    (SELECT
    Id, ClosePrice
    FROM price
    ASSUMING ASC Id, ASC TradeDate
    WHERE Id IN stock10 AND
    TradeDate >= startYear10 AND
    TradeDate <= startYear10 + 365 * 2
    GROUP BY Id)

    pairsGrouped(Id1, Id2, ClosePrice1, ClosePrice2) AS
    (SELECT
    st1.Id, st2.Id,
    st1.ClosePrice, st2.ClosePrice
    FROM
    stockGrouped st1, stockksGrouped st2
    GROUP BY st1.Id, st2.Id)

    corrTable(Id1, Id2, corrCoeff) AS
    (SELECT
    Id1, Id2,
    pairCorrEach(ClosePrice1, ClosePrice2)
    FROM pairsGrouped
    WHERE Id1 != Id2)

    SELECT *
    FROM corrTable
    ASSUMING ASC corrCoeff
```



