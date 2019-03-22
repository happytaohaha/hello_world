#include <stdio.h>  //标准输入输出
#include <math.h>
//搞清五大分区
//传递地址   *取值
void copy_string(char *from,char *to){
    for(;*from != '\0';from++,to++){
        *to = *from;
    }
    *to = '\0';
};
int main() {
    char *a= "I am a teacher";// 这里 *a 是直接把后面的地址直接给后面
    char b[] = "You are a student";
    // 用 char *b = "You are a student";就会错误    "You are a student" 是在常量区，因此只能读不能写
    /**
     * 因为 b[]  拷贝的时候按字段每次考 一个
     */

    printf("String a = %s\n String b =%s\n",a,b);
    printf("copy string a to string b:\n");

    copy_string(a,b);

    printf("String a = %s\n String b =%s\n",a,b);
    return 0;
};

/**   五大分区文档
 * 在C/C++中，内存分成5个区，他们分别是堆、栈、全局/静态存储区和常量存储区，程序代码区。

   1. 栈，就是那些由编译器在需要的时候分配，在不需要的时候自动清楚的变量的存储区。里面的变量通常是局部变量、函数参数等。

    2.堆，就是那些由new分配的内存块，他们的释放编译器不去管，由我们的应用程序去控制，一般一个new就要对应一个delete。如果程序员没有释放掉，那么在程序结束后，操作系统会自动回收。   （new的方法是C++才支持的）C 是 malloc
自由存储区（也是堆区），就是那些由malloc等分配的内存块，他和堆是十分相似的，不过它是用free来结束自己的生命的。

  3.  全局/静态存储区，全局变量和静态变量被分配到同一块内存中，在以前的C语言中，全局变量又分为初始化的和未初始化的，在C++里面没有这个区分了，他们共同占用同一块内存区。

 4. 文字常量存储区，这是一块比较特殊的存储区，他们里面存放的是常量，不允许修改（当然，你要通过非正当手段也可以修改，而且方法很多） 在程序中使用的常量存储在此区域。程序结束后，由系统释放。
在程序中使用的常量，都会到文字常量区获取。

5.程序代码区 —存放函数体的二进制代码。存放函数体的二进制代码。
运行程序就是执行代码，代码要执行就要加载进内存。




明确区分堆与栈
    在bbs上，堆与栈的区分问题，似乎是一个永恒的话题，由此可见，初学者对此往往是混淆不清的，所以我决定拿他第一个开刀。



    首先，我们举一个例子：
    void f() { int* p=new int[5]; }
    这条短短的一句话就包含了堆与栈，看到new，我们首先就应该想到，我们分配了一块堆内存，那么指针p呢？他分配的是一块栈内存，所以这句话的意思就是：在栈内存中存放了一个指向一块堆内存的指针p。在程序会先确定在堆中分配内存的大小，然后调用operator new分配内存，然后返回这块内存的首地址，放入栈中，他在VC6下的汇编代码如下：
    00401028   push        14h
    0040102A   call        operator new (00401060)
    0040102F   add         esp,4
    00401032   mov         dword ptr [ebp-8],eax
    00401035   mov         eax,dword ptr [ebp-8]
    00401038   mov         dword ptr [ebp-4],eax

    这里，我们为了简单并没有释放内存，那么该怎么去释放呢？是delete p么？澳，错了，应该是delete []p，这是为了告诉编译器：我删除的是一个数组，VC6就会根据相应的Cookie信息去进行释放内存的工作。
    好了，我们回到我们的主题：堆和栈究竟有什么区别？
    主要的区别由以下几点：
    1、管理方式不同；
    2、空间大小不同；
    3、能否产生碎片不同；
    4、生长方向不同；
    5、分配方式不同；
    6、分配效率不同；

    管理方式：对于栈来讲，是由编译器自动管理，无需我们手工控制；对于堆来说，释放工作由程序员控制，容易产生memory leak。
    空间大小：一般来讲在32位系统下，堆内存可以达到4G的空间，从这个角度来看堆内存几乎是没有什么限制的。但是对于栈来讲，一般都是有一定的空间大小的，例如，在VC6下面，默认的栈空间大小是1M（好像是，记不清楚了）。当然，我们可以修改：
    打开工程，依次操作菜单如下：Project->Setting->Link，在Category 中选中Output，然后在Reserve中设定堆栈的最大值和commit。
注意：reserve最小值为4Byte；commit是保留在虚拟内存的页文件里面，它设置的较大会使栈开辟较大的值，可能增加内存的开销和启动时间。

    碎片问题：对于堆来讲，频繁的new/delete势必会造成内存空间的不连续，从而造成大量的碎片，使程序效率降低。对于栈来讲，则不会存在这个问题，因为栈是先进后出的队列，他们是如此的一一对应，以至于永远都不可能有一个内存块从栈中间弹出，在他弹出之前，在他上面的后进的栈内容已经被弹出，详细的可以参考数据结构，这里我们就不再一一讨论了。
    生长方向：对于堆来讲，生长方向是向上的，也就是向着内存地址增加的方向；对于栈来讲，它的生长方向是向下的，是向着内存地址减小的方向增长。

    分配方式：堆都是动态分配的，没有静态分配的堆。栈有2种分配方式：静态分配和动态分配。静态分配是编译器完成的，比如局部变量的分配。动态分配由alloca函数进行分配，但是栈的动态分配和堆是不同的，他的动态分配是由编译器进行释放，无需我们手工实现。

    分配效率：栈是机器系统提供的数据结构，计算机会在底层对栈提供支持：分配专门的寄存器存放栈的地址，压栈出栈都有专门的指令执行，这就决定了栈的效率比较高。堆则是C/C++函数库提供的，它的机制是很复杂的，例如为了分配一块内存，库函数会按照一定的算法（具体的算法可以参考数据结构/操作系统）在堆内存中搜索可用的足够大小的空间，如果没有足够大小的空间（可能是由于内存碎片太多），就有可能调用系统功能去增加程序数据段的内存空间，这样就有机会分到足够大小的内存，然后进行返回。显然，堆的效率比栈要低得多。

    从这里我们可以看到，堆和栈相比，由于大量new/delete的使用，容易造成大量的内存碎片；由于没有专门的系统支持，效率很低；由于可能引发用户态和核心态的切换，内存的申请，代价变得更加昂贵。所以栈在程序中是应用最广泛的，就算是函数的调用也利用栈去完成，函数调用过程中的参数，返回地址，EBP和局部变量都采用栈的方式存放。所以，我们推荐大家尽量用栈，而不是用堆。

    虽然栈有如此众多的好处，但是由于和堆相比不是那么灵活，有时候分配大量的内存空间，还是用堆好一些。
    无论是堆还是栈，都要防止越界现象的发生（除非你是故意使其越界），因为越界的结果要么是程序崩溃，要么是摧毁程序的堆、栈结构，产生以想不到的结果,就算是在你的程序运行过程中，没有发生上面的问题，你还是要小心，说不定什么时候就崩掉，那时候debug可是相当困难的：）

    对了，还有一件事，如果有人把堆栈合起来说，那它的意思是栈，可不是堆，呵呵，清楚了？
 */

