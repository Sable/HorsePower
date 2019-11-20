# HorseSQL: A plan-to-HorseIR Translator

## Overview

When a query is processed, it is first translated to an optimized execution
plan (short as "plan") by a database plan optimizer.  Later, the plan can be
used directly in an interpreter-based or compiler-based system.

We take the plan as the input for our system as database research has done
solid optimizations for database queries in the past decades.  Therefore,
we only need a translator to bridge the gap between a database plan and our
HorseIR language.

To make sure the step of translation is correct and accurate, we adopt the
data model of in-memory column store so that a column data can be presented
as a simple vector in HorseIR.  Moreover, HorseIR supports list type to store
compound data with various types in order to handle mixed data types after
database operations, such as the groupby operation.

In our design, we provide two front-ends from SQL to HorseIR due to two different
database systems, i.e. HyPer and MonetDB.  Both systems have relatively distinct
features on their own that makes it impossible to have a general solution.
Therefore, we decide to support both database systems:
(1) HyPer's translator is mainly for standard queries; and
(2) MonetDB's translator is mainly for standard queries and UDFs.

It should be noted that the HyPer's plan has a clean format in JSON with
annotations in details, while the MonetDB's plan is in a plain format with
little annotations.  To reduce the level of difficulty to understand MonetDB's
plan, we transform a MonetDB's plan to a JSON object (different from HyPer's)
as an intermediate layer before generating HorseIR code.


## Internal Links

- [HyPer's plan-to-HorseIR translator](hyper.md): For standard SQL queries.
- [MonetDB's plan-to-HorseIR translator](hyper.md): For standard queries and UDFs.
- [TPC-H Queries](tpch.md): List of 22 standard queries.



