all: vm input_gen

vm: vm.c vm.h replacement.c pagetable.c pagetable.h disk.h disk.c
		gcc vm.c replacement.c pagetable.c disk.c -g -o vm

input_gen: input_gen.c vm.h
		gcc input_gen.c -g -o input_gen

clean:
		rm -f vm input_gen
