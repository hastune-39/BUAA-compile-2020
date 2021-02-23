.data
thisisaenter: .asciiz "\n"
str0: .asciiz "18182651"
str1: .asciiz "empty_sentence"
str2: .asciiz "small"
str3: .asciiz "not equal"
.text
#变量存储基准:
#$gp
li $k0, 269549568
li $k1, 270598144
##############start############## 

#全局变量初始化:
li $s2, 0
sw $s2, 0($gp)
li $s2, 0
sw $s2, 4($gp)
li $s2, 0
sw $s2, 8($gp)
li $s2, 0
sw $s2, 12($gp)

#跳转到主函数:
j main
test_forsub: 
#保存返回地址: 
sw $ra, 4($k0)
li $s2, 0
sw $s2, 12($k0)
li $s2, 3
sw $s2, 8($k0)
for_b1: 

#for语句,在源代码第5行
lw $s1, 8($k0)
li $s2, 0
ble $s1, $s2, for_end3
lw $s2, 12($k0)
lw $s3, 8($k0)
add $s1, $s2, $s3
sw $s1, 20($k1)
lw $s2, 20($k1)
sw $s2, 12($k0)
lw $s2, 8($k0)
li $s3, 1
sub $s1, $s2, $s3
sw $s1, 12($k1)
lw $s2, 12($k1)
sw $s2, 8($k0)
j for_b1
for_end3: 
#函数返回值为: 
lw $a0, 12($k0)
#从函数退出中,清除空间并准备跳回... 
lw $s1, 0($k0)
lw $ra, 4($k0)
sub $k0, $k0, $s1
sub $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
test_foradd: 
#保存返回地址: 
sw $ra, 4($k0)
li $s2, 0
sw $s2, 12($k0)
li $s2, 0
sw $s2, 8($k0)
for_b4: 

#for语句,在源代码第13行
lw $s1, 8($k0)
li $s2, 3
bgt $s1, $s2, for_end6
lw $s2, 12($k0)
lw $s3, 8($k0)
add $s1, $s2, $s3
sw $s1, 48($k1)
lw $s2, 48($k1)
sw $s2, 12($k0)
lw $s2, 8($k0)
li $s3, 1
add $s1, $s2, $s3
sw $s1, 40($k1)
lw $s2, 40($k1)
sw $s2, 8($k0)
j for_b4
for_end6: 
#函数返回值为: 
lw $a0, 12($k0)
#从函数退出中,清除空间并准备跳回... 
lw $s1, 0($k0)
lw $ra, 4($k0)
sub $k0, $k0, $s1
sub $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
cal: 
#保存返回地址: 
sw $ra, 4($k0)
lw $s2, 8($k0)
lw $s3, 8($k0)
mul $s1, $s2, $s3
sw $s1, 60($k1)
#函数返回值为: 
lw $a0, 60($k1)
#从函数退出中,清除空间并准备跳回... 
lw $s1, 0($k0)
lw $ra, 4($k0)
sub $k0, $k0, $s1
sub $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
add: 
#保存返回地址: 
sw $ra, 4($k0)
lw $s2, 8($k0)
lw $s3, 12($k0)
add $s1, $s2, $s3
sw $s1, 68($k1)
#函数返回值为: 
lw $a0, 68($k1)
#从函数退出中,清除空间并准备跳回... 
lw $s1, 0($k0)
lw $ra, 4($k0)
sub $k0, $k0, $s1
sub $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
test_function_call: 
#保存返回地址: 
sw $ra, 4($k0)
li $s2, 1
sw $s2, 20($k0)
li $s2, 2
sw $s2, 20($k0)
#准备进入函数,正在分配空间...
add $k0, $k0, 12
add $k1, $k1, 10000 #奇怪的临时空间分配
li $s1, 12
sw $s1, 0($k0)
#跳转函数
jal cal
#取得有返回值函数调用结果,在源代码第29行
sw $a0, 92($k1)
lw $s2, 92($k1)
sw $s2, 24($k0)
#准备进入函数,正在分配空间...
add $k0, $k0, 12
add $k1, $k1, 10000 #奇怪的临时空间分配
li $s1, 12
sw $s1, 0($k0)
#跳转函数
jal add
#取得有返回值函数调用结果,在源代码第29行
sw $a0, 96($k1)
lw $s2, 96($k1)
sw $s2, 20($k0)
li $s2, 1
sw $s2, 20($k0)
li $s2, 2
sw $s2, 24($k0)
#准备进入函数,正在分配空间...
add $k0, $k0, 12
add $k1, $k1, 10000 #奇怪的临时空间分配
li $s1, 12
sw $s1, 0($k0)
#跳转函数
jal add
#取得有返回值函数调用结果,在源代码第30行
sw $a0, 116($k1)
lw $s2, 116($k1)
sw $s2, 20($k0)
#准备进入函数,正在分配空间...
add $k0, $k0, 12
add $k1, $k1, 10000 #奇怪的临时空间分配
li $s1, 12
sw $s1, 0($k0)
#跳转函数
jal cal
#取得有返回值函数调用结果,在源代码第30行
sw $a0, 120($k1)
lw $s2, 120($k1)
sw $s2, 24($k0)
#准备进入函数,正在分配空间...
add $k0, $k0, 12
add $k1, $k1, 10000 #奇怪的临时空间分配
li $s1, 12
sw $s1, 0($k0)
#跳转函数
jal add
#取得有返回值函数调用结果,在源代码第31行
sw $a0, 124($k1)
lw $s2, 124($k1)
sw $s2, 8($k0)
#写语句,在源代码第32行
li $v0, 1
lw $a0, 8($k0)
syscall
li $v0, 4
la $a0, thisisaenter
syscall
#从函数退出中,清除空间并准备跳回... 
lw $s1, 0($k0)
lw $ra, 4($k0)
sub $k0, $k0, $s1
sub $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
main: 
#写语句,在源代码第41行
li $v0, 4
la, $a0, str0
syscall
li $v0, 4
la $a0, thisisaenter
syscall
#准备进入函数,正在分配空间...
add $k0, $k0, 20
add $k1, $k1, 10000 #奇怪的临时空间分配
li $s1, 20
sw $s1, 0($k0)
#跳转函数
jal test_forsub
#取得有返回值函数调用结果,在源代码第42行
sw $a0, 136($k1)
lw $s2, 136($k1)
sw $s2, 0($k0)
#写语句,在源代码第43行
li $v0, 1
lw $a0, 0($k0)
syscall
li $v0, 4
la $a0, thisisaenter
syscall
#准备进入函数,正在分配空间...
add $k0, $k0, 20
add $k1, $k1, 10000 #奇怪的临时空间分配
li $s1, 20
sw $s1, 0($k0)
#跳转函数
jal test_foradd
#取得有返回值函数调用结果,在源代码第44行
sw $a0, 148($k1)
lw $s2, 148($k1)
sw $s2, 8($k0)
#写语句,在源代码第45行
li $v0, 1
lw $a0, 8($k0)
syscall
li $v0, 4
la $a0, thisisaenter
syscall
#写语句,在源代码第46行
li $v0, 1
lw $a0, 24($gp)
syscall
li $v0, 4
la $a0, thisisaenter
syscall
#写语句,在源代码第47行
li $v0, 4
la, $a0, str1
syscall
li $v0, 4
la $a0, thisisaenter
syscall
li $v0, 5
syscall
sw $v0, 4($k0)
#写语句,在源代码第51行
li $v0, 1
lw $a0, 4($k0)
syscall
li $v0, 4
la $a0, thisisaenter
syscall
lw $s2, 4($k0)
sw $s2, 28($k0)
#准备进入函数,正在分配空间...
add $k0, $k0, 20
add $k1, $k1, 10000 #奇怪的临时空间分配
li $s1, 20
sw $s1, 0($k0)
#跳转函数
jal cal
#取得有返回值函数调用结果,在源代码第52行
sw $a0, 180($k1)
lw $s2, 180($k1)
sw $s2, 4($k0)
#写语句,在源代码第53行
li $v0, 1
lw $a0, 4($k0)
syscall
li $v0, 4
la $a0, thisisaenter
syscall
#准备进入函数,正在分配空间...
add $k0, $k0, 20
add $k1, $k1, 10000 #奇怪的临时空间分配
li $s1, 20
sw $s1, 0($k0)
#跳转函数
jal test_function_call
li $v0, 5
syscall
sw $v0, 12($k0)
#写语句,在源代码第56行
li $v0, 1
lw $a0, 12($k0)
syscall
li $v0, 4
la $a0, thisisaenter
syscall
li $v0, 5
syscall
sw $v0, 16($k0)

#if条件语句,源代码第58行
lw $s1, 12($k0)
lw $s2, 16($k0)
bge $s1, $s2, else7
#写语句,在源代码第59行
li $v0, 4
la, $a0, str2
syscall
li $v0, 4
la $a0, thisisaenter
syscall
j end8
else7: #else标签，源代码第60行
#写语句,在源代码第61行
li $v0, 4
la, $a0, str3
syscall
li $v0, 4
la $a0, thisisaenter
syscall
end8: 
#主函数退出: 
li $v0, 10
syscall
