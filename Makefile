all: clean tspart

clean:
	rm -rf ./deps
	rm -rf venv

tspart: libs venv
	. ./venv/bin/activate;\
	bf variation=release;\
	./deps/bin/ninja

venv:
	python3 -m venv venv;\
	. ./venv/bin/activate;\
	pip install -r requirements.txt

libs: sfml ninja

.PHONY: all clean tspart venv libs sfml ninja

sfml:
	mkdir -p deps/src/;\
	cd deps/src;\
	curl -L "https://www.sfml-dev.org/files/SFML-2.4.2-sources.zip" >SFML.zip;\
	unzip SFML.zip;\
	cd SFML-2.4.2;\
	cmake .;\
	cp -r include lib ../..

ninja:
	mkdir -p deps/src/;\
	mkdir -p deps/bin/;\
	cd deps/src/;\
	curl -L "https://github.com/ninja-build/ninja/releases/download/v1.7.2/ninja-linux.zip" >ninja.zip;\
	unzip ninja.zip;\
	cp ninja ../bin/
