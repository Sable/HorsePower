## How to add a built-in function?

Supposedly, we have the following four basic function.

```
  plus  /  minus  /  mul   /  div
```

Then, we could define a method for the following formula.

```
  y = x^2 + 1;  // method 'gold'
```

where the variable `x` is the input argument.  However, we may have no idea
what its type and shape so far.  So we can have the following code to include
every possible case.

```
module Builtin {
    def gold(x:?):?{
        t0:? = @mul(x, x);
        t1:? = @plus(t0, 1:64);
        return t1;
    }
}
```

We use `?` to represent an unknown type.  Also, we support function overloading.

```
module Builtin {
    def gold(x:?):?{
        t0:? = @mul(x, x);
        t1:? = @plus(t0, 1:64);
        return t1;
    }
    def gold(x:f32, y:f32):f32{
        t0:f32 = @mul(x, y);
        t1:f32 = @plus(t0, 1:64);
        return t1;
    }
}
```



