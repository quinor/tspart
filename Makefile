all: clean configure tspart package

.PHONY: all clean conigure tspart libs sfml tgui package

clean:
	rm -rf deps
	rm -rf venv

configure: libs
	cmake . -B .build

tspart:
	cmake --build .build

libs: sfml tgui
	rm -rf deps/src
	mkdir -p libs
	cp deps/lib/libsfml-*.so.* deps/lib/libtgui.so.* libs/

sfml:
	mkdir -p deps/src/;\
	cd deps/src;\
	curl -L "https://www.sfml-dev.org/files/SFML-2.5.1-sources.zip" >SFML.zip;\
	unzip -n SFML.zip;\
	cd SFML-*;\
	cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_INSTALL_PREFIX=../../ .;\
	make -j12;\
	make install

tgui:
	mkdir -p deps/src/;\
	cd deps/src;\
	curl -L "https://github.com/texus/TGUI/archive/v0.8.3.zip" >TGUI.zip;\
	unzip -n TGUI.zip;\
	cd TGUI-*;\
	cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_INSTALL_PREFIX=../../ .;\
	make -j12;\
	make install

package:
	mkdir -p package
	mkdir -p package/res
	cp -r  tsp_art tspart_gui shaders libs misc package/
	cp res/klaudia.jpg package/res/
	tar czf package.tar.gz package
