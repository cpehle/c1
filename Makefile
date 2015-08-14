


all: C1.sv sim.cpp
	verilator -trace -Wall --exe sim.cpp --cc C1.sv
	cd obj_dir && make -f VC1.mk
	./obj_dir/VC1

model: model.cpp
	c++ -Wall -g model.cpp -o model
