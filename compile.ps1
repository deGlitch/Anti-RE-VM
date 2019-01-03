Get-ChildItem output -File -Name | % { "output/$_" } |  Remove-Item
iex "nasm source/logic.vasm -o output/vm_asm"
iex "objcopy --input-target binary --output-target pe-i386 --binary-architecture i386:x86-64 output/vm_asm output/vm_asm.o"
iex "g++ -Wall source/main.cpp output/vm_asm.o -o output/vm.exe"
Get-ChildItem output -File -Name | ? { !$_.EndsWith(".exe")} | % { "output/$_" } |  Remove-Item