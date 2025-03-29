; ModuleID = 'matrix_mul'
source_filename = "matrix_mul.cpp"

define void @matmul() {
entry:
  %i = alloca i32
  %j = alloca i32
  %k = alloca i32
  %a = load i32, i32* %i
  %b = load i32, i32* %j
  %mul = mul i32 %a, %b
  store i32 %mul, i32* %k
  ret void
}
