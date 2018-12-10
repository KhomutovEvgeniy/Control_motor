SET PATH=C:\My programms\Keil\ARM\ARMCC\Bin;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\My programms\MATLAB\R2014a\runtime\win64;C:\My programms\MATLAB\R2014a\bin;C:\My programms\MATLAB\R2014a\polyspace\bin;C:\Program Files\Git\cmd;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;C:\WINDOWS\System32\OpenSSH\;C:\Program Files\TortoiseGit\bin;C:\My programms\OpenCV\opencv\build\x64\vc12\bi;C:\Program Files (x86)\PuTTY\;C:\Users\Evgeny\AppData\Local\Programs\Python\Python37-32\Scripts\;C:\Users\Evgeny\AppData\Local\Programs\Python\Python37-32\;C:\Users\Evgeny\AppData\Local\Microsoft\WindowsApps;C:\Program Files\Java\jdk-11.0.1\bin;
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32F103C6
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=newprj
SET CPU_CLOCK=0x007A1200
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\output\core_cm3.__i"
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\output\system_stm32f10x.__i"
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmAsm" --Via ".\output\startup_stm32f10x_md._ia"
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_rcc.__i"
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_gpio.__i"
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\output\misc.__i"
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_tim.__i"
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_usart.__i"
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_adc.__i"
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_dma.__i"
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_flash.__i"
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_iwdg.__i"
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_exti.__i"
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_spi.__i"
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\output\main.__i"
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\output\adc.__i"
"C:\My programms\Keil\ARM\ARMCC\Bin\ArmLink" --Via ".\output\output.lnp"
"C:\My programms\Keil\ARM\ARMCC\Bin\fromelf.exe" ".\output\output.elf" --i32combined --output ".\output\output.hex"
