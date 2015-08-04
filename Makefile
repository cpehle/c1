


all: c1.sv sim.cpp
	verilator -trace -Wall --exe sim.cpp --cc c1.sv
	cd obj_dir && make -f Vc1.mk
	./obj_dir/Vc1
