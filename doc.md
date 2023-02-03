## Basic syntax
### Comments
```owl
# A comment
var := 10 # Another comment
```
### Number literals
```owl
i1 := 100
f1 := 12.34
i2 := 1_000_000 # We can add '_' for better readability
f2 := 2.3e4 # We also have scientific notation
```
### String literals
```owl
c := 'a'
str := "this is a string"
str2 := "\n\t" # We have special characters
str3 := "\x41" # And also hexa encoding
```
## Basic types
### Integers
```
i8, i16, i32, i64
int ~ i32
```
### Unsigned integers
```
u8, u16, u32, u64
uint ~ u32
```
### Floats
```
f16, f32, f64
float ~ f32
```
### Other
```
bool ~ u8
char ~ u8 
string
```
## Variable declaration
```owl
x : int     # Variable declaration
x = 42      # Variable assignation
x: int = 42 # We can do both in one line
y := 123    # Type can be omitted - it will be inferred
```
## Constant declaration
```owl
x: int : 55
str :: "Bonjour" # Constant type can be infered
```