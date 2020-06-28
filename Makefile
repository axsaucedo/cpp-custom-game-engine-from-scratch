
install-linux-deps:
	sudo apt-get install \
		libsdl2-dev \
		libsdl2-image-dev \
		libsdl2-ttf-dev \
		libsdl2-mixer-dev \
		lua5.3

build:
	g++ -w -std=c++14 -Wfatal-errors \
	./src/*.cpp \
	-o game \
	-I"./lib/lua" \
	-L"./lib/lua" \
	-llua \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer;

clean:
	rm ./game;

run:
	./game;

