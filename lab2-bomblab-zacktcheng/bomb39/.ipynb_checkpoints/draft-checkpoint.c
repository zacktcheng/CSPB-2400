int phase_2() {
  // Six ags are firstly stored in rsp, rsp+4, rsp+8, etc.
  // Those six numbers are then copied to rbx, rbx + 4, etc.
  // Ommit initalization and args check
  int arr[6] = {0, 1, 1, 2, 3, 5};
  int i = 0;
  int fifth = arr[4];
  do {
    if (arr[i + 1] + arr[i] != arr[i + 2]) explode_bomb();
    i++;
  } while (i != fifth);
  // Termination
}

// Initialization
<+0>:     endbr64 
<+4>:     push   %rbp
<+5>:     push   %rbx
<+6>:     sub    $0x28,%rsp
<+10>:    mov    %rsp,%rsi
<+13>:    call   0x555555555ce7 <read_six_numbers>

// Args check
<+18>:    cmpl   $0x0,(%rsp) // test 1st arg against 0
<+22>:    jne    0x55555555560a <phase_2+31> // goto bomb if above operands unequal
<+24>:    cmpl   $0x1,0x4(%rsp) // test 2nd arg against 1
<+29>:    je     0x55555555560f <phase_2+36> // goto loop setup if above operands equal
<+31>:    call   0x555555555c9a <explode_bomb>

// Array steup
<+36>:    mov    %rsp,%rbx // copy over args to int arr[6]
<+39>:    lea    0x10(%rsp),%rbp // set int fifth = arr[4]

// Do-while loop
<+44>:    jmp    0x555555555622 <phase_2+55> // step in the control flow
<+46>:    add    $0x4,%rbx // increment i by 1 (4 bits)
<+50>:    cmp    %rbp,%rbx // test i against fifth
<+53>:    je     0x555555555633 <phase_2+72> // goto termination if above operands equal

<+55>:    mov    0x4(%rbx),%eax // arr[1]
<+58>:    add    (%rbx),%eax // arr[1] + arr[0]
<+60>:    cmp    %eax,0x8(%rbx) // test (arr[1] + arr[0]) against arr[2]
<+63>:    je     0x555555555619 <phase_2+46> // goto increment i if above operands equal
<+65>:    call   0x555555555c9a <explode_bomb>
<+70>:    jmp    0x555555555619 <phase_2+46> // after bomb, goto increment i

// Termination
<+72>:    add    $0x28,%rsp
<+76>:    pop    %rbx
<+77>:    pop    %rbp
<+78>:    ret

int phase_3() {

}

<+0>:     endbr64 

// Passing args for sscanf
<+4>:     sub    $0x18,%rsp
<+8>:     lea    0x8(%rsp),%rcx // for arg2
<+13>:    lea    0xc(%rsp),%rdx // for arg1
<+18>:    lea    0x1d73(%rip),%rsi # 0x5555555573c6 // arg format template = "%d %d"
<+25>:    mov    $0x0,%eax // set argCount = 0
<+30>:    call   0x5555555552e0 <__isoc99_sscanf@plt>

// Args check
<+35>:    cmp    $0x1,%eax // compare argCount with 1
<+38>:    jle    0x55555555567e <phase_3+68> // goto bomb if argCount <= 1
<+40>:    cmpl   $0x7,0xc(%rsp) // compare arg1 with 7
<+45>:    ja     0x5555555556bf <phase_3+133> // goto bomb if arg1 > 7 (default)

// Jump table setup
<+47>:    mov    0xc(%rsp),%eax // set temp = arg1
<+51>:    lea    0x1b4c(%rip),%rdx # 0x5555555571c0 // set a jump table jtb
<+58>:    movslq (%rdx,%rax,4),%rax // assign signed 32-bit jtb[i] to 64-bit temp
<+62>:    add    %rdx,%rax // temp becomes the address for indirect table jump
<+65>:    notrack jmp *%rax // redirect to some snippet based on temp address
<+68>:    call   0x555555555c9a <explode_bomb> // goto bomb
<+73>:    jmp    0x555555555662 <phase_3+40>

// arg1 == 1
<+75>:    mov    $0x2ce,%eax

// Termination
<+80>:    cmp    %eax,0x8(%rsp) // compare arg2 with new temp value
<+84>:    jne    0x5555555556d2 <phase_3+152> // goto bomb if above operands uneaqual
<+86>:    add    $0x18,%rsp
<+90>:    ret

// arg1 == 2
<+91>:    mov    $0x192,%eax
<+96>:    jmp    0x55555555568a <phase_3+80>
// arg1 == 3
<+98>:    mov    $0x34d,%eax
<+103>:   jmp    0x55555555568a <phase_3+80>
// arg1 == 4
<+105>:   mov    $0x1a3,%eax
<+110>:   jmp    0x55555555568a <phase_3+80>
// arg1 == 5
<+112>:   mov    $0x188,%eax
<+117>:   jmp    0x55555555568a <phase_3+80>
// arg1 == 6
<+119>:   mov    $0x2ba,%eax
<+124>:   jmp    0x55555555568a <phase_3+80>
// arg1 == 7
<+126>:   mov    $0x2de,%eax
<+131>:   jmp    0x55555555568a <phase_3+80>
// default
<+133>:   call   0x555555555c9a <explode_bomb>

<+138>:   mov    $0x0,%eax
<+143>:   jmp    0x55555555568a <phase_3+80>
// arg1 == 0
<+145>:   mov    $0xfa,%eax
<+150>:   jmp    0x55555555568a <phase_3+80>

<+152>:   call   0x555555555c9a <explode_bomb>
<+157>:   jmp    0x555555555690 <phase_3+86>

int phase_4(int arg1, int arg2) {
  if (sscanf(arg1, arg2) != 2) explode_bomb();
  if (arg2 < 4) explode_bomb();
  int res = func4(9, arg2);
  if (res != arg1) explode_bomb();
  return res;
}

<+0>:     endbr64

// Passing args for sscanf
<+4>:     sub    $0x18,%rsp
<+8>:     lea    0xc(%rsp),%rcx // for arg2
<+13>:    lea    0x8(%rsp),%rdx // for arg1
<+18>:    lea    0x1c99(%rip),%rsi # 0x5555555573c6 // arg format template = "%d %d"
<+25>:    mov    $0x0,%eax // set argCount = 0
<+30>:    call   0x5555555552e0 <__isoc99_sscanf@plt>

// Args check
<+35>:    cmp    $0x2,%eax // compare argCount with 2
<+38>:    jne    0x555555555748 <phase_4+52> // goto bomb if argCount != 2

// 
<+40>:    mov    0xc(%rsp),%eax // arg2
<+44>:    sub    $0x2,%eax // arg2 -= 2
<+47>:    cmp    $0x2,%eax // compare arg2 - 2 with 2
<+50>:    jbe    0x55555555574d <phase_4+57> // goto <+57> if arg - 2 >= 2
<+52>:    call   0x555555555c9a <explode_bomb>

<+57>:    mov    0xc(%rsp),%esi // arg2
<+61>:    mov    $0x9,%edi // 9
<+66>:    call   0x5555555556d9 <func4> // res = func(9, arg2)
<+71>:    cmp    %eax,0x8(%rsp) // compare res with arg1
<+75>:    jne    0x555555555766 <phase_4+82> // goto bomb if res != arg1
<+77>:    add    $0x18,%rsp
<+81>:    ret    

<+82>:    call   0x555555555c9a <explode_bomb>
<+87>:    jmp    0x555555555761 <phase_4+77>

int func4(int arg1, int arg2) {
  if (arg1 < 1) return 0;
  if (arg1 == 1) return arg2;
  return func4(arg1 - 2, arg2) + func4(arg1 - 1, arg2) + arg2;
}

<+0>:     endbr64

<+4>:     mov    $0x0,%eax // res

<+9>:     test   %edi,%edi // compare arg1 with 0
<+11>:    jle    0x555555555713 <func4+58> // goto ret if arg1 <= 0

<+13>:    push   %r12 // callee-saved reg
<+15>:    push   %rbp // frame pointer
<+16>:    push   %rbx // base pointer

<+17>:    mov    %edi,%ebx // hold current stack arg1
<+19>:    mov    %esi,%ebp // hold current stack arg2
<+21>:    mov    %esi,%eax // res = arg2

<+23>:    cmp    $0x1,%edi // compare arg1 with 1
<+26>:    je     0x55555555570e <func4+53> // goto termination if arg1 == 1

<+28>:    lea    -0x1(%rdi),%edi // arg1 - 1
<+31>:    call   0x5555555556d9 <func4>
<+36>:    lea    (%rax,%rbp,1),%r12d // func_res + arg2
<+40>:    lea    -0x2(%rbx),%edi // arg1 - 2
<+43>:    mov    %ebp,%esi // arg2 = arg2
<+45>:    call   0x5555555556d9 <func4> 
<+50>:    add    %r12d,%eax

// Termination
<+53>:    pop    %rbx
<+54>:    pop    %rbp
<+55>:    pop    %r12
<+57>:    ret    
<+58>:    ret

int phase_5(arg1, arg2) { // arg1 must not be 16n+15
  int eax = 0;
  if (sscanf(arg1, arg2) < 2) explode_bomb();
  eax = arg1 % 16;
  arg1 = eax;
  if (arg1 == 15) explode_bomb();
  
  int sum = 0;
  int i = 0;
  
  do {
    i++;
    eax = arr[eax];
    sum += eax;
  } while (eax != 15);
  
  arg1 = 15;
  if (i != 15) explode_bomb();
  if (sum != arg2) explode_bomb();
  return eax;
}

<+0>:     endbr64 
<+4>:     sub    $0x18,%rsp
<+8>:     lea    0x8(%rsp),%rcx // for arg2
<+13>:    lea    0xc(%rsp),%rdx // for arg1
<+18>:    lea    0x1c40(%rip),%rsi # 0x5555555573c6 // arg format template = "%d %d"
<+25>:    mov    $0x0,%eax
<+30>:    call   0x5555555552e0 <__isoc99_sscanf@plt>
<+35>:    cmp    $0x1,%eax // compare argCount with 1
<+38>:    jle    0x5555555557e2 <phase_5+117> // goto bomb if argCount <= 1

<+40>:    mov    0xc(%rsp),%eax
<+44>:    and    $0xf,%eax // extract 4 LS bits, n = arg1 & 16
<+47>:    mov    %eax,0xc(%rsp) // arg1 &= 16
<+51>:    cmp    $0xf,%eax // compare arg1 with 15
<+54>:    je     0x5555555557d8 <phase_5+107> // goto bomb if arg1 == 15

<+56>:    mov    $0x0,%ecx // sum = 0
<+61>:    mov    $0x0,%edx // i = 0
<+66>:    lea    0x1a2a(%rip),%rsi # 0x5555555571e0 <array.0> // int arr[16]

// Loop
<+73>:    add    $0x1,%edx // i++;
<+76>:    cltq // shorthand for movslq %eax,%rax
<+78>:    mov    (%rsi,%rax,4),%eax // arr[eax]
<+81>:    add    %eax,%ecx // sum += arr[eax]
<+83>:    cmp    $0xf,%eax // compare arr[eax] with 15
<+86>:    jne    0x5555555557b6 <phase_5+73> // goto i++ if arr[eax] != 15

// Validation
<+88>:    movl   $0xf,0xc(%rsp) // arg1 = 15
<+96>:    cmp    $0xf,%edx // compare i with 15
<+99>:    jne    0x5555555557d8 <phase_5+107> // goto bomb if i != 15
<+101>:   cmp    %ecx,0x8(%rsp) // compare arg2 and sum
<+105>:   je     0x5555555557dd <phase_5+112> // goto termination if arg2 == sum 
<+107>:   call   0x555555555c9a <explode_bomb>

<+112>:   add    $0x18,%rsp
<+116>:   ret    
<+117>:   call   0x555555555c9a <explode_bomb>
<+122>:   jmp    0x555555555795 <phase_5+40>

x/16w $rsi
<array.0>: 10     2      14      7
<array.0>: 8      12     15      11
<array.0>: 0      4      1       13
<array.0>: 3      9      6       5
  
1 12 12
2 3 15
3 7 22
4 11 33
5 13 46
6 9 55
7 4 59
8 8 67
9 0 67
10 10 77
11 1 78
12 2 80
13 14 94
14 6 100
15 15 115
i = 15 , sum = 115

int phase_6(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6) {
  read_six_numbers(arg1, arg2, arg3, arg4, arg5, arg6);
  struct nodeR12 = node1;
  struct nodeR13 = node1;
  struct nodeR14 = node1;
  int r15 = 1;
  // Args check <+254>
  // <+266>
  while (nodeR14->val <= 6) {
    // <+282>
    if (r15 > 5) {
      // <+96>
      struct rdx = node1;
      rdx = rdx->next->next->next->next; // node6
      do {
        // <+110>
        nodeR12->val = 7 - nodeR12;
        nodeR12 = nodeR12->next;
      } while (node1 != nodeR12);
      // <+129>
      for (int i = 0; i != 6; i++) {
        // <+134>
      }
      // <+181> chaining nodes
      
      // <+239>
      for (int j = 5; j >= 0; j--) {
        if (curr->val < next->val) explode_bomb();
        curr = curr->next;
      }
      return;
    } else {
      // <+79>
      for (int i = r15; i < 6; i++) {
        nodeR13 = nodeR13->next;
        if (node1->val == nodeR13->next->val) explode_bomb();
      }
      // <+246>
      r15++;
      nodeR14 = nodeR14->next;
    }
  }
  // <+56>
  explode_bomb();
}

<+0>:     endbr64

<+4>:     push   %r15
<+6>:     push   %r14
<+8>:     push   %r13
<+10>:    push   %r12
<+12>:    push   %rbp
<+13>:    push   %rbx

<+14>:    sub    $0x68,%rsp
<+18>:    lea    0x40(%rsp),%rax // address rsp+64 stores six args
<+23>:    mov    %rax,%r14 // nodeR14 points to node1
<+26>:    mov    %rax,0x8(%rsp) // store node1
<+31>:    mov    %rax,%rsi // node6->val
<+34>:    call   0x555555555ce7 <read_six_numbers>
<+39>:    mov    %r14,%r12 // nodeR12 points to node1
<+42>:    mov    $0x1,%r15d // r15 = 1
<+48>:    mov    %r14,%r13 // nodeR13 points to node1
<+51>:    jmp    0x5555555558e7 <phase_6+254> // check args
<+56>:    call   0x555555555c9a <explode_bomb> // goto bomb if args check failed
<+61>:    jmp    0x5555555558f9 <phase_6+272>

<+66>:    add    $0x1,%rbx // i++
<+70>:    cmp    $0x5,%ebx // compare i with 5
<+73>:    jg     0x5555555558df <phase_6+246> // goto if i > 5
<+79>:    mov    0x0(%r13,%rbx,4),%eax // nodeR13->next->val
<+84>:    cmp    %eax,0x0(%rbp) // compare node1.val with node1->nodeR13->val
<+87>:    jne    0x55555555582b <phase_6+66> // goto i++ if node1.val != nodeR13->next->val

<+89>:    call   0x555555555c9a <explode_bomb>
<+94>:    jmp    0x55555555582b <phase_6+66> // goto i++

<+96>:    mov    0x8(%rsp),%rdx // node1
<+101>:   add    $0x18,%rdx // rdx += 24
<+105>:   mov    $0x7,%ecx // 7
<+110>:   mov    %ecx,%eax // eax = 7
<+112>:   sub    (%r12),%eax // 7 - nodeR12->val
<+116>:   mov    %eax,(%r12) // nodeR12->val = 7 - nodeR12->val
<+120>:   add    $0x4,%r12 // nodeR12 = nodeR12->next;
<+124>:   cmp    %r12,%rdx // compare node1 with nodeR12
<+127>:   jne    0x555555555857 <phase_6+110> // goto <+110> if node1 != nodeR12

<+129>:   mov    $0x0,%esi // 0
<+134>:   mov    0x40(%rsp,%rsi,4),%ecx // node1
<+138>:   mov    $0x1,%eax // eax = 1
<+143>:   lea    0x3db1(%rip),%rdx # 0x555555559630 <node1> // rdx = node1
<+150>:   cmp    $0x1,%ecx // compare Mem[rsp[0]+64] with 1
<+153>:   jle    0x55555555588f <phase_6+166> // goto <+166>
<+155>:   mov    0x8(%rdx),%rdx // rdx = rdx->next
<+159>:   add    $0x1,%eax // eax++
<+162>:   cmp    %ecx,%eax // compare eax with ecx
<+164>:   jne    0x555555555884 <phase_6+155> // goto <+155> if eax != ecx
<+166>:   mov    %rdx,0x10(%rsp,%rsi,8) // rsp[]
<+171>:   add    $0x1,%rsi
<+175>:   cmp    $0x6,%rsi
<+179>:   jne    0x55555555586f <phase_6+134>

<+181>:   mov    0x10(%rsp),%rbx // node2
<+186>:   mov    0x18(%rsp),%rax // node3
<+191>:   mov    %rax,0x8(%rbx) // 
<+195>:   mov    0x20(%rsp),%rdx
<+200>:   mov    %rdx,0x8(%rax)
<+204>:   mov    0x28(%rsp),%rax
<+209>:   mov    %rax,0x8(%rdx)
<+213>:   mov    0x30(%rsp),%rdx
<+218>:   mov    %rdx,0x8(%rax)
<+222>:   mov    0x38(%rsp),%rax
<+227>:   mov    %rax,0x8(%rdx)
<+231>:   movq   $0x0,0x8(%rax)

<+239>:   mov    $0x5,%ebp // j = 5
<+244>:   jmp    0x555555555914 <phase_6+299>

<+246>:   add    $0x1,%r15 // r15++
<+250>:   add    $0x4,%r14 // r14 += 4

// Args check
<+254>:   mov    %r14,%rbp // nodeR14
<+257>:   mov    (%r14),%eax // node1->val
<+260>:   sub    $0x1,%eax // node1->val - 1

<+263>:   cmp    $0x5,%eax // compare node1->val - 1 with 5
<+266>:   ja     0x555555555821 <phase_6+56> // goto bomb if node1.val > 6

<+272>:   cmp    $0x5,%r15d // compare r15 with 5
<+276>:   jg     0x555555555849 <phase_6+96> // goto <+96> if r15 > 5

<+282>:   mov    %r15,%rbx // i = r15 = 1
<+285>:   jmp    0x555555555838 <phase_6+79> // goto <+79>

<+290>:   mov    0x8(%rbx),%rbx
<+294>:   sub    $0x1,%ebp
<+297>:   je     0x555555555925 <phase_6+316>

<+299>:   mov    0x8(%rbx),%rax // next = curr->next
<+303>:   mov    (%rax),%eax // next->val
<+305>:   cmp    %eax,(%rbx) // compare curr->val with next->val
<+307>:   jge    0x55555555590b <phase_6+290> // goto <+290> if curr->val >= next->val
<+309>:   call   0x555555555c9a <explode_bomb>
<+314>:   jmp    0x55555555590b <phase_6+290>

<+316>:   add    $0x68,%rsp
<+320>:   pop    %rbx
<+321>:   pop    %rbp
<+322>:   pop    %r12
<+324>:   pop    %r13
<+326>:   pop    %r14
<+328>:   pop    %r15
<+330>:   ret

<node1>: 887
<node2>: 492
<node3>: 966
<node4>: 172
<node5>: 912
<node6>: 1000;

6 -> 3 -> 5 -> 1 -> 2 -> 4;
(7-1) -> (7-3) -> (7-5) -> (7-1) -> (7-2) -> (7-4);
1 -> 4 -> 2 -> 6 -> 5 -> 3;


<+0>:     endbr64 
<+4>:     push   %rbx
<+5>:     call   0x555555555d2c <read_line>
<+10>:    mov    %rax,%rdi
<+13>:    mov    $0xa,%edx
<+18>:    mov    $0x0,%esi
<+23>:    call   0x5555555552c0 <strtol@plt>
<+28>:    mov    %eax,%ebx
<+30>:    sub    $0x1,%eax
<+33>:    cmp    $0x3e8,%eax
<+38>:    ja     0x5555555559c3 <secret_phase+78>
<+40>:    mov    %ebx,%esi
<+42>:    lea    0x3baa(%rip),%rdi        # 0x555555559550 <n1>
<+49>:    call   0x555555555934 <fun7>
<+54>:    cmp    $0x1,%eax
<+57>:    jne    0x5555555559ca <secret_phase+85>
<+59>:    lea    0x17d1(%rip),%rdi        # 0x555555557188
<+66>:    call   0x555555555210 <puts@plt>
<+71>:    call   0x555555555e5b <phase_defused>
<+76>:    pop    %rbx
<+77>:    ret    
<+78>:    call   0x555555555c9a <explode_bomb>
<+83>:    jmp    0x55555555599d <secret_phase+40>
<+85>:    call   0x555555555c9a <explode_bomb>
<+90>:    jmp    0x5555555559b0 <secret_phase+59>

0x555555559550 <n1>:    0x0000000000000024      0x0000555555559570
0x555555559560 <n1+16>: 0x0000555555559590      0x0000000000000000
0x555555559570 <n21>:   0x0000000000000008      0x00005555555595f0
0x555555559580 <n21+16>:        0x00005555555595b0      0x0000000000000000
0x555555559590 <n22>:   0x0000000000000032      0x00005555555595d0
0x5555555595a0 <n22+16>:        0x0000555555559610      0x0000000000000000
0x5555555595b0 <n32>:   0x0000000000000016      0x00005555555590c0
0x5555555595c0 <n32+16>:        0x0000555555559080      0x0000000000000000
0x5555555595d0 <n33>:   0x000000000000002d      0x0000555555559020
0x5555555595e0 <n33+16>:        0x00005555555590e0      0x0000000000000000
0x5555555595f0 <n31>:   0x0000000000000006      0x0000555555559040
0x555555559600 <n31+16>:        0x00005555555590a0      0x0000000000000000
0x555555559610 <n34>:   0x000000000000006b      0x0000555555559060
0x555555559620 <n34+16>:        0x0000555555559100      0x0000000000000000
0x555555559040 <n41>:   0x0000000000000001      0x0000000000000000
0x555555559050 <n41+16>:        0x0000000000000000      0x0000000000000000
0x5555555590a0 <n42>:   0x0000000000000007      0x0000000000000000
0x5555555590b0 <n42+16>:        0x0000000000000000      0x0000000000000000
0x5555555590c0 <n43>:   0x0000000000000014      0x0000000000000000
0x5555555590d0 <n43+16>:        0x0000000000000000      0x0000000000000000
0x555555559080 <n44>:   0x0000000000000023      0x0000000000000000
0x555555559090 <n44+16>:        0x0000000000000000      0x0000000000000000
0x555555559020 <n45>:   0x0000000000000028      0x0000000000000000
0x555555559030 <n45+16>:        0x0000000000000000      0x0000000000000000
0x5555555590e0 <n46>:   0x000000000000002f      0x0000000000000000
0x5555555590f0 <n46+16>:        0x0000000000000000      0x0000000000000000
0x555555559060 <n47>:   0x0000000000000063      0x0000000000000000
0x555555559070 <n47+16>:        0x0000000000000000      0x0000000000000000
0x555555559100 <n48>:   0x00000000000003e9      0x0000000000000000
0x555555559110 <n48+16>:        0x0000000000000000      0x0000000000000000

int fun7(node root, int input) {
  if (root == NULL) return -1;
  if (root->val > input) {
    root = root->left;
    res = fun7(root);
    return res *= 2;
  } else if (root->val != input) {
    root = root->right;
    res = func7(root);
    res = res * 2 + 1;
    return res;
  } else return 0;
}

<+0>:     endbr64 
<+4>:     test   %rdi,%rdi // if root == null (root->val == 0)
<+7>:     je     0x55555555596f <fun7+59> // goto <+59>
<+9>:     sub    $0x8,%rsp
<+13>:    mov    (%rdi),%edx // root->val
<+15>:    cmp    %esi,%edx // compare root->val with input
<+17>:    jg     0x555555555953 <fun7+31> // goto <+31> if root->val > input
<+19>:    mov    $0x0,%eax // res = 0
<+24>:    jne    0x555555555960 <fun7+44>
<+26>:    add    $0x8,%rsp
<+30>:    ret    
<+31>:    mov    0x8(%rdi),%rdi // root = root->left
<+35>:    call   0x555555555934 <fun7> // fun7(root)
<+40>:    add    %eax,%eax // eax *= 2
<+42>:    jmp    0x55555555594e <fun7+26>
<+44>:    mov    0x10(%rdi),%rdi // root = root->right
<+48>:    call   0x555555555934 <fun7> // fun7(root)
<+53>:    lea    0x1(%rax,%rax,1),%eax
<+57>:    jmp    0x55555555594e <fun7+26>
<+59>:    mov    $0xffffffff,%eax // -1
<+64>:    ret

   0x0000555555555934 <+0>:     endbr64 
=> 0x0000555555555938 <+4>:     test   %rdi,%rdi
   0x000055555555593b <+7>:     je     0x55555555596f <fun7+59>
   0x000055555555593d <+9>:     sub    $0x8,%rsp
   0x0000555555555941 <+13>:    mov    (%rdi),%edx
   0x0000555555555943 <+15>:    cmp    %esi,%edx
   0x0000555555555945 <+17>:    jg     0x555555555953 <fun7+31>
   0x0000555555555947 <+19>:    mov    $0x0,%eax
   0x000055555555594c <+24>:    jne    0x555555555960 <fun7+44>
   0x000055555555594e <+26>:    add    $0x8,%rsp
   0x0000555555555952 <+30>:    ret    
   0x0000555555555953 <+31>:    mov    0x8(%rdi),%rdi
   0x0000555555555957 <+35>:    call   0x555555555934 <fun7>
   0x000055555555595c <+40>:    add    %eax,%eax
   0x000055555555595e <+42>:    jmp    0x55555555594e <fun7+26>
   0x0000555555555960 <+44>:    mov    0x10(%rdi),%rdi
   0x0000555555555964 <+48>:    call   0x555555555934 <fun7>
   0x0000555555555969 <+53>:    lea    0x1(%rax,%rax,1),%eax
   0x000055555555596d <+57>:    jmp    0x55555555594e <fun7+26>
   0x000055555555596f <+59>:    mov    $0xffffffff,%eax
   0x0000555555555974 <+64>:    ret 