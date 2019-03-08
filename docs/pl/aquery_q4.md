## Query 4

```sql
SELECT avg(ClosePrice AS avg_close_price
FROM price
WHERE Id IN Russell2000 AND
TradeDate = startPeriod
```
