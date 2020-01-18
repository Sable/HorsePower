select
       sum(l_extendedprice*l_discount) as revenue
from
       lineitem
where
       q6_cond_proc(l_shipdate, l_quantity, l_discount)
;
