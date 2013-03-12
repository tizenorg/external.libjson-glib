testbuildtype=normal
#export testbuildtype 

all:	TCParser  TC_Generator  TC_Types  TC_Gobject
	echo done
	
TCParser:
	make -s -C TC_Parser	
	
TC_Generator:
	make -s -C TC_Generator	

TC_Types:
	make -s -C TC_Types	

TC_Gobject:
	make -s -C TC_Gobject	
	
clean:
	make clean -C TC_Parser
	make clean -C TC_Generator
	make clean -C TC_Types
	make clean -C TC_Gobject
		