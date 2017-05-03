
## MAL syntax

[syntax](https://www.monetdb.org/book/export/html/316)

## MAL SQL - select

Type

    explain select x.id from (select cast(id as double) as id from sys.functions) as x where x.id > 1;

Output

    function user.main():void;
    barrier X_44 := language.dataflow();
        X_20 := bat.new(nil:oid,nil:str);
        X_28 := bat.append(X_20,"sys.x");
        X_23 := bat.new(nil:oid,nil:str);
        X_30 := bat.append(X_23,"id");
        X_24 := bat.new(nil:oid,nil:str);
        X_31 := bat.append(X_24,"double");
        X_25 := bat.new(nil:oid,nil:int);
        X_33 := bat.append(X_25,53);
        X_27 := bat.new(nil:oid,nil:int);
        X_35 := bat.append(X_27,0);
        X_1 := sql.mvc();
        X_5:bat[:int] := sql.bind(X_1,"sys","functions","id",0);
        (C_8,r1_8) := sql.bind(X_1,"sys","functions","id",2);
        X_11:bat[:int] := sql.bind(X_1,"sys","functions","id",1);
        X_13 := sql.delta(X_5,C_8,r1_8,X_11);
        X_14 := batcalc.dbl(X_13);
        C_2:bat[:oid] := sql.tid(X_1,"sys","functions");
        C_16 := algebra.thetasubselect(X_14,C_2,1,">");
        X_18 := algebra.projection(C_16,X_14);
        language.pass(X_14);
    exit X_44;
        sql.resultSet(X_28,X_30,X_31,X_33,X_35,X_18);
    end user.main;

[Source](https://www.monetdb.org/bugzilla/show_bug.cgi?id=6073)

## MAL example - function

     function sample(nme:str, val:any_1):bit;
        c := 2 * 3;
        b := bbp.bind(nme);  #find a BAT
        h := algebra.select(b,val,val);
        t := aggr.count(h);
        x := io.print(t);
        y := io.print(val);
     end sample;

[Source](https://www.monetdb.org/pipermail/users-list/2014-March/007200.html)

