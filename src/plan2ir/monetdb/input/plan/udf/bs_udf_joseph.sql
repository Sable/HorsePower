SELECT
    *
FROM
    myudf_bs((select * from blackscholes)) bu,
    myoptions mo
WHERE
    bu.sptprice = mo.sptprice
    AND bu.optionprice > mo.optionprice
;
-- return 1049 rows
