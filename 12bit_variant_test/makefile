CXX=g++ -std=c++17
CXXFLAG=-O3
LIB=
BIN = bin_sand_ddt_12bit bin_sand_lat_12bit \
	  bin_sand_ddt_16bit bin_sand_lat_16bit

all: $(BIN)

DDT_SRC=sand_ddt_12bit.cpp ddt_ssb.h
bin_sand_ddt_12bit: $(DDT_SRC)
	$(CXX) $(CXXFLAG) sand_ddt_12bit.cpp -o $@ $(LIB)

LAT_SRC=sand_lat_12bit.cpp lat_ssb.h
bin_sand_lat_12bit: $(LAT_SRC)
	$(CXX) $(CXXFLAG) sand_lat_12bit.cpp -o $@ $(LIB)

bin_sand_ddt_16bit: sand_ddt_16bit.cpp ddt_ssb.h
	$(CXX) $(CXXFLAG) sand_ddt_16bit.cpp -o $@ $(LIB)

bin_sand_lat_16bit: sand_lat_16bit.cpp lat_ssb.h
	$(CXX) $(CXXFLAG) sand_lat_16bit.cpp -o $@ $(LIB)

.PHONY:
	clean
clean:
	@echo "Removing bins......"
	rm -rf $(BIN)
