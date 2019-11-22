
## Combinations

Environment

    - MonetDB

Plans

    - TPC-H queries
    - AIDA queries

Formats

    - Input       : raw
    - Intermediate: JSON
    - Output      : HorseIR

Output

    - dump: show JSON objects
    - show: show operators
    - dot : create a dot diagram for operators

## UDF Functions

### Example from the Froid paper

Derived query 6

	-- query 6
	select
		   sum(l_extendedprice*l_discount) as revenue
	from
		   lineitem
	where
	   q6_cond_proc(l_shipdate, l_quantity, l_discount)
	;

Procedure defined in the WHERE clause

	-- procedure
	CREATE OR REPLACE FUNCTION q6_cond_proc(shipdate date, quantity float, discount float)
	RETURNS boolean
	BEGIN
	   declare date1 date;
	   declare date2 date;
	   set date1 = '1994-01-01';
	   set date2 = '1995-01-01';
	   IF (quantity >= 24)
	   THEN RETURN 0;
	   END IF;
	   IF (shipdate < date1)
	   THEN RETURN 0;
	   END IF;
	   IF (shipdate >= date2)
	   THEN RETURN 0;
	   END IF;
	   IF (discount < 0.05)
	   THEN RETURN 0;
	   END IF;
	   IF (discount > 0.07)
	   THEN RETURN 0;
	   END IF;
	   RETURN 1;
	END;

Execution plans fetched

    ./genplan_udf.sh q6_proc | mclient -i -d tpch2 -f raw > plans/udf/q6_proc.plan

Run translator to generate HorseIR code (key)

    ./run.sh udf q6_proc


