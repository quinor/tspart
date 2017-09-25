all: clean tspart package

.PHONY: all clean tspart venv libs sfml ninja glm boost package

clean:
	rm -rf deps
	rm -rf venv

tspart: libs venv
	. ./venv/bin/activate;\
	bf variation=release;\
	./deps/bin/ninja

venv:
	python3 -m venv venv;\
	. ./venv/bin/activate;\
	pip install -r requirements.txt

libs: sfml ninja glm boost tgui
	rm -rf deps/src
	mkdir -p libs
	cp deps/lib/*.so.2.4 deps/lib/libtgui.so.0.7.4 libs/

sfml:
	mkdir -p deps/src/;\
	cd deps/src;\
	curl -L "https://www.sfml-dev.org/files/SFML-2.4.2-sources.zip" >SFML.zip;\
	unzip -n SFML.zip;\
	cd SFML-*;\
	cmake -DCMAKE_CXX_COMPILER=clang++ .;\
	make -j12;\
	cp -r include lib ../..

tgui:
	mkdir -p deps/src/;\
	cd deps/src;\
	curl -L "https://github.com/texus/TGUI/archive/0.7.4.zip" >TGUI.zip;\
	unzip -n TGUI.zip;\
	cd TGUI-*;\
	export SFML_ROOT=../../;\
	cmake -DCMAKE_CXX_COMPILER=clang++ .;\
	make -j12;\
	cp -r include lib ../..

ninja:
	mkdir -p deps/src/;\
	mkdir -p deps/bin/;\
	cd deps/src/;\
	curl -L "https://github.com/ninja-build/ninja/releases/download/v1.7.2/ninja-linux.zip" >ninja.zip;\
	unzip -n ninja.zip;\
	cp ninja ../bin/

glm:
	mkdir -p deps/src;\
	mkdir -p deps/include;\
	cd deps/src/;\
	curl -L https://github.com/g-truc/glm/releases/download/0.9.8.5/glm-0.9.8.5.zip >glm.zip;\
	unzip -n glm.zip;\
	cp -r glm/glm ../include

boost:
	mkdir -p deps/src;\
	cd deps/src;\
	curl -L http://dl.bintray.com/boostorg/release/1.65.0/source/boost_1_65_0.tar.gz >boost.tar.gz;\
	tar xf boost.tar.gz;\
	cp -r boost_*/boost ../include

package:
	mkdir -p package
	mkdir -p package/res
	cp -r grayscale_normalization color_normalization tsp_art tspart_gui shaders libs misc package/
	cp res/klaudia.jpg package/res/
	tar czf package.tar.gz package
