# gpio示例

## 内核空间

> gpio 一般在进行开发板设计的时候就已经固定好了， 有的 gpio 只能作为设备复用功能管脚，有的 gpio 作为普通的输入输出和中断检测功能， 对于固定设备复用的功能管脚在以下文件中定义：
>
> `arch/mips/boot/dts/ingenic/x2000-v12-pinctrl.dtsi`
>
> 在 arch/mips/boot/dts/ingenic/zebra.dts 会根据驱动配置， 选中相应的设备功能管脚。内核的 gpio 驱动基于 gpio 子系统实现，很方便的进行 gpio 控制。

### 1.1 设备树配置

> 在arch/mips/boot/dts/ingenic/x2000-v12-pinctrl.dtsi 下配置，部分GPIO配置如下：

```
#include <dt-bindings/pinctrl/ingenic-pinctrl.h>

&pinctrl {
        uart0_pin: uart0-pin {
                uart0_pd: uart0-pd {
                        ingenic,pinmux = <&gpd 23 26>;
                        ingenic,pinmux-funcsel = <PINCTL_FUNCTION2>;
                };
        };
        uart1_pin: uart1-pin {
                uart1_pc: uart1-pc {
                        ingenic,pinmux = <&gpc 21 24>;
                        ingenic,pinmux-funcsel = <PINCTL_FUNCTION1>;
                };
        };

        uart2_pin: uart2-pin {
                uart2_pd: uart2-pd {
                        ingenic,pinmux = <&gpd 30 31>;
                        ingenic,pinmux-funcsel = <PINCTL_FUNCTION0>;
                };
        };

        uart3_pin: uart3-pin {
                uart3_pd: uart3-pd {
                        ingenic,pinmux = <&gpd 0 3>;
                        ingenic,pinmux-funcsel = <PINCTL_FUNCTION1>;
                };
        };
}
```

### 1.2 驱动配置

> 在电脑端输入 make menuconfig， 然后配置：

```
 -*- GPIO Support  ---> 
         --- GPIO Support
         [*]   /sys/class/gpio/... (sysfs interface)
```

## 用户空间

> 在内核导出 gpio 节点的前提下， 可以操作/sys/class/gpio 节点， 控制 gpio 输入输出。
>
> 节点路径：
>
> ```
>     /sys/class/gpio/
> ```
>
> GPIO导入导出说明：
>
> > "export" ... 用户空间可以通过写其编号到这个文件， 要求内核导出，一个 GPIO 的控制到用户空间。
> >
> > > 例如: 如果内核代码没有申请 GPIO \#19,"echo 19 &gt; export"
> > >
> > > 将会为 GPIO \#19 创建一个 "gpio19" 节点。
> >
> > "unexport" ... 导出到用户空间的逆操作。
> >
> > > 例如: "echo 19 &gt; unexport" 将会移除使用"export"文件导出的
>
>
>
> GPIO 信号的路径类似 /sys/class/gpio/gpio42/ \(对于 GPIO \#42 来说\)，
>
> 属性路径：
>
> ```
> /sys/class/gpio/gpioN/
> ```
>
> 读/写属性说明：
>
> > "direction" ... 读取得到 "in" 或 "out"。 这个值通常运行写入。
> >
> > > 写入"out" 时,其引脚的默认输出为低电平。 为了确保无故障运行，"low" 或 "high" 的电平值应该写入 GPIO 的配置， 作为初始输出值。
> > >
> > > 注意:如果内核不支持改变 GPIO 的方向， 或者在导出时内核代码没
> >
> > "value" ... 读取得到 0 \(低电平\) 或 1 \(高电平\)。 如果 GPIO 配置为
> >
> > > 如果引脚可以配置为中断信号， 且如果已经配置了产生中断的模式
> >
> > "edge" ... 读取得到“none”、“rising”、“falling” 或者“both”。
> >
> > > 将这些字符串写入这个文件可以选择沿触发模式， 会使得轮询操作
> > >
> > > 这个文件仅有在这个引脚可以配置为可产生中断输入引脚时， 才存
> >
> > "active\_low" ... 读取得到 0 \(假\) 或 1 \(真\)。 写入任何非零值可以


