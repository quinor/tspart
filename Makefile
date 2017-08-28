all: clean tspart

.PHONY: all clean tspart venv sfml ninja glm

clean:
	rm -rf ./deps
	rm -rf venv

tspart: sfml ninja glm venv
	. ./venv/bin/activate;\
	bf variation=release;\
	./deps/bin/ninja

venv:
	python3 -m venv venv;\
	. ./venv/bin/activate;\
	pip install -r requirements.txt

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

glm:
	mkdir -p deps/src;\
	mkdir -p deps/include;\
	cd deps/src/;\
	curl -L https://github.com/g-truc/glm/releases/download/0.9.8.5/glm-0.9.8.5.zip >glm.zip;\
	unzip glm.zip;\
	cp -r glm/glm ../include
