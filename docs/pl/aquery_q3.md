## Query 3

```sql
SELECT avg(ClosePrice) as avg_close_price
FROM price
WHERE Id IN SP500 AND
TradeDate = startPeriod
```
