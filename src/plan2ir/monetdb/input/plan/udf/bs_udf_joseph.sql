-- CREATE TABLE myoptions
-- (
--     sptprice DECIMAL(15,6)
--     ,optionprice DECIMAL(15,6)
-- );
--  
-- INSERT INTO myoptions VALUES(42, 4.749423);


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
