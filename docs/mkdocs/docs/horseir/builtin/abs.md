# abs

!!! Note "Math Support Available"
    This website is going to provide examples in math, such as \( y = \sqrt{x^2-1} \).  Please come back to check later.

### Description

| Invocation      |  # of parameters | # of returns | isElementwise | Note                 |
|:---------------:|:----------------:|:------------:|:-------------:|:--------------------:|
| @abs(x)         | 1                | 1            | Yes           | Return absolute value|


### Type Rules

```no-highlight
bool -> bool
i8   -> i8
i16  -> i16
i32  -> i32
i64  -> i64
f32  -> f32
f64  -> f64
_    -> domain error
```

<!--
<table class="typetable">
<tr>
    <th>Input</th>
    <th>Output</th>
</tr>
<tr>
    <td><script>hgType('B', 'bool')</script></td>
    <td><script>hgType('B', 'bool')</script></td>
</tr>
<tr>
    <td><script>hgType('H', 'short')</script></td>
    <td><script>hgType('H', 'short')</script></td>
</tr>
<tr>
    <td><script>hgType('I', 'int (i32)')</script></td>
    <td><script>hgType('I', 'int (i32)')</script></td>
</tr>
<tr>
    <td><script>hgType('L', 'int (i64)')</script></td>
    <td><script>hgType('L', 'int (i64)')</script></td>
</tr>
<tr>
    <td><script>hgType('F', 'float (f32)')</script></td>
    <td><script>hgType('F', 'float (f32)')</script></td>
</tr>
<tr>
    <td><script>hgType('E', 'float (f64)')</script></td>
    <td><script>hgType('E', 'float (f64)')</script></td>
</tr>
<tr>
    <td>Others</td>
    <td>Domain Error</td>
</tr>
</table>
-->

### Shape Rules

[Monadic elementwise shape rules](../../../horseir/#monadic-elementwise)


### Examples

```no-highlight
    @abs((-1,2,3):i32)
(1,2,3):i32
```
