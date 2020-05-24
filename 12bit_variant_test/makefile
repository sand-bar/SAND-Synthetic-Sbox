CXX=g++ -std=c++17
CXXFLAG=-O3
LIB=
BIN = bin_bat_ddt_12bit bin_bat_lat_12bit

all: $(BIN)

DDT_SRC=bat_ddt_12bit.cpp ddt_ssb.h
bin_bat_ddt_12bit: $(DDT_SRC)
	$(CXX) $(CXXFLAG) bat_ddt_12bit.cpp -o $@ $(LIB)

LAT_SRC=bat_lat_12bit.cpp lat_ssb.h
bin_bat_lat_12bit: $(LAT_SRC)
	$(CXX) $(CXXFLAG) bat_lat_12bit.cpp -o $@ $(LIB)

.PHONY:
	clean
clean:
	@echo "Removing bins......"
	rm -rf $(BIN)
